/*
 * Acc_test_fresh.c
 *
 * Created: 05-09-2014 08:13:16
 *  Author: Andi Degn
 */

/* #define F_CPU 11059200UL */


/*********************************//**
 * UNCOMMENT FOR RUN ALL UNIT TESTS  *
 *************************************/
#define UNIT_TEST 0
#define ANALYSIS 0

#define UART0 1
#define UART1 0
#define SEND_TO_UART 1
#define UART_LOOP_BACK 1

#define DELAY_BETWEEN_CHARS 1

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "sensors/tc72/tc72.h"
#include "sensors/accelerometer/accelerometer.h"
#include "sensors/sensor_scheduler.h"
#include "data_comm/uart/uart.h"
#include "hardware_boards/lcd_board/lcd/lcd.h"
#include "indicators/led_on_plane/lop.h"
#include "hardware_boards/lcd_board/button_led/btn_led_lcd.h"

void uart0_callback(char data);
void uart1_callback(char data);

static char _uart_callback_data[16];
static uint8_t _index;
static bool _sending = false;
static bool _listening = false;
int main(void) {
#if ANALYSIS
    scheduler_start();
    sei();
    while (1) {

    }
#else
#if UART0
    uart0_setup_async(	UART_MODE_DOUBLE,
                        UART_BAUD_115K2,
                        UART_PARITY_DISABLED,
                        UART_ONE_STOP_BIT,
                        UART_8_BIT,
                        uart0_callback);
#endif // UART0
#if UART1
    uart1_setup_async(	UART_MODE_NORMAL,
						UART_BAUD_115K2,
						UART_PARITY_DISABLED,
						UART_ONE_STOP_BIT,
						UART_8_BIT,
						uart1_callback);
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

#if UNIT_TEST
#include "unit_test.h"
    char* result = run_all_tests();
    lcd_clrscr();
    lcd_gotoxy(0,0);
    lcd_puts(result);
    lcd_gotoxy(0,1);
    lcd_puts("Tests run: ");
    lcd_puts(itoa(tests_run, buf, 10));
#else // !UNIT_TEST
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

#if SEND_TO_UART
        /* listening for switch press */
        if (!btn_lcd_is_pressed(BTN_PIN0)) {
            _sending = !_sending;
            led_lcd_toggle(LED_RED);
            led_lcd_toggle(LED_GREEN);
            while (!btn_lcd_is_pressed(BTN_PIN0));
        }
        if (_sending) {
	#if UART0
            _delay_ms(DELAY_BETWEEN_CHARS);
            uart0_send_char(x_axis >> 8);	uart0_send_char(x_axis & 0xff);
            _delay_ms(DELAY_BETWEEN_CHARS);
            uart0_send_char(y_axis >> 8);	uart0_send_char(y_axis & 0xff);
            _delay_ms(DELAY_BETWEEN_CHARS);
            uart0_send_char(z_axis >> 8);	uart0_send_char(z_axis & 0xff);
            _delay_ms(DELAY_BETWEEN_CHARS);
            uart0_send_char(0x7f);			uart0_send_char(0xff);
	#endif // UART0
	#if UART1
            uart1_send_char(x_axis >> 8);	uart1_send_char(x_axis & 0xff);
            uart1_send_char(y_axis >> 8);	uart1_send_char(y_axis & 0xff);
            uart1_send_char(z_axis >> 8);	uart1_send_char(z_axis & 0xff);
            uart1_send_char(0x7f);			uart1_send_char(0xff);
	#endif // UART1
        } else {
            _delay_ms(3 * DELAY_BETWEEN_CHARS);
        }
#endif // SEND_TO_UART

        _delay_ms(0);
    }
#endif // UNIT_TEST
#endif // ANALYSIS
}
#if UART0
void uart0_callback(char data) {
    *(_uart_callback_data + (_index++ % 16)) = data;
	#if UART_LOOP_BACK
    uart0_send_char(data);
	#endif
}
#endif // UART0
#if UART1
void uart1_callback(char data) {
    *(_uart_callback_data + (_index++ % 16)) = data;
    #if UART_LOOP_BACK
    uart1_send_char(data);
    #endif
}
#endif // UART1
