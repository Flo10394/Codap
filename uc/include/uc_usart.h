/*
 * uc_uart.h
 *
 *  Created on: 08.02.2020
 *      Author: Florian
 */

#ifndef UC_INCLUDE_UC_UART_H_
#define UC_INCLUDE_UC_UART_H_

#include <stdint.h>

/*
 * \brief	Initializes the USART peripheral and GPIO Pins
 */
extern void UC_USART_Init(void);

/*
 * \brief		transmits a given string or single character
 *
 * \param[in]	str		pointer to string or character
 * \param[in]	size	length of str
 */
extern void UC_USART_sendString(const char * str, uint32_t size);



#endif /* UC_INCLUDE_UC_UART_H_ */
