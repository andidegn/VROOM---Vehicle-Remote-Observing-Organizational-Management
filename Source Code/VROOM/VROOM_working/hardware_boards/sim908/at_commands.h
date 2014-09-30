/*
 * at_commands.h
 *
 * Created: 01-10-2014 00:20:07
 *  Author: Andi Degn
 */


#ifndef AT_COMMANDS_H_
#define AT_COMMANDS_H_

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

/* diagnostics */
#define AT_DIAG_TEST "AT"
#define AT_DIAG_ECHO_ENABLE "ATE1"
#define AT_DIAG_ECHO_DISABLE "ATE0"
#define AT_DIAG_MODEL_NO "AT+CGMM"
#define AT_DIAG_MANUFACTURER "AT+CGMI"

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
#define AT_GPS_GET_LOCATION "AT+CGPSINF=0" // Response <mode>,<longitude>,<latitude>,<altitude>,<UTCtime>,<TTFF>,<num>,<speed>,<course >
#define AT_GPS_GET_STATUS "AT+CGPSSTATUS?"
/* EOL */
#define AT_EOL '\r'
/* CTRL+Z */
#define AT_CTRL_Z 26

#endif /* AT_COMMANDS_H_ */