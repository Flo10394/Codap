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
extern void UC_GPIO_setInputAnalog(GPIO_TypeDef* GPIOx, uint8_t pin);
extern void UC_GPIO_setInputFloating(GPIO_TypeDef* GPIOx, uint8_t pin);
extern void UC_GPIO_setInputPullUpDown(GPIO_TypeDef* GPIOx, uint8_t pin);

extern void UC_GPIO_setOutputPushPull(GPIO_TypeDef* GPIOx, uint8_t pin);
extern void UC_GPIO_setOutputOpenDrain(GPIO_TypeDef* GPIOx, uint8_t pin);
extern void UC_GPIO_setOutputPushPullAlternateFunction(GPIO_TypeDef* GPIOx, uint8_t pin);

extern void UC_GPIO_setAlternateFunction(GPIO_TypeDef* GPIOx, uint8_t pin, uint8_t val);

extern void UC_GPIO_setSpeedLow(GPIO_TypeDef* GPIOx, uint8_t pin);
extern void UC_GPIO_setSpeedMiddle(GPIO_TypeDef* GPIOx, uint8_t pin);
extern void UC_GPIO_setSpeedHigh(GPIO_TypeDef* GPIOx, uint8_t pin);

extern void UC_GPIO_writeVal(GPIO_TypeDef* GPIOx, uint8_t pin, bool val);

//Set
extern void UC_GPIO_setPin(GPIO_TypeDef* GPIOx, uint8_t pin);
extern void UC_GPIO_clearPin(GPIO_TypeDef* GPIOx, uint8_t pin);




#endif /* INCLUDE_UC_GPIO_H_ */
