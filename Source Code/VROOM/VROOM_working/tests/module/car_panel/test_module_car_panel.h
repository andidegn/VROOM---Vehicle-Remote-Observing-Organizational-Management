/********************************************//**
@file test_module_car_panel.h
@author: Kenneth René Jensen
@Version: 0.1
@defgroup test Module Test - Car Panel
@{
	Test for for the Car Panel module
@}
************************************************/
#ifndef TEST_MODULE_CAR_PANEL_H_
#define TEST_MODULE_CAR_PANEL_H_

#include <stdint.h>

void init_module_test_car_panel(void);
void test_status_LED(uint8_t iterations);
void test_control_LED(uint8_t iterations);

#endif /* TEST_MODULE_CAR_PANEL_H_ */