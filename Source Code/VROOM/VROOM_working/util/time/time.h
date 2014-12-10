/**********************************************************************//**
 * @file time.h
 * @author Kenneth René Jensen
 * @author Andi Degn
 * @version 0.2
 * @defgroup time UTC Seconds
 * @{
	 @ingroup util
	 @brief Calculates UTC seconds since EPOCH
	 @note Complies MISRA 2004 standards
 * @}
 *************************************************************************/

#ifndef TIME_H_
#define TIME_H_

#include <avr/io.h>

/**********************************************************************//**
 * @ingroup time
 * @brief struct containing all parameters for UTC time
 *************************************************************************/
typedef struct 
{
	uint16_t year;  /* Year */
	uint8_t mon;    /* Months since January [1, 12] */
	uint8_t day;	/* Day of the month [1, 31] */
	uint8_t hour;   /* Hours since midnight [0, 23] */
	uint8_t min;    /* Minutes after the hour [0, 59] */
	uint8_t sec;    /* Seconds after the minute [0, 59] */
} FIXED_TIME;

/**********************************************************************//**
 * @ingroup ac_dat_pub
 * @brief Calculates UTC seconds since epoch
 *
 * @param struct of FIXED_TIME
 *
 * @return UTC seconds as uint32_t 
 *************************************************************************/
uint32_t calc_UTC_seconds(FIXED_TIME *buf); 

#endif /* TIME_H_ */