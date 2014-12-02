/********************************************//**
@file tc72.h
@author: Kenneth René Jensen
@Version: 0.3
@defgroup tc72 TC72_driver
@{
	This is the driver for the temperature sensor tc72
	on the ATMEGA family processors.
@}
@note Complies MISRO 2004 standards
************************************************/

#ifndef TC72_H_
#define TC72_H_

#include <avr/io.h>

/**********************************************************************//**
 @ingroup tc72
 @brief Initiates tc72 using SPI
 @param uint8_t cs_pin - chip select pin
 @return void
*************************************************************************/
void init_tc72(uint8_t cs_pin);

/**********************************************************************//**
 @ingroup tc72
 @brief Function to read the last measured temperature from the tc72 sensor.
 @return Temperature as floating point value
 @note TC72 Operation Range: -55°C to 125°C
*************************************************************************/
float get_temperature(void);

/**********************************************************************//**
 @ingroup tc72
 @brief Function which starts measuring the temperature.
 @return void
*************************************************************************/
void measure_temperature(void);

/**********************************************************************//**
 @ingroup tc72
 @brief Function to calculate the temperature
 @return Temperature as floating point value
 @note Should only be used in Unit Testing
*************************************************************************/
float calculate_temperature(uint8_t MSB, uint8_t LSB);

#endif /* TC72_H_ */