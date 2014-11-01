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
#include "../lcd_board/lcd/lcd.h"

/* Changing port will maybe require changes in the interrupt setup */
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

void car_panel_init(void)
{
	/* Saves the current state of the status register and disables global interrupt */
	uint8_t SREG_cpy = SREG;
	cli();

	/* Set buttons to input and LEDs to output */
	DDR(PORT) &= ~(1<<CANCEL | 1<<ALARM);
	PORT |= _BV(CANCEL) | _BV(ALARM);
	DDR(PORT) |= (1<<CONTROL) | (1<<STATUS_RED) | (1<<STATUS_BLUE) | (1<<STATUS_GREEN);

	/* Setup external interrupts for PJ1 (PCINT10) */
	PCICR |= (1<<PCIE1);
	PCMSK1 |= (1<<PCINT10);
	//init_Timer3_CTC(TIMER_PS256, TIMER_10HZ);
	//start_timer3();
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
		case WAITING :
			PORT ^= (1<<CONTROL);
		break;

		case ACTIVATED :
			PORT |= (1<<CONTROL);
		break;

		case DEACTIVATED :
			PORT &= ~(1<<CONTROL);
		break;
	}
}

void wait_cancel_emmergency()
{
	// Implement 3 sec. timer
	while ((PIN(PORT) & (1<<CANCEL)));
}
volatile char buf[10];
ISR (PCINT1_vect)
{

	if(!(PIN(PORT) & (1<<ALARM)))
	{
		/* PCINT10 changed */
		// ToDo
		// Start 3 seconds timer
		// Check if bit has changed
		//   if bit has not changed within 3 seconds, trigger ALARM
		//    else break
		// Stop timer
		while (_car_panel_counter < BUTTON_PRESS_TIME && !(PIN(PORT) & (1<<ALARM)))
		{
			_delay_ms(100);
			if (_car_panel_counter++ % 5 == 0) /* Blinking */
			{
				car_panel_set_control(WAITING);
				lcd_clrscr();
				lcd_gotoxy(0,0);
				lcd_puts("BLINK");
			}
		}

		if (_car_panel_counter >= BUTTON_PRESS_TIME )
		{

			lcd_clrscr();
			lcd_gotoxy(0,0);
			lcd_puts("ACTIVATED");
			car_panel_set_control(ACTIVATED);
		}
		else
		{
			lcd_clrscr();
			lcd_gotoxy(0,0);
			lcd_puts("DEACTIVATED");
			car_panel_set_control(DEACTIVATED);
		}
    }

    //else if(!(PIN(PORT) & (1<<CANCEL)))
    //{
		//set_status(ONLINE);
		///* PCINT9 changed */
		//// ToDo
		//// if No alarm, break;
		//// Start 3 seconds timer
		//// Check if bit has changed
		////   if bit has changed within 3 seconds, do not trigger alarm
		////    else trigger alarm
		//// Stop timer
		//lcd_clrscr();
		//lcd_gotoxy(0,0);
		//lcd_puts("CANCEL");
    //}
	else
	{

		lcd_puts(itoa(_car_panel_counter, buf, 10));
		_car_panel_counter = 0;
		//stop_timer3();
		//lcd_clrscr();
		//lcd_gotoxy(0,0);
		//lcd_puts("NOTHING");
	}
}
