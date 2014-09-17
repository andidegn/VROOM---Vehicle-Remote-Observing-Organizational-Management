/********************************************//**
@file timer.c
@author: Kenneth Ren� Jensen
@Version: 0.2
@defgroup timer Timer
@{
	Setup of timer 1 to CTC mode.
@}
************************************************/
#include "timer.h"

#ifndef F_CPU
	#error F_CPU must be defined!!!
#endif

/*! Macro for calculating the value for clock count based on frequency and prescaler */
#define TOP_VALUE(frequency_in_hz, prescaler) (F_CPU/(prescaler * frequency_in_hz)-1)

/********************************************************************************************************************//**
 @ingroup timer
 @brief Initiates timer 1 in CTC mode. Enables interrupt based on topvalue
 @param TIMER_PRESCALER enum, TIMER_FREQUENCY enum
************************************************************************************************************************/
void init_Timer1_CTC(TIMER_PRESCALER prescaler, TIMER_FREQUENCY hz)
{
	/* saves the current state of the status register and disables global interrupt */
	uint8_t _sreg = SREG;
	cli();

	/* Set top value to */
	OCR1A = TOP_VALUE(hz, prescaler);

	/* Set Timer mode 4: CTC */
	TCCR1B |= _BV(WGM12);

	/* Set prescaler */
	switch(prescaler)
	{
		case TIMER_PS1 :
			TCCR1B &= ~_BV(CS11) & ~_BV(CS12);
			TCCR1B |= _BV(CS10);
		break;

		case TIMER_PS8 :
			TCCR1B &= ~_BV(CS10) & ~_BV(CS12);
			TCCR1B |= _BV(CS11);
		break;

		case TIMER_PS64 :
			TCCR1B &= ~_BV(CS12);
			TCCR1B |= _BV(CS10) | _BV(CS11);
		break;

		case TIMER_PS256 :
			TCCR1B &= ~_BV(CS10) & ~_BV(CS11);
			TCCR1B |= _BV(CS12);
		break;

		case TIMER_PS1024 :
			TCCR1B |= _BV(CS10) | _BV(CS11) | _BV(CS12);
		break;

		default: break;
	}

	/* Enable interrupts */
	TIMSK1 |= _BV(OCIE1A);

	/* restoring status register */
	SREG = _sreg;
}

ISR(TIMER1_COMPA_vect)
{
	scheduler_release();
}
