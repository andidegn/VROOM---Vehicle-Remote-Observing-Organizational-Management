/**********************************************************************//**
 * @file accident_data.c
 *
 * @author: Kenneth Ren� Jensen
 * @Version: 0.7
 * @{
	This is the data for an Accident report.
	Comply eCall standards for MSD data structure.
	@note NOT YET Complies MISRO 2004 standards
 * @}
 *************************************************************************/

#include "accident_data.h"
#include "../hardware_boards/sim908/sim908.h"
#include "../vroom_config.h"

/**********************************************************************//**
 * @ingroup ad_priv
 * @brief define for the space char ' '
 *************************************************************************/
#define BLANK_CHAR 0x20

/* Global variables */
AD_MSD EXT_MSD;
AD_EMERGENCY_FLAG EXT_EMERGENCY_FLAG = EMERGENCY_NO_ALARM;
AT_CONNECTION_STATUS_FLAG EXT_CONNECTION_CREG_FLAG = CREG_NOT_REG_NOT_SEARCHING;

/* Local variables */
static bool _confidence_in_position;

/* Prototypes */
static void _set_control_byte(bool __position_can_be_trusted, bool __test_call, bool __manual_alarm, bool __auto_alarm);
static void _set_VIN(const char *__VIN);
static void _set_optional_data(const char *__s);

/**********************************************************************//**
 * @ingroup ad_pub
 * Starts the sequence required for placing an emergency call and send
 * an MSD packet.
 * Steps in function:\n
 * 1. Record needed data\n
 * 2. Create MSD structure\n
 * 3. sent structure over FTP\n
 * 4. call to emergency number
 **************************************************************************/
void ad_emergency_alarm(void)
{
	EXT_MSD.version = CONFIG_MSD_FORMAT_VERSION;
	EXT_MSD.vehicle_class = CONFIG_VEHICLE_CLASS;
	EXT_MSD.fuel_type = CONFIG_FUEL_TYPE;

	set_MSD_data(&EXT_MSD.time_stamp, &EXT_MSD.latitude, &EXT_MSD.longitude, &EXT_MSD.direction);
	/* ToDo - Can position be trusted ?? */
	_confidence_in_position = (EXT_MSD.latitude != 0 || EXT_MSD.longitude != 0) ? true : false;
	_set_control_byte(_confidence_in_position, CONFIG_TEST_CALL, EXT_EMERGENCY_FLAG == EMERGENCY_MANUAL_ALARM, EXT_EMERGENCY_FLAG == EMERGENCY_AUTO_ALARM);
	_set_VIN(CONFIG_VIN);

	/* ToDo - get optional data */
	_set_optional_data("ACC [G]: ? | Temp [�C]: ? | Passengers: ? | Speed [km/h]: ?");

	send_MSD(CONFIG_VROOM_ID);

	/*call_PSAP();*/

	EXT_EMERGENCY_FLAG = EMERGENCY_ALARM_SENT;
}

/**********************************************************************//**
 * @ingroup ad_pub
 * Gets the accelerometer data and analysis them to see if a crash has
 * occurred
 * Steps in function:\n
 * 1. Read accelerometer data\n
 * 2. Analysis data\n
 * 3. Sets EXT_EMERGENCY_FLAG to EMERGENCY_AUTO_ALARM if crash is detected\n
 **************************************************************************/
void ad_check_for_crash(void) {
	volatile bool _alarm = true;

	int16_t *_acc_buffer = malloc(10 * sizeof(int16_t));

	scheduler_get_last_readings_sum(_acc_buffer);

	for (uint8_t i = 0; i < CONFIG_NO_OF_READINGS; i++) {
		if (*(_acc_buffer + i) < CONFIG_ALARM_TRIGGER_VALUE) {
			_alarm = false;
			break;
		}
	}
	free(_acc_buffer);

	if (_alarm && EXT_EMERGENCY_FLAG == EMERGENCY_NO_ALARM) {
		if (!car_panel_wait_cancel_emmergency()) {
			EXT_EMERGENCY_FLAG = EMERGENCY_AUTO_ALARM;
		}
	}
}

/**********************************************************************//**
 * @ingroup ad_priv
 * @brief function to set the control byte
 *	Control byte: | Automatic activation | Manual activation | Test call | Confidence in position |---Reserved--- |\n
 *	Bit:		  |	       7			 |	       6		 |     5	 |			  4			  | 3 | 2 | 1 | 0 |\n
 *
 * @param bool position_can_be_trusted - confidence in position
 * @param bool test_call - Flag for testing purposes
 * @param bool manual_alarm - Flag to show if it is a manual activated alarm
 * @param bool auto_alarm - Flag to show if it is an automatic activated alarm
 *
 * @return void
 *************************************************************************/
static void _set_control_byte(bool __position_can_be_trusted, bool __test_call, bool __manual_alarm, bool __auto_alarm)
{
	EXT_MSD.control = __position_can_be_trusted<<4 | __test_call<<5 |  __manual_alarm<<6 | __auto_alarm<<7;
}

/**********************************************************************//**
 * @ingroup ad_priv
 * @brief function to set the vehicle identification number
 *
 * @param VIN - an array of 4 bytes
 *
 * @return void
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
 * @ingroup ad_priv
 * @brief function to set further data (e.g. crash information, number of passengers, temperature)
 *
 * @param s - Maximum 102 bytes string allowed
 *
 * @return void
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
