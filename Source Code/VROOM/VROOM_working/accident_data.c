/********************************************//**
@file accident_data.c
@author: Kenneth René Jensen
@Version: 0.1
@defgroup ad Accident Data
@{
	This is the data for an Accident report.
	Followed eCall standards for data structure.
@}
@note Complies MISRO 2004 standards
************************************************/

#include "accident_data.h"
#include "util/time.h"
#include <string.h>
#include <stdlib.h>

#define POSITION_TRUSTED		(1<<4)
#define TEST_CALL				(1<<5)
#define MANUAL_ACTIVATION		(1<<6)
#define AUTOMATIC_ACTIVATION	(1<<7)

MSD _msd;

static void _raw_to_array(char **__output, char *__raw_at_str);
static void _set_lat_long(char *__lat_raw, char *__long_raw);
static void _set_UTC_sec(char *__utc_raw);
static void _set_direction(char *__direction_raw);
static void _set_VIN(char *VIN);
static void _set_optional_data(char *s);
static void _set_service_provider(uint8_t *sp);

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
void set_control_byte(bool test_call, bool position_can_be_trusted, bool automatic_activation)
{
    if (test_call)
	{
        _msd.control |= TEST_CALL;
    }
	else
	{
        _msd.control &= ~TEST_CALL;
    }

    if (position_can_be_trusted)
	{
        _msd.control |= POSITION_TRUSTED;
    }
	else
	{
        _msd.control &= ~POSITION_TRUSTED;
    }

    if (automatic_activation)
	{
        _msd.control |= AUTOMATIC_ACTIVATION;
        _msd.control &= ~MANUAL_ACTIVATION;
    }
	else
	{
        _msd.control |= MANUAL_ACTIVATION;
        _msd.control &= ~AUTOMATIC_ACTIVATION;
    }
}

/* mode, longitude, latitude, altitude, UTC time, TTFF, Satelite in view, speed over ground, course over ground */
void set_MSD(char *__GPS_AT_respons, char *__VIN, uint8_t *__SP_IPV4, char *__optional)
{
	char *output[9];

	_raw_to_array(output, __GPS_AT_respons);

	/* GPS raw data: <mode>,<longitude>,<latitude>,<altitude>,<UTC time>,<TTFF>,<num>,<speed>,<course> */
	_set_lat_long(output[2], output[1]);
	_set_UTC_sec(output[4]);
	_set_direction(output[8]);

	_set_VIN(__VIN);
	_set_service_provider(__SP_IPV4);
	_set_optional_data(__optional);		/* MAX SIZE OF DATA = 102 bytes */
}

void get_UTC_string(char *__raw_at_str, char *__UTC_time_string)
{
	__raw_at_str = &__raw_at_str[0];
	char *utc_raw;
	strtok(__raw_at_str, ",");
	strtok(NULL, ",");
	strtok(NULL, ",");
	strtok(NULL, ",");
	utc_raw = strtok(NULL, ",");

	uint8_t i = 0;
	while (*utc_raw != '\0')
	{
		if (i == 4 || i == 7)
		{
			__UTC_time_string[i++] = '-';
		}
		else if (i == 10)
		{
			__UTC_time_string[i++] = '_';
		}
		else if (i == 13 || i == 16)
		{
			__UTC_time_string[i++] = '.';
		}
		else
		{
			__UTC_time_string[i++] = *utc_raw++;
		}
	}
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

static void _set_direction(char *__direction_raw)
{
	/* (0 ? __direction_raw ? 255) */
	_msd.direction = 360.0*atoi(__direction_raw)/255.0;
}


/**********************************************************************//**
 * @ingroup ad
 * @brief function to set the vehicle identification number
 * @return void
 * @param VIN - an array of 4 bytes
 * @note The number consist of 17 characters not including the letters I, O or Q.
 *************************************************************************/
static void _set_VIN(char *VIN)
{
    _msd.VIN[0] = VIN[0];
    _msd.VIN[1] = VIN[1];
    _msd.VIN[2] = VIN[2];
    _msd.VIN[3] = VIN[3];
}

/**********************************************************************//**
 * @ingroup ad
 * @brief function to set further data (e.g. crash information, number of passengers, temperature)
 * @return void
 * @param s - Maximum 102 bytes string allowed
 * @note May also be blank field
 *************************************************************************/
static void _set_optional_data(char *s)
{
    /* MAX SIZE OF DATA = 102 bytes */
    _msd.optional_data = s;
}

/**********************************************************************//**
 * @ingroup ad
 * @brief function to set the service provider
 * @return void
 * @param sp - An array of 4 bytes consisting the SP in IPV4 format
 * @note May also be blank field
 *************************************************************************/
static void _set_service_provider(uint8_t *sp)
{
    _msd.sp[0] = sp[0];
    _msd.sp[1] = sp[1];
    _msd.sp[2] = sp[2];
    _msd.sp[3] = sp[3];
}