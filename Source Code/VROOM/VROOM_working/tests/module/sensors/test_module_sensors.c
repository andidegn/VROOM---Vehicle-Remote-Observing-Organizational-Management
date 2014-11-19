/********************************************//**
@file test_module_sensors.c
@author: Kenneth Ren� Jensen
@Version: 0.1
@defgroup test Module_test_sensors
@{
	This is a test for for the sensor module consisting of TC72 temperature sensor and accelerometer using SPI.
	The module test uses the LCD display to visualize the sensors output.
@}
@note NOT YET Complies MISRO 2004 standards
************************************************/
#include <stdlib.h>
#include "test_module_sensors.h"
#include "../../../hardware_boards/lcd_board/lcd/lcd.h"
#include "../../../sensors/sensor_scheduler.h"

const char degree = 0b011011111;
static int x_axis, y_axis, z_axis;
static float temp;
static char buf[10];
 
void sensors_init()
{
	x_axis = 0;
	y_axis = 0;
	z_axis = 0;
	temp = 0.0;
	scheduler_start(NULL);
}

void sensors_read_data(void)
{
	x_axis = (int)(acc_get_x_axis()*100);
	y_axis = (int)(acc_get_y_axis()*100);
	z_axis = (int)(acc_get_z_axis()*100);
	temp = get_temperature();
}

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