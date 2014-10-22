/********************************************//**
@file accident_data.h
@author: Kenneth René Jensen
@Version: 0.5
@defgroup ad Accident Data
@{
	This is the data for an Accident report.
	Followed eCall standards for MSD data structure.
@}
@note NOT YET Complies MISRO 2004 standards
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
	/*  Control byte: | Automatic activation | Manual activation | Test call | Confidence in position |---Reserved--- | 
		Bit no:		  |	       7			 |	       6		 |     5	 |			  4			  | 3 | 2 | 1 | 0 | */
	volatile uint8_t control;
	volatile char VIN[20];			/* VIN number 17-characters according to ISO 3779 (last 3 char is blank) */
	volatile uint32_t time_stamp;	/* UTC Seconds */
	volatile int32_t latitude;		/* Latitude (WGS-84) in milliarcseconds (-324000000 ? value ? 324000000) */
	volatile int32_t longitude;		/* Longitude (WGS-84) in milliarcseconds (-648000000 ? value ? 648000000) */
	volatile uint8_t direction;		/* Direction in degrees. The nearest integer of 360.0*value/255.0 (0 ? value ? 255) */
	volatile uint8_t sp[4];			/* Optional. Service provider IPV4 format or blank field */
	volatile char optional_data[102];/* Optional. Further data (e.g. crash information, number of passengers) or blank field */
} MSD;

extern MSD _msd;
extern unsigned char _msd_file_stream[120];

void set_MSD(bool __test_call, bool __manual_alarm, bool __auto_alarm, char *__GPS_AT_respons, char *__VIN, uint8_t *__SP_IPV4, char *__optional);

extern char UTC_string[25];

#endif /* ACCIDENT_DATA_H_ */