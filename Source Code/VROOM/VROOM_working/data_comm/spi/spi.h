/**********************************************************************//**
 * @file: spi.h
 *
 * @Created: 10-04-2014 11:44:05
 * @Author: Andi Degn
 * @Version: 0.4
 * @defgroup spi SPI Driver
 * @{
	 This is a driver for the SPI bus
	 on the ATMEGA family processors.
	 @defgroup spi_priv Private
	 @defgroup spi_pub Public
 * @}
 * @note Complies MISRO 2004 standards
 *************************************************************************/

#ifndef SPI_H_
#define SPI_H_

#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/**********************************************************************//**
 * @ingroup spi_pub
 * @brief Specifies the available modes for SPI transfer format \n
 * * SPI_MODE_0: Polarity positive, trigger rising edge \n
 * * SPI_MODE_1: Polarity positive, trigger falling edge \n
 * * SPI_MODE_2: Polarity negative, trigger rising edge \n
 * * SPI_MODE_3: Polarity negative, trigger falling edge \n
 * @{
 *************************************************************************/
typedef enum {
	SPI_MODE_0 = 0,
	SPI_MODE_1 = _BV(CPHA),
	SPI_MODE_2 = _BV(CPOL),
	SPI_MODE_3 = _BV(CPOL) | _BV(CPHA)
} SPI_DATA_MODE;
/* @} */

/**********************************************************************//**
 * @ingroup spi_pub
 * @brief defines for the data direction
 * @{
 *************************************************************************/
typedef enum {
	SPI_MSB_FIRST = 0,
	SPI_LST_FIRST = _BV(DORD)
} SPI_DATA_DIRECTION;
/* @} */

/**********************************************************************//**
 * @ingroup spi_pub
 * @brief defines for the CS/CE pin active level
 * @{
 *************************************************************************/
typedef enum {
	SPI_CS_ACTIVE_LOW = 0,
	SPI_CS_ACTIVE_HIGH = 1
} SPI_CS_ACTIVE_LEVEL;
/* @} */

/**********************************************************************//**
 * @ingroup spi_pub
 * @brief defines for the frequency divider
 * @{
 *************************************************************************/
typedef enum {
	SPI_DIVIDER_4 = 0,
	SPI_DIVIDER_16 = _BV(SPR0),
	SPI_DIVIDER_64 = _BV(SPR1),
	SPI_DIVIDER_128 = _BV(SPR0) | _BV(SPR1),
	SPI_DIVIDER_2 = SPI_DIVIDER_4,
	SPI_DIVIDER_8 = SPI_DIVIDER_16,
	SPI_DIVIDER_32 = SPI_DIVIDER_64
} SPI_DIVIDER;
/* @} */

/**********************************************************************//**
 * @ingroup spi_priv
 * @brief struct containing the setup parameters for a single SPI setup
 *************************************************************************/
typedef struct {
	uint8_t mode;
	uint8_t data_direction;
	uint8_t freq_divider;
	uint8_t cs_pin;
	uint8_t cs_active_level;
	void (*callback_function_ptr)(uint8_t *__data);
} handle_param;

/**********************************************************************//**
 * @ingroup spi_pub
 * @brief Sets up the SPI bus as master using supplied parameters
 *
 * @param SPI_DATA_MODE __mode - SPI mode
 * @param SPI_DATA_DIRECTION __data_direction - the data direction (MSB first or LSB first)
 * @param SPI_DIVIDER __freq_devider - the frequency divider
 * @param uint8_t __cs_pin - the pin number of the CS/CE pin
 * @param SPI_CS_ACTIVE_LEVEL __cs_active_level - the level of which the chip is active (high/low)
 * @param void *__callback_function_ptr - the pointer to the function which should be called when the SPI interrupt is triggered
 *
 * @return uint8_t - a handle for the setup
 *************************************************************************/
int8_t spi_master_setup(SPI_DATA_MODE __mode,
						SPI_DATA_DIRECTION __data_direction,
						SPI_DIVIDER __freq_divider,
						uint8_t __cs_pin,
						SPI_CS_ACTIVE_LEVEL __cs_active_level,
						void (*__callback_function_ptr)(uint8_t *__cfp));

/**********************************************************************//**
 * @ingroup spi_pub
 * @brief Sends the 'data' on the SPI bus using the parameters corresponding to the 'handle'
 *
 * @param uint8_t __handle - a handle corresponding to an SPI setup
 * @param uint8_t __data - the data to be sent
 *
 * @return int8_t - '0' if unsuccessful, '1' if successful
 *************************************************************************/
int8_t spi_send_byte(int8_t __handle, uint8_t __data);

/**********************************************************************//**
 * @ingroup spi_pub
 * @brief Sends the 'data' array on the SPI bus using the parameters corresponding to the 'handle'
 *
 * @param uint8_t __handle - a handle corresponding to an SPI setup
 * @param uint8_t *__data - the data array to be sent
 * @param uint8_t __no_of_bytes - the total number of bytes to be sent
 *
 * @return int8_t - '0' if unsuccessful, '1' if successful
 *************************************************************************/
int8_t spi_send(int8_t __handle, uint8_t *__data_array, uint8_t __no_of_bytes);

/**********************************************************************//**
 * @ingroup spi_pub
 * @brief Releases the SPI
 *
 * @return void
 *************************************************************************/
void spi_release(void);

#endif /* SPI_H_ */