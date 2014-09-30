/********************************************//**
@file sim908_gsm.h
@author: Kenneth René Jensen
@Version: 0.1
@defgroup sim908 Sim908_GSM
@{
	This is the driver for GSM/GPRS/GPS module sim908
@}
@note Complies MISRO 2004 standards
************************************************/

#ifndef SIM908_GSM_H_
#define SIM908_GSM_H_

#include <avr/io.h>

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

/* AT SET Commands */
#define AT_TEST "AT"
#define AT_AUDIO_SET_RINGER_VOLUME	"AT+CRSL="		// Add number 0-4
#define AT_AUDIO_SET_SPEAKER_VOLUME "AT+CLVL="		// Add number 0-100
#define AT_AUDIO_SET_CURRENT_ALERT_SOUND "AT+CALS=" // Add number 0-19

#define AT_CALL_EMERGENCY	"ATD112;"

#define AT_CALL_KENNETH		"ATD60192949;"
#define AT_CALL_ANDI		"ATD60257898;"

/* AT Responds */
#define OK		"OK"
#define ERROR	"ERROR"

/* Error List for AT Command response */
#define SIM908_OK					 1
#define SIM908_INVALID_COMMAND		-1
#define SIM908_INVALID_RESPONSE		-2
#define SIM908_FAIL					-3
#define SIM908_TIMEOUT				-4

int8_t SIM908_init(void);
int8_t SIM908_cmd(const char *cmd, const char *res);
void GSM_enable(void);
void GPS_enable(void);
int8_t call_PSAP(void);

#endif /* SIM908_GSM_H_ */