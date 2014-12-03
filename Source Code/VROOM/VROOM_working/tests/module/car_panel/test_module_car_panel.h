/*
 * test_module_car_panel.h
 *
 * Created: 28-11-2014 08:50:34
 *  Author: Kenneth
 */ 


#ifndef TEST_MODULE_CAR_PANEL_H_
#define TEST_MODULE_CAR_PANEL_H_

#include <stdint.h>

void init_module_test_car_panel(void);
void test_status_LED(uint8_t iterations);
void test_control_LED(uint8_t iterations);

#endif /* TEST_MODULE_CAR_PANEL_H_ */