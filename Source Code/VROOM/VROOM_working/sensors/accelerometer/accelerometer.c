/**********************************************************************//**
 * @file: accelerometer.c
 *
 * @Created: 01-09-2014 09:22:18
 * @Author: Andi Degn
 * @Version: 0.2
 * @defgroup acc Accelerometer Driver
 * @{
	 This is a driver for the accelerometer LIS331HH
	 on the ATMEGA family processors.
	 @defgroup acc_priv Private
	 @defgroup acc_pub Public
 * @}
 * @note Complies MISRO 2004 standards
 *************************************************************************/
#include <avr/interrupt.h>

#include "accelerometer.h"
#include "../../data_comm/spi/spi.h"
#include "../sensor_scheduler.h"
#include <util/delay.h>

/* visually calibrated */
#define ACC_SCALE_FACTOR 0.00017

/* Full-scale g/(2^16) = 12/(2^16) */
//#define ACC_SCALE_FACTOR 0.00018310546875

/* states for state machine */
typedef enum {
	INIT,
	RUNNING
} ACC_STATE;

/* local variables */
static int16_t _x_axis = 0;
static int16_t _y_axis = 0;
static int16_t _z_axis = 0;

static int8_t _handle = -1;
static uint8_t _send_setup[6];
static uint8_t _send_read[7];

static ACC_STATE _state;

/* prototypes */
static void _acc_callback(uint8_t *__data);
static void _acc_read_from_reg(uint8_t __reg, uint8_t __no_of_dummy_bytes);

/**********************************************************************//**
 * @ingroup acc_pub
 * Sets up the SPI and writes the setup parameters to the accelerometer chip
 **************************************************************************/
void acc_init(uint8_t __cs_pin, ACC_POWER_MODE __power_mode, ACC_OUTPUT_DATA_RATE __output_data_rate, ACC_FULL_SCALE __full_scale) {

	/* Disable interrupt */
	uint8_t SREG_cpy = SREG;
	cli();

	/* Setup SPI */
	_handle = spi_master_setup(SPI_MODE_3,
							   SPI_MSB_FIRST,
							   SPI_DIVIDER_128,
							   __cs_pin,
							   SPI_CS_ACTIVE_LOW,
							   _acc_callback);

	/* Restore interrupt */
	SREG = SREG_cpy;

	/* zeroing bytes */
	_send_setup[0] = 0;
	_send_setup[1] = 0;
	_send_setup[2] = 0;
	_send_setup[3] = 0;
	_send_setup[4] = 0;
	_send_setup[5] = 0;

	/* sending configuration data to the accelerometer using multi-write.
		index 0 : address and multiple byte data bit
		index 1-5 : data for the different control registers
		Note: Further information can be found on pages 24-28 in the datasheet
	*/

	_send_setup[0] = ACC_CTRL_REG1 | _BV(ACC_MULTI_BIT);
	_send_setup[1] = __power_mode | __output_data_rate | _BV(ACC_Xen) | _BV(ACC_Yen) | _BV(ACC_Zen);
	_send_setup[4] = __full_scale | _BV(ACC_BDU);

	spi_send(_handle, _send_setup, 6U);

	_state = INIT;
}

/**********************************************************************//**
 * @ingroup acc_pub
 * Sends a command to the accelerometer with the information to allow
 * multiple byte read 00110111
 **************************************************************************/
void acc_measure(void) {
	_acc_read_from_reg(0x28, 6);
}

/**********************************************************************//**
 * @ingroup acc_priv
 * Sends seven bytes to the SPI. First byte with the command.
 * The command is a constructed like so:
 * Bit0 - RW: Read/Write
 * * Read: 1
 * * Write: 0
 *
 * Bit1 - MS: Multi/Single
 * * Multi: 1
 * * SIngle:0
 *
 * Bit2-7 - Address
 * |bit7|bit6|bit5|bit4|bit3|bit2|bit1|bit0|
 * | RW | MS |ADR5|ADR4|ADR3|ADR2|ADR1|ADR0|
 *
 * @param uint8_t reg - the register in the acc to write to
 * @param uint8_t no_of_dummy_bytes - the number of dummy bytes to send
 * @note The number of bytes for the SPI is the total number of bytes, ie. no_of_dummy_bytes + 1
 * @return void
 **************************************************************************/
static void _acc_read_from_reg(uint8_t reg, uint8_t no_of_dummy_bytes) {
	_send_read[0] = (1 << ACC_READ_BIT) | ((no_of_dummy_bytes > 1 ? 1 : 0) << ACC_MULTI_BIT) | reg;
	for (uint8_t i = 1; i <= no_of_dummy_bytes; i++) {
		_send_read[i] = 0;
	}

	spi_send(_handle, _send_read, no_of_dummy_bytes + 1U);
}

/**********************************************************************//**
 * @ingroup acc_pub
 * Returns the x axis value multiplied with the scale factor
 **************************************************************************/
float acc_get_x_axis(void) {
	return _x_axis * ACC_SCALE_FACTOR;
}

/**********************************************************************//**
 * @ingroup acc_pub
 * Returns the y axis value multiplied with the scale factor
 **************************************************************************/
float acc_get_y_axis(void) {
	return _y_axis * ACC_SCALE_FACTOR;
}

/**********************************************************************//**
 * @ingroup acc_pub
 * Returns the z axis value multiplied with the scale factor
 **************************************************************************/
float acc_get_z_axis(void) {
	return _z_axis * ACC_SCALE_FACTOR;
}

/**********************************************************************//**
 * @ingroup acc_priv
 * Handles the callback from the SPI driver by storing the returned data
 * locally
 *
 * @param uint8_t *data - a pointer to an array of the same size as was passed to the SPI driver
 *
 * @return void
 **************************************************************************/
static void _acc_callback(uint8_t *__data) {
	switch (_state)	{
		case INIT:
			_state = RUNNING;
			scheduler_release();
		break;

		case RUNNING: {
			/* Disable interrupt */
			uint8_t SREG_cpy = SREG;
			cli();

			/* reading the data and storing them in 16 bit signed integers */
			_x_axis = (__data[2] << 8) | __data[1];
			_y_axis = (__data[4] << 8) | __data[3];
			_z_axis = (__data[6] << 8) | __data[5];

			/* Restore interrupt */
			SREG = SREG_cpy;

			/* Calling the scheduler */
			scheduler_release();
			break;
		}

		default:
			_state = INIT;
			break;
	}
}
