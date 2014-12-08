/**********************************************************************//**
 * @file accident_data.c
 *************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "accident_data.h"
#include "../application/vroom_config.h"
#include "../application/scheduler/scheduler.h"
#include "../hardware_boards/sim908/sim908.h"

#define BLANK_CHAR 0x20U	/**> Define for the space char ' ' */

/* Global variables */
AD_MSD EXT_MSD;
AD_EMERGENCY_FLAG EXT_EMERGENCY_FLAG = EMERGENCY_NO_ALARM;
AT_CONNECTION_STATUS_FLAG EXT_CONNECTION_CREG_FLAG = CREG_NOT_REG_NOT_SEARCHING;
float EXT_TEMPERATURE = 0;
float EXT_TOTAL_ACCELERATION_AVG = 0;

/* Local variables */
static bool _confidence_in_position;

/* Prototypes */
static void _set_control_byte(bool __position_can_be_trusted, bool __test_call, bool __manual_alarm, bool __auto_alarm);
static void _set_VIN(const char *__VIN);
static void _set_optional_data(void);

/**********************************************************************//**
 * @ingroup ac_dat_pub
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
	EXT_MSD.vehicle_class = CONFIG_MSD_VEHICLE_CLASS;
	EXT_MSD.fuel_type = CONFIG_MSD_FUEL_TYPE;

	set_MSD_data(&EXT_MSD.time_stamp, &EXT_MSD.latitude, &EXT_MSD.longitude, &EXT_MSD.direction);
	/* ToDo - Can position be trusted ?? */
	_confidence_in_position = ((EXT_MSD.latitude != 0 ) || (EXT_MSD.longitude != 0)) ? true : false;
	_set_control_byte(_confidence_in_position, CONFIG_MSD_TEST_CALL, EXT_EMERGENCY_FLAG == EMERGENCY_MANUAL_ALARM, EXT_EMERGENCY_FLAG == EMERGENCY_AUTO_ALARM);
	_set_VIN(CONFIG_MSD_VIN);
	_set_optional_data();

	send_MSD(CONFIG_VROOM_ID);

	#ifdef CONFIG_ENABLE_EMERGENCY_PHONE_CALL
		call_PSAP();
	#endif
	
	EXT_EMERGENCY_FLAG = EMERGENCY_ALARM_SENT;
}

/**********************************************************************//**
 * @ingroup ac_dat_priv
 * @brief function to set the control byte
 * |Control byte  | Automatic activation | Manual activation | Test call | Confidence in position |Res|Res|Res|Res|
 * |:-------------|:--------------------:|:-----------------:|:---------:|:----------------------:|:-:|:-:|:-:|:-:|
 * |Bit           |           7          |         6         |     5     |            4           | 3 | 2 | 1 | 0 |
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
	EXT_MSD.control = (uint8_t)(__position_can_be_trusted<<4U) | (uint8_t)(__test_call<<5U) |  (uint8_t)(__manual_alarm<<6U) | (uint8_t)(__auto_alarm<<7U);
}

/**********************************************************************//**
 * @ingroup ac_dat_priv
 * @brief function to set the vehicle identification number
 *
 * @param VIN - an array of 4 bytes
 *
 * @return void
 * @note The number consist of 17 characters not including the letters I, O or Q.
 *************************************************************************/
static void _set_VIN(const char *__VIN)
{
	uint8_t i = 0U;
	while (*__VIN != '\0')
	{
		EXT_MSD.VIN[i++] = *__VIN++;
	}
	EXT_MSD.VIN[17] = BLANK_CHAR;
	EXT_MSD.VIN[18] = BLANK_CHAR;
	EXT_MSD.VIN[19] = BLANK_CHAR;
}

/**********************************************************************//**
 * @ingroup ac_dat_priv
 * @brief function to set further data (e.g. crash information, number of passengers, temperature)
 *
 * @param s - Maximum 102 bytes string allowed
 * 
 * @return void
 * @note Encoding optional data depends on MSD version. 
 *************************************************************************/
static void _set_optional_data(void)
{
	uint8_t i = 0U;
	char *buf = malloc(10U * sizeof(char));
	
	int16_t *_acc_buffer = malloc(CONFIG_ALARM_CRASH_NO_OF_READINGS * sizeof(int16_t));
	scheduler_acc_get_last_readings_sum(_acc_buffer);
	for (i = 0U; i < CONFIG_ALARM_CRASH_NO_OF_READINGS; i++) 
	{
		EXT_TOTAL_ACCELERATION_AVG += (float)(*(_acc_buffer + i));
	}
	EXT_TOTAL_ACCELERATION_AVG /= ((float)i*100.0F);
	free(_acc_buffer);
	
	EXT_TEMPERATURE = scheduler_temp_get_last_reading();

	strcpy(EXT_MSD.optional_data, dtostrf( EXT_TOTAL_ACCELERATION_AVG, 2, 2, buf ));
	strcat(EXT_MSD.optional_data, "\n");
	strcat(EXT_MSD.optional_data, dtostrf( EXT_TEMPERATURE, 2, 2, buf ));
	strcat(EXT_MSD.optional_data, "\n");
	strcat(EXT_MSD.optional_data, itoa(0, buf, 10));
	strcat(EXT_MSD.optional_data, "\n");
	strcat(EXT_MSD.optional_data, itoa(0, buf, 10));

	for (i = 14U; i < CONFIG_MSD_OPTIONAL_DATA_SIZE; i++)
	{
		EXT_MSD.optional_data[i] = BLANK_CHAR;
	}

	free(buf);
}
