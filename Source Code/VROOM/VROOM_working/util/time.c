/*
 * time.c
 *
 * Created: 20-10-2014 13:25:46
 *  Author: Kenneth René Jensen
	Note: File is a modified version of Michael Ringgaard's time.c
 */ 

#include "time.h"

#define YEAR0                   1900
#define EPOCH_YR                1970
#define SECS_DAY                (24L * 60L * 60L)
#define LEAPYEAR(year)          (!((year) % 4) && (((year) % 100) || !((year) % 400)))
#define YEARSIZE(year)          (LEAPYEAR(year) ? 366 : 365)
#define FIRSTSUNDAY(timp)       (((timp)->tm_yday - (timp)->tm_wday + 420) % 7)
#define FIRSTDAYOF(timp)        (((timp)->tm_wday - (timp)->tm_yday + 420) % 7)

#define TIME_MAX                2147483647L

int _daylight = 0;                  // Non-zero if daylight savings time is used
long _dstbias = 0;                  // Offset for Daylight Saving Time
long _timezone = 0;                 // Difference in seconds between GMT and local time

const int _ytab[2][12] = 
{
	{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};


uint32_t calc_UTC_seconds(FIXED_TIME *buf) 
{
	long _day, _year;
	int _tm_year, _yday, _month;
	long _seconds;
		
	buf->year -= YEAR0;
	buf->mon -= 1;
	
	buf->min += buf->sec / 60;
	buf->sec %= 60;
	if (buf->sec < 0) 
	{
		buf->sec += 60;
		buf->min--;
	}
	  
	buf->hour += buf->min / 60;
	buf->min = buf->min % 60;
	if (buf->min < 0) 
	{
		buf->min += 60;
		buf->hour--;
	}
	  
	_day = buf->hour / 24;
	buf->hour = buf->hour % 24;
	if (buf->hour < 0) 
	{
		buf->hour += 24;
		_day--;
	}
	  
	buf->year += buf->mon / 12;
	buf->mon %= 12;
	if (buf->mon < 0) 
	{
		buf->mon += 12;
		buf->year--;
	}
	  
	_day += (buf->day - 1);
	  
	while (_day < 0) 
	{
		if(--buf->mon < 0) 
		{
			buf->year--;
			buf->mon = 11;
		}
		_day += _ytab[LEAPYEAR(YEAR0 + buf->year)][buf->mon];
	}
	  
	while (_day >= _ytab[LEAPYEAR(YEAR0 + buf->year)][buf->mon]) 
	{
		_day -= _ytab[LEAPYEAR(YEAR0 + buf->year)][buf->mon];
		if (++(buf->mon) == 12) 
		{
			buf->mon = 0;
			buf->year++;
		}
	}
	  
	buf->day = _day + 1;
	  
	_year = EPOCH_YR;
	  
	if (buf->year < _year - YEAR0) 
	return  -1;
		
	_seconds = 0;
	_day = 0;                      // Means days since day 0 now

	// Assume that when day becomes negative, there will certainly
	// be overflow on seconds.
	// The check for overflow needs not to be done for leapyears
	// divisible by 400.
	// The code only works when year (1970) is not a leapyear.
	_tm_year = buf->year + YEAR0;
		
	_day = (_tm_year - _year) * 365;
	  	
	_day += (_tm_year - _year) / 4 + ((_tm_year % 4) && _tm_year % 4 < _year % 4);
	_day -= (_tm_year - _year) / 100 + ((_tm_year % 100) && _tm_year % 100 < _year % 100);
	_day += (_tm_year - _year) / 400 + ((_tm_year % 400) && _tm_year % 400 < _year % 400);

	_yday = _month = 0;
	while (_month < buf->mon) 
	{
		_yday += _ytab[LEAPYEAR(_tm_year)][_month];
		_month++;
	}

	_yday += (buf->day - 1);
	  
	_day += _yday;

	_seconds = ((buf->hour * 60L) + buf->min) * 60L + buf->sec;
		
	_seconds += _day * SECS_DAY;

	_seconds += _timezone;
 
		 
	return _seconds;
}
