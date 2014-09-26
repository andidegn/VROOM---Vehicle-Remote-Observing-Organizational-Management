/********************************************//**
@file sim908_gsm.h
@author: Kenneth René Jensen
@Version: 0.1
@defgroup sim908 Sim908_GSM
@{
	This is the driver for GSM module sim908
@}
@note Complies MISRO 2004 standards
************************************************/

#ifndef SIM908_GSM_H_
#define SIM908_GSM_H_

#include "..\..\data_comm\uart\uart.h"
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

void SIM908_init(void);
void SIM908_cmd(const char *cmd);
void GSM_enable(void);
void GPS_enable(void);
void call_PSAP(void);

#endif /* SIM908_GSM_H_ */