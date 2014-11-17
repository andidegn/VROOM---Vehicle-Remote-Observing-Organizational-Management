/********************************************//**
@file car_panel.c
@author: Kenneth René Jensen
@Version: 0.4
@defgroup cp Car_Panel
@{
	This is the driver for the car panel in VROOM system.
	The panel includes two tact switches, a RGB LED and a single LED.
@}
@note NOT YET Complies MISRO 2004 standards
************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#include "car_panel.h"
#include "../../accident_data.h"

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

/********************************************//**
 @ingroup cp
 @brief Initiates the Car Panel
 @return void
************************************************/
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
	
	/* Restore interrupt */
	SREG = SREG_cpy;
	
	car_panel_set_status(STATUS_OFFLINE);
}

/********************************************//**
 @ingroup cp
 @brief Enable external interrupt on ALARM button PJ1 (PCINT10)
 @return void
************************************************/
void car_panel_start(void)
{
	PCICR |= (1<<PCIE1);
	PCMSK1 |= (1<<PCINT10);
}

/********************************************//**
 @ingroup cp
 @brief Set the status LED
 @param STATUS_ATTENTION_TOGGLE - blue LED toggle
		STATUS_ATTENTION_CONSTANT - blue LED ON
		STATUS_ONLINE - green LED ON
		STATUS_OFFLINE - red LED ON
		STATUS_RESET - all LEDS OFF
 @return void
************************************************/
void car_panel_set_status(Status s)
{
	switch (s)
	{
		case STATUS_ATTENTION_TOGGLE :
			PORT &= ~(1<<STATUS_RED | 1<<STATUS_GREEN);
			PORT ^= (1<<STATUS_BLUE);
		break;
		
		case STATUS_ATTENTION_CONSTANT :
			PORT &= ~(1<<STATUS_RED | 1<<STATUS_GREEN);
			PORT |= (1<<STATUS_BLUE);
		break;
		
		case STATUS_ONLINE :
			PORT &= ~(1<<STATUS_RED | 1<<STATUS_BLUE);
			PORT |= (1<<STATUS_GREEN);
		break;

		case STATUS_OFFLINE :
			PORT &= ~(1<<STATUS_BLUE | 1<<STATUS_GREEN);
			PORT |= (1<<STATUS_RED);
		break;
		
		case STATUS_RESET :	
			PORT &= ~(1<<STATUS_RED | 1<<STATUS_GREEN | 1<<STATUS_BLUE);
		break;
	}
}

/********************************************//**
 @ingroup cp
 @brief Set the control LED
 @param ALARM_WAITING - LED toggle
		ALARM_ACTIVATED - LED ON
		ALARM_NOT_ACTIVATED - LED OFF
 @return void
************************************************/
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

/********************************************//**
 @ingroup cp
 @brief Cancel alarm button. Wait 3 seconds to determine false alarm
 @return true if alarm is canceled else false
************************************************/
bool car_panel_wait_cancel_emmergency(void)
{
	/* Saves the current state of the status register and disables global interrupt */
	uint8_t SREG_cpy = SREG;
	cli();

	_alarm_cancelled = false;
	_car_panel_counter = 0;	
	
	while (_car_panel_counter < BUTTON_PRESS_TIME)
	{					
		_car_panel_counter % 2 == 0 ? car_panel_set_status(STATUS_ATTENTION_TOGGLE) : car_panel_set_status(STATUS_ATTENTION_CONSTANT);			
				
		if(!(PIN(PORT) & (1<<CANCEL)))
		{
			_car_panel_counter = 0;
			while (_car_panel_counter < BUTTON_PRESS_TIME && !(PIN(PORT) & (1<<CANCEL)))
			{
				_delay_ms(100);
				_car_panel_counter++;
				car_panel_set_control(ALARM_WAITING);
				car_panel_set_status(STATUS_ATTENTION_TOGGLE);
			}
			_alarm_cancelled = (_car_panel_counter >= BUTTON_PRESS_TIME) ? true : false;
			car_panel_set_control(ALARM_NOT_ACTIVATED); 		
		}
		else
		{
			_delay_ms(100);
			_car_panel_counter++;			
		}	
	}
	
	_alarm_cancelled ? car_panel_set_control(ALARM_NOT_ACTIVATED) : car_panel_set_control(ALARM_ACTIVATED);		
	_alarm_cancelled ? car_panel_set_status(STATUS_RESET) : car_panel_set_status(STATUS_ATTENTION_CONSTANT);
					
	_car_panel_counter = 0;		
					
	/* Restore interrupt */
	SREG = SREG_cpy;	
	
	return _alarm_cancelled;
}

/********************************************//**
 @ingroup cp
 @brief ISR for ALARM button
************************************************/
ISR (PCINT1_vect)
{
	if(!(PIN(PORT) & (1<<ALARM)))
	{
		while (_car_panel_counter < BUTTON_PRESS_TIME && !(PIN(PORT) & (1<<ALARM)))
		{
			_delay_ms(100);
			_car_panel_counter++;
			car_panel_set_control(ALARM_WAITING);
		}

		if (_car_panel_counter >= BUTTON_PRESS_TIME )
		{
			/* Disable interrupts */
			PCMSK1 &= ~(1<<PCINT10);
			
			if (!car_panel_wait_cancel_emmergency())
			{
				EXT_EMERGENCY_FLAG = EMERGENCY_MANUAL_ALARM;
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
			_car_panel_counter = 0;
			car_panel_set_control(ALARM_NOT_ACTIVATED);
		}
    }
}
