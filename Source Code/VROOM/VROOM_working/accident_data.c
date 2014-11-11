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
#include "hardware_boards/sim908/sim908.h"
#include "vroom_config.h"
#include <util/delay.h>

#define BLANK_CHAR 0x20

MSD _msd;
bool emergency_flag = false;
uint8_t connection_status_flag = CREG_VALUE_NOT_CONN;

static void _set_control_byte(bool __position_can_be_trusted, bool __test_call, bool __manual_alarm, bool __auto_alarm);
static void _set_VIN(char *__VIN);
static void _set_optional_data(char *__s);

void emergency_alarm(bool __manual_alarm, bool __auto_alarm)
{
	set_MSD_data(&_msd.time_stamp, &_msd.latitude, &_msd.longitude, &_msd.direction, &_msd.sp[0]);
	
	/* ToDo - Can position be trusted ?? */
	bool __confidence_in_position = true;
	_set_control_byte(__confidence_in_position, CONFIG_TEST_CALL, __manual_alarm, __auto_alarm);
	_set_VIN(CONFIG_VIN);
	/* ToDo - get optional data */
	_set_optional_data("ACC [G]: ? | Temp [C]: ?");
	
	//send_MSD(CONFIG_VROOM_ID);

//	call_PSAP();
	
	emergency_flag = false;
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

	do { _msd.optional_data[i++] = *__s; }  while (*__s++ != '\0');
	
    while (i < 102)
    {
		_msd.optional_data[i++] = BLANK_CHAR;
    }
}
