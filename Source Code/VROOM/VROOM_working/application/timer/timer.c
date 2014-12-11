/********************************************************************//**
 * @file timer.c
 ***********************************************************************/
#include <avr/interrupt.h>
#include "timer.h"
#include "../vroom_config.h"
#include "../scheduler/scheduler.h"

#ifndef F_CPU
	#error F_CPU must be defined!!!
#endif


/**********************************************************************//**
 * @ingroup timer_priv
 * @brief Macro for calculating the value for clock count based on frequency and prescaler
 * @defgroup timer_top_value Timer top value equation
 * @{
 *************************************************************************/
#define TOP_VALUE(frequency_in_hz, prescaler) (F_CPU/(prescaler * frequency_in_hz)-1)
/** @} */

static uint8_t _TIFR1_cpy = 0U;
static uint8_t _TIMSK1_cpy = 0U;

/********************************************************************//**
 * @ingroup timer_pub
 * Sets up all the ports and registers for timer1
 ***********************************************************************/
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

/********************************************************************//**
 * @ingroup timer_pub
 * Stores a copy of registers and disables the timer mask interrupt
 ***********************************************************************/
void timer_pause(void)
{
	_TIFR1_cpy = TIFR1;
	_TIMSK1_cpy = TIMSK1;
	TIMSK1 &= ~_BV(OCIE1A);
}

/********************************************************************//**
 * @ingroup timer_pub
 * Sets the registers back to the stored copy
 ***********************************************************************/
void timer_resume(void)
{
	TIFR1 = _TIFR1_cpy;
	TIMSK1 = _TIMSK1_cpy;
}

/********************************************************************//**
 * @ingroup timer_pub
 * Disable the timer mask interrupt
 ***********************************************************************/
void timer_stop(void)
{
	TIMSK1 &= ~_BV(OCIE1A);
}

/********************************************************************//**
 * @ingroup timer_pub
 * Enables the timer mask interrupt
 ***********************************************************************/
void timer_start(void)
{
	TIFR1 |= _BV(OCIE1A);
	TIMSK1 |= _BV(OCIE1A);
}

/********************************************************************//**
 * @ingroup timer_priv
 * Interrupt service routine which releases the scheduler
 ***********************************************************************/
ISR(TIMER1_COMPA_vect)
{
	scheduler_release();
}
