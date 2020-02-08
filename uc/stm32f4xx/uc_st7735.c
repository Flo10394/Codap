/*
 * uc_st7735.c
 *
 *  Created on: 11.01.2020
 *      Author: Florian
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stm32f10x.h>
#include <uc_gpio.h>
#include <uc_st7735.h>
#include <uc_st7735_config.h>
#include <uc_delay.h>


/*******************************
 * 	private variables
 *******************************/

static uint16_t spiTxCounter = 0;
static uint8_t spiDataBuffer[SPI_TX_BUFFER_SIZE];
static uint8_t busyflag = 0;

/*******************************
 * 	private functions
 *******************************/
static void UC_ST7735_SpiInit(void);
static void UC_ST7735_SendSPI(uint16_t data);
static void UC_ST7735_EnableTxInterrupt(void);
static void UC_ST7735_DisableTxInterrupt(void);
static bool UC_ST7735_GetSpiRxFlag(void);
static bool UC_ST7735_GetSpiTxFlag(void);
static bool UC_ST7735_GetSpiBusyFlag(void);
static void UC_ST7735_DisableErrInterrupt(void);

static void UC_ST7735_EnableTxInterrupt(void)
{
	SET_BIT(UC_ST7735_SPI->CR2,  (1u << 7));
}

static void UC_ST7735_DisableTxInterrupt(void)
{
	CLEAR_BIT(UC_ST7735_SPI->CR2,  (1u << 7));
}

static void UC_ST7735_DisableErrInterrupt(void)
{
	CLEAR_BIT(UC_ST7735_SPI->CR2,  (1u << 5));
}

static bool UC_ST7735_GetSpiRxFlag(void)
{
	return READ_BIT(UC_ST7735_SPI->SR, 1 << 0);
}

static bool UC_ST7735_GetSpiTxFlag(void)
{
	return (READ_BIT(UC_ST7735_SPI->SR, 1 << 1) >> 1);
}

static bool UC_ST7735_GetSpiBusyFlag(void)
{
	return (READ_BIT(UC_ST7735_SPI->SR, 1 << 7) >> 7);
}

 static void UC_ST7735_SpiInit(void)
{
	//TODO: Set gpio alternate functions
	AFIO->MAPR &= ~0x00000001; // SPI Pins A4-A7

	UC_GPIO_setOutputSpeedMiddle(UC_ST7735_SCK_PORT, UC_ST7735_SCK_PIN);
	UC_GPIO_setOutputPushPullAlternateFunction(UC_ST7735_SCK_PORT, UC_ST7735_SCK_PIN);
	UC_GPIO_setOutputSpeedMiddle(UC_ST7735_MOSI_PORT, UC_ST7735_MOSI_PIN);
	UC_GPIO_setOutputPushPullAlternateFunction(UC_ST7735_MOSI_PORT, UC_ST7735_MOSI_PIN);


	CLEAR_REG(UC_ST7735_SPI->CR1);
	UC_ST7735_SPI->CR1 |= ( (1u << 2u) | UC_ST7735_BAUD_DIV | (1u << 14u) | (1u << 15u) );

	MODIFY_REG(UC_ST7735_SPI->CR2, 0xFFFF, (0x0007 << 5)); //Enable Interrupts

	SET_BIT(UC_ST7735_SPI->CR1, (1u << 6u));// Enable SPI (Bit 6)

	return;
}

static void UC_ST7735_SendSPI(uint16_t data)
{
	while(UC_ST7735_GetSpiBusyFlag()){}
	while(busyflag == 1){}
	spiTxCounter = 1;
//	spiDataBuffer[0] = ((data >> 8) & 0xFF);
//	spiDataBuffer[1] = (data & 0xFF);
	spiDataBuffer[0] = (data & 0xFF);
	UC_GPIO_ClearPin(UC_ST7735_CS_PORT, UC_ST7735_CS_PIN);
	UC_ST7735_EnableTxInterrupt();
	return;
}

/*******************************
 * 	public functions
 *******************************/
extern void UC_ST7735_init(void)
{
	// INIT DC PIN
	UC_GPIO_setOutputSpeedLow(UC_ST7735_DC_PORT, UC_ST7735_DC_PIN);
	UC_GPIO_setOutputPushPull(UC_ST7735_DC_PORT, UC_ST7735_DC_PIN);
	UC_GPIO_setOutputSpeedLow(UC_ST7735_CS_PORT, UC_ST7735_CS_PIN);
	UC_GPIO_setOutputPushPull(UC_ST7735_CS_PORT, UC_ST7735_CS_PIN);
	UC_GPIO_setOutputSpeedLow(UC_ST7735_RST_PORT, UC_ST7735_RST_PIN);
	UC_GPIO_setOutputPushPull(UC_ST7735_RST_PORT, UC_ST7735_RST_PIN);
	UC_GPIO_SetPin(UC_ST7735_CS_PORT, UC_ST7735_CS_PIN);
	UC_ST7735_SpiInit();

	// Reset Device
	UC_GPIO_ClearPin(UC_ST7735_RST_PORT, UC_ST7735_RST_PIN);
	UC_DELAY_ms(500);
	UC_GPIO_SetPin(UC_ST7735_RST_PORT, UC_ST7735_RST_PIN);

	UC_ST7735_DisableErrInterrupt();
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	return;
}

extern void UC_ST7735_sendCommand(UC_ST7735_COMMAND_t command)
{
	UC_GPIO_SetPin(UC_ST7735_DC_PORT, UC_ST7735_DC_PIN);
	UC_GPIO_ClearPin(UC_ST7735_DC_PORT, UC_ST7735_DC_PIN);
	UC_ST7735_SendSPI((uint16_t)command);
	return;
}

extern void UC_ST7735_sendData(uint8_t data)
{
	UC_GPIO_SetPin(UC_ST7735_DC_PORT, UC_ST7735_DC_PIN);
	UC_ST7735_SendSPI((uint16_t)data);
	return;
}

void SPI1_IRQHandler(void){
		if(UC_ST7735_GetSpiTxFlag()) // Transmit Buffer Empty
		{
			if(spiTxCounter != 0)
			{
				UC_ST7735_SPI->DR = spiDataBuffer[spiTxCounter-1];
				spiTxCounter--;
			}
			else
			{
			  busyflag = 0;
			  UC_ST7735_DisableTxInterrupt();
			}
		}
	return;
}
