/*
 * @file: test_module_uart.h
 *
 * @Created: 26-09-2014 15:47:38
 * @Author: Andi Degn
 * @defgroup test_uart UART module test
 * @{
	 This tests the uart0 and uart 1 on the ATMega2560 by
	 first sending a test string using uart0, receiving it
	 in uart1 callback function and sending it back to uart0
	 in a callback function.
	 Second it is tested again without using the callback
	 feature by using the uart_read_char() function.
	 Both tests received data is compared to the original
	 string for validation.
	 @defgroup test_uart_priv Private
	 @defgroup test_uart_pub Public
 * @}
 */


#ifndef TEST_MODULE_UART_H_
#define TEST_MODULE_UART_H_

#include <avr/io.h>

#define UART_PASSED 1
#define UART_FAILED 0

/**********************************************************************//**
 * @ingroup test_uart_pub
 * @brief Starts the testing routine for uart0 and uart1
 *
 * @param void
 *
 * @return uint8_t - UART_PASSED (1) or UART_FAILED (0)
 * @note To run this test uart0 and uart1 must be connect crossed. TX0->RX1, TX1->RX0
 *************************************************************************/
uint8_t test_module_uart_run(void);

#endif /* TEST_MODULE_UART_H_ */