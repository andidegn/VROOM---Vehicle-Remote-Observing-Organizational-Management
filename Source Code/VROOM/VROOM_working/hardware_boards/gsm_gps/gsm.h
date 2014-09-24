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
#define AT_ANSWER "ATA"
#define AT_HANG_UP "ATH"
#define AT_DIAL "ATD"
#define AT_SHOW_NUM "AT+CNUM"

/* diagnostics */
#define AT_TEST "AT"
#define AT_MODEL_NO "AT+CGMM"
#define AT_MANUFACTURER "AT+CGMI"

/* connection */
#define AT_ATTACH_STATUS "AT+CGATT?"

/* EOL */
#define AT_EOL '\r'

void gsm_init(void);
void gsm_start(void);
void gsm_send(const char *command);
void gsm_answer(void);
void gsm_hang_up(void);

#endif /* GSM_H_ */