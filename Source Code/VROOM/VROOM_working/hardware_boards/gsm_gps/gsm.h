/*
 * gsm.h
 *
 * Created: 24-09-2014 00:00:07
 *  Author: Andi Degn
 */


#ifndef GSM_H_
#define GSM_H_

#define GSM_PORT PORTE
#define GSM_GSM_ENABLE_PIN 3
#define GSM_GPS_ENABLE_PIN 4
#define GSM_MODULE_START_PIN 5

/* call */
#define AT_CALL_ANSWER "ATA"
#define AT_CALL_HANG_UP "ATH"
#define AT_CALL_DIAL "ATD"
#define AT_CALL_SHOW_NUM "AT+CNUM"

/* msg */
#define AT_MSG_READ = "AT+CMGR=" // Add number in end to select msg to read
#define AT_MSG_DEL_ALL = "AT+CMGD=1,4"

/* diagnostics */
#define AT_DIAG_TEST "AT"
#define AT_DIAG_MODEL_NO "AT+CGMM"
#define AT_DIAG_MANUFACTURER "AT+CGMI"

/* connection */
#define AT_CONN_SIGNAL_STRENGTH "AT+CSQ"
#define AT_CONN_ATTACH_STATUS "AT+CGATT?"

/* EOL */
#define AT_EOL '\r'
/* CTRL+Z */
#define AT_CTRL_Z 26

void gsm_init(void);
void gsm_start(void);
void gsm_send(const char *command);
void gsm_answer(void);
void gsm_hang_up(void);

#endif /* GSM_H_ */