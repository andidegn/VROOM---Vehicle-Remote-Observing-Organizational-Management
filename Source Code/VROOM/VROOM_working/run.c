/**********************************************************************//**
 * Acc_test_fresh.c
 *
 * Created: 05-09-2014 08:13:16
 *  Author: Andi Degn
 *************************************************************************/

/* #define F_CPU 11059200UL */

#define ON 1
#define OFF 0

#define F_CPU 11059200UL

#define UNIT_TEST						OFF
#define MODULE_TEST_SENSORS				OFF
#define MODULE_TEST_SIM908				OFF
#define MODULE_TEST_CAR_PANEL			OFF
#define MODULE_TEST_UART				OFF
#define INTEGRATION_TEST_SIM908_SENSORS	ON

#define UNIT_TEST_MSD					OFF

#include "hardware_boards/lcd_board/lcd/lcd.h"
#include "sensors/sensor_scheduler.h"
#include "hardware_boards/sim908/sim908.h"
#include "hardware_boards/car_panel/car_panel.h"
#include "accident_logic/accident_data.h"
#include "tests/unit/unit_test.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

int main (void)
{
	char buf[20];

	lcd_init(LCD_DISP_ON);

	#if UNIT_TEST
		char* result = run_all_tests();
		lcd_clrscr();
		lcd_gotoxy(0,0);
		lcd_puts(result);
		lcd_gotoxy(0,1);
		lcd_puts("Tests run: ");
		lcd_puts(itoa(tests_run, buf, 10));
	#endif /* UNIT_TEST */

	#if MODULE_TEST_SENSORS
		#include "tests/module/sensors/test_module_sensors.h"
		sensors_init();
		while (1)
		{
			sensors_read_data();
			sensors_print_data();
			_delay_ms(500);
		}
	#endif /* MODULE_TEST_SENSORS */

	#if MODULE_TEST_SIM908
		lcd_clrscr();
		lcd_gotoxy(0,0);
		lcd_puts("Init...");

		SIM908_init();
		sei();
		SIM908_start();

		lcd_puts("...DONE");

		while (1)
		{

		}
	#endif /* MODULE_TEST_SIM908 */

	#if MODULE_TEST_CAR_PANEL
		#include "hardware_boards/car_panel/car_panel.h"
		car_panel_init();
		sei();
		while (1)
		{

		}
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

		for (uint8_t k = 0; k < 4; k++) {
			lcd_puts(test_module_uart_run(_test_strings[k], _compare_strings[k]) ? "PASSED" : "FAILED");
			lcd_putc('\n');
		}

		while (1);
	#endif /* MODULE_TEST_UART */

	#if INTEGRATION_TEST_SIM908_SENSORS
		int16_t *_acc_buffer = malloc(3 * sizeof(int16_t));
		volatile uint32_t acc_total = 0;
		const char degree = 0b011011111;
		int16_t x_axis, y_axis, z_axis;
		float temp;

		sei();
		car_panel_init();
		SIM908_init();

		SIM908_start();
		scheduler_start(NULL);
		car_panel_start();
		_delay_ms(500);
		while (1)
		{
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

			EXT_CONNECTION_CREG_FLAG == CREG_REGISTERED ? car_panel_set_status(STATUS_GREEN) : car_panel_set_status(STATUS_RED);

			if (EXT_EMERGENCY_FLAG == EMERGENCY_AUTO_ALARM ||  EXT_EMERGENCY_FLAG == EMERGENCY_MANUAL_ALARM)
			{
				emergency_alarm();

				/* Enable cancel button for reset purpose */
				car_panel_set_cancel_button_state(true);
			}

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

			_delay_ms(200);
		}
	#endif /* INTEGRATION_TEST_SIM908_SENSORS */

	#if UNIT_TEST_MSD
		emergency_alarm(true, false);
		while (1)
		{
			lcd_clrscr();
			lcd_gotoxy(0,0);
			lcd_puts("Control: ");
			lcd_puts(itoa(EXT_MSD.control, buf, 16));
			lcd_gotoxy(0,1);
			lcd_puts(EXT_MSD.VIN);
			_delay_ms(5000);

			lcd_clrscr();
			lcd_gotoxy(0,0);
			lcd_puts(ultoa(EXT_MSD.time_stamp, buf, 10) );
			lcd_gotoxy(0,1);
			lcd_puts(itoa(EXT_MSD.direction, buf, 10));
			_delay_ms(5000);

			lcd_clrscr();
			lcd_gotoxy(0,0);
			lcd_puts("Long: ");
			lcd_puts(ltoa(EXT_MSD.longitude, buf, 16));
			lcd_gotoxy(0,1);
			lcd_puts("Lati: ");
			lcd_puts(ltoa(EXT_MSD.latitude, buf, 16));
			_delay_ms(5000);

			lcd_clrscr();
			lcd_gotoxy(0,0);
			lcd_puts("SP: ");
			lcd_puts(itoa(EXT_MSD.sp[0], buf, 10)); lcd_putc('.');
			lcd_puts(itoa(EXT_MSD.sp[1], buf, 10)); lcd_putc('.');
			lcd_puts(itoa(EXT_MSD.sp[2], buf, 10)); lcd_putc('.');
			lcd_puts(itoa(EXT_MSD.sp[3], buf, 10));
			lcd_gotoxy(0,1);
			lcd_puts(EXT_MSD.optional_data);
			_delay_ms(5000);
		}
	#endif /* UNIT_TEST_MSD */

	while (1)
	{

	}
}
