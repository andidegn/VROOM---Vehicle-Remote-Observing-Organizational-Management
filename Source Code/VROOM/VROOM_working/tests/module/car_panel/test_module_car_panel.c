/********************************************//**
@file test_module_car_panel.c
@author: Kenneth René Jensen
@Version: 0.1
@defgroup test Module Test - Car Panel
@{
	Test for for the Car Panel module
@}
************************************************/
#include "test_module_car_panel.h"
#include <util/delay.h>
#include "../../../hardware_boards/car_panel/car_panel.h"

void init_module_test_car_panel(void)
{	
	car_panel_init();
	car_panel_start();
}

void test_status_LED(uint8_t iterations)
{
	uint8_t i = 0;
	for (i = 0; i < iterations; i++)
	{
		car_panel_set_status(STATUS_BLUE);
		_delay_ms(500);
		car_panel_set_status(STATUS_GREEN);
		_delay_ms(500);
		car_panel_set_status(STATUS_RED);
		_delay_ms(500);
	}
}

void test_control_LED(uint8_t iterations)
{
	uint8_t i = 0;
	for (i = 0; i < iterations; i++)
	{
		car_panel_set_control(ALARM_WAITING);
		_delay_ms(500);
		car_panel_set_control(ALARM_ACTIVATED);
		_delay_ms(500);		
		car_panel_set_control(ALARM_NOT_ACTIVATED);
		_delay_ms(500);
	}
}


