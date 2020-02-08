/*
 * uc_st7735.h
 *
 *  Created on: 11.01.2020
 *      Author: Florian
 */

#ifndef UC_INCLUDE_UC_ST7735_H_
#define UC_INCLUDE_UC_ST7735_H_

#include <stddef.h>
#include <stdbool.h>


typedef enum
{
    NO_CMD = 0x00,
	A_CMD = 0x01
} UC_ST7735_COMMAND_t;


extern void UC_ST7735_init(void);
extern void UC_ST7735_sendCommand(UC_ST7735_COMMAND_t command);
extern void UC_ST7735_sendData(uint8_t data);





#endif /* UC_INCLUDE_UC_ST7735_H_ */
