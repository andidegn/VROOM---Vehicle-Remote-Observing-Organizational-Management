/**********************************************************************//**
 * @file test_module_sensors.c
 *************************************************************************/
#include <stdlib.h>
#include "test_module_sensors.h"
#include "../../../util/lcd_board/lcd/lcd.h"
#include "../../../application/scheduler/scheduler.h"

const char degree = 0b011011111;
static int16_t x_axis, y_axis, z_axis;
static float temp;
static char buf[10];
static int16_t _acc_buffer[3];

/**********************************************************************//**
 * @ingroup test_sensors_pub
 * Sets values to zero and starts the scheduler
 *************************************************************************/
void sensors_init()
{
	x_axis = 0;
	y_axis = 0;
	z_axis = 0;
	temp = 0.0;
	scheduler_start(NULL);
}

/**********************************************************************//**
 * @ingroup test_sensors_pub
 * Gets the readings from the scheduler and stores them locally
 *************************************************************************/
void sensors_read_data(void)
{
	scheduler_acc_get_last_readings(_acc_buffer);
	x_axis = _acc_buffer[0];
	y_axis = _acc_buffer[1];
	z_axis = _acc_buffer[2];
	temp = scheduler_temp_get_last_reading();
}

/**********************************************************************//**
 * @ingroup test_sensors_pub
 * Prints the values to the LCD display
 *************************************************************************/
void sensors_print_data(void)
{
	lcd_clrscr();
	lcd_gotoxy(0, 0);
	lcd_puts("x ");
	lcd_puts((char *)itoa(x_axis, buf, 10));

	lcd_gotoxy(8, 0);
	lcd_puts("y ");
	lcd_puts((char *)itoa(y_axis, buf, 10));;

	lcd_gotoxy(0, 1);
	lcd_puts("z ");
	lcd_puts((char *)itoa(z_axis, buf, 10));

	lcd_gotoxy(8, 1);
	lcd_puts((char *)dtostrf( temp, 2, 2, buf ));
	lcd_putc(degree);
	lcd_putc('C');
}