/***********************************************************
	file: 				uc_spi.c
	
	file description: 	TODO: add your file description

	author:				Florian
	created on:			04.04.2020
***********************************************************/


/***********************************************************
						includes
***********************************************************/
#include <uc_spi.h>
#include <uc_spi_config.h>
#include <uc_gpio.h>
#include <RTOS.h>



/***********************************************************
					configuration options
***********************************************************/



/***********************************************************
					private definitions
***********************************************************/



/***********************************************************
					   private types
***********************************************************/



/***********************************************************
					private variables
***********************************************************/
uint8_t dataToSend = 0;


/***********************************************************
				private function declarations
***********************************************************/



/***********************************************************
				public function definitions
***********************************************************/
extern void UC_SPI_init(void)
{
	/*
	 * Enable Interrupts
	 */
	NVIC_SetPriority(UC_SPI_IRQ ,UC_SPI_IRQ_PRIO);
	NVIC_EnableIRQ(UC_SPI_IRQ);

	/*
	 * Init GPIOs
	 */
	UC_GPIO_setModeAlternateFunction(UC_SPI_SCK_PORT, UC_SPI_SCK_PIN);
	UC_GPIO_setOutputTypePushPull(UC_SPI_SCK_PORT, UC_SPI_SCK_PIN);
	UC_GPIO_setSpeedHigh(UC_SPI_SCK_PORT, UC_SPI_SCK_PIN);
	UC_GPIO_setAlternateFunction(UC_SPI_SCK_PORT, UC_SPI_SCK_PIN, UC_SPI_AF);

	UC_GPIO_setModeAlternateFunction(UC_SPI_MOSI_PORT, UC_SPI_MOSI_PIN);
	UC_GPIO_setOutputTypePushPull(UC_SPI_MOSI_PORT, UC_SPI_MOSI_PIN);
	UC_GPIO_setSpeedHigh(UC_SPI_MOSI_PORT, UC_SPI_MOSI_PIN);
	UC_GPIO_setAlternateFunction(UC_SPI_MOSI_PORT, UC_SPI_MOSI_PIN, UC_SPI_AF);

	UC_GPIO_setModeAlternateFunction(UC_SPI_MISO_PORT, UC_SPI_MISO_PIN);
	UC_GPIO_setPullup(UC_SPI_MISO_PORT, UC_SPI_MISO_PIN);
	UC_GPIO_setSpeedHigh(UC_SPI_MISO_PORT, UC_SPI_MISO_PIN);
	UC_GPIO_setAlternateFunction(UC_SPI_MISO_PORT, UC_SPI_MISO_PIN, UC_SPI_AF);

	UC_GPIO_setModeOutput(UC_SPI_CS_PORT, UC_SPI_CS_PIN);
	UC_GPIO_setOutputTypePushPull(UC_SPI_CS_PORT, UC_SPI_CS_PIN);
	UC_GPIO_setSpeedHigh(UC_SPI_CS_PORT, UC_SPI_CS_PIN);
	UC_GPIO_setPullup(UC_SPI_CS_PORT, UC_SPI_CS_PIN);
	UC_GPIO_setPin(UC_SPI_CS_PORT, UC_SPI_CS_PIN);

	/*
	 * Init SPI
	 */
	MODIFY_REG(UC_SPI->CR1, ~UC_SPI_CR1, UC_SPI_CR1);
	MODIFY_REG(UC_SPI->CR2, ~UC_SPI_CR2, UC_SPI_CR2);
	OS_Delay(100);
	SET_BIT(UC_SPI->CR1, SPI_CR1_SPE);
	return;
}

extern void UC_SPI_sendByte(uint8_t data)
{
	dataToSend = data;
	SET_BIT(UC_SPI->CR2, SPI_CR2_TXEIE); // Enable TXE Interrupt
	return;
}

/***********************************************************
				private function definitions
***********************************************************/
void UC_SPI_ISR(void)
{
	OS_EnterNestableInterrupt();
	if(READ_BIT(UC_SPI->SR, SPI_SR_RXNE) && READ_BIT(UC_SPI->CR2, SPI_CR2_RXNEIE))
	{
		CLEAR_BIT(UC_SPI->SR, SPI_SR_RXNE); // Clear interrupt flag
		UC_GPIO_setPin(UC_SPI_CS_PORT, UC_SPI_CS_PIN); // Set CS high
		//empty data register
		(void)UC_SPI->DR;
	}
	if(READ_BIT(UC_SPI->SR, SPI_SR_TXE) && READ_BIT(UC_SPI->CR2, SPI_CR2_TXEIE)) // if tx buffer empty
	{
		CLEAR_BIT(UC_SPI->SR, SPI_SR_TXE); // Clear interrupt flag
		CLEAR_BIT(UC_SPI->CR2, SPI_CR2_TXEIE); // disable txe interrupt
		UC_GPIO_clearPin(UC_SPI_CS_PORT, UC_SPI_CS_PIN); // Set CS low
		UC_SPI->DR = dataToSend;
	}

	OS_LeaveNestableInterrupt();
}


