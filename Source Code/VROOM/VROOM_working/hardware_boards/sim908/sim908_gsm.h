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

// Error List
#define SIM908_OK					 1
#define SIM908_INVALID_RESPONSE		-1
#define SIM908_FAIL					-2
#define SIM908_TIMEOUT				-3

#define SIM908_SIM_PRESENT			1
#define SIM908_SIM_NOT_PRESENT		0

void SIM908_init(void);
int8_t SIM908_cmd(const char *cmd);
int8_t SIM908_sim_status(void);
int8_t SIM300WaitForResponse(uint16_t timeout);


#endif /* SIM908_GSM_H_ */