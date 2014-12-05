/********************************************//**
@file timer.c
@author: Kenneth René Jensen
@Version: 0.3
@defgroup timer Timer
@{
	Setup of timer 1 to CTC mode for stating the scheduler.
@}
************************************************/
#include <avr/interrupt.h>
#include "timer.h"
#include "../../vroom_config.h"
#include "../../scheduler.h"

#ifndef F_CPU
	#error F_CPU must be defined!!!
#endif

/*! Macro for calculating the value for clock count based on frequency and prescaler */
#define TOP_VALUE(frequency_in_hz, prescaler) (F_CPU/(prescaler * frequency_in_hz)-1)

static uint8_t _TIFR1_cpy = 0;
static uint8_t _TIMSK1_cpy = 0;

/**********************************************************************//**
 @ingroup timer
 @brief Initiates timer 1 in CTC mode
 @param TIMER_PRESCALER enum, TIMER_FREQUENCY enum
 @note Enables interrupt
*************************************************************************/
void timer1_init_CTC(TIMER_PRESCALER prescaler, TIMER_FREQUENCY hz)
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

/**********************************************************************//**
 @ingroup timer
 @brief Stores the mask and interrupt register for Timer 1 and Timer 3 and disables interrupts
*************************************************************************/
void timer_pause_all(void)
{
	_TIFR1_cpy = TIFR1;
	_TIMSK1_cpy = TIMSK1;
	TIMSK1 &= ~_BV(OCIE1A);
}

/**********************************************************************//**
 @ingroup timer
 @brief Restores the mask and interrupt register for Timer 1 and Timer 3 and enables interrupts
*************************************************************************/
void timer_resume_all(void)
{
	TIFR1 = _TIFR1_cpy;
	TIMSK1 = _TIMSK1_cpy;
}

/**********************************************************************//**
 @ingroup timer
 @brief Disables interrupt for timer1 and timer3
*************************************************************************/
void timer_stop_all(void)
{
	TIMSK1 &= ~_BV(OCIE1A);
}

/**********************************************************************//**
 @ingroup timer
 @brief Enables interrupt for timer1 and timer3
*************************************************************************/
void timer_start_all(void)
{
	TIFR1 |= _BV(OCIE1A);
	TIMSK1 |= _BV(OCIE1A);
}

ISR(TIMER1_COMPA_vect)
{
	scheduler_release();
}
