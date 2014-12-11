/**********************************************************************//**
 * test_module_uart.c
 *
 * Created: 26-09-2014 15:48:21
 *  Author: Andi Degn
 *************************************************************************/
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include "test_module_uart.h"
#include "../../../data_comm/uart/uart.h"

#define RECEIVED_DATA_LENGTH 255

/* Local variables */
static char *_uart_compare_string;
static char *_uart_received_data;
static char _index;
static uint16_t _timeout_ctr;
static bool _roundtrip_complete = false;

/* Prototypes */
static bool _validate_data(void);
static void _data0_received(char data);
static void _data1_received(char data);

/**********************************************************************//**
 * @ingroup test_uart_pub
 * Sets up both uart0 and uart1, sends data on round trip through both
 * uarts and checks that the result is the same as the __uart_compare_string
 *************************************************************************/
bool test_module_uart_run(const char *__uart_test_string, const char *__uart_compare_string) {
	volatile bool test_result = false;
	volatile uint16_t tmp_char = 0;

	_uart_compare_string = (char*) __uart_compare_string;

	/*******************************
	 **** TESTING WITH CALLBACK ****
	 *******************************/
	/* init both uarts with callback */
	uart0_setup_async(UART_MODE_DOUBLE, UART_BAUD_115K2, UART_PARITY_DISABLED, UART_ONE_STOP_BIT, UART_8_BIT, _data0_received);
	uart1_setup_async(UART_MODE_DOUBLE, UART_BAUD_115K2, UART_PARITY_DISABLED, UART_ONE_STOP_BIT, UART_8_BIT, _data1_received);

	/* sending test string */
	_index = 0;
	_uart_received_data = malloc(RECEIVED_DATA_LENGTH * sizeof(char));
	uart0_send_string(__uart_test_string);
	_delay_ms(0); /* Apparently needed when using low baud rate */

	/* waiting until round-trip has been completed */
	_timeout_ctr = 0;
	_roundtrip_complete = false;
	while (!_roundtrip_complete && _timeout_ctr++ < 1000) {
		_delay_ms(1);
	}

	test_result = _validate_data();
	free(_uart_received_data);

	if (test_result == true) {
		/**********************************
		 **** TESTING WITHOUT CALLBACK ****
		 **********************************/
		/* init both uarts without callback */
		uart0_setup_async(UART_MODE_DOUBLE, UART_BAUD_460K8, UART_PARITY_DISABLED, UART_ONE_STOP_BIT, UART_8_BIT, NULL);
		uart1_setup_async(UART_MODE_DOUBLE, UART_BAUD_460K8, UART_PARITY_DISABLED, UART_ONE_STOP_BIT, UART_8_BIT, NULL);

		_index = 0;
		_uart_received_data = malloc(RECEIVED_DATA_LENGTH * sizeof(char));

		/* sending test string */
		uart0_send_string(__uart_test_string);

		while ((tmp_char = uart1_read_char()) != UART_NO_DATA) {
			uart1_send_char(tmp_char);
			_delay_ms(1);
			while ((tmp_char = uart0_read_char()) != UART_NO_DATA) {
				*(_uart_received_data + _index++) = tmp_char;
			}
		}


		test_result = _validate_data();
		free(_uart_received_data);
	}
	return test_result;
}

/**********************************************************************//**
 * @ingroup test_uart_priv
 * @brief Validates the data received against the given string literal
 *
 * @param void
 *
 * @return void
 *************************************************************************/
static bool _validate_data(void) {

	uint8_t i = 0;
	bool _result = false;

	while (*(_uart_compare_string + i) != '\0') {
		if (*(_uart_compare_string + i) != *(_uart_received_data + i)) {
			_result = false;
			break;
		}
		_result = true;
		i++;
	}
	return _result;
}

/**********************************************************************//**
 * @ingroup test_uart_priv
 * @brief Callback function for uart0
 * Stores received data in buffer and sets _roundtrip_complete flag if
 * the end length of the _uart_compare_string is reached
 *
 * @param char data - the data received
 *
 * @return void
 *************************************************************************/
static void _data0_received(char data) {
	*(_uart_received_data + _index++) = data;

	if (*(_uart_compare_string + _index) == '\0') {
		_roundtrip_complete = true;
	}
}

/**********************************************************************//**
 * @ingroup test_module
 * @brief Callback function for uart1
 * Echoes data
 *
 * @param char data - the data received
 *
 * @return void
 *************************************************************************/
static void _data1_received(char data) {
	uart1_send_char(data);
}
