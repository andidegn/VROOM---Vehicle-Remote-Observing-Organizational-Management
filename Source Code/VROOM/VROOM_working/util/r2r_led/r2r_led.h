#ifndef R2R_LED_H_
#define R2R_LED_H_

/**********************************************************************//**
 * @file r2r_led.h
 *
 * @created: 03-04-2014 19:26:46
 * @author: Andi Degn
 * @version 1.2
 * @defgroup dac 4bit R2R with LEDs
 * @ingroup test_r2r
 * @{
 *	This is a driver for the 4bit R2R DAC with 4 LEDs
 *  @defgroup dac_pub Public
 *  @defgroup dac_priv Private
 * @}
 *************************************************************************/

#include <avr/io.h>
#include "../../application/vroom_config.h"

/**********************************************************************//**
 * @ingroup dac_pub
 * @brief Port define for the r2r_led
 * @defgroup dac_port Port
 * @{
 *************************************************************************/
#define R2R_PORT CONFIG_PORT_R2R
/** @} */

/**********************************************************************//**
 * @ingroup dac_pub
 * LED on/off define
 * @defgroup dac_led LED define
 * @{
 *************************************************************************/
#define R2R_LED_ON 1
#define R2R_LED_OFF 0
/** @} */

/**********************************************************************//**
 * @ingroup dac_pub
 * R2R pin define
 * @defgroup dac_pins Pins
 * @{
 *************************************************************************/
#define R2R_BIT_0 0
#define R2R_BIT_1 1
#define R2R_BIT_2 2
#define R2R_BIT_3 3
#define R2R_LED_0 4
#define R2R_LED_1 5
#define R2R_LED_2 6
#define R2R_LED_3 7
/** @} */

/**********************************************************************//**
 * @ingroup dac_pub
 * Initializes the port for the R2R_LED board
 *************************************************************************/
void r2r_init(void);

/**********************************************************************//**
 * @ingroup dac_pub
 * Sets the state of the chosen LED
 * @param uint8_t led - the chosen led: R2R_LED_n
 * @param uint8_t state - the state: R2R_LED_ON / R2R_LED_OFF
 *************************************************************************/
void r2r_led(uint8_t led, uint8_t state);

/**********************************************************************//**
 * @ingroup dac_pub
 * Toggles the chosen LED: R2R_LED_n
 * @note: If connected to a button, continuality has to be handled
 *************************************************************************/
void r2r_led_toggle(uint8_t led);

/**********************************************************************//**
 * @ingroup dac_pub
 * Prints the binary value on the LEDs
 * @param uint8_t value - a value in the range 0-15
 *************************************************************************/
void r2r_led_print(uint8_t value);

/**********************************************************************//**
 * @ingroup dac_pub
 * Switches all the LEDs on
 *************************************************************************/
void r2r_leds_on(void);

/**********************************************************************//**
 * @ingroup dac_pub
 * Switches all the LEDs off
 *************************************************************************/
void r2r_leds_off(void);

/**********************************************************************//**
 * @ingroup dac_pub
 * Passes a value to the DAC
 *
 * @param uint8_t value - a value in the range 0-15
 *
 * @return void
 *************************************************************************/
void r2r_set_dac(uint8_t __value);

/**********************************************************************//**
 * @ingroup dac_pub
 * @brief Starts a task measure and returns the id of the already running task
 *
 * @param uint8_t __id - the id of the task to be measured
 *
 * @return void
 * @note the __id must be larger than zero
 *************************************************************************/
uint8_t r2r_start_task(uint8_t __id);

/**********************************************************************//**
 * @ingroup dac_pub
 * @brief Takes the value of the previous task and stops measuring
 *
 * @param uint8_t __prev_id - the id of the task that was returned when R2R_start_task() was called
 *
 * @return void
 * @note the __id must be larger than zero
 *************************************************************************/
void r2r_stop_task(uint8_t __prev_id);

#endif /* R2R_LED_H_ */