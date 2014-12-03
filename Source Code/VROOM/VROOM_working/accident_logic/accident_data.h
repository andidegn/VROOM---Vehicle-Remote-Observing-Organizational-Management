/**********************************************************************//**
 * @file accident_data.h
 *
 * @author Kenneth René Jensen
 * @version 0.7
 * @defgroup ac_dat Accident Data
 * @{
	@ingroup accident_logic
	@brief This is the data for an Accident report.
	Comply eCall standards for MSD data structure.
	@note NOT YET Complies MISRO 2004 standards
	@defgroup ac_dat_pub Public
	@defgroup ac_dat_priv Private
* @}
 *************************************************************************/

#ifndef ACCIDENT_DATA_H_
#define ACCIDENT_DATA_H_

#include <avr/io.h>
#include "../vroom_config.h"
#include "../hardware_boards/sim908/at_commands.h"

/**********************************************************************//**
 * @ingroup ac_dat_pub
 * @brief struct containing the MSD for an accident report
 * @note According eCall standard EN 15722 the MSD consists of 140 bytes
 *************************************************************************/
typedef struct 
{
	uint8_t version;									/**> Referring to a public register to determine the meaning and encoding of the optional data */
	uint8_t msg_identifier;								/**> Numbers of re-transmission */
	uint8_t control;									/**> | bit 7: Automatic activation | bit 6: Manual activation | bit 5: Test call | bit 4: Confidence in position | bit 3-0: Reserved | */
	uint8_t vehicle_class;								/**> | bit 7-4 = classification | bit 3-0: category | */
	char VIN[20];										/**> VIN number 17-characters (last 3 char is blank) ISO 3779 */
	uint8_t fuel_type;
	uint32_t time_stamp;								/**> UTC Seconds */
	int32_t latitude;									/**> Latitude (WGS-84) in milliarcseconds (-324000000 >= value <= 324000000) ISO 6709 */
	int32_t longitude;									/**> Longitude (WGS-84) in milliarcseconds (-648000000 >= value <= 648000000) ISO 6709 */
	uint8_t direction;									/**> Direction in degrees. The nearest integer of 255.0*value/360.0 (0 <= value <= 255) */
	char optional_data[CONFIG_MSD_OPTIONAL_DATA_SIZE];	/**> Optional. Further data (e.g. crash information, number of passengers) or blank field */
} AD_MSD;

/**********************************************************************//**
 * @ingroup ac_dat_pub
 * @brief Constant vehicle fuel type definition
 * @defgroup ac_dat_fule Fule types
 * @{
 *************************************************************************/
#define AD_FUEL_TYPE_OTHER			0
#define AD_FUEL_TYPE_GASOLINE		1
#define AD_FUEL_TYPE_DIESEL			2
#define AD_FUEL_TYPE_NATURALGAS		3
#define AD_FUEL_TYPE_PROPANE		4
#define AD_FUEL_TYPE_ELECTRIC		5
#define AD_FUEL_TYPE_HYDROGEN		6
/** @} */

/**********************************************************************//**
 * @ingroup ac_dat_pub
 * @brief Constant vehicle class type definition
 * @note\n
 * - Upper nibble contains class definition: [SA, L, M, N, O, T, G]\n
 * - Lower nibble contains class category definition: [1 - 7]\n
 * - Details: http://eur-lex.europa.eu/legal-content/EN/TXT/PDF/?uri=CELEX:32007L0046&from=EN
 * @defgroup ac_dat_vehicle_class Vehicle class
 * @{
 *************************************************************************/
#define AD_VEHICLE_CLASS_L(category)	(1<<4 | category)	/**> Motor vehicles with less than four wheels */
#define AD_VEHICLE_CLASS_M(category)	(2<<4 | category)	/**> Power-driven vehicles having at least four wheels and used for the carriage of passengers */
#define AD_VEHICLE_CLASS_N(category)	(3<<4 | category)	/**> Power-driven vehicles having at least four wheels and used for the carriage of goods */
#define AD_VEHICLE_CLASS_O(category)	(4<<4 | category)	/**> Trailers (including semi–trailers) */
#define AD_VEHICLE_CLASS_T(category)	(5<<4 | category)	/**> Agricultural and Forestry tractors */
#define AD_VEHICLE_CLASS_G(category)	(6<<4 | category)	/**> Off-road vehicles */
#define AD_VEHICLE_CLASS_SA(category)	(7<<4 | category)	/**> Special purpose vehicles (e.g. Ambulance, Hearse, Armoured vehicle or Motor caravan)	*/
/** @} */

/**********************************************************************//**
 * @ingroup ac_dat_pub
 * @brief Flag to detect if an alarm has been triggered
 *************************************************************************/
typedef enum {	EMERGENCY_NO_ALARM = 0,
				EMERGENCY_MANUAL_ALARM = 1,
				EMERGENCY_AUTO_ALARM = 2,
				EMERGENCY_ALARM_SENT = 3
} AD_EMERGENCY_FLAG;

extern char EXT_MSD_FILENAME[24];
extern AD_MSD EXT_MSD;
extern AD_EMERGENCY_FLAG EXT_EMERGENCY_FLAG;
extern AT_CONNECTION_STATUS_FLAG EXT_CONNECTION_CREG_FLAG;
extern float EXT_TEMPERATURE;
extern float EXT_TOTAL_ACCELERATION_AVG;

/**********************************************************************//**
 * @ingroup ac_dat_pub
 * @brief Function to call when an emergency alarm is triggered.
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void ad_emergency_alarm(void);

#endif /* ACCIDENT_DATA_H_ */