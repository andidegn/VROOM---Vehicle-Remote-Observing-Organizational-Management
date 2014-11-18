/********************************************//**
@file accident_data.c
@author: Kenneth René Jensen
@Version: 0.6
@defgroup ad Accident Data
@{
	This is the data for an Accident report.
	Comply eCall standards for MSD data structure.
@}
@note NOT YET Complies MISRO 2004 standards
************************************************/

#include "accident_data.h"
#include "../hardware_boards/sim908/sim908.h"
#include "../vroom_config.h"

#define BLANK_CHAR 0x20

/* Global variables */
MSD EXT_MSD;
EMERGENCY_FLAG EXT_EMERGENCY_FLAG = EMERGENCY_NO_ALARM;
CONNECTION_STATUS_FLAG EXT_CONNECTION_STATUS_FLAG = CREG_NOT_REGISTERED;

/* Local variables */
static bool _confidence_in_position;

/* Prototypes */
static void _set_control_byte(bool __position_can_be_trusted, bool __test_call, bool __manual_alarm, bool __auto_alarm);
static void _set_VIN(const char *__VIN);
static void _set_optional_data(const char *__s);

/********************************************************************************************************************//**
 @ingroup ad
 @brief Function to call when an emergency alarm is triggered. Steps in function: 
		Record needed data -> create MSD structure -> sent structure over FTP -> call to emergency number
 @return void
************************************************************************************************************************/
void emergency_alarm(void)
{
	set_MSD_data(&EXT_MSD.time_stamp, &EXT_MSD.latitude, &EXT_MSD.longitude, &EXT_MSD.direction, &EXT_MSD.sp[0]);
	
	/* ToDo - Can position be trusted ?? */
	_confidence_in_position = (EXT_MSD.latitude != 0 || EXT_MSD.longitude != 0) ? true : false;
	
	_set_control_byte(_confidence_in_position, CONFIG_TEST_CALL, EXT_EMERGENCY_FLAG == EMERGENCY_MANUAL_ALARM, EXT_EMERGENCY_FLAG == EMERGENCY_AUTO_ALARM);
	_set_VIN(CONFIG_VIN);
	
	/* ToDo - get optional data */
	_set_optional_data("ACC [G]: ? | Temp [°C]: ? | Passengers: ? | Speed [km/h]: ?");
	
	send_MSD(CONFIG_VROOM_ID);

	/* call_PSAP(); */
	
	EXT_EMERGENCY_FLAG = EMERGENCY_ALARM_SENT;
}

/**********************************************************************//**
 * @ingroup ad
 * @brief function to set the control byte \n
 *	Control byte: | Automatic activation | Manual activation | Test call | Confidence in position |---Reserved--- |\n
 *	Bit:		  |	       7			 |	       6		 |     5	 |			  4			  | 3 | 2 | 1 | 0 |\n
 * @return void
 * @param position_can_be_trusted - confidence in position
 * @param test_call - Flag for testing purposes
 * @param manual_alarm - Flag to show if it is a manual activated alarm
 * @param auto_alarm - Flag to show if it is an automatic activated alarm
 *************************************************************************/
static void _set_control_byte(bool __position_can_be_trusted, bool __test_call, bool __manual_alarm, bool __auto_alarm)
{
	EXT_MSD.control |= __position_can_be_trusted<<4 | __test_call<<5 |  __manual_alarm<<6 | __auto_alarm<<7;
}

/**********************************************************************//**
 * @ingroup ad
 * @brief function to set the vehicle identification number
 * @return void
 * @param VIN - an array of 4 bytes
 * @note The number consist of 17 characters not including the letters I, O or Q.
 *************************************************************************/
static void _set_VIN(const char *__VIN)
{
	uint8_t i = 0;
	while (*__VIN != '\0')
	{
		EXT_MSD.VIN[i++] = *__VIN++;
	}
	EXT_MSD.VIN[17] = BLANK_CHAR;
	EXT_MSD.VIN[18] = BLANK_CHAR;
	EXT_MSD.VIN[19] = BLANK_CHAR;
}

/**********************************************************************//**
 * @ingroup ad
 * @brief function to set further data (e.g. crash information, number of passengers, temperature)
 * @return void
 * @param s - Maximum 102 bytes string allowed
 * @note May also be blank field
 *************************************************************************/
static void _set_optional_data(const char *__s)
{
	uint8_t i = 0;

	do { EXT_MSD.optional_data[i++] = *__s; }  while (*__s++ != '\0');
	
    while (i < 102)
    {
		EXT_MSD.optional_data[i++] = BLANK_CHAR;
    }
}
