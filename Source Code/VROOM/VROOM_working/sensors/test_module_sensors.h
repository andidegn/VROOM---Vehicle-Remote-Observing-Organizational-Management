/********************************************//**
@file test_module_sensors.h
@author: Kenneth René Jensen
@Version: 0.1
@defgroup test Module_test_sensors
@{
	This is a test for for the sensor module consisting of TC72 temperature sensor and accelerometer using SPI.
	The module test uses the LCD display to visualize the sensors output.
@}
@note NOT YET Complies MISRO 2004 standards
************************************************/

#ifndef TEST_MODULE_SENSORS_H_
#define TEST_MODULE_SENSORS_H_

#include <avr/io.h>
#include "../hardware_boards/lcd_board/lcd/lcd.h"
#include "sensor_scheduler.h"

void sensors_init(void);
void sensors_read_data(void);
void sensors_print_data(void);

#endif /* TEST_MODULE_SENSORS_H_ */