/********************************************************************//**
 * @file timer.h
 * @author: Kenneth René Jensen
 * @version: 0.5
 * @defgroup timer Timer
 * @{
	 @ingroup app
	 @brief Setup of timer 1 to CTC mode for starting scheduler.
	 @note Complies MISRA 2004 standards
	 @defgroup timer_pub Public
	 @defgroup timer_priv Private
 * @}
 ***********************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>

/********************************************************************//**
 * @ingroup timer_pub
 * @brief Specifies the available clock dividers for Timer 1
 ***********************************************************************/
typedef enum { TIMER_PS1 = 1,
			   TIMER_PS8 = 8,
			   TIMER_PS64 = 64,
			   TIMER_PS256 = 256,
			   TIMER_PS1024 = 1024 /* DOES NOT WORK? */
} TIMER_PRESCALER;

/********************************************************************//**
 * @ingroup timer_pub
 * @brief Specifies constants for setting the top value.
 ***********************************************************************/
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
 * @ingroup timer_pub
 * @brief Initiates timer 1 in CTC mode
 *
 * @param TIMER_PRESCALER enum, TIMER_FREQUENCY enum
 *
 * @return void
 * @note Enables interrupt
 *************************************************************************/
void timer1_init_CTC(TIMER_PRESCALER prescaler, TIMER_FREQUENCY hz);

/**********************************************************************//**
 * @ingroup timer_pub
 * @brief Stores the mask and interrupt register for Timer 1 and disables interrupts
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void timer_pause(void);

/**********************************************************************//**
 * @ingroup timer_pub
 * @brief Restores the mask and interrupt register for Timer 1 and enables interrupts
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void timer_resume(void);

/**********************************************************************//**
 * @ingroup timer_pub
 * @brief Disables interrupt for timer1 and timer3
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void timer_stop(void);

/**********************************************************************//**
 * @ingroup timer_pub
 * @brief Enables interrupt for timer1 and timer3
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void timer_start(void);

#endif /* TIMER_H_ */