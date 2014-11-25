/**********************************************************************//**
 * @file accident_data.h
 *
 * @author: Kenneth René Jensen
 * @Version: 0.7
 * @defgroup ad Accident Data
 * @{
	This is the data for an Accident report.
	Comply eCall standards for MSD data structure.
	@defgroup ad_priv Private
	@defgroup ad_pub Public
 * @}
 * @note NOT YET Complies MISRO 2004 standards
 *************************************************************************/

#ifndef ACCIDENT_DATA_H_
#define ACCIDENT_DATA_H_

#include <avr/io.h>

/**********************************************************************//**
 * @struct MSD
 * @ingroup ad_pub
 * @brief struct containing the MSD for an accident report
 * @note According eCall standard EN 15722 the MSD consists of 140 bytes
 * @{
 *************************************************************************/
typedef struct __attribute__((packed))
{
	uint8_t version;			/* Referring to a public register to determine the meaning and encoding of the optional data */
	uint8_t msg_identifier;		/* Numbers of re-transmission */
	uint8_t control;			/* | bit 7: Automatic activation | bit 6: Manual activation | bit 5: Test call | bit 4: Confidence in position | bit 3-0: Reserved | */
	uint8_t vehicle_class;		/* | bit 7-4 = classification | bit 3-0: category | */
	char VIN[20];				/* VIN number 17-characters (last 3 char is blank) ISO 3779 */
	uint8_t fuel_type;		
	uint32_t time_stamp;		/* UTC Seconds */
	int32_t latitude;			/* Latitude (WGS-84) in milliarcseconds (-324000000 ? value ? 324000000) ISO 6709 */ 
	int32_t longitude;			/* Longitude (WGS-84) in milliarcseconds (-648000000 ? value ? 648000000) ISO 6709 */
	uint8_t direction;			/* Direction in degrees. The nearest integer of 255.0*value/360.0 (0 <= value >= 255) */
	char optional_data[102];	/* Optional. Further data (e.g. crash information, number of passengers) or blank field */
} MSD;
/** @} */

/**********************************************************************//**
 * @define FUEL_TYPES
 * @ingroup ad_pub
 * @brief Constant vehicle fuel type definition
 * @{
 *************************************************************************/
#define FUEL_TYPE_OTHER			0
#define FUEL_TYPE_GASOLINE		1
#define FUEL_TYPE_DIESEL		2
#define FUEL_TYPE_NATURALGAS	3
#define FUEL_TYPE_PROPANE		4
#define FUEL_TYPE_ELECTRIC		5
#define FUEL_TYPE_HYDROGEN		6
/** @} */

/**********************************************************************//**
 * @define VEHICLE_CLASS
 * @ingroup ad_pub
 * @brief Constant vehicle class type definition
 * @note Upper nibble contains class definition: [SA, L, M, N, O, T, G]
 * @	 Lower nibble contains class category definition: [1 - 7]
 * @	 Details: http://eur-lex.europa.eu/legal-content/EN/TXT/PDF/?uri=CELEX:32007L0046&from=EN 
 * @{
 *************************************************************************/
#define VEHICLE_CLASS_L(category)	(1<<4 | category)	/* Motor vehicles with less than four wheels */
#define VEHICLE_CLASS_M(category)	(2<<4 | category)	/* Power-driven vehicles having at least four wheels and used for the carriage of passengers */
#define VEHICLE_CLASS_N(category)	(3<<4 | category)	/* Power-driven vehicles having at least four wheels and used for the carriage of goods */
#define VEHICLE_CLASS_O(category)	(4<<4 | category)	/* Trailers (including semi–trailers) */
#define VEHICLE_CLASS_T(category)	(5<<4 | category)	/* Agricultural and Forestry tractors */
#define VEHICLE_CLASS_G(category)	(6<<4 | category)	/* Off-road vehicles */
#define VEHICLE_CLASS_SA(category)	(7<<4 | category)	/* Special purpose vehicles (e.g. Ambulance, Hearse, Armoured vehicle or Motor caravan)	*/
/** @} */

/**********************************************************************//**
 * @ingroup ad_pub
 * @brief Flag to detect if an alarm has been triggered
 * @{
 *************************************************************************/
typedef enum {	EMERGENCY_NO_ALARM = 0,
				EMERGENCY_MANUAL_ALARM = 1,
				EMERGENCY_AUTO_ALARM = 2,
				EMERGENCY_ALARM_SENT = 3
} EMERGENCY_FLAG;
/** @} */

/**********************************************************************//**
 * @ingroup ad_pub
 * @brief Flag to detect the connection status
 * @note Corresponding to CREG response in SIM908 p. 77 in AT Commands manual v. 1.02
 * @{
 *************************************************************************/
typedef enum {	CREG_NOT_REGISTERED = 0,
				CREG_REGISTERED = 1,
				CREG_SEARCHING = 2,
				CREG_REGISTRATION_DENIED = 3,
				CREG_UNKNOWN = 4,
				CREG_REGISTERED_ROOMING = 5,
} CONNECTION_STATUS_FLAG;
/** @} */

extern char EXT_MSD_FILENAME[24];
extern MSD EXT_MSD;
extern EMERGENCY_FLAG EXT_EMERGENCY_FLAG;
extern CONNECTION_STATUS_FLAG EXT_CONNECTION_CREG_FLAG;

/**********************************************************************//**
 * @ingroup ad_pub
 * @brief Function to call when an emergency alarm is triggered.
 *
 * @param void
 *
 * @return void
 *************************************************************************/
void emergency_alarm(void);

#endif /* ACCIDENT_DATA_H_ */