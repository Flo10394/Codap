/*
 * uc_delay.h
 *
 *  Created on: 12.01.2020
 *      Author: Florian
 */

#ifndef INCLUDE_UC_DELAY_H_
#define INCLUDE_UC_DELAY_H_

#include <core_cm3.h>
#include <stdint.h>

extern void UC_DELAY_init();

extern void UC_DELAY_ms(uint32_t ms);

extern void UC_DELAY_s(uint32_t s);

extern void UC_DELAY_100us(uint32_t us100);



#endif /* INCLUDE_UC_DELAY_H_ */
