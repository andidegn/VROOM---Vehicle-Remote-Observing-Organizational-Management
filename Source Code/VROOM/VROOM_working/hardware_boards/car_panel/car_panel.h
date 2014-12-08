/**********************************************************************//**
 * @file car_panel.h
 *
 * @author Kenneth René Jensen
 * @cersion 0.4
 * @defgroup cp Car Panel driver
 * @{
	@ingroup hardware_boards
	@brief This is the driver for the car panel in VROOM system.
	The panel includes two tact switches, a RGB LED and a single LED.
 	@note NOT YET Complies MISRA 2004 standards
 	@defgroup cp_pub Public
	@defgroup cp_priv Private
 * @}
 *************************************************************************/

#ifndef CAR_PANEL_H_
#define CAR_PANEL_H_

#include <stdbool.h>
/**********************************************************************//**
 * @ingroup cp_pub
 * @brief Defines the state of status LED
 *************************************************************************/
typedef enum
{
	STATUS_BLUE_TOGGLE,
	STATUS_BLUE,
	STATUS_RED,
	STATUS_GREEN,
	STATUS_RESET
} Status;

/**********************************************************************//**
 * @ingroup cp_pub
 * @brief Defines the state of control LED
 *************************************************************************/
typedef enum
{
	ALARM_WAITING,
	ALARM_ACTIVATED,
	ALARM_NOT_ACTIVATED
} Control;

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

/**********************************************************************//**
 * @ingroup cp_pub
 * @brief Sets the activation state of the alarm button
 *
 * @param bool state - true = activate; false = de-activate
 *
 * @return void
 *************************************************************************/
void car_panel_set_alarm_button_state(bool state);

/**********************************************************************//**
 * @ingroup cp_pub
 * @brief Sets the activation state of the cancel button
 *
 * @param bool state - true = activate; false = de-activate
 *
 * @return void
 *************************************************************************/
void car_panel_set_cancel_button_state(bool state);

#endif /* CAR_PANEL_H_ */