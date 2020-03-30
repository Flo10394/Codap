/*
 * uc_uartc.c
 *
 *  Created on: 08.02.2020
 *      Author: Florian
 */

#include "uc_gpio.h"
#include "uc_usart_config.h"

uint8_t rxBuffer[100];

extern void UC_USART_Init(void)
{
	//Enable Interrupts
	NVIC_SetPriority(USART_IRQ, USART_IRQ_PRIO);
	NVIC_EnableIRQ(USART_IRQ);
	NVIC_SetPriority(USART_DMA_TX_STREAM_IRQ, USART_DMA_STREAM_IRQ_PRIO_TX);
	NVIC_EnableIRQ(USART_DMA_TX_STREAM_IRQ);
	NVIC_SetPriority(USART_DMA_RX_CHANNEL_IRQ, USART_DMA_STREAM_IRQ_PRIO_RX);
	NVIC_EnableIRQ(USART_DMA_RX_CHANNEL_IRQ);

	// GPIO Settings
	//TX
	UC_GPIO_setModeAlternateFunction(UART_TX_PORT, UART_TX_PIN);
	UC_GPIO_setOutputTypePushPull(UART_TX_PORT, UART_TX_PIN);
	UC_GPIO_setSpeedVeryHigh(UART_TX_PORT, UART_TX_PIN);
	UC_GPIO_setAlternateFunction(UART_TX_PORT, UART_TX_PIN, UART_AF);
	//RX
	UC_GPIO_setModeInput(UART_RX_PORT, UART_RX_PIN);
	UC_GPIO_setPullup(UART_RX_PORT, UART_RX_PIN);
	UC_GPIO_setSpeedVeryHigh(UART_RX_PORT, UART_RX_PIN);
	UC_GPIO_setAlternateFunction(UART_RX_PORT, UART_RX_PIN, UART_AF);

	// Baudrate Settings BRR = APB1Clock/(16*Baudrate)
	uint16_t mant = (uint16_t)(APB_CLOCK/(USART_BAUDRATE*16)) << 4;
	uint8_t frac = ((APB_CLOCK << 4) / ((USART_BAUDRATE*16))) & 0xF;
	USART->BRR |= (mant | frac);

	//Configure UART
	CLEAR_REG(USART->CR1);
	USART->CR1 |= (USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_SBK);
	CLEAR_REG(USART->CR3);
	USART->CR3 |= (USART_CR3_DMAT | USART_CR3_DMAR);

	//Configure TX DMA
	CLEAR_REG(USART_DMA_TX->CR);
	USART_DMA_TX->CR |= ((USART_DMA_TX_CHANNEL << 25) | DMA_SxCR_PL_1 | DMA_SxCR_MINC | DMA_SxCR_DIR_0 | DMA_SxCR_TCIE);
	CLEAR_REG(USART_DMA_TX->PAR);
	USART_DMA_TX->PAR = (uint32_t)(&USART->DR); // Set Destination Address (USART Data Register)

	//Configure RX DMA
	CLEAR_REG(USART_DMA_RX->CR);
	USART_DMA_RX->CR |= ((USART_DMA_RX_CHANNEL << 25) | DMA_SxCR_PL_1 | DMA_SxCR_MINC | DMA_SxCR_PFCTRL);
	CLEAR_REG(USART_DMA_RX->PAR);
	USART_DMA_RX->PAR |= (uint32_t)(&USART->DR); //This Address is fixed
	CLEAR_REG(USART_DMA_RX->M0AR);
	USART_DMA_RX->M0AR |= (uint32_t)(&rxBuffer);

	USART->CR1 |= USART_CR1_UE; // Enable USART

}

extern void UC_USART_sendString(const char * str, uint32_t size)
{
	CLEAR_BIT(USART_DMA_TX->CR, 1 << 0); // Disable DMA

	USART->CR1 &= ~USART_CR1_RE;
	USART->CR1 |= USART_CR1_TE;

	USART_DMA_TX->NDTR = (uint32_t)size; // set amount of data to transmit
	USART_DMA_TX->M0AR = (uintptr_t)str;

	SET_BIT(USART_DMA_TX->CR, 1 << 4); //Enable DMA Transfer complete Interrupt
	SET_BIT(USART_DMA_TX->CR, 1 << 0); // Enable DMA

	return;
}


void USART_DMA_TX_ISR(void)
{
	if(READ_BIT(USART_DMA_TX_TCIF_REG, USART_DMA_TX_TCIF_BITMASK)) // If transfer complete interrupt
	{
		CLEAR_BIT(USART_DMA_TX->CR, 1 << 0); // Disable DMA
		CLEAR_BIT(USART_DMA_TX->CR, 1 << 4); // Disble DMA Transfer complete Interrupt
		SET_BIT(USART_DMA_TX_CTCIF_REG, USART_DMA_TX_CTCIF_BITMASK); // Clear interrupt flag
		SET_BIT(USART->CR1, 1 << 6); // Enable USART transmission complete interrupt
	}
	return;
}


void USART_DMA_RX_ISR(void)
{
	if(READ_BIT(USART_DMA_RX_TCIF_REG, USART_DMA_RX_TCIF_BITMASK)) // If transfer complete interrupt
	{
		// MAYBE: ADD THE BUFFER SIZE TO BE TRANSFERED!
		CLEAR_BIT(USART_DMA_RX->CR, 1 << 0); // Disable DMA
		CLEAR_BIT(USART_DMA_RX->CR, 1 << 4); // Disble DMA Transfer complete Interrupt
		SET_BIT(USART_DMA_RX_CTCIF_REG, USART_DMA_RX_CTCIF_BITMASK); // Clear interrupt flag

		// Receive complete
	}

	return;
}

void USART_ISR(void)
{
	if(READ_BIT(USART1->SR, 1 << 5)) // if RX Data is received
	{
		SET_BIT(USART_DMA_RX->CR, 1 << 0); // Enable RX DMA
		SET_BIT(USART_DMA_RX->CR, 1 << 4); // Enable RX DMA Transfer Complete Interrupt
	}
	if(READ_BIT(USART->SR, 1 << 6)) // if transmission complete flag is set
	{
		USART->CR1 &= ~USART_CR1_TE;
		USART->CR1 |= USART_CR1_RE;
		CLEAR_BIT(USART->SR, 1 << 6); // Clear Interrupt Flag
		CLEAR_BIT(USART->CR1, 1 << 6); // Disable transmission complete interrupt
	}
	return;
}
