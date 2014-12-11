/**********************************************************************//**
 * @file scheduler.h
 *
 * @created 03-09-2014 13:55:08
 * @author Andi Degn
 * @author Kenneth Ren� Jensen
 * @version 0.1
 * @defgroup scheduler Scheduler
 * @{
	 @ingroup app
	 @brief Basic task scheduler running cyclic execution of all the main tasks
	 in the system
	 @note Complies MISRA 2004 standards
	 @defgroup scheduler_pub Public
 * @}
 *************************************************************************/

#ifndef COLLECT_SENSOR_DATA_H_
#define COLLECT_SENSOR_DATA_H_

#include <stdbool.h>
#include <stdint.h>

/**********************************************************************//**
 * @ingroup scheduler_pub
 * @brief Initializes the scheduler
 *
 * @param void (*callback_function_ptr)(char __data) - callback function
 *
 * @return void
 *************************************************************************/
void scheduler_start(void (*callback_function_ptr)(char __data));

/**********************************************************************//**
 * @ingroup scheduler_pub
 * @brief Pause the scheduler
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void scheduler_pause(void);

/**********************************************************************//**
 * @ingroup scheduler_pub
 * @brief Resumes the scheduler
 *
 * @param bool - true if interrupt flag register should be cleared else false
 *
 * @return void
 *************************************************************************/
void scheduler_resume(bool __clear_TIFR);

/**********************************************************************//**
 * @ingroup scheduler_pub
 * @brief Halts the scheduler
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void scheduler_halt(void);

/**********************************************************************//**
 * @ingroup scheduler_pub
 * @brief Fills up the supplied memory with vector lengths from the accelerometer
 *
 * @param int16_t *__buffer - supplied pointer to memory
 *
 * @return void
 *
 * @note *__buffer must have reserved memory corresponding to 'CONFIG_ALARM_CRASH_NO_OF_READINGS'
 *************************************************************************/
void scheduler_acc_get_last_readings_sum(int16_t *__buffer);

/**********************************************************************//**
 * @ingroup scheduler_pub
 * @brief Fills up the supplied memory with the last readings from the accelerometer (x, y, z)
 *
 * @param int16_t *__buffer - supplied pointer to memory
 *
 * @return void

 * @note *__buffer must have reserved memory of 3 * sizeof(int16_t)
 *************************************************************************/
void scheduler_acc_get_last_readings(int16_t *__buffer);

/**********************************************************************//**
 * @ingroup scheduler_pub
 * @brief Returns the last temperature reading
 *
 * @param void
 *
 * @return float - temperature in floating point value
 *************************************************************************/
float scheduler_temp_get_last_reading(void);

/**********************************************************************//**
 * @ingroup scheduler_pub
 * @brief Releases the scheduler
 *
 * @param void
 *
 * @return void
 *
 * @note Should be called every time the scheduler is done with a task
 *************************************************************************/
void scheduler_release(void);

#endif /* COLLECT_SENSOR_DATA_H_ */