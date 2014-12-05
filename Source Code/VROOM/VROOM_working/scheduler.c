/**********************************************************************//**
 * @file scheduler.c
 *************************************************************************/
#include <util/delay.h>
#include <math.h>
#include "scheduler.h"
#include "vroom_config.h"
#include "sensors/thermometer/tc72.h"
#include "sensors/accelerometer/lis331hh.h"
#include "accident_logic/accident_detection.h"
#include "util/timer/timer.h"

#ifdef DEBUG_TASK_MEASURE
#include "util/r2r_led/r2r_led.h"
#endif

typedef enum {state_tc72_init,
			  state_acc_init,
			  state_timer_init,
			  state_idle,
			  state_tc72_read,
			  state_acc_read,
			  state_store_in_buffers,
			  state_detect_accident,
			  state_detect_fire
} SENSOR_STATE;

/* local variables */
static int16_t _x_axis_buffer[CONFIG_ALARM_CRASH_NO_OF_READINGS];
static int16_t _y_axis_buffer[CONFIG_ALARM_CRASH_NO_OF_READINGS];
static int16_t _z_axis_buffer[CONFIG_ALARM_CRASH_NO_OF_READINGS];
static float _temperature = CONFIG_ALARM_FIRE_TEMP_INIT;

static SENSOR_STATE _state;
static volatile uint16_t _execution_counter = 0;

static void (*_callback_function_ptr)(char __data); /* not implemented yet */
// static uint8_t _acc_buffer_head = 0; /* may not be needed */
static uint8_t _acc_buffer_tail = 0;

#ifdef DEBUG_TASK_MEASURE
static uint8_t _task_prev_id_temp = 0;
static uint8_t _task_prev_id_acc = 0;
static uint8_t _task_prev_id_read = 0;
static uint8_t _task_prev_id_crash_det = 0;
static uint8_t _task_prev_id_fire_det = 0;
#endif

void scheduler_start(void (*callback_function_ptr)(char __data)) {
	_state = state_tc72_init;
	_callback_function_ptr = callback_function_ptr;
	scheduler_release();
}

void scheduler_acc_get_last_readings_sum(int16_t *__buffer) {
	uint8_t _index = 0;
	for (uint8_t i = 0; i < CONFIG_ALARM_CRASH_NO_OF_READINGS; i++) {
		_index = (_acc_buffer_tail + i + 1) % CONFIG_ALARM_CRASH_NO_OF_READINGS;
		*(__buffer + i) = sqrt(pow(_x_axis_buffer[_index], 2) + pow(_y_axis_buffer[_index], 2) + pow(_z_axis_buffer[_index], 2));
	}
}

void scheduler_acc_get_last_readings(int16_t *__buffer) {
	*__buffer = _x_axis_buffer[_acc_buffer_tail];
	*(__buffer + 1) = _y_axis_buffer[_acc_buffer_tail];
	*(__buffer + 2) = _z_axis_buffer[_acc_buffer_tail];
}

float scheduler_temp_get_last_reading(void)
{
	return _temperature;
}

void scheduler_release(void) {
	switch(_state) {

		/************************************************************************/
		/* Init state for temperature sensor                                    */
		/************************************************************************/
		case state_tc72_init :
			_state = state_acc_init;
			init_tc72(PB4);
		break;

		/************************************************************************/
		/* Init state for accelerometer sensor                                  */
		/************************************************************************/
		case state_acc_init :
			_state = state_timer_init;
			acc_init(PB0, ACC_NORMAL_MODE, ACC_ODR_400, ACC_24G);
		break;

		/************************************************************************/
		/* Init state for timer                                                 */
		/************************************************************************/
		case state_timer_init :
			_state = state_idle;
			timer1_init_CTC(TIMER_PS256, CONFIG_SCHEDULER_FREQUENCY);
		break;


		/************************************************************************/
		/* Main routine starts here.                                            */
		/*                                                                      */
		/* Idle state                                                           */
		/************************************************************************/
		case state_idle :
			_state = state_tc72_read;
		break;

		/************************************************************************/
		/* Read state for temperature sensor                                    */
		/************************************************************************/
		case state_tc72_read :
#ifdef DEBUG_TASK_MEASURE
			_task_prev_id_temp = r2r_start_task(DEBUG_TASK_ID_SENSOR_SCHEDULER_TEMP_REQ);
#endif
			_state = state_acc_read;
			measure_temperature();
#ifdef DEBUG_TASK_MEASURE
			r2r_stop_task(_task_prev_id_temp);
#endif
		break;

		/************************************************************************/
		/* Read state for accelerometer sensor                                  */
		/************************************************************************/
		case state_acc_read :
#ifdef DEBUG_TASK_MEASURE
			_task_prev_id_acc = r2r_start_task(DEBUG_TASK_ID_SENSOR_SCHEDULER_ACC_REQ);
#endif
			_state = state_store_in_buffers;
			acc_measure();
#ifdef DEBUG_TASK_MEASURE
			r2r_stop_task(_task_prev_id_acc);
#endif
		break;

		/************************************************************************/
		/* State where measurements are read and stored in variables/buffers    */
		/************************************************************************/
		case state_store_in_buffers :
#ifdef DEBUG_TASK_MEASURE
			_task_prev_id_read = r2r_start_task(DEBUG_TASK_ID_SENSOR_SCHEDULER_SENSORS_READ);
#endif
			_state = state_idle;
			_x_axis_buffer[_acc_buffer_tail] = (int)(acc_get_x_axis() * 100);/* any higher than 4000 will risk hitting the limit of 16 bit signed variable */
			_y_axis_buffer[_acc_buffer_tail] = (int)(acc_get_y_axis() * 100);
			_z_axis_buffer[_acc_buffer_tail] = (int)(acc_get_z_axis() * 100);
			_acc_buffer_tail = (_acc_buffer_tail + 1) % CONFIG_ALARM_CRASH_NO_OF_READINGS;

			_temperature = get_temperature();

			_state = state_detect_accident;
#ifdef DEBUG_TASK_MEASURE
			r2r_stop_task(_task_prev_id_read);
#endif
			scheduler_release();
		break;

		/************************************************************************/
		/* State where an accident is detected                                  */
		/************************************************************************/
		case state_detect_accident :
#ifdef DEBUG_TASK_MEASURE
			_task_prev_id_crash_det = r2r_start_task(DEBUG_TASK_ID_ACCIDENT_DETECTION_CRASH_DETECTION);
#endif
			_state = ++_execution_counter % (CONFIG_ALARM_FIRE_TRIGGER_TIME / (1000 / CONFIG_SCHEDULER_FREQUENCY)) == 0? state_detect_fire : state_idle;
			check_for_crash();
			scheduler_release();
#ifdef DEBUG_TASK_MEASURE
			r2r_stop_task(_task_prev_id_crash_det);
#endif
		break;

		/************************************************************************/
		/* State where fire is detected                                         */
		/************************************************************************/
		case state_detect_fire :
#ifdef DEBUG_TASK_MEASURE
			_task_prev_id_fire_det = r2r_start_task(DEBUG_TASK_ID_ACCIDENT_DETECTION_FIRE_DETECTION);
#endif
			_state = state_idle;
			check_for_fire();
			scheduler_release();
#ifdef DEBUG_TASK_MEASURE
			r2r_stop_task(_task_prev_id_fire_det);
#endif
		break;

		default: break;
	}
}