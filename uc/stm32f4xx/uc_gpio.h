/*
 * uc_gpio.h
 *
 *  Created on: 11.01.2020
 *      Author: Florian
 */

#ifndef INCLUDE_UC_GPIO_H_
#define INCLUDE_UC_GPIO_H_

#include <stm32f4xx.h>
#include <stdbool.h>

//Init

extern void UC_GPIO_setModeInput(GPIO_TypeDef* GPIOx, uint8_t pin);
extern void UC_GPIO_setModeOutput(GPIO_TypeDef* GPIOx, uint8_t pin);
extern void UC_GPIO_setModeAlternateFunction(GPIO_TypeDef* GPIOx, uint8_t pin);
extern void UC_GPIO_setModeAnalog(GPIO_TypeDef* GPIOx, uint8_t pin);

extern void UC_GPIO_setOutputTypePushPull(GPIO_TypeDef* GPIOx, uint8_t pin);
extern void UC_GPIO_setOutputTypeOpenDrain(GPIO_TypeDef* GPIOx, uint8_t pin);

extern void UC_GPIO_setSpeedLow(GPIO_TypeDef* GPIOx, uint8_t pin);
extern void UC_GPIO_setSpeedMiddle(GPIO_TypeDef* GPIOx, uint8_t pin);
extern void UC_GPIO_setSpeedHigh(GPIO_TypeDef* GPIOx, uint8_t pin);
extern void UC_GPIO_setSpeedVeryHigh(GPIO_TypeDef* GPIOx, uint8_t pin);

extern void UC_GPIO_setFloating(GPIO_TypeDef* GPIOx, uint8_t pin);
extern void UC_GPIO_setPullup(GPIO_TypeDef* GPIOx, uint8_t pin);
extern void UC_GPIO_setPulldown(GPIO_TypeDef* GPIOx, uint8_t pin);

extern void UC_GPIO_setAlternateFunction(GPIO_TypeDef* GPIOx, uint8_t pin, uint8_t value);

extern void UC_GPIO_setPin(GPIO_TypeDef* GPIOx, uint8_t pin);
extern void UC_GPIO_clearPin(GPIO_TypeDef* GPIOx, uint8_t pin);
extern void UC_GPIO_writeVal(GPIO_TypeDef* GPIOx, uint8_t pin, bool val);

extern void UC_GPIO_Unit_Test(void);

#endif /* INCLUDE_UC_GPIO_H_ */
