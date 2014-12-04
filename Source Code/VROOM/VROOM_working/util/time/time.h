/*
 * time.h
 *
 * Created: 20-10-2014 13:25:58
 *  Author: Kenneth René Jensen
 * Note: File is a modified version of Michael Ringgaard's time.h
 */ 

#ifndef TIME_H_
#define TIME_H_

#include <stdio.h>

typedef struct 
{
	uint16_t year;  // Year
	uint8_t mon;    // Months since January [1, 12]
	uint8_t day;	// Day of the month [1, 31]
	uint8_t hour;   // Hours since midnight [0, 23]
	uint8_t min;    // Minutes after the hour [0, 59]
	uint8_t sec;    // Seconds after the minute [0, 59]
} FIXED_TIME;

uint32_t calc_UTC_seconds(FIXED_TIME *buf); 

#endif /* TIME_H_ */