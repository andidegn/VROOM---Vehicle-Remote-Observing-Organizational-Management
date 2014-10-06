/********************************************//**
@file sim908_gsm.h
@author: Kenneth Ren� Jensen
@Version: 0.2
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

#include "at_commands.h"
#include "../../data_comm/uart/uart.h"
#include "../../timer.h"

/* Timeout value in 1/10 sec - Minimum 5 seconds because of the internal delay in function */
#define SIM908_TIMEOUT_VALUE	100

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

/* AT Responds */
#define OK		"OK"
#define ERROR	"ERROR"

/* Error List for return */
#define SIM908_OK					 1
#define SIM908_INVALID_COMMAND		-1
#define SIM908_INVALID_RESPONSE		-2
#define SIM908_FAIL					-3
#define SIM908_TIMEOUT				-4

uint8_t SIM908_init(void);
int8_t SIM908_cmd(const char *cmd, const char *res);
void GSM_enable(void);
void GPS_enable(void);
void call_PSAP(void);

#endif /* SIM908_GSM_H_ */