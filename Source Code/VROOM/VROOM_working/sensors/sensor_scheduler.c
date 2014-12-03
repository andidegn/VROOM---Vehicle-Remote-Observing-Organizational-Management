/**********************************************************************//**
 * @file collect_sensor_data.c
 *************************************************************************/
#include <util/delay.h>
#include <math.h>
#include "sensor_scheduler.h"
#include "../vroom_config.h"

typedef enum {state_tc72_init,
			  state_acc_init,
			  state_timer_init,
			  state_idle,
			  state_tc72_read,
			  state_acc_read,
			  state_store_in_buffers
} SENSOR_STATE;

/* local variables */
static int16_t _x_axis_buffer[CONFIG_ALARM_CRASH_NO_OF_READINGS];
static int16_t _y_axis_buffer[CONFIG_ALARM_CRASH_NO_OF_READINGS];
static int16_t _z_axis_buffer[CONFIG_ALARM_CRASH_NO_OF_READINGS];
static float _temperature = CONFIG_ALARM_FIRE_TEMP_INIT;

static SENSOR_STATE _state;
static void (*_callback_function_ptr)(char __data); /* not implemented yet */
// static uint8_t _acc_buffer_head = 0; /* may not be needed */
static uint8_t _acc_buffer_tail = 0;

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
		case state_tc72_init :
			_state = state_acc_init;
			init_tc72(PB4);
		break;

		case state_acc_init :
			_state = state_timer_init;
			acc_init(PB0, ACC_NORMAL_MODE, ACC_ODR_400, ACC_24G);
		break;

		case state_timer_init :
			_state = state_tc72_read;
			timer1_init_CTC(TIMER_PS256, CONFIG_ALARM_SENSOR_READ_FREQUENCY);
		break;

		/* reoccurring */
		case state_idle :
			_state = state_tc72_read;
		break;

		case state_tc72_read :
			_state = state_acc_read;
			measure_temperature();
		break;

		case state_acc_read :
			_state = state_store_in_buffers;
			acc_measure();
		break;
		case state_store_in_buffers :
			_state = state_idle;
			_x_axis_buffer[_acc_buffer_tail] = (int)(acc_get_x_axis() * 100);/* any higher than 4000 will risk hitting the limit of 16 bit signed variable */
			_y_axis_buffer[_acc_buffer_tail] = (int)(acc_get_y_axis() * 100);
			_z_axis_buffer[_acc_buffer_tail] = (int)(acc_get_z_axis() * 100);
			_acc_buffer_tail = (_acc_buffer_tail + 1) % CONFIG_ALARM_CRASH_NO_OF_READINGS;

			_temperature = get_temperature();

			_state = state_idle;
			scheduler_release();
		break;

		default: break;
	}
}