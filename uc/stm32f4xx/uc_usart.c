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
#include <stdint.h>
#include <uc_gpio.h>
#include <uc_usart_config.h>
#include <RTOS.h>


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
typedef struct {
	uint8_t	buffer[UC_USART_RX_BUFFER_SIZE];
	uint8_t index;
} UC_USART_RX_Databuffer;

typedef struct {
	const char* pData;
	uint32_t NumDataToSend;
	uint32_t dataSent;
} UC_USART_TX_Databuffer;



/***********************************************************
					private variables
***********************************************************/
UC_USART_RX_Databuffer rx_buffer;
UC_USART_TX_Databuffer tx_buffer;



/***********************************************************
				private function declarations
***********************************************************/



/***********************************************************
				public function definitions
***********************************************************/
extern void UC_USART_Init(void)
{
	//Enable Interrupts
	NVIC_SetPriority(UC_USART_IRQ, UC_USART_IRQ_PRIO);
	NVIC_EnableIRQ(UC_USART_IRQ);

	// Baudrate Settings BRR = APB1Clock/(16*Baudrate)
	uint16_t mant = (uint16_t)(UC_USART_APB_CLOCK/(UC_USART_BAUDRATE*16)) << 4;
	uint8_t frac = ((UC_USART_APB_CLOCK << 4) / ((UC_USART_BAUDRATE*16))) & 0xF;
	UC_USART->BRR |= (mant | frac);

	//Configure UART
	CLEAR_REG(UC_USART->CR2);
	CLEAR_REG(UC_USART->CR3);
	CLEAR_REG(UC_USART->CR1);
	UC_USART->CR1 |= (USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_TE | USART_CR1_SBK);
	UC_USART->CR1 |= USART_CR1_UE; // Enable USART

	OS_Delay(100);

	// GPIO Settings
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

	return;
}

extern void UC_USART_sendString(const char * str, uint32_t size)
{
	tx_buffer.NumDataToSend = size;
	tx_buffer.pData = str;
	tx_buffer.dataSent = 0;
	UC_USART->CR1 |= USART_CR1_TXEIE;
	return;
}

void USART_ISR(void)
{
	if(READ_BIT(UC_USART->SR, USART_SR_RXNE)) // if RX Data is received
	{
		CLEAR_BIT(UC_USART->SR, USART_SR_RXNE); // Clear Interrupt Flag
		rx_buffer.buffer[rx_buffer.index] = UC_USART->DR;
		rx_buffer.index++;
		SET_BIT(UC_USART->CR1, USART_CR1_IDLEIE); // Enable Idle Interrupt
	}
	if(READ_BIT(UC_USART->SR, USART_SR_TXE)) // if transmission complete flag is set
	{
		CLEAR_BIT(UC_USART->SR, 1 << 6); // Clear Interrupt Flag

		UC_USART->DR = tx_buffer.pData[tx_buffer.dataSent];
		tx_buffer.dataSent++;
		if(tx_buffer.dataSent >= tx_buffer.NumDataToSend)
		{
			UC_USART->CR1 &= ~USART_CR1_TXEIE; // Disable TXE Interrupt if all data is tranmitted
		}
	if(READ_BIT(UC_USART->SR, USART_SR_IDLE))
	{
		// all data is received
		rx_buffer.index = 0;
		CLEAR_BIT(UC_USART->CR1, USART_CR1_IDLEIE); // Disable Idle Interrupt
	}

	}
	return;
}

#ifdef UC_USART_USE_PRINTF
#if UC_USART_USE_PRINTF == 1

#endif
#endif

/***********************************************************
				private function definitions
***********************************************************/
