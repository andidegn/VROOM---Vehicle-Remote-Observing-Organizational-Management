/*
 * lop.h
 *
 * Created: 14-09-2014 13:31:02
 *  Author: Andi Degn
 */



#ifndef LOP_H_
#define LOP_H_

#include <avr/io.h>

typedef enum {
	LOP_GREEN = _BV(PB5),
	LOP_RED = _BV(PB6),
	LOP_YELLOW = _BV(PB7)
} LOP_COLOR;

#define LOP_ON 1
#define LOP_OFF 0

void lop_init(void);

void lop_set_led(LOP_COLOR led, uint8_t state);
void lop_toggle_led(LOP_COLOR led);

#endif /* LOP_H_ */