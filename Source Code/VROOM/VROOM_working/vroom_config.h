/********************************************//**
@file vroom_config.h
@author: Kenneth René Jensen
@Version: 0.1
@note NOT YET Complies MISRO 2004 standards
************************************************/

#ifndef VROOM_CONFIG_H_
#define VROOM_CONFIG_H_

#include <string.h>
#include <stdbool.h>

/* ToDo - Config file (not as header), to set the VIN number for the specific vehicle, and flag for test call */
const bool CONFIG_TEST_CALL = true;
#define CONFIG_VIN "W0L000036V1940069"

#endif /* VROOM_CONFIG_H_ */