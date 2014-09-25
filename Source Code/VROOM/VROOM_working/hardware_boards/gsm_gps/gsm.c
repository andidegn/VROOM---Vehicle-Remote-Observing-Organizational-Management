/*
 * gsm.c
 *
 * Created: 24-09-2014 00:04:57
 *  Author: Andi Degn
 */

#include <util/delay.h>
#include "gsm.h"
#include "../../data_comm/uart/uart.h"
#include "../lcd_board/lcd/lcd.h"

#define LOOP_TO_PC 1

#define DDR(x) (*(&x - 1))
#define PIN(x) (*(&x - 2))

#define GSM_ENABLE GSM_PORT &= ~_BV(GSM_GSM_ENABLE_PIN)
#define GSM_DISABLE GSM_PORT |= _BV(GSM_GSM_ENABLE_PIN)

#define GPS_ENABLE GSM_PORT &= ~_BV(GSM_GPS_ENABLE_PIN)
#define GPS_DISABLE GSM_PORT |= _BV(GSM_GPS_ENABLE_PIN)

/* prototypes */
void uart0_callback(char c);
void uart1_callback(char c);

void gsm_init(void) {
	/* setting the selection pins to output */
	DDR(GSM_PORT) |= _BV(GSM_GSM_ENABLE_PIN) | _BV(GSM_GPS_ENABLE_PIN) | _BV(GSM_MODULE_START_PIN);

	/* setting up uart for communication with the module */
	uart0_setup_async(UART_MODE_DOUBLE, UART_BAUD_115K2, UART_PARITY_DISABLED, UART_ONE_STOP_BIT, UART_8_BIT, uart0_callback);

	#if LOOP_TO_PC
	/* setting up uart for communication with pc for diag */
	uart1_setup_async(UART_MODE_DOUBLE, UART_BAUD_115K2, UART_PARITY_DISABLED, UART_ONE_STOP_BIT, UART_8_BIT, uart1_callback);
	#endif

	/* waiting for proper startup */
	_delay_ms(2000);
	/* starting the module */
	GSM_PORT |= _BV(GSM_MODULE_START_PIN);
	_delay_ms(1500);
	GSM_PORT &= ~_BV(GSM_MODULE_START_PIN);
}

void gsm_start(void) {
	GPS_DISABLE;
	GSM_ENABLE;
	_delay_ms(2000);
	gsm_send(AT_DIAG_TEST);
}

void gsm_send(const char *command) {
	uart0_send_string(command);
	uart0_send_char(AT_EOL);
}

void gsm_answer(void) {
	gsm_send(AT_CALL_ANSWER);
}

void gsm_hang_up(void) {
	gsm_send(AT_CALL_HANG_UP);
}

/*
 *****************************************
 *****************************************
 ****************** GPS ******************
 *****************************************
 *****************************************
 */


/* callbacks */
void uart0_callback(char c) {
	#if LOOP_TO_PC
	uart1_send_char(c);
	#endif
	lcd_putc(c == '\r'? '\n': c);
}

#if LOOP_TO_PC
void uart1_callback(char c) {
	uart0_send_char(c);
}
#endif

