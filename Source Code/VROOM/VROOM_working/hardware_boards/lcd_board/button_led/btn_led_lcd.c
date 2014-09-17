/*
 * btn_led_lcd.c
 *
 * Created: 14-09-2014 22:43:28
 *  Author: Andi Degn
 */

#include "btn_led_lcd.h"

#define DDR(x) (*(&x - 1))
#define PIN(x) ( &PORTF==&(x) ? _SFR_IO8(0x00) : (*(&x - 2)) )

void btn_led_lcd_init() {
	DDR(BTN_LED_PORT) &= ~0x0f;
	DDR(BTN_LED_PORT) |= 0x30;
	BTN_LED_PORT |= _BV(BTN_PIN0) | _BV(BTN_PIN1) | _BV(BTN_PIN2) | _BV(BTN_PIN3);
	led_lcd_set(LED_GREEN, LED_OFF);
	led_lcd_set(LED_RED, LED_OFF);
}

bool btn_lcd_is_pressed(uint8_t pin) {
	return PIN(BTN_LED_PORT) & _BV(pin);
}

void led_lcd_set(uint8_t led, uint8_t state) {
	if (led >= LED_GREEN && led <= LED_RED) {
		if (state == LED_ON) {
			BTN_LED_PORT |= _BV(led);
		} else {
			BTN_LED_PORT &= ~_BV(led);
		}
	}
}

void led_lcd_toggle(uint8_t led) {
	BTN_LED_PORT ^= _BV(led);
}
