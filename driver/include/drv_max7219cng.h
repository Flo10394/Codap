/*
 * DRV_max7219cng.h
 *
 *  Created on: 27.03.2020
 *      Author: Florian
 */

#ifndef DRV_INCLUDE_DRV_MAX7219CNG_H_
#define DRV_INCLUDE_DRV_MAX7219CNG_H_

#include <stdio.h>
#include <stdint.h>

typedef uint8_t ledMatrix[8][8];

extern void DRV_MAX7219CNG_init();

extern void DRV_MAX7219CNG_setLED(ledMatrix* pLeds);

extern void DRV_MAX7219CNG_clear(void);


#endif /* DRV_INCLUDE_DRV_MAX7219CNG_H_ */
