/**********************************************************************//**
 * @file: spi.c
 *
 * @Created: 10-04-2014 12:03:35
 * @Author: Andi Degn
 * @Version: 0.4
 * @{
	 This is a driver for the SPI bus
	 on the ATMEGA family processors.
 * @}
 * @note Complies MISRO 2004 standards
 **************************************************************************/

#include"spi.h"
#include <util/delay.h>

/**********************************************************************//**
 * @ingroup spi_priv
 * Defines for the ports and pins used by the SPI
 * @{
 **************************************************************************/
#define DDR_SPI DDRB
#define SS PB0
#define SCK PB1
#define MOSI PB2
#define MISO PB3
/* @} */

/**********************************************************************//**
 * @ingroup spi_priv
 * defines the max number of handles that are able to be handled
 * @{
 **************************************************************************/
#define MAX_HANDLES 16U
/* @} */

/**********************************************************************//**
 * @ingroup spi_priv
 * defines inactive/active level for CS/CE
 * @{
 **************************************************************************/
#define CS_INACTIVE 0U
#define CS_ACTIVE 1U
/* @} */

/* local variables */
static int8_t _handle_count = 0;
static int8_t _current_handle = -1;
static handle_param _handles[MAX_HANDLES];
static uint8_t _cs_pin;
static uint8_t _cs_active_level;
static volatile bool _is_busy = false;

static uint8_t *_data_array;
static uint8_t _no_of_bytes;
static uint8_t _bytes_sent_ctr;

/* Prototypes */
static void _setup_spi(handle_param *__param);
static inline void _set_cs_level(uint8_t __level);
static void _send(uint8_t __data);

/**********************************************************************//**
 * @ingroup spi_pub
 * Takes the supplied SPI parameters and stores them in a
 * handle_param array and returns a handle
 * @note Max 16 different handles are available
 **************************************************************************/
int8_t spi_master_setup(SPI_DATA_MODE __mode,
						SPI_DATA_DIRECTION __data_direction,
						SPI_DIVIDER __freq_divider,
						uint8_t __cs_pin,
						SPI_CS_ACTIVE_LEVEL __cs_active_level,
						void (*__callback_function_ptr)(uint8_t *__data)) {
	int8_t ret = -1;

    if (_handle_count < MAX_HANDLES) {
	    _handles[_handle_count].mode = __mode;
	    _handles[_handle_count].data_direction = __data_direction;
	    _handles[_handle_count].freq_divider = __freq_divider;
	    _handles[_handle_count].cs_pin = __cs_pin;
	    _handles[_handle_count].cs_active_level = __cs_active_level;
	    _handles[_handle_count].callback_function_ptr = __callback_function_ptr;
	    ret = _handle_count++;
    }
	_cs_pin = __cs_pin;
	_cs_active_level = __cs_active_level;
	DDR_SPI |= __cs_pin;
	_set_cs_level(CS_INACTIVE);

    return ret;
}

/**********************************************************************//**
 * @ingroup spi_priv
 * * Sets up the SPI ports as output
 * * Clears the SPRC register before setup to make sure it is clear
 * * Sets up the SPRC register based on the supplied parameters
 * * * SPIE, SPE and master are fixed
 * * Sets up the SPSR register if divider 2, 8 or 32 is chosen
 *
 * SPI mode: | CPOL | CHAP |\n
 * SPI_MODE0 |    0 |    0 |\n
 * SPI_MODE1 |    0 |    1 |\n
 * SPI_MODE2 |    1 |    0 |\n
 * SPI_MODE3 |    1 |    1 |\n
 *\n
 * Data direction: | DORD |\n
 * SPI_MSB_FIRST   |    0 |\n
 * SPI_LSB_FIRST   |    1 |\n
 *\n
 * Frequency divider: | SPI2X | SPR1 | SPR0 |\n
 * SPI_DIVIDER_2	  |     1 |    0 |    0 |\n
 * SPI_DIVIDER_4	  |     0 |    0 |    0 |\n
 * SPI_DIVIDER_8	  |     1 |    0 |    1 |\n
 * SPI_DIVIDER_16	  |     0 |    0 |    1 |\n
 * SPI_DIVIDER_32	  |     1 |    1 |    0 |\n
 * SPI_DIVIDER_64	  |     0 |    1 |    0 |\n
 * SPI_DIVIDER_128	  |     0 |    1 |    1 |\n
 *
 * @param handle_param *__param - a struct containing the parameters for the SPI setup
 *
 * @return void
 **************************************************************************/
static void _setup_spi(handle_param *__param) {
	/* saves the current state of the status register and disables global interrupt */
	uint8_t _sreg = SREG;
	cli();

	/* Sets the Clock, MOSI and SS as outputs */
	DDR_SPI |= _BV(SCK) | _BV(MOSI) | _BV(SS);
	DDR_SPI &= ~_BV(MISO);

	/* Clear the SPCR register before setting values */
	SPCR = 0x00;

	/* Sets the active CS/CE pin and pin level */
	_cs_pin = __param->cs_pin;
	_cs_active_level = __param->cs_active_level;
	DDR_SPI |= _BV(_cs_pin);

	/* Sets an additional bit if the divider is 2, 8 or 32 */
	if ((__param->freq_divider == SPI_DIVIDER_2) ||
		(__param->freq_divider == SPI_DIVIDER_8) ||
		(__param->freq_divider == SPI_DIVIDER_32)) {
		SPSR |= _BV(SPI2X);
	} else {
		SPSR &= ~_BV(SPI2X);
	}

	/* Enables SPI, sets it as Master and sets all related bits (p.202)
				Bit  |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
		 0x2C (0x4C) |SPIE| SPE|DORD|MSTR|CPOL|CPHA|SPR1|SPR0| SPCR
	Read/Write		 | R/W| R/W| R/W| R/W| R/W| R/W| R/W| R/W|
	*/
	SPCR |= _BV(SPE) | __param->data_direction | _BV(MSTR) | __param->mode | __param->freq_divider;
	/* restore the status register */
	SREG = _sreg;
}

/**********************************************************************//**
 * @ingroup spi_priv
 * Sets the CS/CE level based on the level
 *
 * @param uint8_t __level - CS_ACTIVE or CS_INACTIVE
 *
 * @return void
 **************************************************************************/
static inline void _set_cs_level(uint8_t __level) {
	if (_cs_active_level == SPI_CS_ACTIVE_LOW) {
		__level = !__level;
	}
	if (__level == CS_INACTIVE) {
		PORTB &= ~_BV(_cs_pin);
	} else {
		PORTB |= _BV(_cs_pin);
	}
}

/**********************************************************************//**
 * @ingroup spi_priv
 * Sets up the SPI with 'handle' if it is not already the current
 * handle. Then sends the data and returns '1'.
 * If the SPI driver is busy performing another task, the data
 * is not sent and a '0' is returned
 *
 * @param uint8_t __data - the data to be sent
 *
 * @return void
 **************************************************************************/
static void _send(uint8_t __data) {
    /* putting data in output register and incrementing the _bytes_sent_ctr counter */
    SPDR = __data;
    _bytes_sent_ctr++;
}

/**********************************************************************//**
 * @ingroup spi_pub
 * Sends 1 (one) byte of data by passing __handle and __data to spi_send()
 * and setting length to 1 (one)
 **************************************************************************/
int8_t spi_send_byte(int8_t __handle, uint8_t __data) {
	return spi_send(__handle, &__data, 1);
}

/**********************************************************************//**
 * @ingroup spi_pub
 * Stores the "data" pointer and the no_of_bytes and calls "_send()"
 * with the first data slot
 * @note This "data" pointer is being used to store the returning data as well
 **************************************************************************/
int8_t spi_send(int8_t __handle, uint8_t *__data_array, uint8_t __no_of_bytes) {
	int8_t ret = -1;

	/* checking if the SPI driver is in use, if so it checks if it is the current handle that is using it */
	if (!_is_busy || (__handle == _current_handle)) {
		_data_array = __data_array;
		_no_of_bytes = __no_of_bytes;
		_bytes_sent_ctr = 0U;

		/* saves the current state of the status register and disables global interrupt */
		uint8_t _sreg = SREG;
		cli();

		/* checks if the device calling the SPI is the same as is already registered. If not, set it up */
		if (_current_handle != __handle) {
			_current_handle = __handle;
			_setup_spi(&_handles[_current_handle]);
		}

		/* setting the SPI in busy mode */
		_is_busy = true;

		/* activating chip select on the slave */
		_set_cs_level(CS_ACTIVE);

		/* enabling SPI interrupt */
		SPCR |= _BV(SPIE);

		/* sending the data */
		_send(*_data_array);

		/* restore status register */
		SREG = _sreg;

		ret = 1;
	}
	return ret;
}

/**********************************************************************//**
 * @ingroup spi_pub
 * Sets is_busy to 0 indicating that no tasks is being handled by the SPI
 **************************************************************************/
void spi_release(void) {
	SPCR &= ~_BV(SPIE);
	_set_cs_level(CS_INACTIVE);
	_no_of_bytes = 0U;
	_is_busy = false;
}

/**********************************************************************//**
 * @ingroup spi_priv
 * Interrupt service routine for the SPI. If a callback function pointer is
 * supplied when setting up the SPI, a callback to that function is being performed.
 * Else the SPI is released.
 **************************************************************************/
ISR(SPI_STC_vect, ISR_BLOCK) {
	/* store data from SPI interrupt */
	*(_data_array +_bytes_sent_ctr - 1) = SPDR;

	/* checks if more bytes need to be sent */
	if (_bytes_sent_ctr < _no_of_bytes) {
		_send(*(_data_array + _bytes_sent_ctr));
	} else {
		spi_release();
		if (_handles[_current_handle].callback_function_ptr != NULL) {
			_handles[_current_handle].callback_function_ptr(_data_array);
		}
	}
}
