/********************************************//**
@file accident_data.h
@author: Kenneth René Jensen
@Version: 0.6
@defgroup ad Accident Data
@{
	This is the data for an Accident report.
	Followed eCall standards for MSD data structure.
@}
@note NOT YET Complies MISRO 2004 standards
************************************************/

#ifndef ACCIDENT_DATA_H_
#define ACCIDENT_DATA_H_

#include <string.h>
#include <stdbool.h>
#include <stdio.h>

/**********************************************************************//**
 * @ingroup ad
 * @brief struct containing the MSD for an accident report
 * @note According eCall standard EN 15722 the MSD consists of 140 bytes
 *************************************************************************/
typedef struct __attribute__((packed))
{
	/*  Control byte: | Automatic activation | Manual activation | Test call | Confidence in position |---Reserved--- | 
		Bit no:		  |	       7			 |	       6		 |     5	 |			  4			  | 3 | 2 | 1 | 0 | */
	uint8_t control;
	char VIN[20];			/* VIN number 17-characters according to ISO 3779 (last 3 char is blank) */
	uint32_t time_stamp;	/* UTC Seconds */
	int32_t latitude;		/* Latitude (WGS-84) in milliarcseconds (-324000000 ? value ? 324000000) */
	int32_t longitude;		/* Longitude (WGS-84) in milliarcseconds (-648000000 ? value ? 648000000) */
	uint8_t direction;		/* Direction in degrees. The nearest integer of 360.0*value/255.0 (0 ? value ? 255) */
	uint8_t sp[4];			/* Optional. Service provider IPV4 format or blank field */
	char optional_data[102];/* Optional. Further data (e.g. crash information, number of passengers) or blank field */
} MSD;

extern MSD _msd;
extern char MSD_filename[24];
extern bool emergency_flag;

void emergency_alarm(bool __manual_alarm, bool __auto_alarm);

#endif /* ACCIDENT_DATA_H_ */