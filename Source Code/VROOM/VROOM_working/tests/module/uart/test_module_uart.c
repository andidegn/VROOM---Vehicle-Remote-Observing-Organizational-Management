/*
 * test_module_uart.c
 *
 * Created: 26-09-2014 15:48:21
 *  Author: Andi Degn
 */

#include <avr/io.h>
#include <stdbool.h>
#include "test_module_uart.h"
#include "../../../data_comm/uart/uart.h"

#define TEST_STRING_LENGTH 17
static volatile char *_uart_test_string = "UART testing...\r\n";
static char _eol_char = '\n';
static char *_uart_recieved_data[TEST_STRING_LENGTH];
static char _i;
static uint16_t _timeout_ctr;
static bool _roundtrip_complete = false;

static inline uint8_t _validate_data();
static void _data0_received(char data);
static void _data1_received(char data);

uint8_t test_module_uart_run(void) {
	uint8_t test_result = UART_PASSED;
	uint16_t tmp_char = 0;

	/*******************************
	 **** TESTING WITH CALLBACK ****
	 *******************************/
	/* init both uarts with callback */
	uart0_setup_async(UART_MODE_DOUBLE, UART_BAUD_115K2, UART_PARITY_DISABLED, UART_ONE_STOP_BIT, UART_8_BIT, _data0_received);
	uart1_setup_async(UART_MODE_DOUBLE, UART_BAUD_115K2, UART_PARITY_DISABLED, UART_ONE_STOP_BIT, UART_8_BIT, _data1_received);

	/* sending test string */
	_i = 0;
	uart0_send_string(_uart_test_string);

	/* waiting until round-trip has been completed */
	_timeout_ctr = 0;
	while (!_roundtrip_complete && _timeout_ctr++ < 1000) {
		_delay_ms(1);
	}

	test_result = _validate_data();

	/**********************************
	 **** TESTING WITHOUT CALLBACK ****
	 **********************************/
	/* init both uarts without callback */
	uart0_setup_async(UART_MODE_DOUBLE, UART_BAUD_115K2, UART_PARITY_DISABLED, UART_ONE_STOP_BIT, UART_8_BIT, NULL);
	uart1_setup_async(UART_MODE_DOUBLE, UART_BAUD_115K2, UART_PARITY_DISABLED, UART_ONE_STOP_BIT, UART_8_BIT, NULL);

	/* sending test string */
	_i = 0;
	uart0_send_string(_uart_test_string);

	while (tmp_char = uart1_read_char() != UART_NO_DATA) {
		_uart_recieved_data[_i++] = tmp_char;
	}

	test_result = _validate_data(_uart_test_string, _uart_recieved_data);

	return test_result;
}

static inline uint8_t _validate_data() {
	uint8_t test_result = UART_PASSED;
	
	for (_i = 0; _i < TEST_STRING_LENGTH; _i++) {
		if (_uart_test_string[_i] != _uart_recieved_data[_i]) {
			test_result = UART_FAILED;
		}
		_i++;
	}
	return test_result;
}

static void _data0_received(char data) {
	_uart_recieved_data[_i++] = data;
	if (data == _eol_char || _i > TEST_STRING_LENGTH) {
		_roundtrip_complete = true;
	}
}

static void _data1_received(char data) {
	uart1_send_char(data);
}
