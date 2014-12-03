/********************************************//**
@file timer.h
@author: Kenneth René Jensen
@Version: 0.3
@defgroup timer Timer
@{
	Setup of timer 1 to CTC mode for starting sensor scheduler.
	Setup of timer 3 to CTC mode for accident check
@}
************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>

/********************************************************************//**
 @enum TIMER_PRESCALER
 @ingroup timer
 @brief Specifies the available clock dividers for Timer 1
************************************************************************/
typedef enum { TIMER_PS1 = 1,
			   TIMER_PS8 = 8,
			   TIMER_PS64 = 64,
			   TIMER_PS256 = 256,
			   TIMER_PS1024 = 1024 /* DOES NOT WORK? */
			 } TIMER_PRESCALER;

/********************************************************************//**
 @enum TOP_VALUE
 @ingroup timer
 @brief Specifies constants for setting the top value.
************************************************************************/
typedef enum { TIMER_1HZ = 1,
			   TIMER_10HZ = 10,
			   TIMER_50HZ = 50,
			   TIMER_100HZ = 100,
			   TIMER_150HZ = 150,
			   TIMER_200HZ = 200,
			   TIMER_500HZ = 500,
			   TIMER_1KHZ = 1000
			 } TIMER_FREQUENCY;

/**********************************************************************//**
 @ingroup timer
 @brief Initiates timer 1 in CTC mode
 @param TIMER_PRESCALER enum, TIMER_FREQUENCY enum
 @note Enables interrupt
*************************************************************************/
void timer1_init_CTC(TIMER_PRESCALER prescaler, TIMER_FREQUENCY hz);

/**********************************************************************//**
 @ingroup timer
 @brief Initiates timer 3 in CTC mode.
 @param TIMER_PRESCALER enum, TIMER_FREQUENCY enum
*************************************************************************/
void timer3_init_CTC(TIMER_PRESCALER prescaler, TIMER_FREQUENCY hz);

/**********************************************************************//**
 @ingroup timer
 @brief Disables interrupt for timer1 and timer3
*************************************************************************/
void timer_pause_all(void);

/**********************************************************************//**
 @ingroup timer
 @brief Enables interrupt for timer1 and timer3
*************************************************************************/
void timer_resume_all(void);

/**********************************************************************//**
 @ingroup timer
 @brief Enables interrupt for timer1 and timer3
*************************************************************************/
void timer_stop_all(void);

/**********************************************************************//**
 @ingroup timer
 @brief Disables interrupt for timer1 and timer3
*************************************************************************/
void timer_start_all(void);

#endif /* TIMER_H_ */