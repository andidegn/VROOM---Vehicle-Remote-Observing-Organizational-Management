/********************************************//**
@file accident_data.h
@author: Kenneth Ren� Jensen
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
	char VIN[20];			/* VIN number 17-characters according to ISO 3779 */
	uint32_t time_stamp;	/* UTC Seconds */
	int32_t latitude;		/* Latitude (WGS-84) in milliarcseconds (-324000000 ? value ? 324000000) */
	int32_t longitude;		/* Longitude (WGS-84) in milliarcseconds (-648000000 ? value ? 648000000) */
	uint8_t direction;		/* Direction in degrees. The nearest integer of 360.0*value/255.0 (0 ? value ? 255) */
	uint8_t sp[4];			/* IPV4 format or blank field */
	char *optional_data;	/* Further data (e.g. crash information, number of passengers) or blank field */
} MSD;

extern MSD _msd;

void set_MSD(bool __test_call, bool __manual_alarm, bool __auto_alarm, char *__GPS_AT_respons, char *__VIN, uint8_t *__SP_IPV4, char *__optional);

extern char UTC_string[25];

#endif /* ACCIDENT_DATA_H_ */