/**********************************************************************//**
 * @file tc72.h
 *
 * @author Kenneth René Jensen
 * @version 0.4
 * @defgroup tc72 TC72_driver
 * @{
	@ingroup sensors
	@brief This is the driver for the temperature sensor tc72
	on the ATMEGA family processors.
	@note Complies MISRA 2004 standards
	@defgroup tc72_pub Public
	@defgroup tc72_priv Private
 * @}
 *************************************************************************/

#ifndef TC72_H_
#define TC72_H_

#include <avr/io.h>

/**********************************************************************//**
 * @ingroup tc72_pub
 * @brief Initiates tc72 using SPI
 *
 * @param uint8_t cs_pin - chip select pin
 *
 * @return void
 *************************************************************************/
void init_tc72(uint8_t __cs_pin);

/**********************************************************************//**
 * @ingroup tc72_pub
 * @brief Function to read the last measured temperature from the tc72 sensor.
 *
 * @param void
 *
 * @return Temperature as floating point value
 *
 * @note TC72 Operation Range: -55°C to 125°C
 *************************************************************************/
float get_temperature(void);

/**********************************************************************//**
 * @ingroup tc72_pub
 *
 * @param void
 *
 * @brief Function which starts measuring the temperature.
 *
 * @return void
 *************************************************************************/
void measure_temperature(void);

/**********************************************************************//**
 * @ingroup tc72_pub
 * @brief Function to calculate the temperature
 *
 * @param uint8_t __msb - most significant byte
 * @param uint8_t __lsb - least significant byte
 *
 * @return Temperature as floating point value
 * @note Should only be used in Unit Testing
 *************************************************************************/
float calculate_temperature(uint8_t __msb, uint8_t __lsb);

#endif /* TC72_H_ */