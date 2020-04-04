/***********************************************************
	file: 				uc_usart.c

	file description: 	TODO: add your file description

	author:				Florian
	created on:			02.04.2020
***********************************************************/


/***********************************************************
						includes
***********************************************************/
#include <stdio.h>
#include <uc_gpio.h>
#include <uc_usart_config.h>
#include <RTOS.h>
#include <util_ringbuffer_u8.h>


/***********************************************************
					configuration options
***********************************************************/
/*
APB_CLOCK						Define used clock on APB Bus. Needed to calculate Baudrate correctly

USART							Debug USART
USART_IRQ						USART Interrupt
USART_IRQ_PRIO					USART Interrupt Priority
USART_ISR						USART ISR

USART_BAUDRATE					Used baudrate

UART_TX_PORT					Port of TX pin
UART_TX_PIN						Pin number of TX pin
UART_RX_PORT					Port of RX pin
UART_RX_PIN						Pin number of RX pin
UART_AF							Alternate Function for GPIO pins

USE_PRINTF						Set this to true, if you wish to use printf with the debug usart

UC_USART_RX_BUFFER_SIZE			Set the RX buffer size
*/


/**********************************************************
					private definitions
***********************************************************/



/***********************************************************
					   private types
***********************************************************/



/***********************************************************
					private variables
***********************************************************/
static UTIL_RINGBUFFER_U8 rx_ringbuffer;
static UTIL_RINGBUFFER_U8 tx_ringbuffer;

/***********************************************************
				private function declarations
***********************************************************/



/***********************************************************
				public function definitions
***********************************************************/
extern void UC_USART_Init(uint8_t* tx_databuffer, uint32_t tx_buffersize, uint8_t* rx_databuffer, uint32_t rx_buffersize)
{
	/*
	 * Init Buffers
	 */
	UTIL_RINGBUFFER_U8_init(&tx_ringbuffer, tx_databuffer, tx_buffersize);
	UTIL_RINGBUFFER_U8_init(&rx_ringbuffer, rx_databuffer, rx_buffersize);

	/*
	 * Init USART
	 */
	// Baudrate Settings BRR = APB1Clock/(16*Baudrate)
	uint16_t mant = (uint16_t)(UC_USART_APB_CLOCK/(UC_USART_BAUDRATE*16)) << 4;
	uint8_t frac = ((UC_USART_APB_CLOCK << 4) / ((UC_USART_BAUDRATE*16))) & 0xF;
	UC_USART->BRR |= (mant | frac);


	CLEAR_REG(UC_USART->CR2);
	CLEAR_REG(UC_USART->CR3);
	CLEAR_REG(UC_USART->CR1);
	UC_USART->CR1 |= (USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_TE);
	UC_USART->CR1 |= USART_CR1_UE; // Enable USART
	OS_Delay(100);

	/*
	 * Init GPIOs
	 */
	//TX
	UC_GPIO_setPullup(UC_USART_TX_PORT, UC_USART_TX_PIN);
	UC_GPIO_setAlternateFunction(UC_USART_TX_PORT, UC_USART_TX_PIN, UC_USART_AF);
	UC_GPIO_setModeAlternateFunction(UC_USART_TX_PORT, UC_USART_TX_PIN);
	UC_GPIO_setOutputTypePushPull(UC_USART_TX_PORT, UC_USART_TX_PIN);
	UC_GPIO_setSpeedVeryHigh(UC_USART_TX_PORT, UC_USART_TX_PIN);

	//RX
	UC_GPIO_setPullup(UC_USART_RX_PORT, UC_USART_RX_PIN);
	UC_GPIO_setAlternateFunction(UC_USART_RX_PORT, UC_USART_RX_PIN, UC_USART_AF);
	UC_GPIO_setModeAlternateFunction(UC_USART_RX_PORT, UC_USART_RX_PIN);
	UC_GPIO_setSpeedVeryHigh(UC_USART_RX_PORT, UC_USART_RX_PIN);

	/*
	 * Enable Interrupts
	 */
	NVIC_SetPriority(UC_USART_IRQ, UC_USART_IRQ_PRIO);
	NVIC_EnableIRQ(UC_USART_IRQ);

	return;
}

extern void UC_USART_sendString(const char* str, uint32_t size)
{
	for(uint32_t i = 0; i < size; i++)
	{
		UTIL_U8_RINGBUFFER_put(&tx_ringbuffer, str[i]);
	}
	UC_USART->CR1 |= USART_CR1_TXEIE;
	return;
}

extern bool UC_USART_getMessage(char* buffer, uint32_t size)
{
	uint32_t dataRead = 0;
	while(rx_ringbuffer.data[rx_ringbuffer.out] != '\0')
	{
		*buffer = UTIL_U8_RINGBUFFER_get(&rx_ringbuffer);
		buffer++;
		dataRead++;
		if(dataRead >= size)
		{
			return false;
		}
	}
	*buffer = '\0';
	return true;
}

void UC_USART_ISR(void)
{
	OS_EnterNestableInterrupt();
	if(READ_BIT(UC_USART->SR, USART_SR_RXNE)) // if RX Data is received
	{
		uint8_t data = (uint8_t)(UC_USART->DR & 0xFF);
		if(data == 0)
		{
			UC_USART_MESSAGE_RECEIVED_CALLBACK();
		}
		else
		{
			UTIL_U8_RINGBUFFER_put(&rx_ringbuffer, data);
		}

		CLEAR_BIT(UC_USART->SR, USART_SR_RXNE); // Clear Interrupt Flag
	}
	if(READ_BIT(UC_USART->SR, USART_SR_TXE)) // if transmission complete flag is set
	{
		CLEAR_BIT(UC_USART->SR, USART_SR_TXE); // Clear Interrupt Flag
		if(UTIL_U8_RINGBUFFER_isEmpty(&tx_ringbuffer))
		{
			UC_USART->CR1 &= ~USART_CR1_TXEIE; // Disable TXE Interrupt if tx buffer is empty
			CLEAR_BIT(UC_USART->CR1, USART_CR1_TXEIE); // Disable TXE interrupt
		}
		else
		{
			UC_USART->DR = UTIL_U8_RINGBUFFER_get(&tx_ringbuffer);
		}
	}
	OS_LeaveNestableInterrupt();
	return;
}


#ifdef UC_USART_USE_PRINTF
#if UC_USART_USE_PRINTF

int _write(int file, char *data, int len)
{
	UC_USART_sendString(data, len);
	return len;
}

#endif
#endif

/***********************************************************
				private function definitions
***********************************************************/
