/**********************************************************************//**
 * @file: at_commands.h
 *
 * @Created: 01-10-2014 00:20:07
 * @Author: Andi Degn
 * @Version: 0.8
 * @defgroup at AT Commands
 * @{
	 This is a header containing all the most
	 relevant AT commands for this system
	 @note NOT YET Complies MISRO 2004 standards
 * @}
 *************************************************************************/

#ifndef AT_COMMANDS_H_
#define AT_COMMANDS_H_

/**********************************************************************//**
 * @ingroup at
 * @brief Flag to detect the connection status
 * @note Corresponding to CREG response in SIM908 p. 77 in AT Commands manual v. 1.02
 *************************************************************************/
typedef enum {
	CREG_NOT_REG_NOT_SEARCHING		= 0,
	CREG_REGISTERED_HOME_NETWORK	= 1,
	CREG_NOT_REG_SEARCHING			= 2,
	CREG_REGISTRATION_DENIED		= 3,
	CREG_UNKNOWN					= 4,
	CREG_REGISTERED_ROAMING			= 5
} AT_CONNECTION_STATUS_FLAG;

/**********************************************************************//**
 * @ingroup at
 * @brief Defines for the Data carrier type
 * @defgroup at_data Connection type
 * @{
 *************************************************************************/
#define AT_CONTYPE_GPRS					"GPRS"
#define AT_CONTYPE_CSD					"CSD"
/** @} */

/**********************************************************************//**
 * @ingroup at
 * @brief Defines for the diagnostics
 * @defgroup at_diag Diag
 * @{
 *************************************************************************/
#define AT_DIAG_TEST					"AT"
#define AT_DIAG_ECHO_ENABLE				"ATE1"
#define AT_DIAG_ECHO_DISABLE			"ATE0"
#define AT_DIAG_AUTO_ANSWER(NO_OF_RINGS)"ATS0=" NO_OF_RINGS
#define AT_DIAG_MODEL_NO				"AT+CGMM"
#define AT_DIAG_MANUFACTURER			"AT+CGMI"
/** @} */

/**********************************************************************//**
 * @ingroup at
 * @brief Define for the enable +CREG: unsolicited result code
 * @defgroup at_creg_en CREG enable
 * @{
 *************************************************************************/
#define AT_ENABLE_CREG					"AT+CREG=1"
/** @} */

/**********************************************************************//**
 * @ingroup at
 * @brief Defines for the baud rate
 * @note autobauding does not support 115200bps
 * @defgroup at_baud Baud rate
 * @{
 *************************************************************************/
#define AT_BAUD_AUTO					"AT+IPR=0"
#define AT_BAUD_1200					"AT+IPR=1200"
#define AT_BAUD_2400					"AT+IPR=2400"
#define AT_BAUD_4800					"AT+IPR=4800"
#define AT_BAUD_9600					"AT+IPR=9600"
#define AT_BAUD_19K2					"AT+IPR=19200"
#define AT_BAUD_38K4					"AT+IPR=38400"
#define AT_BAUD_57K6					"AT+IPR=57600"
#define AT_BAUD_115K2					"AT+IPR=115200"
/** @} */

/**********************************************************************//**
 * @ingroup at
 * @brief Defines for the phone functionality
 * @defgroup at_func Functionality
 * @{
 *************************************************************************/
#define AT_MINIMUM_FUNCTIONALITY		"AT+CFUN=0"
#define AT_FULL_FUNCTIONALITY			"AT+CFUN=1"
#define AT_DISABLE_TX_RX				"AT+CFUN=4"
#define AT_RESET_MODULE					"AT+CFUN=1,1"
#define AT_ENTER_SIM_PIN(PIN)			"AT+CPIN=" PIN
/** @} */

/**********************************************************************//**
 * @ingroup at
 * @brief Defines for the incoming call allowance setting
 * @defgroup at_call_all Call allowance
 * @{
 *************************************************************************/
#define AT_ENABLE_INCOMING_CALLS		"AT+GSMBUSY=0"
#define AT_FORBID_INCOMING_CALLS		"AT+GSMBUSY=1"
#define AT_FORBID_VOICE_CALLS_ONLY		"AT+GSMBUSY=2"	/* Circuit-Switched Data (CSD) calls enabled */
/** @} */

/**********************************************************************//**
 * @ingroup at
 * @brief Define for the calls white list
 * @defgroup at_white White list (emergency number)
 * @{
 *************************************************************************/
#define AT_EMERGENCY_CALL_ONLY(NUMBER)	"AT+CWHITELIST=1,1," NUMBER
/** @} */

/**********************************************************************//**
 * @ingroup at
 * @brief Defines for the flow control
 * @defgroup at_flow_ctrl Flow control
 * @{
 *************************************************************************/
#define AT_NO_FLOW_CONTROL				"AT+IFC=0,0"
#define AT_SOFTWARE_FLOW_CONTROL		"AT+IFC=1,1" /* Should not be used for data calls */
#define AT_HARDWARE_FLOW_CONTROL		"AT+IFC=2,2"
/** @} */

/**********************************************************************//**
 * @ingroup at
 * @brief Defines for profile management
 * @defgroup at_profile Profile
 * @{
 *************************************************************************/
#define AT_STORE_PROFILE0				"AT&W0"
#define AT_STORE_PROFILE1				"AT&W1"
#define AT_RESTORE_PROFILE0				"ATZ0"
#define AT_RESTORE_PROFILE1				"ATZ1"
#define AT_MANUFACTURER_DEFAULTS		"AT&F0"
/** @} */

/**********************************************************************//**
 * @ingroup at
 * @brief Defines GPRS PDP context
 * @defgroup at_pdp PDP context
 * @{
 *************************************************************************/
#define AT_GPRS_PDP_ACTIVATE_CONTEXT	"AT+CGACT=1,1"
#define AT_GPRS_PDP_GET_IP				"AT+CGPADDR=1"
#define AT_GPRS_PDP_SETUP_CONTEXT(APN)	"AT+CGDCONT=1,\"IP\",\"" APN "\""
/** @} */

/**********************************************************************//**
 * @ingroup at
 * @brief Defines FTP setup and communication
 * @defgroup at_ftp FTP communication
 * @{
 *************************************************************************/
#define AT_FTP_BEARER1_CONTYPE(TYPE)	"AT+SAPBR=3,1,\"Contype\",\"" TYPE "\""
#define AT_FTP_BEARER1_APN(NAME)		"AT+SAPBR=3,1,\"APN\",\"" NAME "\""

#define AT_FTP_SET_SERVER_ADDRESS(ADR)	"AT+FTPSERV=\"" ADR "\""
#define AT_FTP_SET_CONTROL_PORT(PORT)	"AT+FTPPORT=" PORT
#define AT_FTP_SET_USER_NAME(NAME)		"AT+FTPUN=\"" NAME "\""
#define AT_FTP_SET_PASSWORD(PW)			"AT+FTPPW=\"" PW "\""
#define AT_FTP_PUT_FILE_PATH(PATH)		"AT+FTPPUTPATH=\"" PATH "\""
#define AT_FTP_PUT_FILE_NAME			"AT+FTPPUTNAME=" /* remember to add '"' at either end of the filename */

#define AT_FTP_OPEN_BEARER1				"AT+SAPBR=1,1"
#define AT_FTP_CLOSE_BEARER1			"AT+SAPBR=0,1"
#define AT_FTP_USE_PROFILE1				"AT+FTPCID=1"

#define AT_FTP_SET_DATA_TYPE_ASCII		"AT+FTPTYPE=\"A\""
#define AT_FTP_SET_DATA_TYPE_BINARY		"AT+FTPTYPE=\"I\""

#define AT_FTP_PUT_FILE_STORING			"AT+FTPPUTOPT=\"STOR\""
#define AT_FTP_PUT_FILE_APPENDING		"AT+FTPPUTOPT=\"APPE\""
#define AT_FTP_PUT_FILE_UNIQUE			"AT+FTPPUTOPT=\"STOU\""		/* Not supported in FileZilla */

#define AT_FTP_PUT_OPEN_SESSION			"AT+FTPPUT=1"
#define AT_FTP_PUT_CLOSE_SESSION		"AT+FTPPUT=2,0"
#define AT_FTP_PUT_FILE_SIZE(SIZE)		"AT+FTPPUT=2," SIZE
/** @} */

/**********************************************************************//**
 * @ingroup at
 * @brief Define for the module device status
 * The different responses are:\n
 * 0 ready\n
 * 1 unavailable\n
 * 2 unknown\n
 * 3 ringing\n
 * 4 call in progress\n
 * 5 asleep\n
 * @defgroup at_status Module status
 * @{
 *************************************************************************/
#define AT_MODULE_DEVICE_STATUS "AT+CPAS"
/** @} */

/**********************************************************************//**
 * @ingroup at
 * @brief Defines for the call
 * @defgroup at_call Call
 * @{
 *************************************************************************/
#define AT_CALL_ANSWER "ATA"
#define AT_CALL_HANG_UP "ATH0" // Disconnects all calls on the channel the command is requested
#define AT_CALL_DIAL(NUMBER) "ATD" NUMBER ";" // Add phone number and end with an ';'
#define AT_CALL_REDIAL "ATDL"
#define AT_CALL_SHOW_NUM "AT+CNUM"
/** @} */

/**********************************************************************//**
 * @ingroup at
 * @brief Defines for the message
 * @defgroup at_msg Messages
 * @{
 *************************************************************************/
#define AT_MSG_READ "AT+CMGR=" // Add number in end to select msg to read
#define AT_MSG_DEL_ALL "AT+CMGD=1,4"
#define AT_MSG_FORMAT "AT+CMGF=" // Add number
#define AT_MSG_SEND "AT+CMGS\""
/** @} */

/**********************************************************************//**
 * @ingroup at
 * @brief Defines for the audio setup
 * @defgroup at_audio Audio
 * @{
 *************************************************************************/
#define AT_AUDIO_GET_RINGER_VOLUME "AT+CRSL?"
#define AT_AUDIO_SET_RINGER_VOLUME "AT+CRSL=" // Add number 0-4
#define AT_AUDIO_GET_SPEAKER_VOLUME "AT+CLVL?"
#define AT_AUDIO_SET_SPEAKER_VOLUME "AT+CLVL=" // Add number 0-100
#define AT_AUDIO_GET_CURRENT_ALERT_SOUND "AT+CALS?"
#define AT_AUDIO_SET_CURRENT_ALERT_SOUND "AT+CALS=" // Add number 0-19
/** @} */

/**********************************************************************//**
 * @ingroup at
 * @brief Defines for the connection
 * @defgroup at_conn Connection
 * @{
 *************************************************************************/
#define AT_CONN_SIGNAL_STRENGTH "AT+CSQ"
#define AT_CONN_ATTACH_STATUS "AT+CGATT?"
#define AT_CONN_NETWORK_REGISTRATION_STATUS "AT+CREG?"
/** @} */

/**********************************************************************//**
 * @ingroup at
 * @brief Defines for the GPS
 * @defgroup at_gps GPS
 * @{
 *************************************************************************/
#define AT_GPS_POWER_STATE "AT+CGPSPWR?"
#define AT_GPS_POWER_ON "AT+CGPSPWR=1"
#define AT_GPS_POWER_OFF "AT+CGPSPWR=0"
#define AT_GPS_RST_MODE "AT+CGPSRST?"
#define AT_GPS_RST_COLD "AT+CGPSRST=0"
#define AT_GPS_RST_AUTONOMY "AT+CGPSRST=1"
#define AT_GPS_GET_LOCATION "AT+CGPSINF=0" // Response <mode>,<longitude>,<latitude>,<altitude>,<UTCtime>,<TTFF>,<num>,<speed>,<course>
#define AT_GPS_GET_STATUS "AT+CGPSSTATUS?"
/** @} */

/**********************************************************************//**
 * @ingroup at
 * @brief Define for the ctrl+Z command
 * @defgroup at_ctrl_z Ctrl+Z
 * @{
 *************************************************************************/
#define AT_CTRL_Z 26
/** @} */

#endif /* AT_COMMANDS_H_ */