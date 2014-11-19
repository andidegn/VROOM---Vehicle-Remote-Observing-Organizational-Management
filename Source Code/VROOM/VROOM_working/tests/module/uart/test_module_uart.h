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

#include <stdbool.h>

/**********************************************************************//**
 * @ingroup test_uart_pub
 * @brief Starts the testing routine for uart0 and uart1
 *
 * @param const char *__uart_test_string - string to be send
 * @param const char *__uart_compare_string - string to be compared to
 *
 * @return bool - true = passed; false = failed
 * @note To run this test uart0 and uart1 must be connect crossed. TX0->RX1, TX1->RX0
 *************************************************************************/
bool test_module_uart_run(const char *__uart_test_string, const char *__uart_compare_string);

#endif /* TEST_MODULE_UART_H_ */