/*
 * collect_sensor_data.c
 *
 * Created: 03-09-2014 13:53:18
 *  Author: Kenneth Ren� Jensen
 */
#include <util/delay.h>
#include "sensor_scheduler.h"

#define ANALYSIS 0

typedef enum {state_uart_init,
			  state_tc72_init,
			  state_acc_init,
			  state_timer_init,
			  state_idle,
			  state_tc72_read,
			  state_acc_read,
			  state_uart_send
} SENSOR_STATE;

/* local variables */
static SENSOR_STATE _state;
static void (*_callback_function_ptr)(char cfp);

int16_t _x_axis_buffer[ACC_BUFFER_SIZE];
int16_t _y_axis_buffer[ACC_BUFFER_SIZE];
int16_t _z_axis_buffer[ACC_BUFFER_SIZE];
static uint8_t _acc_buffer_head = 0; /* may not be needed */
static uint8_t _acc_buffer_tail = 0;

void scheduler_start(void (*callback_function_ptr)(char cfp)) {
	#if ANALYSIS
		#define DELAY_BETWEEN_CHARS 0
		_state = state_uart_init;
	#else // ANALYSIS
		_state = state_tc72_init;
	#endif // ANALYSIS
	_callback_function_ptr = callback_function_ptr;
	scheduler_release();
}

void scheduler_release(void) {
	switch(_state) {
		case state_uart_init :
			_state = state_tc72_init;
			uart1_setup_async(UART_MODE_DOUBLE, UART_BAUD_57K6, UART_PARITY_DISABLED, UART_ONE_STOP_BIT, UART_8_BIT, _callback_function_ptr);
			scheduler_release();
		break;

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
			/* Insert check for emergency_flag */
			_state = state_tc72_read;
		break;

		case state_tc72_read :
			_state = state_acc_read;
			measure_temperature();
		break;

		case state_acc_read :
			_state = state_uart_send;
			acc_measure();
		break;
		case state_uart_send :
			_state = state_idle;
			_x_axis_buffer[_acc_buffer_tail] = (int)(acc_get_x_axis() * 4000);/* any higher than 4000 will risk hitting the limit of 16 bit signed variable */
			_y_axis_buffer[_acc_buffer_tail] = (int)(acc_get_y_axis() * 4000);
			_z_axis_buffer[_acc_buffer_tail] = (int)(acc_get_z_axis() * 4000);

#if ANALYSIS
			_delay_ms(DELAY_BETWEEN_CHARS);
			uart1_send_char(_x_axis_buffer[_acc_buffer_tail] >> 8);	uart1_send_char(_x_axis_buffer[_acc_buffer_tail] & 0xff);
			_delay_ms(DELAY_BETWEEN_CHARS);
			uart1_send_char(_y_axis_buffer[_acc_buffer_tail] >> 8);	uart1_send_char(_y_axis_buffer[_acc_buffer_tail] & 0xff);
			_delay_ms(DELAY_BETWEEN_CHARS);
			uart1_send_char(_z_axis_buffer[_acc_buffer_tail] >> 8);	uart1_send_char(_z_axis_buffer[_acc_buffer_tail] & 0xff);
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