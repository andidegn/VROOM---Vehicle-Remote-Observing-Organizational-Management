/*
 * Acc_test_fresh.c
 *
 * Created: 05-09-2014 08:13:16
 *  Author: Andi Degn
 */

#define F_CPU 11059200UL


/*********************************//**
 * UNCOMMENT FOR RUN ALL UNIT TESTS  *
 *************************************/
//#define UNIT_TEST
#define SEND_TO_UART
#define UART0
//#define UART1
//#define ANALYSIS

#define DELAY_BETWEEN_CHARS 1

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "sensors/tc72/tc72.h"
#include "sensors/accelerometer/accelerometer.h"
#include "sensors/sensor_scheduler.h"
#include "hardware_boards/lcd_board/lcd/lcd.h"
#include "indicators/led_on_plane/lop.h"
#include "hardware_boards/lcd_board/button_led/btn_led_lcd.h"

static bool is_pressed(uint8_t pin);
void uart0_callback(unsigned char data);
void uart1_callback(unsigned char data);

static char _uart_callback_data[16];
static uint8_t _index;
static bool _sending = false;
static bool _listening = false;
int main(void) {
#ifndef ANALYSIS
		#ifdef UART0
			#include "data_comm/uart/uart0.h"
			uart0_setup_async(UART0_MODE_DOUBLE, UART0_BAUD_115K2, UART0_PARITY_DISABLED, UART0_ONE_STOP_BIT, UART0_8_BIT, uart0_callback);
		#endif // UART0
		#ifdef UART1
			#include "data_comm/uart/uart1.h"
			uart1_setup_async(UART1_MODE_NORMAL, UART1_BAUD_115K2, UART1_PARITY_DISABLED, UART1_ONE_STOP_BIT, UART1_8_BIT, uart1_callback);
		#endif // UART1
		lcd_init(LCD_DISP_ON);
		btn_led_lcd_init();
		led_lcd_set(LED_RED, LED_ON);
		lop_init();
		scheduler_start();

		sei();

		const char degree = 0b011011111;
		int x_axis, y_axis, z_axis;
		float temp;
		char buf[10];
		char buf2[32];

	#ifdef UNIT_TEST
		#include "unit_test.h"
		char* result = run_all_tests();
		lcd_clrscr();
		lcd_gotoxy(0,0);
		lcd_puts(result);
		lcd_gotoxy(0,1);
		lcd_puts("Tests run: ");
		lcd_puts(itoa(tests_run, buf, 10));
	#endif // UNIT_TEST

	#ifndef UNIT_TEST
		while (1) {

			uint8_t SREG_cpy = SREG;
			cli();
			temp = get_temperature();
			x_axis = (int)(acc_get_x_axis() * 1000);
			y_axis = (int)(acc_get_y_axis() * 1000);
			z_axis = (int)(acc_get_z_axis() * 1000);
			SREG = SREG_cpy;

			lcd_clrscr();
			lcd_gotoxy(0, 0);
			sprintf(buf, "x %05d", x_axis);
			lcd_puts(buf);

			lcd_gotoxy(8, 0);
			sprintf(buf, "y %05d", y_axis);
			lcd_puts(buf);

			/* listening for switch press */
			if (!btn_lcd_is_pressed(BTN_PIN1)) {
				_listening = !_listening;
				lop_toggle_led(LOP_YELLOW);
				while (!btn_lcd_is_pressed(BTN_PIN1));
			}

			if (_listening) {
				lcd_gotoxy(0, 1);
				lcd_puts(_uart_callback_data);
			} else {
				lcd_gotoxy(0, 1);
				sprintf(buf, "z %05d", z_axis);
				lcd_puts(buf);

				lcd_gotoxy(8, 1);
				lcd_puts(dtostrf( temp, 2, 2, buf ));
				lcd_putc(degree);
				lcd_putc('C');
			}

			#ifdef SEND_TO_UART
				/* listening for switch press */
				if (!btn_lcd_is_pressed(BTN_PIN0)) {
					_sending = !_sending;
					led_lcd_toggle(LED_RED);
					led_lcd_toggle(LED_GREEN);
					while (!btn_lcd_is_pressed(BTN_PIN0));
				}
				if (_sending) {
					#ifdef UART0
						_delay_ms(DELAY_BETWEEN_CHARS);
						uart0_send_char(x_axis >> 8);	uart0_send_char(x_axis & 0xff);
						_delay_ms(DELAY_BETWEEN_CHARS);
						uart0_send_char(y_axis >> 8);	uart0_send_char(y_axis & 0xff);
						_delay_ms(DELAY_BETWEEN_CHARS);
						uart0_send_char(z_axis >> 8);	uart0_send_char(z_axis & 0xff);
						_delay_ms(DELAY_BETWEEN_CHARS);
						uart0_send_char(0x7f);			uart0_send_char(0xff);
					#endif // UART0
					#ifdef UART1
						uart1_send_char(x_axis >> 8);	uart1_send_char(x_axis & 0xff);
						uart1_send_char(y_axis >> 8);	uart1_send_char(y_axis & 0xff);
						uart1_send_char(z_axis >> 8);	uart1_send_char(z_axis & 0xff);
						uart1_send_char(0x7f);			uart1_send_char(0xff);
					#endif // UART1
				} else {
					_delay_ms(3 * DELAY_BETWEEN_CHARS);
				}
			#endif

			_delay_ms(0);
		}
		#endif
	#endif // ANALYSIS

	#ifdef ANALYSIS
		scheduler_start();
		sei();
		while (1) {

		}
	#endif // ANALYSIS
}
#ifdef UART0
void uart0_callback(unsigned char data) {
	*(_uart_callback_data + (_index++ % 16)) = data;
	uart0_send_string("The received data from uart0 was: ", 34);
	uart0_send_char(data);
	uart0_send_string("\n\r", 2);
}
#endif // UART0
#ifdef UART1
void uart1_callback(unsigned char data) {
	*(_uart_callback_data + (_index++ % 16)) = data;
	uart1_send_string("The received data from uart1 was: ", 34);
	uart1_send_char(data);
	uart1_send_string("\n\r", 2);
}
#endif // UART1
