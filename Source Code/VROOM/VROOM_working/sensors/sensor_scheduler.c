/*
 * collect_sensor_data.c
 *
 * Created: 03-09-2014 13:53:18
 *  Author: Kenneth Ren� Jensen
 */
#include <util/delay.h>
#include "sensor_scheduler.h"

//#define ANALYSIS

typedef enum {state_uart_init,
			  state_tc72_init,
			  state_acc_init,
			  state_timer_init,
			  state_idle,
			  state_tc72_read,
			  state_acc_read,
			  state_uart_send
} SENSOR_STATE;
static SENSOR_STATE _state;

#ifdef ANALYSIS
#define DELAY_BETWEEN_CHARS 1
static int16_t x_axis = 0;
static int16_t y_axis = 0;
static int16_t z_axis = 0;
#endif // ANALYSIS

void scheduler_start(void) {
#ifdef ANALYSIS
	_state = state_uart_init;
#endif // ANALYSIS
#ifndef ANALYSIS
	_state = state_tc72_init;
#endif // ANALYSIS
	scheduler_release();
}

void scheduler_release(void) {
	switch(_state) {
		case state_uart_init :
			_state = state_tc72_init;
			uart0_setup_async(UART0_MODE_DOUBLE, UART0_BAUD_115K2, UART0_PARITY_DISABLED, UART0_ONE_STOP_BIT, UART0_8_BIT, NULL);
			scheduler_release();
		break;

		case state_tc72_init :
			_state = state_acc_init;
			init_tc72();
		break;

		case state_acc_init :
			_state = state_timer_init;
			acc_init(ACC_NORMAL_MODE, ACC_ODR_400, ACC_24G);
		break;

		case state_timer_init :
			_state = state_tc72_read;
			init_Timer1_CTC(TIMER_PS8, TIMER_500HZ);
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
			_state = state_uart_send;
			acc_measure();
		break;
#ifdef ANALYSIS
		case state_uart_send :
			_state = state_idle;
			x_axis = (int)(acc_get_x_axis() * 1000);
			y_axis = (int)(acc_get_y_axis() * 1000);
			z_axis = (int)(acc_get_z_axis() * 1000);

			_delay_ms(DELAY_BETWEEN_CHARS);
			uart0_send_char(x_axis >> 8);	uart0_send_char(x_axis & 0xff);
			_delay_ms(DELAY_BETWEEN_CHARS);
			uart0_send_char(y_axis >> 8);	uart0_send_char(y_axis & 0xff);
			_delay_ms(DELAY_BETWEEN_CHARS);
			uart0_send_char(z_axis >> 8);	uart0_send_char(z_axis & 0xff);
			_delay_ms(DELAY_BETWEEN_CHARS);
			uart0_send_char(0x7f);			uart0_send_char(0xff);
		break;
#endif // ANALYSIS
#ifndef ANALYSIS
			case state_uart_send :
				_state = state_idle;
				scheduler_release();
			break;
#endif // ANALYSIS




		default: break;
	}
}