/********************************************//**
@file timer.c
@author: Kenneth René Jensen
@Version: 0.3
@defgroup timer Timer
@{
	Setup of timer 1 to CTC mode for stating the scheduler.
	Setup of timer 3 to CTC mode for counting etc. used for timeout
@}
************************************************/
#include <avr/interrupt.h>
#include "timer.h"
#include "../../vroom_config.h"
#include "../../sensors/sensor_scheduler.h"
#include "../../accident_logic/accident_detection.h"

#ifndef F_CPU
	#error F_CPU must be defined!!!
#endif

/*! Macro for calculating the value for clock count based on frequency and prescaler */
#define TOP_VALUE(frequency_in_hz, prescaler) (F_CPU/(prescaler * frequency_in_hz)-1)

static volatile uint8_t _isr_counter = 0;
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
 @brief Initiates timer 3 in CTC mode.
 @param TIMER_PRESCALER enum, TIMER_FREQUENCY enum
*************************************************************************/
void timer3_init_CTC(TIMER_PRESCALER prescaler, TIMER_FREQUENCY hz)
{
	/* saves the current state of the status register and disables global interrupt */
	uint8_t _sreg = SREG;
	cli();

	/* Set top value to */
	OCR3A = TOP_VALUE(hz, prescaler);

	/* Set Timer mode 4: CTC */
	TCCR3B |= _BV(WGM32);

	/* Set prescaler */
	switch(prescaler)
	{
		case TIMER_PS1 :
		TCCR3B &= ~_BV(CS31) & ~_BV(CS32);
		TCCR3B |= _BV(CS30);
		break;

		case TIMER_PS8 :
		TCCR3B &= ~_BV(CS30) & ~_BV(CS32);
		TCCR3B |= _BV(CS31);
		break;

		case TIMER_PS64 :
		TCCR3B &= ~_BV(CS32);
		TCCR3B |= _BV(CS30) | _BV(CS31);
		break;

		case TIMER_PS256 :
		TCCR3B &= ~_BV(CS30) & ~_BV(CS31);
		TCCR3B |= _BV(CS32);
		break;

		case TIMER_PS1024 :
		TCCR3B |= _BV(CS30) | _BV(CS31) | _BV(CS32);
		break;

		default: break;
	}

	/* restoring status register */
	SREG = _sreg;
}

/**********************************************************************//**
 @ingroup timer
 @brief Enables interrupt for timer3
*************************************************************************/
void timer3_start(void)
{
	TIFR3 |= _BV(OCIE3A);
	//TIFR |= (1 << OCF1A);
	/* Enable interrupts */
	TIMSK3 |= _BV(OCIE3A);
}

/**********************************************************************//**
 @ingroup timer
 @brief Disables interrupt for timer3
*************************************************************************/
void timer3_stop(void)
{
	/* Disable interrupts */
	TIMSK3 &= ~_BV(OCIE3A);
}

/**********************************************************************//**
 @ingroup timer
 @brief Disables interrupt for timer1 and timer3
*************************************************************************/
void timer_pause_all(void) {
	TIMSK3 &= ~_BV(OCIE3A);
	TIMSK1 &= ~_BV(OCIE1A);
}

/**********************************************************************//**
 @ingroup timer
 @brief Enables interrupt for timer1 and timer3
*************************************************************************/
void timer_resume_all(void) {
	TIMSK3 |= _BV(OCIE3A);
	TIMSK1 |= _BV(OCIE1A);
}

ISR(TIMER1_COMPA_vect)
{
	scheduler_release();
}

ISR(TIMER3_COMPA_vect)
{
	/* saves the current state of the status register and disables global interrupt */
	uint8_t _sreg = SREG;
	cli();

	ad_check_for_crash();

	if (_isr_counter++%CONFIG_ALARM_FIRE_TRIGGER_TIME == 0)
		ad_check_for_fire();

	/* restoring status register */
	SREG = _sreg;
}
