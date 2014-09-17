/*
 * lop.c
 *
 * Created: 14-09-2014 13:31:18
 *  Author: Andi Degn
 */

#include "lop.h"

void lop_init(void) {
	DDRB |= _BV(DDB5) | _BV(DDB6) | _BV(DDB7);
	PORTB |= LOP_RED | LOP_GREEN | LOP_YELLOW;
}

void lop_set_led(LOP_COLOR led, uint8_t state) {
	if (state == LOP_ON) {
		PORTB &= ~led;
	} else {
		PORTB |= led;
	}
}
void lop_toggle_led(LOP_COLOR led) {
	PORTB ^= led;
}
