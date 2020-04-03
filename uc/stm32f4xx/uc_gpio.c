/*
 * uc_gpio.c
 *
 *  Created on: 11.01.2020
 *      Author: Florian
 */


#include <uc_gpio.h>
#include <stdint.h>

/* MODE */
extern void UC_GPIO_setModeInput(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	MODIFY_REG(GPIOx->MODER, 0x3u << 2*pin, 0);
}

extern void UC_GPIO_setModeOutput(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	MODIFY_REG(GPIOx->MODER, 0x3u << 2*pin, 0x1u << 2*pin);
}

extern void UC_GPIO_setModeAlternateFunction(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	MODIFY_REG(GPIOx->MODER, 0x3u << 2*pin, 0x2u << 2*pin);
}

extern void UC_GPIO_setModeAnalog(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	MODIFY_REG(GPIOx->MODER, 0x3u << 2*pin, 0x3u << 2*pin);
}


/* OUTPUT TYPE */
extern void UC_GPIO_setOutputTypePushPull(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	MODIFY_REG(GPIOx->OTYPER, 0x1u << pin, 0);
}

extern void UC_GPIO_setOutputTypeOpenDrain(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	MODIFY_REG(GPIOx->OTYPER, 0x1u << pin, 0x1u << pin);
}


/* SPEED */
extern void UC_GPIO_setSpeedLow(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	MODIFY_REG(GPIOx->OSPEEDR, 0x3u << 2*pin, 0);
}

extern void UC_GPIO_setSpeedMiddle(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	MODIFY_REG(GPIOx->OSPEEDR, 0x3u << 2*pin, 0x1u << 2*pin);
}

extern void UC_GPIO_setSpeedHigh(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	MODIFY_REG(GPIOx->OSPEEDR, 0x3u << 2*pin, 0x2u << 2*pin);
}

extern void UC_GPIO_setSpeedVeryHigh(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	MODIFY_REG(GPIOx->OSPEEDR, 0x3u << 2*pin, 0x3u << 2*pin);
}


/* Pullup/Pulldown */
extern void UC_GPIO_setFloating(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	MODIFY_REG(GPIOx->PUPDR, 0x3u << 2*pin, 0);
}

extern void UC_GPIO_setPullup(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	MODIFY_REG(GPIOx->PUPDR, 0x3u << 2*pin, 0x1u << 2*pin);
}

extern void UC_GPIO_setPulldown(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	MODIFY_REG(GPIOx->PUPDR, 0x3u << 2*pin, 0x2 << 2*pin);
}


/* Alternate Function */
extern void UC_GPIO_setAlternateFunction(GPIO_TypeDef* GPIOx, uint8_t pin, uint8_t value)
{
	if(pin > 7) // AFRH (Pin 8 - 15)
	{
		MODIFY_REG(GPIOx->AFR[1], 0xFu << 4*(pin-8), (value & 0xFu) << 4*(pin-8));
	}
	else // AFRL (Pin 0 - 7)
	{
		MODIFY_REG(GPIOx->AFR[0], 0xFu << 4*(pin), (value & 0xFu) << 4*(pin));
	}

}


/* Set/Reset */
extern void UC_GPIO_setPin(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	SET_BIT(GPIOx->BSRRL, 0x1u << pin); // BSRRL = Set Register
}

extern void UC_GPIO_clearPin(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	SET_BIT(GPIOx->BSRRH, 0x1u << pin); // BSRRH = Reset Register
}

extern void UC_GPIO_writeVal(GPIO_TypeDef* GPIOx, uint8_t pin, bool val)
{
	if(val) // Set Pin
	{
		UC_GPIO_setPin(GPIOx, pin);
	}
	else // Reset Pin
	{
		UC_GPIO_clearPin(GPIOx, pin);
	}
}


//Unit Test
extern void UC_GPIO_Unit_Test(void)
{
	#define UC_GPIO_UNIT_TEST_GPIO_BASE		GPIOC
	#define UC_GPIO_UNIT_TEST_GPIO_PIN		13

	/* High Pin */
	UC_GPIO_setModeInput(UC_GPIO_UNIT_TEST_GPIO_BASE, UC_GPIO_UNIT_TEST_GPIO_PIN);
	UC_GPIO_setModeOutput(UC_GPIO_UNIT_TEST_GPIO_BASE, UC_GPIO_UNIT_TEST_GPIO_PIN);
	UC_GPIO_setModeAlternateFunction(UC_GPIO_UNIT_TEST_GPIO_BASE, UC_GPIO_UNIT_TEST_GPIO_PIN);
	UC_GPIO_setModeAnalog(UC_GPIO_UNIT_TEST_GPIO_BASE, UC_GPIO_UNIT_TEST_GPIO_PIN);

	UC_GPIO_setOutputTypePushPull(UC_GPIO_UNIT_TEST_GPIO_BASE, UC_GPIO_UNIT_TEST_GPIO_PIN);
	UC_GPIO_setOutputTypeOpenDrain(UC_GPIO_UNIT_TEST_GPIO_BASE, UC_GPIO_UNIT_TEST_GPIO_PIN);

	UC_GPIO_setSpeedLow(UC_GPIO_UNIT_TEST_GPIO_BASE, UC_GPIO_UNIT_TEST_GPIO_PIN);
	UC_GPIO_setSpeedMiddle(UC_GPIO_UNIT_TEST_GPIO_BASE, UC_GPIO_UNIT_TEST_GPIO_PIN);
	UC_GPIO_setSpeedHigh(UC_GPIO_UNIT_TEST_GPIO_BASE, UC_GPIO_UNIT_TEST_GPIO_PIN);
	UC_GPIO_setSpeedVeryHigh(UC_GPIO_UNIT_TEST_GPIO_BASE, UC_GPIO_UNIT_TEST_GPIO_PIN);

	UC_GPIO_setFloating(UC_GPIO_UNIT_TEST_GPIO_BASE, UC_GPIO_UNIT_TEST_GPIO_PIN);
	UC_GPIO_setPullup(UC_GPIO_UNIT_TEST_GPIO_BASE, UC_GPIO_UNIT_TEST_GPIO_PIN);
	UC_GPIO_setPulldown(UC_GPIO_UNIT_TEST_GPIO_BASE, UC_GPIO_UNIT_TEST_GPIO_PIN);

	UC_GPIO_setAlternateFunction(UC_GPIO_UNIT_TEST_GPIO_BASE, UC_GPIO_UNIT_TEST_GPIO_PIN, 12);

	UC_GPIO_setPin(UC_GPIO_UNIT_TEST_GPIO_BASE, UC_GPIO_UNIT_TEST_GPIO_PIN);
	UC_GPIO_clearPin(UC_GPIO_UNIT_TEST_GPIO_BASE, UC_GPIO_UNIT_TEST_GPIO_PIN);
	UC_GPIO_writeVal(UC_GPIO_UNIT_TEST_GPIO_BASE, UC_GPIO_UNIT_TEST_GPIO_PIN, 1);
}


