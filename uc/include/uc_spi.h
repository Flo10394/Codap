/*
 * uc_spi.h
 *
 *  Created on: 27.03.2020
 *      Author: Florian
 */

#ifndef UC_INCLUDE_UC_SPI_H_
#define UC_INCLUDE_UC_SPI_H_

#include <stdio.h>
#include <stdint.h>


extern void UC_SPI_init(void);

extern void UC_SPI_send8BitData(uint8_t data);
extern void UC_SPI_send16BitData(uint16_t data);


#endif /* UC_INCLUDE_UC_SPI_H_ */
