/********************************************//**
@file accident_data.c
@author: Kenneth René Jensen
@Version: 0.5
@defgroup ad Accident Data
@{
	This is the data for an Accident report.
	Followed eCall standards for MSD data structure.
@}
@note NOT YET Complies MISRO 2004 standards
************************************************/

#include "accident_data.h"
#include "util/time.h"
#include <string.h>
#include <stdlib.h>

#define BLANK_CHAR 0x20

MSD _msd;
char UTC_string[24];

static void _raw_to_array(char **__output, char *__raw_at_str);
static void _set_UTC_string(char *__utc_raw);
static void _set_control_byte(bool __position_can_be_trusted, bool __test_call, bool __manual_alarm, bool __auto_alarm);
static void _set_VIN(char *__VIN);
static void _set_UTC_sec(char *__utc_raw);
static void _set_lat_long(char *__lat_raw, char *__long_raw);
static void _set_direction(char *__direction_raw);
static void _set_service_provider(uint8_t *__sp);
static void _set_optional_data(char *__s);

/* mode, longitude, latitude, altitude, UTC time, TTFF, Satelite in view, speed over ground, course over ground */
void set_MSD(bool __test_call, bool __manual_alarm, bool __auto_alarm, char *__GPS_AT_respons, char *__VIN, uint8_t *__SP_IPV4, char *__optional)
{
	char *output[9];

	_raw_to_array(output, __GPS_AT_respons);

	/* GPS raw data: <mode>,<longitude>,<latitude>,<altitude>,<UTC time>,<TTFF>,<num>,<speed>,<course> */
	_set_lat_long(output[2], output[1]);
	_set_UTC_sec(output[4]);
	_set_UTC_string(output[4]);
	_set_direction(output[8]);
	
	/* ToDo - Can position be trusted ?? */
	bool _confidence_in_position = true; 

	_set_control_byte(_confidence_in_position, __test_call, __manual_alarm, __auto_alarm); 
	_set_VIN(__VIN);
	_set_service_provider(__SP_IPV4);
	_set_optional_data(__optional);		/* MAX SIZE OF DATA = 102 bytes */
}

static void _raw_to_array(char **__output, char *__raw_at_str) {
	__output[0] = strtok(__raw_at_str, ",");
	__output[1] = strtok(NULL, ",");
	__output[2] = strtok(NULL, ",");
	__output[3] = strtok(NULL, ",");
	__output[4] = strtok(NULL, ",");
	__output[5] = strtok(NULL, ",");
	__output[6] = strtok(NULL, ",");
	__output[7] = strtok(NULL, ",");
	__output[8] = strtok(NULL, ",");
}

static void _set_UTC_string(char *__utc_raw)
{
	/* Format: 2014-10-12_13.17.34.000 */
	uint8_t i = 0;
	while (*__utc_raw != '\0')
	{
		if (i == 4 || i == 7)
		{
			UTC_string[i++] = '-';
		}
		else if (i == 10)
		{
			UTC_string[i++] = '_';
		}
		else if (i == 13 || i == 16)
		{
			UTC_string[i++] = '.';
		}
		else
		{
			UTC_string[i++] = *__utc_raw++;
		}
	}
	UTC_string[i++] = '\"';
}

/**********************************************************************//**
 * @ingroup ad
 * @brief function to set the control byte \n
 *	Control byte: | Automatic activation | Manual activation | Test call | Confidence in position |---Reserved--- |\n
 *	Bit:		  |	       7			 |	       6		 |     5	 |			  4			  | 3 | 2 | 1 | 0 |\n
 * @return void
 * @param test_call - Flag for testing purposes
 * @param position_can_be_trusted - confidence in position
 * @param auto_alarm - Flag to show if it is an automatic activated alarm
 * @param manual_alarm - Flag to show if it is a manual activated alarm
 *************************************************************************/
static void _set_control_byte(bool __position_can_be_trusted, bool __test_call, bool __manual_alarm, bool __auto_alarm)
{
	_msd.control |= __position_can_be_trusted<<4 | __test_call<<5 |  __manual_alarm<<6 | __auto_alarm<<7;
}

/**********************************************************************//**
 * @ingroup ad
 * @brief function to set the vehicle identification number
 * @return void
 * @param VIN - an array of 4 bytes
 * @note The number consist of 17 characters not including the letters I, O or Q.
 *************************************************************************/
static void _set_VIN(char *__VIN)
{
	uint8_t i = 0;
	while (*__VIN != '\0')
	{
		_msd.VIN[i++] = *__VIN++;
	}
	_msd.VIN[17] = BLANK_CHAR;
	_msd.VIN[18] = BLANK_CHAR;
	_msd.VIN[19] = BLANK_CHAR;
}

static void _set_UTC_sec(char *__utc_raw)
{
	char year[5] = {__utc_raw[0],  __utc_raw[1], __utc_raw[2], __utc_raw[3]};
	char month[3] = {__utc_raw[4],  __utc_raw[5]};
	char day[3] = {__utc_raw[6],  __utc_raw[7]};
	char hour[3] = {__utc_raw[8],  __utc_raw[9]};
	char minute[3] = {__utc_raw[10],  __utc_raw[11]};
	char second[3] = {__utc_raw[12],  __utc_raw[13]};
	
	FIXED_TIME t;
	t.year = atoi(year);
	t.mon = atoi(month);
	t.day = atoi(day);
	t.hour = atoi(hour);
	t.min = atoi(minute);
	t.sec = atoi(second);

	_msd.time_stamp = calc_UTC_seconds(&t);
}
#define SWAP_INT32(x) (((((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))) & 0xFFFF)
static void _set_lat_long(char *__lat_raw, char *__long_raw) {
	int lat_i = 0;
	int long_i = 0;
	int i;
	for (i = 0; i < 6; i++) {
		if (__lat_raw[i] == '.') {
			lat_i = i - 2;
		}
		if (__long_raw[i] == '.') {
			long_i = i - 2;
		}
	}

	char lat_deg[3];
	char long_deg[3];

	for (i = 0; i < lat_i; i++) {
		lat_deg[i] = __lat_raw[i];
	}
	lat_deg[lat_i] = '\0';

	for (i = 0; i < long_i; i++) {
		long_deg[i] = __long_raw[i];
	}
	long_deg[long_i] = '\0';

	/* gg + (mm.mmmmmm * 60 / 100) / 100 = gg.mmmmmmmm */
	// Needs to be change to milliarcseconds (int32_t)
	_msd.latitude = atoi(lat_deg) + atof(&__lat_raw[lat_i]) / 60;
	_msd.longitude = atoi(long_deg) + atof(&__long_raw[long_i]) / 60;
	
	//char buf[20];
	//lcd_clrscr();
	//lcd_gotoxy(0, 0);
	//lcd_puts(itoa( SWAP_INT32(_msd.latitude), buf, 2 ));
	//
	//lcd_gotoxy(0, 1);
	//lcd_puts(itoa( SWAP_INT32( _msd.longitude), buf, 2));
}

static void _set_direction(char *__direction_raw)
{
	/* (0 ? __direction_raw ? 255) */
	_msd.direction = 360.0*atoi(__direction_raw)/255.0;
}

/**********************************************************************//**
 * @ingroup ad
 * @brief function to set the service provider
 * @return void
 * @param sp - An array of 4 bytes consisting the SP in IPV4 format
 * @note May also be blank field
 *************************************************************************/
static void _set_service_provider(uint8_t *__sp)
{	
	if (__sp == NULL)
	{
		_msd.sp[0] = BLANK_CHAR;
		_msd.sp[1] = BLANK_CHAR;
		_msd.sp[2] = BLANK_CHAR;
		_msd.sp[3] = BLANK_CHAR;
	}
	
	else
	{
		_msd.sp[0] = __sp[0];
		_msd.sp[1] = __sp[1];
		_msd.sp[2] = __sp[2];
		_msd.sp[3] = __sp[3];	
	}
}

/**********************************************************************//**
 * @ingroup ad
 * @brief function to set further data (e.g. crash information, number of passengers, temperature)
 * @return void
 * @param s - Maximum 102 bytes string allowed
 * @note May also be blank field
 *************************************************************************/
static void _set_optional_data(char *__s)
{
	uint8_t i = 0;
	
	while (*__s != '\0')
	{
		_msd.optional_data[i++] = *__s++;
	}
	
    while (i < 102)
    {
		_msd.optional_data[i++] = BLANK_CHAR;
    }
}
