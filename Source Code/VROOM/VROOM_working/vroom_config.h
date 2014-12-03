/**********************************************************************//**
 * @file vroom_config.h
 * @author: Andi Degn
 * @author: Kenneth René Jensen
 * @Version: 0.6
 *
 * @defgroup accident_logic Accident Logic
 * @defgroup data_communication Data Communication
 * @defgroup hardware_boards Hardware Boards
 * @defgroup sensors Sensors
 * @defgroup test Test
 * @defgroup util Utility
 *
 * @defgroup vroom_cfg Vroom Config
 * @{
	 Configuration file containing all adjustment and setup parameters
	 for the whole system.
	 @note NOT YET Complies MISRO 2004 standards
 * @}
 *************************************************************************/

#ifndef VROOM_CONFIG_H_
#define VROOM_CONFIG_H_

#define YES 1
#define NO 0
#define ANDI_TEST		NO
#define KENNETH_TEST	YES
#define DEBUG_UART_ENABLE	/* uart1 (PD2/PD3) is used for debugging */
#define DEBUG_UART_ECHO
//#define DEBUG_SIM908_CALLBACK
//#define DEBUG_SIM908_CALLBACK_NOT_CAUGHT
#define DEBUG_LCD_ENABLE	/* LCD should be connected to PORT L */

// #define CONFIG_ENABLE_EMERGENCY_PHONE_CALL

/**********************************************************************//**
 * @ingroup vroom_cfg
 * @brief Parameters for detecting an accident
 * @defgroup vc_adp Accident detection parameters
 * @{
 *************************************************************************/
#define CONFIG_ALARM_SENSOR_READ_FREQUENCY	TIMER_50HZ	/**> Frequency of which the sensors are read in Hz */
#define CONFIG_ALARM_DETECTION_FREQUENCY	TIMER_10HZ	/**> Time interval between checking for accidents in Hz */

#define CONFIG_ALARM_CRASH_NO_OF_READINGS	5			/**> Length of acceleration peek */
#define CONFIG_ALARM_CRASH_TRIGGER_VALUE	400			/**> 800 Total acceleration [G] in 1/100 resolution */

#define CONFIG_ALARM_FIRE_TRIGGER_DEGREE	1			/**> Temperature raise in Celsius degrees */
#define CONFIG_ALARM_FIRE_TRIGGER_TIME		80			/**> Temperature raise time in 1/10 seconds resolution */

#define CONFIG_ALARM_FIRE_TEMP_INIT			-100		/**> Used to be sure there are two temperature readings before comparing values */
/** @} */

/**********************************************************************//**
 * @ingroup vroom_cfg
 * @brief Details for the Minimun Set of Data (MSD)
 * @defgroup vc_mcp MSD config parameters
 * @{
 *************************************************************************/
#define CONFIG_MSD_FORMAT_VERSION		2
#define CONFIG_MSD_TEST_CALL			true
#define CONFIG_MSD_VEHICLE_CLASS		AD_VEHICLE_CLASS_M(1)	/**> M1 = No more than eight seats in addition to the driver’s seat, and having a maximum mass not exceeding 3.5 tons */
#define CONFIG_MSD_FUEL_TYPE			AD_FUEL_TYPE_GASOLINE
#define CONFIG_MSD_VIN					"W0L000036V1940069"
#define CONFIG_MSD_OPTIONAL_DATA_SIZE	102						/**> Calculated size based on total MSD size - compulsory data */
/** @} */

/**********************************************************************//**
 * @ingroup vroom_cfg
 * @brief The emergency number used in the system
 * @defgroup vc_eniu Emergency number in use
 * @{
 *************************************************************************/
#define CONFIG_EMERGENCY_PHONE_NUMBER	CONFIG_PSAP_NUMBER_TEST_KENNETH
/** @} */

/**********************************************************************//**
 * @ingroup vroom_cfg
 * @brief Lisg of emergency numbers, including 2 test numbers
 * @defgroup vc_epn Emergency phone numbers
 * @{
 *************************************************************************/
#define CONFIG_PSAP_NUMBER_EUROPE		"112"
#define CONFIG_PSAP_NUMBER_TEST_KENNETH	"60192949" /* Only for testing */
#define CONFIG_PSAP_NUMBER_TEST_ANDI	"60257898" /* Only for testing */
/** @} */

/**********************************************************************//**
 * @ingroup vroom_cfg
 * @brief List of APN addresses for different ISP's
 * @defgroup vc_ia ISP APN
 * @{
 *************************************************************************/
#define CONFIG_APN_TELIA				"internet.mtelia.dk"
#define CONFIG_APN_CALLME				"websp"
#define CONFIG_APN_TELENOR				"internet"
#define CONFIG_APN_TDC					"internet"
/** @} */

/**********************************************************************//**
 * @ingroup vroom_cfg
 * @brief Details for the data connection setup
 * @defgroup vc_dcs Data connection settings
 * @{
 *************************************************************************/
#define CONFIG_FTP_SERVER_ADDRESS		"ftp.andidegn.dk"
#define CONFIG_FTP_PORT					"1404"
#define CONFIG_FTP_USER					"VROOM"
#define CONFIG_FTP_PW					"6198fg(/G6F/&5(!(!8gf87gMF."
#define CONFIG_FTP_FILE_PATH			"/"
#define CONFIG_FTP_FILE_SIZE			"140" /* no of bytes */
/** @} */

/**********************************************************************//**
 * @ingroup vroom_cfg
 * @brief Test parameters settings
 * @defgroup vc_tp Test parameters
 * @{
 *************************************************************************/
#if KENNETH_TEST
	#define CONFIG_PIN					"5130"
	#define CONFIG_VROOM_ID				"60192949"
	#define CONFIG_APN					CONFIG_APN_TDC
#elif ANDI_TEST
	#define CONFIG_VROOM_ID				"60257898"
	#define CONFIG_APN					CONFIG_APN_CALLME
#endif
/** @} */

#endif /* VROOM_CONFIG_H_ */