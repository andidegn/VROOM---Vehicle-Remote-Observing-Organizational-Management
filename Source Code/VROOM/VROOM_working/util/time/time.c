/*
 * time.c
 * Created: 20-10-2014 13:25:46
 */ 

#include "time.h"
#include <stdbool.h>

#define EPOCH_YR                1970U
#define SECS_DAY                86400UL

#define LEAPYEAR(year)          (((year) % 4 == 0) && (((year) % 100 != 0) || ((year) % 400 == 0)))

const uint8_t DAYS_MONTH[2U][12U] = 
{
	{31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U},
	{31U, 29U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U}
};

uint32_t calc_UTC_seconds(FIXED_TIME *buf) 
{ 
    volatile uint8_t _is_leap_year = LEAPYEAR((int16_t)(buf->year)) ? 1U : 0U;
    uint16_t _days_year = 0U;
	uint32_t _total_seconds = 0U;
    uint16_t i = 0U;
    
    for (i = EPOCH_YR; i < buf->year; i++) {
      _days_year = (LEAPYEAR((int16_t)i) ? 366U : 365U);
      _total_seconds += (_days_year * SECS_DAY);
    }

    for (i = 0U; i < (uint8_t)(buf->mon - 1U); i++)
    {
      _total_seconds += DAYS_MONTH[_is_leap_year][i] * SECS_DAY;
    }
  
    _total_seconds += ((buf->day - 1UL) * SECS_DAY) + (uint32_t)(buf->hour * 3600UL) + (uint32_t)(buf->min * 60UL) + buf->sec;
	
    return _total_seconds;
}
