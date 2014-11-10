/*
 * at_commands.h
 *
 * Created: 01-10-2014 00:20:07
 *  Author: Andi Degn
 */

#ifndef AT_COMMANDS_H_
#define AT_COMMANDS_H_

/* diagnostics */
#define AT_DIAG_TEST "AT"
#define AT_DIAG_ECHO_ENABLE "ATE1"
#define AT_DIAG_ECHO_DISABLE "ATE0"
#define AT_DIAG_MODEL_NO "AT+CGMM"
#define AT_DIAG_MANUFACTURER "AT+CGMI"

/* Set baud rate - Note autobauding does not support 115200bps */
#define AT_BAUD_AUTO				"AT+IPR=0"	
#define AT_BAUD_1200				"AT+IPR=1200"
#define AT_BAUD_2400				"AT+IPR=2400"
#define AT_BAUD_4800				"AT+IPR=4800"
#define AT_BAUD_9600				"AT+IPR=9600"
#define AT_BAUD_19K2				"AT+IPR=19200"
#define AT_BAUD_38K4				"AT+IPR=38400"
#define AT_BAUD_57K6				"AT+IPR=57600"
#define AT_BAUD_115K2				"AT+IPR=115200"

/* Set phone functionality */
#define AT_MINIMUM_FUNCTIONALITY	"AT+CFUN=0"
#define AT_FULL_FUNCTIONALITY		"AT+CFUN=1"
#define AT_DISABLE_TX_RX			"AT+CFUN=4"
#define AT_RESET_MODULE				"AT+CFUN=1,1"

/* Incoming call */
#define AT_ENABLE_INCOMING_CALLS	"AT+GSMBUSY=0"
#define AT_FORBID_INCOMING_CALLS	"AT+GSMBUSY=1"
#define AT_FORBID_VOICE_CALLS_ONLY	"AT+GSMBUSY=2"	/* Circuit-Switched Data (CSD) calls enabled */

/* Acceptable calls white list */
#define AT_EMERGENCY_CALL_ONLY		"AT+CWHITELIST=1,1,112"

/* Flow control */
#define AT_NO_FLOW_CONTROL			"AT+IFC=0,0"
#define AT_SOFTWARE_FLOW_CONTROL	"AT+IFC=1,1" /* Should not be used for data calls */
#define AT_HARDWARE_FLOW_CONTROL	"AT+IFC=2,2"

/* Store active profile */
#define AT_STORE_PROFILE0			"AT&W0"
#define AT_STORE_PROFILE1			"AT&W1"

/* Reset Default Configuration */
#define AT_RESTORE_PROFILE0			"ATZ0"
#define AT_RESTORE_PROFILE1			"ATZ1"
#define AT_MANUFACTURER_DEFAULTS	"AT&F0"

/*********************************** FTP ***************************************/
#define AT_FTP_BEARER1_CONTYPE_GPS	"AT+SAPBR=3,1,\"Contype\",\"GPRS\""
#define AT_FTP_BEARER1_APN_TELIA	"AT+SAPBR=3,1,\"APN\",\"internet.mtelia.dk\""
#define AT_FTP_BEARER1_APN_CALLME	"AT+SAPBR=3,1,\"APN\",\"websp\""
#define AT_FTP_BEARER1_APN_TELENOR	"AT+SAPBR=3,1,\"APN\",\"internet\""
#define AT_FTP_BEARER1_APN_TDC		"AT+SAPBR=3,1,\"APN\",\"internet\""

#define AT_FTP_OPEN_BEARER1			"AT+SAPBR=1,1"
#define AT_FTP_CLOSE_BEARER1		"AT+SAPBR=0,1"
#define AT_FTP_USE_PROFILE1			"AT+FTPCID=1"

#define AT_FTP_SET_DATA_TYPE_ASCII	"AT+FTPTYPE=\"A\""
#define AT_FTP_SET_DATA_TYPE_BINARY	"AT+FTPTYPE=\"I\""

#define AT_FTP_PUT_FILE_STORING		"AT+FTPPUTOPT=\"STOR\""
#define AT_FTP_PUT_FILE_APPENDING	"AT+FTPPUTOPT=\"APPE\""
#define AT_FTP_PUT_FILE_UNIQUE		"AT+FTPPUTOPT=\"STOU\""		/* Not supported in FileZilla */

#define AT_FTP_PUT_OPEN_SESSION		"AT+FTPPUT=1"
#define AT_FTP_PUT_CLOSE_SESSION	"AT+FTPPUT=2,0"
#define AT_FTP_PUT_WRITE_140BYTE	"AT+FTPPUT=2,140"

#define AT_FTP_SET_SERVER_ADDRESS	"AT+FTPSERV=\"ftp.andidegn.dk\""	
#define AT_FTP_SET_CONTROL_PORT		"AT+FTPPORT=1404"	
#define AT_FTP_SET_USER_NAME_VROOM	"AT+FTPUN=\"VROOM\""
#define AT_FTP_SET_PASSWORD			"AT+FTPPW=\"6198fg(/G6F/&5(!(!8gf87gMF.\""
#define AT_FTP_PUT_FILE_NAME		"AT+FTPPUTNAME="
#define AT_FTP_PUT_FILE_PATH		"AT+FTPPUTPATH=\"/\""
/*******************************************************************************/

/* Module device status
   Response: <CR><LF> +CPAS: [status] <CR><LF>
   Status:
			0 ready
			1 unavailable
			2 unknown
			3 ringing
			4 call in progress
			5 asleep
*/
#define AT_MODULE_DEVICE_STATUS "AT+CPAS"

/* call */
#define AT_CALL_ANSWER "ATA"
#define AT_CALL_HANG_UP "ATH0" // Disconnects all calls on the channel the command is requested
#define AT_CALL_DIAL "ATD" // Add phone number and end with an ';'
#define AT_CALL_REDIAL "ATDL"
#define AT_CALL_SHOW_NUM "AT+CNUM"

/* msg */
#define AT_MSG_READ "AT+CMGR=" // Add number in end to select msg to read
#define AT_MSG_DEL_ALL "AT+CMGD=1,4"
#define AT_MSG_FORMAT "AT+CMGF =" // Add number
#define AT_MSG_SEND "AT+CMGS\""

/* audio */
#define AT_AUDIO_GET_RINGER_VOLUME "AT+CRSL?"
#define AT_AUDIO_SET_RINGER_VOLUME "AT+CRSL=" // Add number 0-4
#define AT_AUDIO_GET_SPEAKER_VOLUME "AT+CLVL?"
#define AT_AUDIO_SET_SPEAKER_VOLUME "AT+CLVL=" // Add number 0-100
#define AT_AUDIO_GET_CURRENT_ALERT_SOUND "AT+CALS?"
#define AT_AUDIO_SET_CURRENT_ALERT_SOUND "AT+CALS=" // Add number 0-19

/* connection */
#define AT_CONN_SIGNAL_STRENGTH "AT+CSQ"
#define AT_CONN_ATTACH_STATUS "AT+CGATT?"
#define AT_CONN_NETWORK_REGISTRATION_STATUS "AT+CREG?"

/* GPS */
#define AT_GPS_POWER_STATE "AT+CGPSPWR?"
#define AT_GPS_POWER_ON "AT+CGPSPWR=1"
#define AT_GPS_POWER_OFF "AT+CGPSPWR=0"
#define AT_GPS_RST_MODE "AT+CGPSRST?"
#define AT_GPS_RST_COLD "AT+CGPSRST=0"
#define AT_GPS_RST_AUTONOMY "AT+CGPSRST=1"
#define AT_GPS_GET_LOCATION "AT+CGPSINF=0" // Response <mode>,<longitude>,<latitude>,<altitude>,<UTCtime>,<TTFF>,<num>,<speed>,<course>
#define AT_GPS_GET_STATUS "AT+CGPSSTATUS?"
/* EOL */
#define AT_EOL '\r'
/* CTRL+Z */
#define AT_CTRL_Z 26

#endif /* AT_COMMANDS_H_ */