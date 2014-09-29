/********************************************//**
@file sim908_gsm.h
@author: Kenneth Ren� Jensen
@Version: 0.1
@defgroup sim908 Sim908_GSM
@{
	This is the driver for GSM module sim908
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


/* Error List for AT Command response */
#define SIM908_OK					 1
#define SIM908_INVALID_RESPONSE		-1
#define SIM908_FAIL					-2
#define SIM908_TIMEOUT				-3


int8_t SIM908_init(void);
int8_t SIM908_cmd(const char *cmd);
int8_t GSM_enable(void);
int8_t GPS_enable(void);
int8_t call_PSAP(void);

#endif /* SIM908_GSM_H_ */