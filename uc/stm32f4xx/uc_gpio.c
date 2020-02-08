/*
 * uc_gpio.c
 *
 *  Created on: 11.01.2020
 *      Author: Florian
 */


#include <uc_gpio.h>
#include <stdint.h>


extern void UC_GPIO_setOutputPushPull(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	SET_BIT(GPIOx->MODER, 1 << (2*pin));
	CLEAR_BIT(GPIOx->MODER, 1 << ((2*pin) + 1));
	CLEAR_BIT(GPIOx->OTYPER, 1 << ((2*pin) + 1));
	CLEAR_BIT(GPIOx->OTYPER, 1 << pin);
	return;
}

extern void UC_GPIO_setOutputPushPullAlternateFunction(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	CLEAR_BIT(GPIOx->MODER, 1 << (2*pin));
	SET_BIT(GPIOx->MODER, 1 << ((2*pin) + 1));

	CLEAR_BIT(GPIOx->OTYPER, 1 << pin);
	return;
}

extern void UC_GPIO_setAlternateFunction(GPIO_TypeDef* GPIOx, uint8_t pin, uint8_t val)
{
	if(pin > 7)
	{
		MODIFY_REG(GPIOx->AFR[1], (0xF << (pin * 4)), ((val & 0xF) << (pin * 4)));
	}
	else
	{
		MODIFY_REG(GPIOx->AFR[0], (0xF << (pin * 4)), ((val & 0xF) << (pin * 4)));
	}

}

extern void UC_GPIO_setInputFloating(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	CLEAR_BIT(GPIOx->MODER, 1 << (2*pin));
	CLEAR_BIT(GPIOx->MODER, 1 << ((2*pin) + 1));
	CLEAR_BIT(GPIOx->PUPDR, 1 << (2*pin));
	CLEAR_BIT(GPIOx->PUPDR, 1 << ((2*pin) + 1));

}

extern void UC_GPIO_setSpeedLow(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	SET_BIT(GPIOx->OSPEEDR, 1 << (2*pin));
	CLEAR_BIT(GPIOx->OSPEEDR, 1 << ((2*pin) + 1));
	return;
}

extern void UC_GPIO_setSpeedMiddle(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	CLEAR_BIT(GPIOx->OSPEEDR, 1 << (2*pin));
	CLEAR_BIT(GPIOx->OSPEEDR, 1 << ((2*pin) + 1));
	return;
}

extern void UC_GPIO_setSpeedHigh(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	CLEAR_BIT(GPIOx->OSPEEDR, 1 << (2*pin));
	SET_BIT(GPIOx->OSPEEDR, 1 << ((2*pin) + 1));
	return;
}

extern void UC_GPIO_setPin(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	SET_BIT(GPIOx->ODR, 1 << pin);
	return;
}

extern void UC_GPIO_clearPin(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	CLEAR_BIT(GPIOx->ODR, 1 << pin);
	return;
}

extern void UC_GPIO_writeVal(GPIO_TypeDef* GPIOx, uint8_t pin, bool val)
{
	if(val)
		UC_GPIO_setPin(GPIOx, pin);
	else
		UC_GPIO_clearPin(GPIOx, pin);

	return;
}
