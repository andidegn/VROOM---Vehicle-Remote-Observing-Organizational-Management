/********************************************//**
@file car_panel.c
@author: Kenneth René Jensen
@Version: 0.1
@defgroup
@{

@}
@note NOT YET Complies MISRO 2004 standards
************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#include "car_panel.h"

/* Changing port will require changes in the interrupt setup */
#define PORT			PORTJ

#define DDR(x) (*(&x - 1))
#define PIN(x) (*(&x - 2))

#define CANCEL			0
#define ALARM			1
#define CONTROL			3
#define STATUS_RED		5
#define STATUS_BLUE		6
#define STATUS_GREEN	7

/* Local variables */
static uint8_t _car_panel_counter;
static bool _alarm_cancelled;

void car_panel_init(void)
{
	/* Saves the current state of the status register and disables global interrupt */
	uint8_t SREG_cpy = SREG;
	cli();

	/* Set buttons to input and LEDs to output */
	DDR(PORT) &= ~(1<<CANCEL | 1<<ALARM);
	DDR(PORT) |= (1<<CONTROL) | (1<<STATUS_RED) | (1<<STATUS_BLUE) | (1<<STATUS_GREEN);
	
	/* Pull-up on buttons */
	PORT |= (1<<CANCEL | 1<<ALARM);
	
	/* Setup external interrupts for PJ1 (PCINT10) */
	PCICR |= (1<<PCIE1);
	PCMSK1 |= (1<<PCINT10);
	
	/* Restore interrupt */
	SREG = SREG_cpy;
}

void car_panel_set_status(Status s)
{
	switch (s)
	{
		case INIT :
			PORT &= ~(1<<STATUS_RED | 1<<STATUS_GREEN);
			PORT |= (1<<STATUS_BLUE);
		break;

		case ONLINE :
			PORT &= ~(1<<STATUS_RED | 1<<STATUS_BLUE);
			PORT |= (1<<STATUS_GREEN);
		break;

		case OFFLINE :
			PORT &= ~(1<<STATUS_BLUE | 1<<STATUS_GREEN);
			PORT |= (1<<STATUS_RED);
		break;
	}
}

void car_panel_set_control(Control c)
{
	switch (c)
	{
		case ALARM_WAITING :
			PORT ^= (1<<CONTROL);
		break;

		case ALARM_ACTIVATED :
			PORT |= (1<<CONTROL);
		break;

		case ALARM_NOT_ACTIVATED :
			PORT &= ~(1<<CONTROL);
		break;
	}
}

bool car_panel_wait_cancel_emmergency(void)
{
	/* Saves the current state of the status register and disables global interrupt */
	uint8_t SREG_cpy = SREG;
	cli();

	_alarm_cancelled = false;
	_car_panel_counter = 0;
	
	while (_car_panel_counter < BUTTON_PRESS_TIME)
	{						
		if(!(PIN(PORT) & (1<<CANCEL)))
		{
			_car_panel_counter = 0;
			while (_car_panel_counter < BUTTON_PRESS_TIME && !(PIN(PORT) & (1<<CANCEL)))
			{
				_delay_ms(100);
				_car_panel_counter++;
				
				if (_car_panel_counter % 5 == 0)
				{
					 car_panel_set_control(ALARM_WAITING);
				}
			}
			
			if (_car_panel_counter >= BUTTON_PRESS_TIME )
			{
				car_panel_set_control(ALARM_NOT_ACTIVATED);
				_alarm_cancelled = true;		
			}
			else
			{
				car_panel_set_control(ALARM_ACTIVATED);
				_alarm_cancelled = false;	
			}
		}
		else
		{
			_delay_ms(100);
			_car_panel_counter++;
			_alarm_cancelled = false;
		}
	}
				
	/* Restore interrupt */
	SREG = SREG_cpy;	
	
	return _alarm_cancelled;
}

ISR (PCINT1_vect)
{
	if(!(PIN(PORT) & (1<<ALARM)))
	{
		while (_car_panel_counter < BUTTON_PRESS_TIME && !(PIN(PORT) & (1<<ALARM)))
		{
			_delay_ms(100);
			_car_panel_counter++;
			if (_car_panel_counter % 5 == 0)
			{
				car_panel_set_control(ALARM_WAITING);
			}
		}

		if (_car_panel_counter >= BUTTON_PRESS_TIME )
		{
			/* Disable interrupts */
			PCMSK1 &= ~(1<<PCINT10);
		
			if (!car_panel_wait_cancel_emmergency())
			{
				/* ToDo */
				// SEND ALARM !!!
			}
			else
			{
				_car_panel_counter = 0;
				/* Enable interrupts */
				PCMSK1 |= (1<<PCINT10);
			}
		}
		
		else
		{
			car_panel_set_control(ALARM_NOT_ACTIVATED);
		}
    }
	
	else
	{
		_car_panel_counter = 0;
	}
}
