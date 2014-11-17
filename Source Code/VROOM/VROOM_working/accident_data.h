/********************************************//**
@file accident_data.h
@author: Kenneth René Jensen
@Version: 0.6
@defgroup ad Accident Data
@{
	This is the data for an Accident report.
	Comply eCall standards for MSD data structure.
@}
@note NOT YET Complies MISRO 2004 standards
************************************************/

#ifndef ACCIDENT_DATA_H_
#define ACCIDENT_DATA_H_

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

/**********************************************************************//**
 * @ingroup ad
 * @brief Flag to detect if an alarm has been triggered
 *************************************************************************/
typedef enum {	EMERGENCY_NO_ALARM = 0,
				EMERGENCY_MANUAL_ALARM = 1,
				EMERGENCY_AUTO_ALARM = 2,
				EMERGENCY_ALARM_SENT = 3
} EMERGENCY_FLAG;

/**********************************************************************//**
 * @ingroup ad
 * @brief Flag to detect the connection status
 * @note Corresponding to CREG response in SIM908 p. 77 in AT Commands manual v. 1.02 
 *************************************************************************/
typedef enum {	CREG_NOT_REGISTERED = 0,
				CREG_REGISTERED = 1,
				CREG_SEARCHING = 2,
				CREG_REGISTRATION_DENIED = 3,
				CREG_UNKNOWN = 4,
				CREG_REGISTERED_ROOMING = 5
} CONNECTION_STATUS_FLAG;

extern char EXT_MSD_FILENAME[24];
extern MSD EXT_MSD;
extern EMERGENCY_FLAG EXT_EMERGENCY_FLAG;
extern CONNECTION_STATUS_FLAG EXT_CONNECTION_STATUS_FLAG;

/********************************************************************************************************************//**
 @ingroup ad
 @brief Function to call when an emergency alarm is triggered. Steps in function: 
		Record needed data -> create MSD structure -> sent structure over FTP -> call to emergency number
 @return void
************************************************************************************************************************/
void emergency_alarm(void);

#endif /* ACCIDENT_DATA_H_ */