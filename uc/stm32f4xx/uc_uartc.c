/*
 * uc_uartc.c
 *
 *  Created on: 08.02.2020
 *      Author: Florian
 */

#include "uc_gpio.h"
#include "uc_usart_config.h"

//
//void usart_send_string(const char * data)
//{
//	while(*data) // as long as no \0 char
//	{
//		if(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == SET)
//		{
//			USART_SendData(USART1, *data);
//			data++;
//		}
//	}
//}
//
//uint8_t SendUartDma(const char * str, uint32_t size);


extern void UC_USART_Init(void)
{
	//Enable Interrupts
	NVIC_SetPriority(USART_IRQ, USART_IRQ_PRIO);
	NVIC_EnableIRQ(USART_IRQ);
	NVIC_SetPriority(DMA_TX_CHANNEL_IRQ, DMA_CHANNEL_IRQ_PRIO);
	NVIC_EnableIRQ(DMA_TX_CHANNEL_IRQ);
	NVIC_SetPriority(DMA_RX_CHANNEL_IRQ, DMA_CHANNEL_IRQ_PRIO);
	NVIC_EnableIRQ(DMA_RX_CHANNEL_IRQ);

	UC_GPIO_setOutputPushPullAlternateFunction(DEBUG_UART_TX_PORT, DEBUG_UART_TX_PIN);
	UC_GPIO_setSpeedHigh(DEBUG_UART_TX_PORT, DEBUG_UART_TX_PIN);
	UC_GPIO_setAlternateFunction(DEBUG_UART_TX_PORT, DEBUG_UART_TX_PIN, DEBUG_UART_AF);

	UC_GPIO_setInputFloating(DEBUG_UART_RX_PORT, DEBUG_UART_RX_PIN);
	UC_GPIO_setSpeedHigh(DEBUG_UART_RX_PORT, DEBUG_UART_RX_PIN);
	UC_GPIO_setAlternateFunction(DEBUG_UART_RX_PORT, DEBUG_UART_RX_PIN, DEBUG_UART_AF);

	uint16_t mant = (uint16_t)(APB1_CLOCK/USART_BAUDRATE) << 4;

	uint8_t frac = ((APB1_CLOCK << 4) / (USART_BAUDRATE)) & 0xF;
	USART->BRR |= (mant | frac);

//
//	USART_InitStructure.USART_BaudRate = 115200;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//
//	USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//
//	USART_Init(USART1, &USART_InitStructure);
//
//	/*
//	 * TX DMA
//	 */
//	DMA_InitTypeDef DMA_InitStructure;
//	DMA_InitStructure.DMA_BufferSize = DMA_TX_BUFFER_SIZE;
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
//	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//	DMA_InitStructure.DMA_MemoryBaseAddr = 0;
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
//	DMA_InitStructure.DMA_PeripheralBaseAddr = USART1->DR;
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//	DMA_Init(DMA1_Channel4, &DMA_InitStructure);

}
//	USART_Cmd(USART1, ENABLE);
//	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
//
//	uint32_t i = 0;
//	const char str[10];
//	while(1)
//	{
//		sprintf(&str, "Hello %lu", i);
//		SendUartDma(str, 8);
//		i++;
//		UC_DELAY_s(1);
//	}
//}



//uint8_t SendUartDma(const char * str, uint32_t size)
//{
//	CLEAR_BIT(DMA1_Channel4->CCR, (1 << 0)); //Disable DMA
//
//	DMA1_Channel4->CNDTR = (uint32_t)size; // transmit this amount of data
//	DMA1_Channel4->CMAR = (uintptr_t)str;
//	DMA1_Channel4->CPAR = (uint32_t)&USART1->DR;
//
//	SET_BIT(DMA1_Channel4->CCR, 1 << 1); //Enable Transfer complete Interrupt
//	SET_BIT(DMA1_Channel4->CCR, (1 << 0)); //Enable DMA
//
//	return 0;
//}
//
//void DMA1_Channel4_IRQHandler(void)
//{
//	if(READ_BIT(DMA1->ISR, 1 << 13) ) // If Channel 4 transfer complete
//	{
//		CLEAR_BIT(DMA1_Channel4->CCR, (1 << 0)); 	//Disable DMA
//		CLEAR_BIT(DMA1_Channel4->CCR, 1 << 1); 		//Disable Transfer complete Interrupt
//		SET_BIT(DMA1->IFCR , 1 << 13 );  			//Clear Transfer Complete Interrut flag
//		SET_BIT(DMA1->IFCR , 1 << 14 );  			//Clear Transfer Half Complete Interrut flag
//		SET_BIT(DMA1->IFCR , 1 << 12 );  			//Clear Global Interrut flag
//		SET_BIT(USART1->CR1, 1 << 6); 				// Enable transmission complete interrupt
//	}
//	return;
//}
//
//void USART1_IRQHandler(void)
//{
//	if(READ_BIT(USART1->SR, 1 << 5)) // if RX Data is received
//	{
//
//	}
//	else if(READ_BIT(USART1->SR, 1 << 6)) // if transmission complete flag is set
//	{
//		// Do some data sent finished handling
//
//		CLEAR_BIT(USART1->SR, 1 << 6);
//		CLEAR_BIT(USART1->CR1, 1 << 6); // Disable transmission complete interrupt
//	}
//
//	return;
//}
