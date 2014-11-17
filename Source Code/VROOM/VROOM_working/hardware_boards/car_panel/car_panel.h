/********************************************//**
@file car_panel.h
@author: Kenneth René Jensen
@Version: 0.4
@defgroup
@{
	This is the driver for the car panel in VROOM system.
	The panel includes two tact switches, a RGB LED and a single LED.
@}
@note NOT YET Complies MISRO 2004 standards
************************************************/

#ifndef CAR_PANEL_H_
#define CAR_PANEL_H_

#include <stdbool.h>

/* Time of button press before activation/deactivation in 1/10 sec */
#define BUTTON_PRESS_TIME		30

/**********************************************************************//**
 * @ingroup ad
 * @brief Defines the state of status LED
 *************************************************************************/
typedef enum
{
	STATUS_ATTENTION_TOGGLE,
	STATUS_ATTENTION_CONSTANT,
	STATUS_OFFLINE,
	STATUS_ONLINE,
	STATUS_RESET
} Status;

/**********************************************************************//**
 * @ingroup ad
 * @brief Defines the state of control LED
 *************************************************************************/
typedef enum
{
	ALARM_WAITING,
	ALARM_ACTIVATED,
	ALARM_NOT_ACTIVATED
} Control;

/********************************************//**
 @ingroup cp
 @brief Initiates the Car Panel
 @return void
************************************************/
void car_panel_init(void);

/********************************************//**
 @ingroup cp
 @brief Enable external interrupt on ALARM button PJ1 (PCINT10)
 @return void
************************************************/
void car_panel_start(void);

/********************************************//**
 @ingroup cp
 @brief Set the status LED
 @param STATUS_ATTENTION_TOGGLE - blue LED toggle
		STATUS_ATTENTION_CONSTANT - blue LED ON
		STATUS_ONLINE - green LED ON
		STATUS_OFFLINE - red LED ON
		STATUS_RESET - all LEDS OFF
 @return void
************************************************/
void car_panel_set_status(Status s);

/********************************************//**
 @ingroup cp
 @brief Set the control LED
 @param ALARM_WAITING - LED toggle
		ALARM_ACTIVATED - LED ON
		ALARM_NOT_ACTIVATED - LED OFF
 @return void
************************************************/
void car_panel_set_control(Control c);

/********************************************//**
 @ingroup cp
 @brief Cancel alarm button. Wait 3 seconds to determine false alarm
 @return true if alarm is canceled else false
************************************************/
bool car_panel_wait_cancel_emmergency(void);

#endif /* CAR_PANEL_H_ */