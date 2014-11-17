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

#define YES 1
#define NO 0
#define ANDI_TEST NO
#define KENNETH_TEST YES

/* MSD details */
#define CONFIG_TEST_CALL				true
#define CONFIG_VIN						"W0L000036V1940069"

/* ISP APN */
#define CONFIG_APN_TELIA				"internet.mtelia.dk"
#define CONFIG_APN_CALLME				"websp"
#define CONFIG_APN_TELENOR				"internet"
#define CONFIG_APN_TDC					"internet"

/* Emergency phone numbers */
#define CONFIG_PSAP_NUMBER_EUROPE		"112"
#define CONFIG_PSAP_NUMBER_TEST_KENNETH	"60192949" /* Only for testing */
#define CONFIG_PSAP_NUMBER_TEST_ANDI	"60257898" /* Only for testing */


#define CONFIG_EMERGENCY_PHONE_NUMBER	CONFIG_PSAP_NUMBER_TEST_KENNETH

/* MSD send parameters */
#define CONFIG_FTP_SERVER_ADDRESS		"ftp.andidegn.dk"
#define CONFIG_FTP_PORT					"1404"
#define CONFIG_FTP_USER					"VROOM"
#define CONFIG_FTP_PW					"6198fg(/G6F/&5(!(!8gf87gMF."
#define CONFIG_FTP_FILE_PATH			"/"
#define CONFIG_FTP_FILE_SIZE			"140" /* no of bytes */

/* Test parameters */
#if KENNETH_TEST
	#define CONFIG_PIN					"5130"
	#define CONFIG_VROOM_ID				"60192949"
	#define CONFIG_APN					CONFIG_APN_TDC
#elif ANDI_TEST
	#define CONFIG_VROOM_ID				"60257898"
	#define CONFIG_APN					CONFIG_APN_CALLME
#endif

#endif /* VROOM_CONFIG_H_ */