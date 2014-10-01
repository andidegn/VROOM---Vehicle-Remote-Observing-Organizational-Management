/********************************************//**
@file timer.h
@author: Kenneth René Jensen
@Version: 0.3
@defgroup timer Timer
@{
	Setup of timer 1 to CTC mode for stating the scheduler.
	Setup of timer 3 to CTC mode for counting etc. used for timeout   
@}
************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "sensors/sensor_scheduler.h"

/********************************************************************//**
 @enum TIMER_PRESCALER
 @ingroup timer
 @brief Specifies the available clock dividers for Timer 1
************************************************************************/
typedef enum { TIMER_PS1 = 1,
			   TIMER_PS8 = 8,
			   TIMER_PS64 = 64,
			   TIMER_PS256 = 256,
			   TIMER_PS1024 = 1024 // DOES NOT WORK?
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

void init_Timer1_CTC(TIMER_PRESCALER prescaler, TIMER_FREQUENCY hz);

void init_Timer3_CTC(TIMER_PRESCALER prescaler, TIMER_FREQUENCY hz);
void start_timer3(void);
void stop_timer3(void);

extern uint32_t SIM908_TIMEOUT_COUNTER;

#endif /* TIMER_H_ */