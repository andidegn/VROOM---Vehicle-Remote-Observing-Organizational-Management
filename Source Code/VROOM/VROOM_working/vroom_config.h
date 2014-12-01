/**********************************************************************//**
@file vroom_config.h
@author: Kenneth Ren� Jensen
@Version: 0.1
@note NOT YET Complies MISRO 2004 standards
*************************************************************************/

#ifndef VROOM_CONFIG_H_
#define VROOM_CONFIG_H_

#define YES 1
#define NO 0
#define ANDI_TEST		NO
#define KENNETH_TEST	YES
#define DEBUG_UART_ENABLE	/* uart1 (PD2/PD3) is used for debugging */
#define DEBUG_LCD_ENABLE	/* LCD should be connected to PORT L */

/**********************************************************************//**
 * @define Accident detection parameters
 * @ingroup ad_pub
 * @brief Parameters for detecting an accident
 * @code
 *************************************************************************/
#define CONFIG_ALARM_DETECTION_RESOLUTION	10		/**< Time interval between checking for accidents in hz */

#define CONFIG_ALARM_CRASH_NO_OF_READINGS	5		/**< Length of acceleration peek */
#define CONFIG_ALARM_CRASH_TRIGGER_VALUE	200		/**< Total acceleration [G] in 1/100 resolution */

#define CONFIG_ALARM_FIRE_TRIGGER_DEGREE	1		/**< Temperature raise in Celsius degrees */
#define CONFIG_ALARM_FIRE_TRIGGER_TIME		80		/**< Temperature raise time in 1/10 seconds resolution */
/** @endcode */

/* MSD details */
#define CONFIG_MSD_FORMAT_VERSION		1
#define CONFIG_MSD_TEST_CALL			true
#define CONFIG_MSD_VEHICLE_CLASS		AD_VEHICLE_CLASS_M(1)	/**< M1 = No more than eight seats in addition to the driver�s seat, and having a maximum mass not exceeding 3.5 tons */
#define CONFIG_MSD_FUEL_TYPE			AD_FUEL_TYPE_GASOLINE
#define CONFIG_MSD_VIN					"W0L000036V1940069" 

#define CONFIG_MSD_OPTIONAL_DATA_SIZE	102				/**< Calculated size based on total MSD size - compulsory data */

#define CONFIG_EMERGENCY_PHONE_NUMBER	CONFIG_PSAP_NUMBER_TEST_KENNETH

/* Emergency phone numbers */
#define CONFIG_PSAP_NUMBER_EUROPE		"112"
#define CONFIG_PSAP_NUMBER_TEST_KENNETH	"60192949" /* Only for testing */
#define CONFIG_PSAP_NUMBER_TEST_ANDI	"60257898" /* Only for testing */

/* ISP APN */
#define CONFIG_APN_TELIA				"internet.mtelia.dk"
#define CONFIG_APN_CALLME				"websp"
#define CONFIG_APN_TELENOR				"internet"
#define CONFIG_APN_TDC					"internet"

/* MSD send parameters */
#define CONFIG_FTP_SERVER_ADDRESS		"ftp.andidegn.dk"
#define CONFIG_FTP_PORT					"1404"
#define CONFIG_FTP_USER					"VROOM"
#define CONFIG_FTP_PW					"6198fg(/G6F/&5(!(!8gf87gMF."
#define CONFIG_FTP_FILE_PATH			"/"
#define CONFIG_FTP_FILE_SIZE			"140" /* no of bytes */

/* Test parameters */
#if KENNETH_TEST
	#define CONFIG_PIN					"5130"
	#define CONFIG_VROOM_ID				"60192949"
	#define CONFIG_APN					CONFIG_APN_TDC
#elif ANDI_TEST
	#define CONFIG_VROOM_ID				"60257898"
	#define CONFIG_APN					CONFIG_APN_CALLME
#endif

#endif /* VROOM_CONFIG_H_ */