/**********************************************************************//**
 * @file vroom_config.h
 * @author: Andi Degn
 * @author: Kenneth René Jensen
 * @Version: 0.7
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
	 @brief Configuration file containing all adjustment and setup parameters
	 for the whole system.
	 @note Complies MISRA 2004 standards
 * @}
 *************************************************************************/

#ifndef VROOM_CONFIG_H_
#define VROOM_CONFIG_H_

#define YES 1
#define NO 0
#define ANDI_TEST		YES
#define KENNETH_TEST	NO
 #define DEBUG_UART_ENABLE		/* UART1 (PD2/PD3) is used for RS232 communication with SIM908 module */
 #define DEBUG_UART_ECHO 			/* Gives an echo from UART0 to UART1 */
// #define DEBUG_SIM908_CALLBACK
// #define DEBUG_SIM908_CALLBACK_NOT_CAUGHT
#define DEBUG_LCD_ENABLE			/* LCD should be connected to PORT L */

#define DEBUG_TASK_MEASURE
#ifdef DEBUG_TASK_MEASURE
#define DEBUG_ID_SIM908_START_MODULE		3U
#define DEBUG_ID_SIM908_CMD_SEND			8U
#define DEBUG_ID_SIM908_WAIT_FOR_RESPONSE	1U
#define DEBUG_ID_SIM908_CALLBACK			5U

#define DEBUG_ID_SENSOR_SCHEDULER_TEMP_REQ	10U
#define DEBUG_ID_SENSOR_SCHEDULER_ACC_REQ	11U
#define DEBUG_ID_SENSOR_SCHEDULER_READ		12U

#define DEBUG_ID_ACCIDENT_CRASH_DETECTION	14U
#define DEBUG_ID_ACCIDENT_FIRE_DETECTION	15U
#endif

/* #define CONFIG_ENABLE_EMERGENCY_PHONE_CALL */

/**********************************************************************//**
 * @ingroup vroom_cfg
 * @brief Port definitions for the system
 * @defgroup vc_port MCU port defines
 * @{
 *************************************************************************/
#define CONFIG_PORT_DDR_SPI					DDRB
#define CONFIG_PORT_SS						PB0
#define CONFIG_PORT_SCK						PB1
#define CONFIG_PORT_MOSI					PB2
#define CONFIG_PORT_MISO					PB3
/** @} */

/**********************************************************************//**
 * @ingroup vroom_cfg
 * @brief Port definition for the car panel
 * @defgroup vc_port_car_panel Car panel
 * @{
 *************************************************************************/
#define CONFIG_PORT_CAR_PANEL				PORTJ
/** @} */

/**********************************************************************//**
 * @ingroup vroom_cfg
 * @brief Port definition for the SIM908 module
 * @defgroup vc_port_sim908 SIM908
 * @{
 *************************************************************************/
#define STK600
#ifdef STK600
 #define CONFIG_PORT_DRIVER_PORT			PORTE
 #define CONFIG_PORT_GSM_PORT				PORTE
 #define CONFIG_PORT_GPS_PORT				PORTE
 #define CONFIG_PORT_GSM_ENABLE_PIN			PE3
 #define CONFIG_PORT_GPS_ENABLE_PIN			PE4
 #define CONFIG_PORT_CE_PIN					PE5
#endif /* STK600 */
#ifdef ARDUINO_ATMEGA2560_DEFAULT
 #define CONFIG_PORT_DRIVER_PORT			PORTE
 #define CONFIG_PORT_GSM_PORT				PORTE
 #define CONFIG_PORT_GPS_PORT				PORTG
 #define CONFIG_PORT_GSM_ENABLE_PIN			PE5		/* Pin 3 */
 #define CONFIG_PORT_GPS_ENABLE_PIN			PG5		/* Pin 4 */
 #define CONFIG_PORT_CE_PIN					PE3		/* Pin 5 */
#endif /* ARDUINO_ATMEGA2560_DEFAULT */
/** @} */

/**********************************************************************//**
 * @ingroup vroom_cfg
 * @brief Port definition for the LCD display
 * @note This module is only used for debugging
 * @defgroup vc_port_lcd LCD display
 * @{
 *************************************************************************/
#define CONFIG_PORT_LCD						PORTL
/** @} */

/**********************************************************************//**
 * @ingroup vroom_cfg
 * @brief Port definition for the R2R LED module
 * @note This module is only used for debugging
 * @defgroup vc_port_r2r R2R LED
 * @{
 *************************************************************************/
#define CONFIG_PORT_R2R						PORTH
/** @} */

/**********************************************************************//**
 * @ingroup vroom_cfg
 * @brief Parameters for detecting an accident
 * @defgroup vc_adp Accident detection parameters
 * @{
 *************************************************************************/
#define CONFIG_SCHEDULER_FREQUENCY			TIMER_50HZ	/**> Frequency of which the sensors are read in Hz */

#define CONFIG_ALARM_CRASH_NO_OF_READINGS	5U			/**> Length of acceleration peek */
#define CONFIG_ALARM_CRASH_TRIGGER_VALUE	400			/**> Total acceleration [G] in 1/100 resolution */

#define CONFIG_ALARM_FIRE_TRIGGER_DEGREE	1.0F		/**> Temperature raise in Celsius degrees */
#define CONFIG_ALARM_FIRE_TRIGGER_TIME		8000U		/**> Temperature raise time in 1ms resolution */

#define CONFIG_ALARM_FIRE_TEMP_INIT			-100.0F		/**> Used to be sure there are two temperature readings before comparing values */

#define CONFIG_ALARM_BUTTON_PRESS_TIME		30U
/** @} */

/**********************************************************************//**
 * @ingroup vroom_cfg
 * @brief Details for the Minimun Set of Data (MSD)
 * @defgroup vc_mcp MSD config parameters
 * @{
 *************************************************************************/
#define CONFIG_MSD_FORMAT_VERSION		2U
#define CONFIG_MSD_TEST_CALL			true
#define CONFIG_MSD_VEHICLE_CLASS		AD_VEHICLE_CLASS_M(1)	/**> M1 = No more than eight seats in addition to the driver’s seat, and having a maximum mass not exceeding 3.5 tons */
#define CONFIG_MSD_FUEL_TYPE			AD_FUEL_TYPE_GASOLINE
#define CONFIG_MSD_VIN					"W0L000036V1940069"
#define CONFIG_MSD_OPTIONAL_DATA_SIZE	102U						/**> Calculated size based on total MSD size - compulsory data */
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