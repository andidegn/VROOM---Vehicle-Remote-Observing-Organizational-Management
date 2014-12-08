/********************************************//**
@file test_module_sensors.h
@author: Kenneth Ren� Jensen
@Version: 0.1
@defgroup test Module_test_sensors
@{
	This is a test for for the sensor module consisting of TC72 temperature sensor and accelerometer using SPI.
	The module test uses the LCD display to visualize the sensors output.
@}
@note NOT YET Complies MISRA 2004 standards
************************************************/

#ifndef TEST_MODULE_SENSORS_H_
#define TEST_MODULE_SENSORS_H_

void sensors_init(void);
void sensors_read_data(void);
void sensors_print_data(void);

#endif /* TEST_MODULE_SENSORS_H_ */