/**********************************************************************//**
 * @file tc72.c
 *************************************************************************/
#include <avr/interrupt.h>
#include "tc72.h"
#include "../../scheduler.h"
#include "../../data_comm/spi/spi.h"

/**********************************************************************//**
 * @ingroup tc72_priv
 * @brief define for the addresses for the read and write command
 * @defgroup tc72_reg_adr Register addresses
 * @{
 *************************************************************************/
#define SDI_Read        0x02U
#define SDI_Write       0x80U
/** @} */

/* states for state machine */
typedef enum {
    INIT,
    RUNNING
} TC72_STATE;

/* Local variables */
static int8_t _handle_ID = -1;

static uint8_t _lsb = 0U;
static uint8_t _msb = 0U;
static uint8_t _ID = 0U;

static int16_t _temp = 0U;
static uint8_t _send_read[4];
static uint8_t _send_setup[2];
static TC72_STATE _state;

/* Prototypes */
static void _tc72_callback(uint8_t __data[]);

/**********************************************************************//**
 * @ingroup tc72_pub
 * Sets up the spi communication and sends the setup data to the tc72 chip
 *************************************************************************/
void init_tc72(uint8_t __cs_pin)
{
    _send_setup[0] = SDI_Write;
    _send_setup[1] = 0U;

    /* Disable interrupt */
    uint8_t SREG_cpy = SREG;
    cli();

    /* Setup SPI and gets an ID */
    _handle_ID = spi_master_setup(SPI_MODE_1,
                                  SPI_MSB_FIRST,
                                  SPI_DIVIDER_128,
                                  __cs_pin,
                                  SPI_CS_ACTIVE_HIGH,
                                  _tc72_callback);

    /* Restore interrupt */
    SREG = SREG_cpy;

    spi_send(_handle_ID, _send_setup, 2U);
    _state = INIT;
}

/**********************************************************************//**
 * @ingroup tc72_pub
 *
 * Merges together __msb and __lsb and returns the temperature as a
 * floating point
 *
 * @note Should only be used directly for Unit Testing, else only used locally
 *************************************************************************/
float calculate_temperature(uint8_t __msb, uint8_t __lsb)
{
    /****************************************************************************************************************
     Violation on rule 10.5 MISRA C 2004 (if the bitwise operators ~ and << are applied to an operand of underlying type
    'unsigned char' or 'unsigned short', the result shall be immediately cast to the underlying type of the operand)
     ****************************************************************************************************************/
    _temp = (int16_t)((__msb<<2U) | (__lsb>>6U));

    /****************************************************************************************************************
     Violation on rule 10.1 MISRA C 2004 (illegal implicit conversion from underlying MISRA type "unsigned short" to "signed short")
     ****************************************************************************************************************/
    /****************************************************************************************************************
     Violation on rule 12.7 MISRA C 2004 (bitwise operations shall not be performed on signed integer types)
     ****************************************************************************************************************/
    /* Check if temperature is negative */
    if (_temp & 0x200U)
	{
        _temp |= 0xFE00U;
    }

    return (float)_temp/4.0F;
}

/**********************************************************************//**
 * @ingroup tc72_pub
 * Returns last recorded temperature
 *************************************************************************/
float get_temperature(void)
{
    return calculate_temperature(_msb, _lsb);
}

/**********************************************************************//**
 * @ingroup tc72_pub
 * Sends the read adr. and 3 dummy bytes
 *************************************************************************/
void measure_temperature(void)
{
    _send_read[0] = SDI_Read;
    _send_read[1] = 0U;
    _send_read[2] = 0U;
    _send_read[3] = 0U;

    spi_send(_handle_ID, _send_read, 4U);
}

/**********************************************************************//**
 * @ingroup tc72_isr
 * @brief Callback Function from SPI
 *
 * @param uint8_t data - data sent from SPI
 *
 * @return void
 *************************************************************************/
static void _tc72_callback(uint8_t __data[]) {
    switch (_state)	{
    case INIT:
        _state = RUNNING;
        scheduler_release();
        break;

    case RUNNING: {
        /* Disable interrupt */
        uint8_t SREG_cpy = SREG;
        cli();

        _msb = __data[1];
        _lsb = __data[2];
        _ID = __data[3];

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
