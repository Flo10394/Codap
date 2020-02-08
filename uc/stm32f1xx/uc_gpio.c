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
	if(pin > 7) // USE CRH
	{
		CLEAR_BIT(GPIOx->CRH, 	(uint32_t)(1 << (((pin-8)*4) + 2))  	);
		CLEAR_BIT(GPIOx->CRH, 	(uint32_t)(1 << (((pin-8)*4) + 3))  	);
	}
	else // USE CRL
	{
		CLEAR_BIT(GPIOx->CRL, 	(uint32_t)(1 << ((pin*4) + 2))  	);
		CLEAR_BIT(GPIOx->CRL, 	(uint32_t)(1 << ((pin*4) + 3))  	);
	}

	return;
}

extern void UC_GPIO_setOutputPushPullAlternateFunction(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	if(pin > 7) // USE CRH
	{
		CLEAR_BIT(GPIOx->CRH, 	(uint32_t)(1 << (((pin-8)*4) + 2))  	);
		SET_BIT(GPIOx->CRH, 	(uint32_t)(1 << (((pin-8)*4) + 3))  	);
	}
	else // USE CRL
	{
		CLEAR_BIT(GPIOx->CRL, 	(uint32_t)(1 << ((pin*4) + 2))  	);
		SET_BIT(GPIOx->CRL, 	(uint32_t)(1 << ((pin*4) + 3))  	);
	}

	return;
}

extern void UC_GPIO_setOutputSpeedLow(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	if(pin > 7) // USE CRH
	{
		CLEAR_BIT(GPIOx->CRH, 	(uint32_t)(1 << ((pin-8)*4))  		);
		SET_BIT(GPIOx->CRH, 	(uint32_t)(1 << (((pin-8)*4) + 1))  );
	}
	else // USE CRL
	{
		CLEAR_BIT(GPIOx->CRL, 	(uint32_t)(1 << pin*4)  		);
		SET_BIT(GPIOx->CRL, 	(uint32_t)(1 << ((pin*4) + 1))  );
	}
	return;
}

extern void UC_GPIO_setOutputSpeedMiddle(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	if(pin > 7) // USE CRH
	{
		SET_BIT(GPIOx->CRH, 	(uint32_t)(1 << ((pin-8)*4))  		);
		CLEAR_BIT(GPIOx->CRH, 	(uint32_t)(1 << (((pin-8)*4) + 1))  );
	}
	else // USE CRL
	{
		SET_BIT(GPIOx->CRL, 	(uint32_t)(1 << pin*4)  		);
		CLEAR_BIT(GPIOx->CRL, 	(uint32_t)(1 << ((pin*4) + 1))  );
	}
	return;
}

extern void UC_GPIO_setPin(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	SET_BIT(GPIOx->ODR, (uint32_t)(1 << pin));
	return;
}

extern void UC_GPIO_clearPin(GPIO_TypeDef* GPIOx, uint8_t pin)
{
	CLEAR_BIT(GPIOx->ODR, (uint32_t)(1 << pin));
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
