/*
 * uc_delay.c
 *
 *  Created on: 12.01.2020
 *      Author: Florian
 */

#include <uc_delay.h>


volatile uint32_t us100Ticks = 0;

extern void UC_DELAY_init()
{
   RCC_ClocksTypeDef RCC_Clocks;
   RCC_GetClocksFreq (&RCC_Clocks);
   SysTick_Config (RCC_Clocks.HCLK_Frequency / 10000); // every 100 µs
	return;
}

extern void UC_DELAY_100us(uint32_t us100)
{
	while(us100Ticks < us100)
	{
	}
	us100Ticks = 0;
	return;
}

extern void UC_DELAY_ms(uint32_t ms)
{
	UC_DELAY_100us(10*ms);
	return;
}

extern void UC_DELAY_s(uint32_t s)
{
	UC_DELAY_ms(s*1000);
	return;
}

//Interrupt
void SysTick_Handler()
{
	us100Ticks++;
    return;
}
