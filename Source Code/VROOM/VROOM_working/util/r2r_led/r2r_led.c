/**********************************************************************//**
 * r2r_led.c
 *************************************************************************/

#include "r2r_led.h"

/**********************************************************************//**
 * @ingroup dac_priv
 * Macro setting up the desired port
 *************************************************************************/
#define DDR(PORT) (*(&PORT - 1))

/**********************************************************************//**
* @ingroup dac_priv
 * Defined mask to differentiate
 * between the DAC and the LEDs
 *************************************************************************/
#define mask 0b11110000

/**
 * Local variable
 */
static uint8_t _current_id = 0;

/**********************************************************************//**
 * @ingroup dac_pub
 * Setting up desired port using the setup_port() macro
 *************************************************************************/
void r2r_init() {
	DDR(R2R_PORT) |= 0xff;
}

/**********************************************************************//**
 * @ingroup dac_pub
 * Setting the state of the chosen LED using the
 * set_led_on() and set_led_off() macroes
 *************************************************************************/
void r2r_led(uint8_t led, uint8_t state) {
	if (state == R2R_LED_ON) {
		R2R_PORT |= _BV(led);
	} else {
		R2R_PORT &= ~_BV(led);
	}
}

/**********************************************************************//**
 * @ingroup dac_pub
 * Toggling the chosen LED using the toggle_led() macro
 *************************************************************************/
void r2r_led_toggle(uint8_t led) {
	R2R_PORT ^= _BV(led);
}

/**********************************************************************//**
 * @ingroup dac_pub
 * Prints the value in binary on the LEDs
 * guard in place to make sure overflow is handled
 *************************************************************************/
void r2r_led_print(uint8_t value) {
	uint8_t _value = value % 16;
	R2R_PORT &= ~mask | _value;
	R2R_PORT |= _value << 4;
}

/**********************************************************************//**
 * @ingroup dac_pub
 * Switches all the LEDs on using the defined mask
 *************************************************************************/
void r2r_leds_on() {
	R2R_PORT |= mask;
}

/**********************************************************************//**
 * @ingroup dac_pub
 * Switching all the LEDs off using one's compliment
 * of the defined mask
 *************************************************************************/
void r2r_leds_off() {
	R2R_PORT &= ~mask;
}

/**********************************************************************//**
 * @ingroup dac_pub
 * Passing a value to the DAC using one's complement
 * of the defined mask
 * guard in place to make sure overflow is handled
 *************************************************************************/
void r2r_set_dac(uint8_t __value) {
	uint8_t _value = __value % 16;
	R2R_PORT &= mask | _value;
	R2R_PORT |= _value;
}

/**********************************************************************//**
 * @ingroup dac_pub
 * Sets the DAC to the __id and returns the id it already had
 *************************************************************************/
uint8_t r2r_start_task(uint8_t __id) {
	uint8_t _tmp_id = _current_id;
	_current_id = __id;
	r2r_set_dac(_current_id);
	return _tmp_id;
}

/**********************************************************************//**
 * @ingroup dac_pub
 * Sets the DAC to the previous id
 *************************************************************************/
void r2r_stop_task(uint8_t __prev_id) {
	_current_id = __prev_id;
	r2r_set_dac(_current_id);
}

