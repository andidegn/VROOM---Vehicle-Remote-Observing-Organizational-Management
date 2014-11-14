/*
 * collect_sensor_data.c
 *
 * Created: 03-09-2014 13:53:18
 *  Author: Kenneth René Jensen
 */
#include <util/delay.h>
#include "sensor_scheduler.h"

#define ANALYSIS 0

typedef enum {state_tc72_init,
			  state_acc_init,
			  state_timer_init,
			  state_idle,
			  state_tc72_read,
			  state_acc_read,
			  state_store_in_buffers
} SENSOR_STATE;

int16_t x_axis_buffer[ACC_BUFFER_SIZE];
int16_t y_axis_buffer[ACC_BUFFER_SIZE];
int16_t z_axis_buffer[ACC_BUFFER_SIZE];

/* local variables */
static SENSOR_STATE _state;
static void (*_callback_function_ptr)(char __data); /* not implemented yet */
static uint8_t _acc_buffer_head = 0; /* may not be needed */
static uint8_t _acc_buffer_tail = 0;

void scheduler_start(void (*callback_function_ptr)(char __data)) {
	#if ANALYSIS
		#define DELAY_BETWEEN_CHARS 0
		_state = state_uart_init;
	#else // ANALYSIS
		_state = state_tc72_init;
	#endif // ANALYSIS
	_callback_function_ptr = callback_function_ptr;
	scheduler_release();
}

void scheduler_get_last_readings(int16_t *buffer) {
	*buffer = x_axis_buffer[_acc_buffer_tail];
	*(buffer + 1) = y_axis_buffer[_acc_buffer_tail];
	*(buffer + 2) = z_axis_buffer[_acc_buffer_tail];
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
			init_Timer1_CTC(TIMER_PS256, TIMER_100HZ);
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
			x_axis_buffer[_acc_buffer_tail] = (int)(acc_get_x_axis() * 400);/* any higher than 4000 will risk hitting the limit of 16 bit signed variable */
			y_axis_buffer[_acc_buffer_tail] = (int)(acc_get_y_axis() * 400);
			z_axis_buffer[_acc_buffer_tail] = (int)(acc_get_z_axis() * 400);

#if ANALYSIS
			_delay_ms(DELAY_BETWEEN_CHARS);
			uart1_send_char(x_axis_buffer[_acc_buffer_tail] >> 8);	uart1_send_char(x_axis_buffer[_acc_buffer_tail] & 0xff);
			_delay_ms(DELAY_BETWEEN_CHARS);
			uart1_send_char(y_axis_buffer[_acc_buffer_tail] >> 8);	uart1_send_char(y_axis_buffer[_acc_buffer_tail] & 0xff);
			_delay_ms(DELAY_BETWEEN_CHARS);
			uart1_send_char(z_axis_buffer[_acc_buffer_tail] >> 8);	uart1_send_char(z_axis_buffer[_acc_buffer_tail] & 0xff);
			_delay_ms(DELAY_BETWEEN_CHARS);
			uart1_send_char(0x7f);									uart1_send_char(0xff);
#endif // ANALYSIS
			_acc_buffer_tail = (_acc_buffer_tail + 1) % ACC_BUFFER_SIZE;
			_state = state_idle;
			scheduler_release();
		break;

		default: break;
	}
}