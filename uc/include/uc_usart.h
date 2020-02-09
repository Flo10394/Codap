/*
 * uc_uart.h
 *
 *  Created on: 08.02.2020
 *      Author: Florian
 */

#ifndef UC_INCLUDE_UC_UART_H_
#define UC_INCLUDE_UC_UART_H_

#include <stdint.h>


extern void UC_USART_Init(void);

extern void UC_USART_sendString(const char * str, uint32_t size);



#endif /* UC_INCLUDE_UC_UART_H_ */
