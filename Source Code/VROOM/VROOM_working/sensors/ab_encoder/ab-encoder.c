/*
* ab_encoder.c
*
* Created: 15-03-2014 15:05:25
*  Author: IHA
*/

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ab-encoder.h"

/**
* This module interfaces a AB incremental Encoder.
*
* @note: The module uses the two external interrupt INT6 and INT7
*
* @defgroup AB_encoder AB Encoder
*/
// @{
#define AB_ENCODER_READ_AB ((PINE >> 6) & 3)

static int8_t _current_state;
static int16_t _current_position = 0;

static const uint8_t _STATE[] =
{
	0, 1, 3, 2
};

/**
* Initialises the incremental Encoder interface.
* The position of the encoder is reset to 0.
* @note A call to this function enables the two external interrupts INT6 and INT7
*/
void ab_encoder_init(void)
{
	// Set PE7 & PE6 to input
	DDRE &= ~(_BV(DDE7) | _BV(DDE6));

	/* Setup to external interrupt 6 (A) and 7 (B) for any logical level change */
	EICRB &= ~(_BV(ISC71) | _BV(ISC61));
	EICRB |= _BV(ISC70) | _BV(ISC60);
	EIMSK |= _BV(INT7) | _BV(INT6);

	_current_state = _STATE[ AB_ENCODER_READ_AB ];
}

/**
* Returns the current position of the incremental Encoder
* @param reset_after_read if true (!=0) will the current position be reset after it is read.
* @return current position of encoder
*/
int16_t ab_encoder_current_position(uint8_t reset_after_read)
{
	// disable interrupt
	char cSREG = SREG;
	cli();

	// this driver counts on each edge of the AB pulses, and the AB-encoder on the board steps four edges for each position
	// so we divide with four before returning the value
	int16_t ret = _current_position >> 2;
	if ( reset_after_read )
	{
		_current_position = 0;
	}

	// Restore interrupt
	SREG = cSREG;

	return ret;
}

/**
* Interrupt handler for INT6 interrupts
* SIGNAL is used because nested interrupt is then automatically disabled
* This interrupt handles the A-output from the incremental Encoder
*/
ISR(INT6_vect)
{
	int8_t newState = _STATE[ AB_ENCODER_READ_AB ];
	int8_t diff = newState - _current_state;

	if ( diff == 1 || diff < -1 )
	{
		_current_position--;
	}
	else if ( diff != 0 )
	{
		_current_position++;
	}

	_current_state = newState;
}

/**
* Interrupt handler for INT7 interrupts
* SIGNAL is used because nested interrupt is then automatically disabled
* This interrupt handles the A-output from the incremental Encoder
*/
ISR(INT7_vect)
{
	int8_t newState = _STATE[ AB_ENCODER_READ_AB ];
	int8_t diff = newState - _current_state;

	if ( diff == 1 || diff < -1 )
	{
		_current_position--;
	}
	else if ( diff != 0 )
	{
		_current_position++;
	}

	_current_state = newState;
}
// @}