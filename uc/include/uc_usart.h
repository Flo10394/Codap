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
 *
 * \param[in]	tx_databuffer		pointer to tx_buffer
 * \param[in]	tx_buffersize		size of tx_buffer
 * \param[in]	rx_databuffer		pointer to rx_buffer
 * \param[in]	rx_buffersize		size of rx_buffer
 */
extern void UC_USART_Init(uint8_t* tx_databuffer, uint32_t tx_buffersize, uint8_t* rx_databuffer, uint32_t rx_buffersize);

/*
 * \brief		transmits a given string or single character until \0 appears
 *
 * \param[in]	str		pointer to string or character
 * \param[in]	size	length of str
 */
extern void UC_USART_sendString(const char * str, uint32_t size);



#endif /* UC_INCLUDE_UC_UART_H_ */
