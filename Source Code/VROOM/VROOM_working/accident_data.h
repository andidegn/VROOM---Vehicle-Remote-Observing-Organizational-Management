/********************************************//**
@file accident_data.h
@author: Kenneth René Jensen
@Version: 0.1
@defgroup ad Accident Data
@{
	This is the data for an Accident report. 
	Followed eCall standards for data structure.
@}
@note Complies MISRO 2004 standards
************************************************/

#ifndef ACCIDENT_DATA_H_
#define ACCIDENT_DATA_H_

#include <stdbool.h>
#include <stdio.h>

/**********************************************************************//**
 * @ingroup ad
 * @brief struct containing the MSD for an accident report
 * @note According eCall standard EN 15722 the MSD consists of 140 bytes 
 *************************************************************************/
typedef struct 
{
	/*  Content of control byte: |---Reserved--- | No confidence in position | Test call | Manual activation | Automatic activation | 
		Bit:					 | 0 | 1 | 2 | 3 |			  4				 |     5	 |	    6			 |	       7		    | */
	uint8_t control;		
	char VIN[20];			/* VIN number according to ISO 3779 */
	uint32_t time_stamp;	/* UTC Seconds */
	int32_t latitude;		/* Latitude (WGS-84) in milliarcseconds (-324000000 ? value ? 324000000) */
	int32_t longitude;		/* Longitude (WGS-84) in milliarcseconds (-648000000 ? value ? 648000000) */
	uint8_t direction;		/* Direction in degrees. The nearest integer of 360.0*value/255.0 (0 ? value ? 255) */
	uint8_t sp[4];			/* IPV4 format or blank field */
	char *optional_data;	/* Further data (e.g. crash information, number of passengers) or blank field */
} MSD;

extern MSD _msd;

/**********************************************************************//**
 * @ingroup ad
 * @brief function to set the control byte \n
 * 	Content of control byte: |---Reserved--- | No confidence in position | Test call | Manual activation | Automatic activation | \n
	Bit:					 | 0 | 1 | 2 | 3 |			  4				 |     5	 |	    6			 |	       7		    | \n
 * @return void
 * @param test_call - Flag for testing purposes
 * @param position_can_be_trusted - confidence in position
 * @param automatic_activation - Flag to show if it is an manual or automatic activated alarm
 *************************************************************************/
void set_control_byte(bool test_call, bool position_can_be_trusted, bool automatic_activation);

/**********************************************************************//**
 * @ingroup ad
 * @brief function to set the vehicle identification number 
 * @return void
 * @param VIN - an array of 4 bytes
 * @note The number consist of 17 characters not including the letters I, O or Q.
 *************************************************************************/
void set_VIN(char VIN[]);

/**********************************************************************//**
 * @ingroup ad
 * @brief function to set the vehicle location
 * @return void
 * @param latitude - Latitude (WGS-84) in milliarcseconds (-324000000 ? value ? 324000000) 
 * @param longitude - Longitude (WGS-84) in milliarcseconds (-648000000 ? value ? 648000000)
 *************************************************************************/
void set_location(int32_t latitude, int32_t longitude);

/**********************************************************************//**
 * @ingroup ad
 * @brief function to set the vehicle direction
 * @return void
 * @param value - The nearest integer of 360.0*value/255.0 (0 ? value ? 255)
 *************************************************************************/
void set_direction(uint8_t value);

/**********************************************************************//**
 * @ingroup ad
 * @brief function to set the service provider
 * @return void
 * @param sp - An array of 4 bytes consisting the SP in IPV4 format
 * @note May also be blank field
 *************************************************************************/
void set_service_provider(uint8_t sp[]);

/**********************************************************************//**
 * @ingroup ad
 * @brief function to set further data (e.g. crash information, number of passengers, temperature)
 * @return void
 * @param s - Maximum 102 bytes string allowed
 * @note May also be blank field
 *************************************************************************/
void set_optional_data(char *s);

#endif /* ACCIDENT_DATA_H_ */