/********************************************//**
@file sim908.h
@author: Kenneth René Jensen
@Version: 0.4
@defgroup sim908 Sim908_GSM
@{
	This is the driver for GSM/GPRS/GPS module sim908
@}
@note Complies MISRO 2004 standards
************************************************/

#ifndef SIM908_GSM_H_
#define SIM908_GSM_H_

#include <avr/io.h>
#include <string.h>
#include <stdbool.h>

/* Timeout value in 1/10 sec - Minimum 5 seconds because of the internal delay in call function */
#define SIM908_TIMEOUT_VALUE	50

#if SIM908_TIMEOUT_VALUE < 50
	#error SIM908_TIMEOUT_VALUE must be >= 50
#endif

/* Uncomment for Arduino default port settings */
// #define ARDUINO_ATMEGA2560_DEFAULT
#define STK600

#ifdef STK600
 #define DRIVER_PORT			PORTE
 #define GSM_PORT				PORTE
 #define GPS_PORT				PORTE
 #define GSM_ENABLE_PIN			PE3
 #define GPS_ENABLE_PIN			PE4
 #define CE_PIN					PE5
#endif /* STK600 */

#ifdef ARDUINO_ATMEGA2560_DEFAULT
 #define DRIVER_PORT			PORTE
 #define GSM_PORT				PORTE
 #define GPS_PORT				PORTG
 #define GSM_ENABLE_PIN			PE5		/* Pin 3 */
 #define GPS_ENABLE_PIN			PG5		/* Pin 4 */
 #define CE_PIN					PE3		/* Pin 5 */
#endif /* ARDUINO_ATMEGA2560_DEFAULT */

/* *************************************************************************** */
#define AT_CALL_EMERGENCY	"ATD112;"
#define AT_CALL_KENNETH		"ATD60192949;"
#define AT_CALL_ANDI		"ATD60257898;"

/* Error List for return */
#define SIM908_RESPONSE_WAITING			0
#define SIM908_RESPONSE_OK				1
#define SIM908_RESPONSE_ERROR			2

#define SIM908_RESPONSE_FTP_PUT_OPEN	10
#define SIM908_RESPONSE_FTP_PUT_CLOSE	11
#define SIM908_RESPONSE_FTP_PUT_SUCCESS 12
#define SIM908_RESPONSE_FTP_PUT_ERROR	13

#define SIM908_RESPONSE_GPS_OK			20

#define SIM908_RESPONSE_CREG_OK			1

/* AT Respond strings */
#define RESPONSE_RDY		"RDY"
#define RESPONSE_OK			"OK"
#define RESPONSE_ERROR		"ERROR"
#define RESPONSE_CR_LF		"\r\n" /* ?? */
#define RESPONSE_LF_CR		"\r\n" /* ?? */
#define RESPONSE_AT			"AT"
#define RESPONSE_GPS_READY	"GPS Ready"
/* 	FTP PUT OPEN SESSION:	"+FTPPUT:1,1,1260"
	FTP PUT RESPONSE:		"+FTPPUT:2,140"
	FTP PUT CLOSE SESSION:	"+FTPPUT:1,0"		*/
#define RESPONSE_FTP_PUT	"+FTPPUT:"
#define RESPONSE_FTP_PUT2	"+FTPPUT="
#define RESPONSE_CREG		"+CREG: " /* +CREG: 1 = connected */

void SIM908_init(void);
void SIM908_start(void);
bool SIM908_cmd(const char *cmd, bool __wait_for_ok);
void set_MSD_data(uint32_t *__UTC_sec, int32_t *__latitude, int32_t *__longitude, uint8_t *__course, uint8_t *__IPV4);
void call_PSAP(void);
void send_MSD(char *__vroom_id);

#endif /* SIM908_GSM_H_ */