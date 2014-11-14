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

#define ON 1
#define OFF 0
#define ANDI_TEST ON
#define KENNETH_TEST OFF

/* ToDo - Config file (not as header), to set the VIN number for the specific vehicle, and flag for test call */
#define CONFIG_TEST_CALL	true
#define CONFIG_VIN			"W0L000036V1940069"

#define CONFIG_FTP_SET_SERVER_ADDRESS	"AT+FTPSERV=\"ftp.andidegn.dk\""
#define CONFIG_FTP_SET_CONTROL_PORT		"AT+FTPPORT=1404"
#define CONFIG_FTP_SET_USER_NAME_VROOM	"AT+FTPUN=\"VROOM\""
#define CONFIG_FTP_SET_PASSWORD			"AT+FTPPW=\"6198fg(/G6F/&5(!(!8gf87gMF.\""
#define CONFIG_FTP_PUT_FILE_NAME		"AT+FTPPUTNAME="
#define CONFIG_FTP_PUT_FILE_PATH		"AT+FTPPUTPATH=\"/\""

#if KENNETH_TEST
	#define CONFIG_VROOM_ID		"60192949"
	#define CONFIG_FTP_BEARER_APN		AT_FTP_BEARER1_APN_TDC
#elif ANDI_TEST
	#define CONFIG_VROOM_ID		"60257898"
	#define CONFIG_FTP_BEARER_APN		AT_FTP_BEARER1_APN_CALLME
#endif

#endif /* VROOM_CONFIG_H_ */