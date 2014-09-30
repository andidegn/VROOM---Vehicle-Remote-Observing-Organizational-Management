/*
 * Acc_test_fresh.c
 *
 * Created: 05-09-2014 08:13:16
 *  Author: Andi Degn
 */

/* #define F_CPU 11059200UL */

#define ON 1
#define OFF 0

#define KENNETH_TEST OFF
#define ANDI_TEST ON
#define MODULE_TEST OFF
/*********************************//**
 * UNCOMMENT FOR RUN ALL UNIT TESTS  *
 *************************************/
#define UNIT_TEST OFF
#define ANALYSIS OFF

#define SEND_TO_UART OFF
#define UART0 OFF
#define UART1 OFF
#define UART_LOOP_BACK OFF
#define GSM_TEST ON

#define DELAY_BETWEEN_CHARS 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#if MODULE_TEST

#define NEW_STRING malloc(16 * sizeof(char))

#include "tests/module/uart/test_module_uart.h"
#include "hardware_boards/lcd_board/lcd/lcd.h"
#include "hardware_boards/lcd_board/button_led/btn_led_lcd.h"

char **_results;

int main(void) {
	_results = malloc(10 * sizeof(char));
	int8_t i = 0;
	uint8_t num_of_tests = 0;

	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	lcd_home();
	lcd_puts("Running test\n");

	sei();

	_results[i] = NEW_STRING;
	*(_results + i++) = test_module_uart_run() == UART_PASSED ? "UART PASSED" : "UART FAILED";

	_results[i] = NEW_STRING;
	*(_results + i++) = "RANDOM0 PASSED";

	_results[i] = NEW_STRING;
	*(_results + i++) = "RANDOM1 PASSED";

	_results[i] = NEW_STRING;
	*(_results + i++) = "RANDOM2 PASSED";

	num_of_tests = i;
	i--;
	while (1) {
		lcd_clrscr();
		lcd_home();
		lcd_puts(num_of_tests > 1 ? *(_results + i - 1) : "Only one test");
		lcd_putc('\n');
		lcd_puts(*(_results + i));

        if (btn_lcd_is_pressed(BTN_PIN0)) {
            while (btn_lcd_is_pressed(BTN_PIN0));
			i++;
			if (i > num_of_tests - 1) {
				i = num_of_tests - 1;
			}
        }

        if (btn_lcd_is_pressed(BTN_PIN1)) {
            while (btn_lcd_is_pressed(BTN_PIN1));
			i--;
			if (i < 1) {
				i = 1;
			}
        }
		
		_delay_ms(100);
	}
	return 0;
}

#endif // MODULE_TEST

#if ANDI_TEST
#include "sensors/tc72/tc72.h"
#include "sensors/accelerometer/accelerometer.h"
#include "sensors/sensor_scheduler.h"
#include "data_comm/uart/uart.h"
#include "hardware_boards/lcd_board/lcd/lcd.h"
#include "indicators/led_on_plane/lop.h"
#include "hardware_boards/lcd_board/button_led/btn_led_lcd.h"
#include "hardware_boards/gsm_gps/gsm.h"

void uart0_callback_test(char data);
void uart1_callback_test(char data);

static char _uart_callback_data[16];
static uint8_t _i;
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
                        uart0_callback_test);
#endif // UART0
#if UART1
    uart1_setup_async(	UART_MODE_NORMAL,
						UART_BAUD_115K2,
						UART_PARITY_DISABLED,
						UART_ONE_STOP_BIT,
						UART_8_BIT,
						uart1_callback_test);
#endif // UART1
	if (!btn_lcd_is_pressed(BTN_PIN0)) {
		lcd_init(LCD_DISP_ON);
	}
    btn_led_lcd_init();
    led_lcd_set(LED_RED, LED_ON);
    lop_init();
	#if UART0
    scheduler_start(uart0_callback_test);
	#else
	scheduler_start(NULL);
	#endif
#if GSM_TEST
	gsm_init();
	gsm_start();
#endif

    const char degree = 0b011011111;
    int x_axis, y_axis, z_axis;
    float temp;
    char buf[10];

	uint16_t uart_read;

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
	sei();
    while (1) {

#if GSM_TEST
        /* listening for switch press */
        if (btn_lcd_is_pressed(BTN_PIN0)) {
            while (btn_lcd_is_pressed(BTN_PIN0));
			lcd_clrscr();
			gsm_send(AT_CONN_SIGNAL_STRENGTH);
        }
        /* listening for switch press */
        if (btn_lcd_is_pressed(BTN_PIN1)) {
            while (btn_lcd_is_pressed(BTN_PIN1));
            lcd_clrscr();
			gsm_send(AT_CONN_NETWORK_REGISTRATION_STATUS);
        }
        /* listening for switch press */
        if (btn_lcd_is_pressed(BTN_PIN2)) {
            while (btn_lcd_is_pressed(BTN_PIN2));
            lcd_clrscr();
			gsm_answer();
        }
        /* listening for switch press */
        if (btn_lcd_is_pressed(BTN_PIN3)) {
            while (btn_lcd_is_pressed(BTN_PIN3));
            lcd_clrscr();
			gsm_hang_up();
        }
		while ((uart_read = uart0_read_char()) != UART_NO_DATA) {
			lcd_putc(uart_read == '\r'? '\n': uart_read);
			uart1_send_char(uart_read);
		}
		while ((uart_read = uart1_read_char()) != UART_NO_DATA) {
			uart0_send_char(uart_read);
		}
#else // GSM_TEST

        lcd_clrscr();
        lcd_gotoxy(0, 0);
        sprintf(buf, "x %05d", x_axis);
        lcd_puts(buf);

        lcd_gotoxy(8, 0);
        sprintf(buf, "y %05d", y_axis);
        lcd_puts(buf);

        /* listening for switch press */
        if (btn_lcd_is_pressed(BTN_PIN1)) {
            _listening = !_listening;
            lop_toggle_led(LOP_YELLOW);
            while (btn_lcd_is_pressed(BTN_PIN1));
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
        if (btn_lcd_is_pressed(BTN_PIN0)) {
            _sending = !_sending;
            led_lcd_toggle(LED_RED);
            led_lcd_toggle(LED_GREEN);
            while (btn_lcd_is_pressed(BTN_PIN0));
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
#else // SEND_TO_UART
	_delay_ms(100);
#endif // SEND_TO_UART
#endif // GSM_TEST
    }
#endif // UNIT_TEST
#endif // ANALYSIS
}
#if UART0
void uart0_callback_test(char data) {
	_listening = true;
    *(_uart_callback_data + (_i++ % 16)) = data;
	#if UART_LOOP_BACK
    uart0_send_char(data);
	#endif
}
#endif // UART0
#if UART1
void uart1_callback_test(char data) {
    #if UART_LOOP_BACK
    uart1_send_char(data);
    #endif
}
#endif
#endif


#if KENNETH_TEST

#include "hardware_boards/sim908/sim908_gsm.h"
#include "hardware_boards/lcd_board/lcd/lcd.h"
#include "hardware_boards/lcd_board/button_led/btn_led_lcd.h"
#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 11059200UL



	
int main (void)
{
	DDRA = 0xFF;
	PORTA = 0xFF;
	char buf[10];
	int8_t a = 0;
	int8_t b = 0;
	btn_led_lcd_init();
	sei();
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	lcd_gotoxy(0,0);
	lcd_puts("Init...");
	SIM908_init();
	lcd_puts("-OK!");
	lcd_gotoxy(0,1);
	lcd_puts("Enable GSM...");
	GSM_enable();
	lcd_puts("-OK!");
	
	#if UNIT_TEST
		#include "unit_test.h"
		char* result = run_all_tests();
		lcd_clrscr();
		lcd_gotoxy(0,0);
		lcd_puts(result);
		lcd_gotoxy(0,1);
		lcd_puts("Tests run: ");
		lcd_puts(itoa(tests_run, buf, 10));
	#else //!UNIT_TEST

		lcd_clrscr();
		lcd_puts("CALL...");
		
		while(call_PSAP() != SIM908_OK);
		lcd_puts("-OK!");
		
		while (1)
		{
			if (btn_lcd_is_pressed(BTN_PIN0))
			{
				SIM908_cmd("ATH", OK);
				lcd_gotoxy(0,1);
				lcd_puts("HANG UP!!!");
			}
				
			PORTA ^= 1;
			_delay_ms(1000);
		}
	#endif // !UNIT_TEST
}
#endif