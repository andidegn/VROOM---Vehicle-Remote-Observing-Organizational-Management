/*
 * @file car_panel.h
 *
 * @author: Kenneth René Jensen
 * @Version: 0.4
 * @defgroup cp Car Panel driver
 * @{
	This is the driver for the car panel in VROOM system.
	The panel includes two tact switches, a RGB LED and a single LED.
	@defgroup cp_priv Private
	@defgroup cp_pub Public
 * @}
 * @note NOT YET Complies MISRO 2004 standards
 */

#ifndef CAR_PANEL_H_
#define CAR_PANEL_H_

/**********************************************************************//**
 * @ingroup cp_pub
 * @brief Defines for the time of button press before activation/deactivation in 1/10 sec
 * @{
 *************************************************************************/
#define BUTTON_PRESS_TIME		30
/* @} */

/**********************************************************************//**
 * @ingroup cp_pub
 * @brief Defines the state of status LED
 * @{
 *************************************************************************/
typedef enum
{
	STATUS_ATTENTION_TOGGLE,
	STATUS_ATTENTION_CONSTANT,
	STATUS_OFFLINE,
	STATUS_ONLINE,
	STATUS_RESET
} Status;
/* @} */

/**********************************************************************//**
 * @ingroup cp_pub
 * @brief Defines the state of control LED
 * @{
 *************************************************************************/
typedef enum
{
	ALARM_WAITING,
	ALARM_ACTIVATED,
	ALARM_NOT_ACTIVATED
} Control;
/* @} */

/**********************************************************************//**
 * @ingroup cp_pub
 * @brief Initiates the Car Panel
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void car_panel_init(void);

/**********************************************************************//**
 * @ingroup cp_pub
 * @brief Enable external interrupt on ALARM button PJ1 (PCINT10)
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void car_panel_start(void);

/**********************************************************************//**
 * @ingroup cp_pub
 * @brief Set the status LED
 *
 * @param Status __s - status of the panel. See Status define
 *
 * @return void
 *************************************************************************/
void car_panel_set_status(Status __s);

/**********************************************************************//**
 * @ingroup cp_pub
 * @brief Set the control LED
 *
 * @param Control __c - control of the panel. See Control define
 *
 * @return void
 *************************************************************************/
void car_panel_set_control(Control __c);

/**********************************************************************//**
 * @ingroup cp_pub
 * @brief Cancel alarm button. Wait 3 seconds to determine false alarm
 *
 * @param void
 *
 * @return bool - true if alarm is canceled else false
 *************************************************************************/
bool car_panel_wait_cancel_emmergency(void);

#endif /* CAR_PANEL_H_ */