/**********************************************************************//**
 * Acc_test_fresh.c
 *
 * Created: 05-09-2014 08:13:16
 *  Author: Andi Degn
 *************************************************************************/
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "sensors/sensor_scheduler.h"
#include "hardware_boards/sim908/sim908.h"
#include "hardware_boards/car_panel/car_panel.h"
#include "accident_logic/accident_data.h"
#include "vroom_config.h"
#ifdef DEBUG_LCD_ENABLE
	#include "hardware_boards/lcd_board/lcd/lcd.h"
#endif
#define F_CPU 11059200UL
#define ON	1
#define OFF 0

/* ** Defines what context of the program to be executed ** */
#define UNIT_TEST						OFF
#define UNIT_TEST_MSD					OFF

#define MODULE_TEST_SENSORS				OFF
#define MODULE_TEST_SIM908				OFF
#define MODULE_TEST_CAR_PANEL			OFF
#define MODULE_TEST_UART				OFF

#define INTEGRATION_TEST_SIM908_SENSORS	ON
/* ******************************************************** */
int main (void)
{
	#ifdef DEBUG_LCD_ENABLE
		char buf[20];
		lcd_init(LCD_DISP_ON);
	#endif /* DEBUG_LCD_ENABLE */

	#if UNIT_TEST
		#include "tests/unit/unit_test.h"
		char* result = run_all_tests();
		#ifdef DEBUG_LCD_ENABLE
			lcd_clrscr();
			lcd_gotoxy(0,0);
			lcd_puts(result);
			lcd_gotoxy(0,1);
			lcd_puts("Tests run: ");
			lcd_puts(itoa(tests_run, buf, 10));
		#endif /* DEBUG_LCD_ENABLE */
	#endif /* UNIT_TEST */

	#if UNIT_TEST_MSD	
		/******************************************************************************
		************************************ ToDo ************************************* 
		******************************************************************************/
		/* Don't know if it should be integrated in the existing unit test file */
	#endif /* UNIT_TEST_MSD */

	#if MODULE_TEST_SENSORS
		#include "tests/module/sensors/test_module_sensors.h"
		sensors_init();
		while (1)
		{
			sensors_read_data();
			#if DEBUG_LCD_ENABLE
				sensors_print_data();
			#endif /* DEBUG_LCD_ENABLE */
			_delay_ms(500);
		}
	#endif /* MODULE_TEST_SENSORS */

	#if MODULE_TEST_SIM908
		SIM908_init();
		sei();
		SIM908_start();
	#endif /* MODULE_TEST_SIM908 */

	#if MODULE_TEST_CAR_PANEL
		#include "tests/module/car_panel/test_module_car_panel.h"
			/******************************************************************************
			************************************ ToDo ************************************* 
			******************************************************************************/
		sei();
	#endif /* MODULE_TEST_CAR_PANEL */

	#if MODULE_TEST_UART
		#include "tests/module/uart/test_module_uart.h"

		sei();
		char *_test_strings[] = {
			"½1234567890+´qwertyuiopåäsdfghjklæø'<zxcvbnm,.-§!\"#¤%&/()=?`QWERTYUIOPÅÂSDFGHJKLÆØ*>ZXCVBNM;:_@£$€{[]}|€~\µ'",
			"test_string2",
			"test_string3",
			"test_string4"
		};

		char *_compare_strings[] = {
			"½1234567890+´qwertyuiopåäsdfghjklæø'<zxcvbnm,.-§!\"#¤%&/()=?`QWERTYUIOPÅÂSDFGHJKLÆØ*>ZXCVBNM;:_@£$€{[]}|€~\µ'",	/* pass */
			"test_",		/* pass */
			"fail",			/* fail */
			"test_string4_"	/* fail */
		};
		#ifdef DEBUG_LCD_ENABLE
		for (uint8_t k = 0; k < 4; k++) {
			lcd_puts(test_module_uart_run(_test_strings[k], _compare_strings[k]) ? "PASSED" : "FAILED");
			lcd_putc('\n');
			
		}
		#endif /* DEBUG_LCD_ENABLE */
	#endif /* MODULE_TEST_UART */

	#if INTEGRATION_TEST_SIM908_SENSORS
		int16_t *_acc_buffer = malloc(3 * sizeof(int16_t));
		volatile uint32_t acc_total = 0;
		const char degree = 0b011011111;
		int16_t x_axis, y_axis, z_axis;
		float temp;

		#ifdef DEBUG_LCD_ENABLE
			lcd_clrscr();
			lcd_gotoxy(0, 0);
			lcd_puts("Initializing...");
		#endif /* DEBUG_LCD_ENABLE */	
		
		car_panel_init();
		SIM908_init();
		sei();
		SIM908_start();
		scheduler_start(NULL);
		car_panel_start();
		
		while (1)
		{
			/*************************************************************************
			****************** Has to be moved into accident_logic ******************* 
			*************************************************************************/
			scheduler_get_last_readings(_acc_buffer);
			x_axis = *_acc_buffer;
			y_axis = *(_acc_buffer + 1);
			z_axis = *(_acc_buffer + 2);
			temp = get_temperature();
			acc_total = sqrt(pow(x_axis, 2) + pow(y_axis, 2) + pow(z_axis, 2));

			if (acc_total > 1000 && EXT_EMERGENCY_FLAG == EMERGENCY_NO_ALARM)
			{
				if (!car_panel_wait_cancel_emmergency())
					EXT_EMERGENCY_FLAG = EMERGENCY_AUTO_ALARM;
			}
			/**************************************************************************/

			EXT_CONNECTION_CREG_FLAG == CREG_REGISTERED ? car_panel_set_status(STATUS_GREEN) : car_panel_set_status(STATUS_RED);

			if (EXT_EMERGENCY_FLAG == EMERGENCY_AUTO_ALARM ||  EXT_EMERGENCY_FLAG == EMERGENCY_MANUAL_ALARM)
			{
				emergency_alarm();

				/* Enable cancel button for reset purpose */
				car_panel_set_cancel_button_state(true);
			}

			#ifdef DEBUG_LCD_ENABLE
				lcd_clrscr();
				lcd_gotoxy(0, 0);
				lcd_puts("x ");
				lcd_puts(itoa(x_axis, buf, 10));

				lcd_gotoxy(8, 0);
				lcd_puts("y ");
				lcd_puts(itoa(y_axis, buf, 10));;

				lcd_gotoxy(0, 1);
				lcd_puts("z ");
				lcd_puts(itoa(z_axis, buf, 10));

				lcd_gotoxy(8, 1);
				lcd_puts(dtostrf( temp, 2, 2, buf ));
				lcd_putc(degree);
				lcd_putc('C');
			#endif /* DEBUG_LCD_ENABLE */	
			_delay_ms(200);
		}
	#endif /* INTEGRATION_TEST_SIM908_SENSORS */

	while (1);
}
