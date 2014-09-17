/********************************************//**
@file tc72.c
@author: Kenneth René Jensen
@Version: 0.3
@defgroup tc72 TC72_driver
@{
	This is the driver for the temperature sensor tc72
	on the ATMEGA family processors.
@}
@note Complies MISRO 2004 standards
************************************************/
#include "tc72.h"
#include "../sensor_scheduler.h"

/* SPI commands */
#define SDI_Read        0x02U
#define SDI_Write       0x80U

/* states for state machine */
typedef enum {
    INIT,
    RUNNING
} TC72_STATE;

/* Local variables */
static int8_t _handle_ID = -1;

static uint8_t _LSB = 0U;
static uint8_t _MSB = 0U;
static uint8_t _ID = 0U;

static int16_t _temp = 0U;
static uint8_t _send_read[4];
static uint8_t _send_setup[2];
static TC72_STATE _state;

/* Prototypes */
static void _tc72_callback(uint8_t data[]);

/********************************************************************************************************************//**
 @ingroup tc72
 @brief Initiates tc72 using SPI
 @return void
************************************************************************************************************************/
void init_tc72(void) {
    _send_setup[0] = SDI_Write;
    _send_setup[1] = 0U;

    /* Disable interrupt */
    uint8_t SREG_cpy = SREG;
    cli();

    /* Setup SPI and gets an ID */
    _handle_ID = spi_master_setup(SPI_MODE_1,
                                  SPI_MSB_FIRST,
                                  SPI_DIVIDER_128,
                                  TEMP_CS_PIN,
                                  SPI_CS_ACTIVE_HIGH,
                                  _tc72_callback);

    /* Restore interrupt */
    SREG = SREG_cpy;

    spi_send(_handle_ID, _send_setup, 2U);
    _state = INIT;
}

/********************************************************************************************************************//**
@ingroup tc72
@brief Function to calculate the temperature
@return Temperature as floating point value
@note Should only be used for Unit Testing
************************************************************************************************************************/
float calculate_temperature(uint8_t MSB, uint8_t LSB) {
    /****************************************************************************************************************
     Violation on rule 10.5 MISRA C 2004 (if the bitwise operators ~ and << are applied to an operand of underlying type
    'unsigned char' or 'unsigned short', the result shall be immediately cast to the underlying type of the operand)
     ****************************************************************************************************************/
    _temp = (int16_t)((MSB<<2U) | (LSB>>6U));

    /****************************************************************************************************************
     Violation on rule 10.1 MISRA C 2004 (illegal implicit conversion from underlying MISRA type "unsigned short" to "signed short")
     ****************************************************************************************************************/
    /****************************************************************************************************************
     Violation on rule 12.7 MISRA C 2004 (bitwise operations shall not be performed on signed integer types)
     ****************************************************************************************************************/
    /* Check if temperature is negative */
    if (_temp & 0x200U) {
        _temp |= 0xFE00U;
    }

    return (float)_temp/4.0F;
}

/********************************************************************************************************************//**
 @ingroup tc72
 @brief Function to read the last measured temperature from the tc72 sensor.
 @return Temperature as floating point value
 @note TC72 Operation Range: -55°C to 125°C
************************************************************************************************************************/
float get_temperature(void) {
    return calculate_temperature(_MSB, _LSB);
}

/********************************************************************************************************************//**
 @ingroup tc72
 @brief Function which starts measuring the temperature.
 @return void
************************************************************************************************************************/
void measure_temperature(void) {
    _send_read[0] = SDI_Read;
    _send_read[1] = 0U;
    _send_read[2] = 0U;
    _send_read[3] = 0U;

    spi_send(_handle_ID, _send_read, 4U);
}

/********************************************************************************************************************//**
 @ingroup tc72_isr
 @brief Callback Function from SPI
 @param data sent from SPI
 @return void
************************************************************************************************************************/
static void _tc72_callback(uint8_t data[]) {
    switch (_state)	{
		case INIT:
			_state = RUNNING;
			scheduler_release();
			break;

		case RUNNING: {
			/* Disable interrupt */
			uint8_t SREG_cpy = SREG;
			cli();

			_MSB = data[1];
			_LSB = data[2];
			_ID = data[3];

			/* Restore interrupt */
			SREG = SREG_cpy;
			scheduler_release();
			break;
		}

		default:
			_state = INIT;
			break;
    }
}
