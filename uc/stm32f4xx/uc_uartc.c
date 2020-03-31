/*
 * uc_uartc.c
 *
 *  Created on: 08.02.2020
 *      Author: Florian
 */

#include "uc_gpio.h"
#include "uc_usart_config.h"
#include "RTOS.h"

uint32_t 	numDataToSend;
const char*	pTxDataToSend;

bool		transmitEnable;
uint8_t 	rxBuffer[100];


extern void UC_USART_Init(void)
{
	//Enable Interrupts
	NVIC_SetPriority(USART_IRQ, USART_IRQ_PRIO);
	NVIC_EnableIRQ(USART_IRQ);

	NVIC_SetPriority(USART_DMA_TX_STREAM_IRQ, USART_DMA_STREAM_IRQ_PRIO_TX);
	NVIC_EnableIRQ(USART_DMA_TX_STREAM_IRQ);

	NVIC_SetPriority(USART_DMA_RX_STREAM_IRQ, USART_DMA_STREAM_IRQ_PRIO_RX);
	NVIC_EnableIRQ(USART_DMA_RX_STREAM_IRQ);

	// Baudrate Settings BRR = APB1Clock/(16*Baudrate)
	uint16_t mant = (uint16_t)(APB_CLOCK/(USART_BAUDRATE*16)) << 4;
	uint8_t frac = ((APB_CLOCK << 4) / ((USART_BAUDRATE*16))) & 0xF;
	USART->BRR |= (mant | frac);

	//Configure UART
	CLEAR_REG(USART->CR2);
	CLEAR_REG(USART->CR3);
	CLEAR_REG(USART->CR1);
	USART->CR1 |= (USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_TE | USART_CR1_SBK);
	USART->CR1 |= USART_CR1_UE; // Enable USART

	OS_Delay(100);

	// GPIO Settings
	//TX
	UC_GPIO_setPullup(UART_TX_PORT, UART_TX_PIN);
	UC_GPIO_setAlternateFunction(UART_TX_PORT, UART_TX_PIN, UART_AF);
	UC_GPIO_setModeAlternateFunction(UART_TX_PORT, UART_TX_PIN);
	UC_GPIO_setOutputTypePushPull(UART_TX_PORT, UART_TX_PIN);
	UC_GPIO_setSpeedVeryHigh(UART_TX_PORT, UART_TX_PIN);
	//RX
	UC_GPIO_setModeInput(UART_RX_PORT, UART_RX_PIN);
	UC_GPIO_setPullup(UART_RX_PORT, UART_RX_PIN);
	UC_GPIO_setSpeedVeryHigh(UART_RX_PORT, UART_RX_PIN);
	UC_GPIO_setAlternateFunction(UART_RX_PORT, UART_RX_PIN, UART_AF);
}

extern void UC_USART_sendString(const char * str, uint32_t size)
{
	numDataToSend = size;
	pTxDataToSend = str;
	OS_Delay(100);
	USART->CR1 |= USART_CR1_TXEIE;
	return;
}

void USART_ISR(void)
{
	if(READ_BIT(USART->SR, USART_SR_RXNE)) // if RX Data is received
	{
		SET_BIT(USART_DMA_RX->CR, 1 << 0); // Enable RX DMA
		SET_BIT(USART_DMA_RX->CR, 1 << 4); // Enable RX DMA Transfer Complete Interrupt
	}
	if(READ_BIT(USART->SR, USART_SR_TXE)) // if transmission complete flag is set
	{
		static uint8_t sent = 0;
		CLEAR_BIT(USART->SR, 1 << 6); // Clear Interrupt Flag
		USART->DR = *pTxDataToSend;
		pTxDataToSend++;
		sent++;
		if(sent >= numDataToSend)
		{
			sent = 0;
			USART->CR1 &= ~USART_CR1_TXEIE; // Disable TXE Interrupt if all data is tranmitted
		}

	}
	return;
}
