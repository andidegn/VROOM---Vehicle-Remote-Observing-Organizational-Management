/**********************************************************************//**
@file sim908.c
@author: Kenneth René Jensen
@Version: 0.5
@{
	This is the driver for GSM/GPRS/GPS module sim908
@}
@note NOT YET Complies MISRO 2004 standards
 *************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include "sim908.h"
#include "at_commands.h"
#include "../../data_comm/uart/uart.h"
#include "../../accident_logic/accident_data.h"
#include "../../util/time.h"
#include "../../vroom_config.h"

#define RX_BUFFER_SIZE 128

/* Local variables */
static volatile uint8_t _CR_counter = 0;
static volatile uint8_t _LF_counter = 0;

static volatile char _rx_buffer[RX_BUFFER_SIZE];
static volatile uint8_t _rx_buffer_tail = 0;
static volatile uint8_t _rx_response_length = 0;

static volatile uint8_t _system_running_flag = SIM908_FLAG_WAITING;

static volatile uint8_t _gps_pull_flag = SIM908_FLAG_WAITING;

static volatile uint8_t _ack_response_flag = SIM908_FLAG_WAITING;
static volatile uint8_t _ack_ftp_response_flag = SIM908_FLAG_WAITING;
static volatile uint8_t _ack_gps_response_flag = SIM908_FLAG_WAITING;
static volatile uint8_t _ack_ip_response_flag = SIM908_FLAG_WAITING;
static volatile uint8_t _ack_call_ready_response_flag = SIM908_FLAG_WAITING;

static uint8_t _gps_response_tail = 0;
static uint8_t _gps_response_length = 0;

/* External from accident_data */
char EXT_MSD_FILENAME[24];

/**********************************************************************//**
 * @def CR
 * @ingroup sim908_priv
 * @brief define for the carriage return char '\r'
 * @{
 *************************************************************************/
#define CR	0x0D /* '\r' */
/* @} */

/**********************************************************************//**
 * @def LF
 * @ingroup sim908_priv
 * @brief define for the line feed char '\n'
 * @{
 *************************************************************************/
#define LF	0x0A /* '\n' */
/* @} */

#define DDR(x) (*(&x - 1))
#define PIN(x) (*(&x - 2))

#define RETRY_ATTEMPTS 10
#define CONNECTION_RETRY_DELAY_IN_MS 50

/* Prototypes */
static void _setup_GSM(void);
static void _setup_GPS(void);
static void _setup_GPRS_FTP(void);
static void _GSM_enable(void);
static void _GPS_enable(void);
static void _wait_for_connection(void);
static bool _wait_response(volatile uint8_t *__flag, uint8_t __ok_def);
static bool _check_response(const char *defined_response);
static void _get_GPS_response(void);
static char _char_at(uint8_t __index, uint8_t __tail, uint8_t __length);
static void _raw_to_array(char **__output);
static uint32_t _set_UTC_sec(const char *__utc_raw);
static int32_t _set_lat_long(const char *__lat_long_raw);
static uint8_t _set_direction(const char *__direction_raw);
static void _set_MSD_filename(const char *__UTC_raw);
static void _set_service_provider(uint8_t *__IPV4);

void _SIM908_callback(char data);

#ifdef  DEBUG_UART_ENABLE
void _PC_callback(char data);
#endif

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Initiates the SIM908 module
 @return void
 @note UART0 is used to communicate with the module.
 ************************************************************************************************************************/
void SIM908_init(void)
{
	/* Saves the current state of the status register and disables global interrupt */
	uint8_t SREG_cpy = SREG;
	cli();

	/* Set all related pins to output */
	DDR(DRIVER_PORT) |= (1<<CE_PIN);
	DDR(GSM_PORT) |= (1<<GSM_ENABLE_PIN);
	DDR(GPS_PORT) |= (1<<GPS_ENABLE_PIN);

	/* Toggle driver pin to start up SIM908 module */
	DRIVER_PORT |= _BV(CE_PIN);
	_delay_ms(1000);
	DRIVER_PORT &= ~_BV(CE_PIN);
	_GSM_enable();

	/* Restore interrupt */
	SREG = SREG_cpy;

	/* Setting up UART for internal communication */
 	uart0_setup_async(UART_MODE_DOUBLE, UART_BAUD_115K2, UART_PARITY_DISABLED, UART_ONE_STOP_BIT, UART_8_BIT, _SIM908_callback);

	#ifdef DEBUG_UART_ENABLE
	/* Setting up UART for external communication */
	uart1_setup_async(UART_MODE_DOUBLE, UART_BAUD_115K2, UART_PARITY_DISABLED, UART_ONE_STOP_BIT, UART_8_BIT, _PC_callback);
	#endif
}

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Starts the SIM908 module in following procedure.
		setup global settings -> setup GSM -> setup GPS -> enable GSM communication -> setup FTP
 @return void
 ************************************************************************************************************************/
void SIM908_start(void)
{
	while (_system_running_flag == SIM908_FLAG_WAITING);
	/* Enable Echo */
	SIM908_cmd(AT_DIAG_ECHO_DISABLE, true);

	/* Enable CREG unsolicited result code */
	SIM908_cmd(AT_ENABLE_CREG, true);

	/* Synchronizing baud rate */
	SIM908_cmd(AT_DIAG_TEST, true);

	/* Set baud rate to the host baud rate */
	SIM908_cmd(AT_BAUD_115K2, true);

	#ifdef CONFIG_PIN
	/* wait for +CPIN: SIM PIN */
	_delay_ms(1000);
	SIM908_cmd(AT_ENTER_SIM_PIN(CONFIG_PIN), true);
	#endif

	_setup_GSM();
	_setup_GPRS_FTP();
	_setup_GPS();

	/* Replace this with check - ToDo in ISR */
	_wait_for_connection();

	/* Activate PDP context */
	//SIM908_cmd(AT_GPRS_PDP_ACTIVATE_CONTEXT, true);
}

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Used for sending AT SET commands.
 @param *cmd is the AT command as a string
 @return true if successful else false
 ************************************************************************************************************************/
bool SIM908_cmd(const char *__cmd, bool __wait_for_ok)
{
	/* Implement number of retries functionality / parameter*/
	_ack_response_flag = _ack_ftp_response_flag = _ack_gps_response_flag = SIM908_FLAG_WAITING;

	uart0_send_string(__cmd);
	uart0_send_char(CR);
	uart0_send_char(LF);
	#ifdef DEBUG_UART_ENABLE
		uart1_send_string(__cmd);
		uart1_send_char(CR);
		uart1_send_char(LF);
	#endif

	return __wait_for_ok ? _wait_response(&_ack_response_flag, SIM908_FLAG_OK) : true;
}

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Sets the MSD with relevant information given from GPS response
 @param *__UTC_sec points to the timestamp in MSD structure
		*__latitude points to the latitude in MSD structure
		*__longitude points to the longitude in MSD structure
		*__course points to the direction in MSD structure
		*__IPV4 points to sp in MSD structure
 @return void
 ************************************************************************************************************************/
void set_MSD_data(uint32_t *__UTC_sec, int32_t *__latitude, int32_t *__longitude, uint8_t *__course, uint8_t *__IPV4)
{
	char **output;
	output = (char**)malloc(9 * sizeof(char*));
	for (uint8_t i = 0; i < 9; i++) {
		*(output + i) = malloc(18 * sizeof(char));
	}

	/* GPS raw data: <mode>,<longitude>,<latitude>,<altitude>,<UTC time>,<TTFF>,<num>,<speed>,<course> */
	_get_GPS_response();
	_raw_to_array(output);

	*__longitude = _set_lat_long(*(output + 1));
	*__latitude = _set_lat_long(*(output + 2));
	*__UTC_sec = _set_UTC_sec(*(output + 4));
	*__course = _set_direction(*(output + 8));

	_set_MSD_filename(*(output + 4));

	for (uint8_t i = 0; i < 9; i++) {
		free(*(output + i));
	}
	free(output);
}

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Calling Public-safety answering point
 @return void
 ************************************************************************************************************************/
void call_PSAP(void)
{
	/* Enable incoming calls */
	SIM908_cmd(AT_ENABLE_INCOMING_CALLS, true);

	SIM908_cmd(AT_CALL_DIAL(CONFIG_EMERGENCY_PHONE_NUMBER), true);
}

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Sends MSD binary file to FTP server
	*   1:	Create filename:		AT+FTPPUTNAME="<filename>"
	*   2:	Open bearer				AT+SAPBR=1,1
	*   3:	Open FTP PUT session	AT+FTPPUT=1
	*	4:  Set write data			AT+FTPPUT=2,140
	*	5:	Write text (140 bytes)
	*	6:	End write session		AT+FTPPUT=2,0
	*	7:  Close bearer			AT+SAPBR=0,1
 @param const char array with the VROOM ID (defined in vroom_config.h)
 @return void
 ************************************************************************************************************************/
void send_MSD(const char *__vroom_id) {
    _wait_for_connection();

    uint8_t _retry_ctr = RETRY_ATTEMPTS;
    char *filename = malloc(60 * sizeof(char));
    /* 2014-10-12_13.17.34-(60192949).vroom */
    strcpy(filename, AT_FTP_PUT_FILE_NAME);
    strcat(filename, "\"");
    strcat(filename, EXT_MSD_FILENAME);
    strcat(filename, "-(");
    strcat(filename, __vroom_id);
    strcat(filename, ").vroom\"");

    SIM908_cmd(filename, true);
    free(filename);

    while (!SIM908_cmd(AT_FTP_OPEN_BEARER1, true) && _retry_ctr-- > 0) {
        _delay_ms(1000);
    }

    if (_retry_ctr > -1) {
        _retry_ctr = RETRY_ATTEMPTS;
        do {
            SIM908_cmd(AT_FTP_PUT_OPEN_SESSION, false);
        } while (!_wait_response(&_ack_ftp_response_flag, SIM908_FLAG_FTP_PUT_OPEN) && _retry_ctr-- > 0);
    }

    if (_retry_ctr > -1) {
        _retry_ctr = RETRY_ATTEMPTS;
        do {
            SIM908_cmd(AT_FTP_PUT_FILE_SIZE(CONFIG_FTP_FILE_SIZE), false);
        } while (!_wait_response(&_ack_ftp_response_flag, SIM908_FLAG_FTP_PUT_SUCCESS) && _retry_ctr-- > 0);
    }

    if (_retry_ctr > -1) {
        _retry_ctr = RETRY_ATTEMPTS;
        do {
            uart0_send_data((char*)(&EXT_MSD.control), 1);
            uart0_send_data(&EXT_MSD.VIN[0], 20);
            uart0_send_data((char*)(&EXT_MSD.time_stamp), 4);
            uart0_send_data((char*)(&EXT_MSD.latitude), 4);
            uart0_send_data((char*)(&EXT_MSD.longitude), 4);
            uart0_send_data((char*)(&EXT_MSD.direction), 1);
            uart0_send_data((char*)(&EXT_MSD.sp[0]), 4);
            uart0_send_data(&EXT_MSD.optional_data[0], 102);

            uart0_send_char(CR);
            uart0_send_char(LF);
        } while (!_wait_response(&_ack_ftp_response_flag, SIM908_FLAG_FTP_PUT_OPEN) && _retry_ctr-- > 0);
    }

    if (_retry_ctr > -1) {
        _retry_ctr = RETRY_ATTEMPTS;
        _delay_ms(100);
        do {
            SIM908_cmd(AT_FTP_PUT_CLOSE_SESSION, true);
        } while (!_wait_response(&_ack_ftp_response_flag, SIM908_FLAG_FTP_PUT_CLOSE) && _retry_ctr-- > 0);
    }

    if (_retry_ctr > -1) {
        SIM908_cmd(AT_FTP_CLOSE_BEARER1, true);
    }
}

/********************************************************************************************************************//**
 @ingroup sim908
 @brief GSM settings on SIM908 module
 @return void
 ************************************************************************************************************************/
static void _setup_GSM(void)
{
	/* Setup phone functionality */
	SIM908_cmd(AT_FULL_FUNCTIONALITY, true);

	/* Forbid incoming calls */
	SIM908_cmd(AT_FORBID_INCOMING_CALLS, true);
}

/********************************************************************************************************************//**
 @ingroup sim908
 @brief GPS settings on SIM908 module
 @return void
 ************************************************************************************************************************/
static void _setup_GPS(void)
{
	while(_ack_gps_response_flag == SIM908_FLAG_WAITING);
	/* Enable GPS */
	SIM908_cmd(AT_GPS_POWER_ON, true);

	/* Set GPS reset to autonomous */
	SIM908_cmd(AT_GPS_RST_AUTONOMY, true);
}

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Settings up GPRS - FTP on SIM908 module
	*	1:	Set bearer parameter	AT+SAPBR=3,1,"Contype","GPRS"
	*								AT+SAPBR=3,1,"APN","<APN>"
	*	2:	Use bearer profile		AT+FTPCID=1
	*   3:	FTP login				AT+FTPSERV="<Server add>"
	*								AT+FTPPORT=<Server port>
	*								AT+FTPUN="<Username>"
	*								AT+FTPPW=<Password>
	*   4:  Configure put			AT+FTPPUTPATH="<Path>"
	*		- Binary				AT+FTPTYPE="I"
	*		- Store file			AT+FTPPUTOPT="STOR"
 @return void
 ************************************************************************************************************************/
static void _setup_GPRS_FTP(void)
{
	/* Set bearer parameters */
	SIM908_cmd(AT_FTP_BEARER1_APN(AT_CONTYPE_GPRS), true);
	SIM908_cmd(AT_FTP_BEARER1_APN(CONFIG_APN), true);

	/* Use bearer profile 1 */
	SIM908_cmd(AT_FTP_USE_PROFILE1, true);

	/* FTP login */
	SIM908_cmd(AT_FTP_SET_SERVER_ADDRESS(CONFIG_FTP_SERVER_ADDRESS), true);
	SIM908_cmd(AT_FTP_SET_CONTROL_PORT(CONFIG_FTP_PORT), true);
	SIM908_cmd(AT_FTP_SET_USER_NAME(CONFIG_FTP_USER), true);
	SIM908_cmd(AT_FTP_SET_PASSWORD(CONFIG_FTP_PW), true);

	/* Set put information */
	SIM908_cmd(AT_FTP_SET_DATA_TYPE_BINARY, true);
	SIM908_cmd(AT_FTP_PUT_FILE_STORING, true);
	SIM908_cmd(AT_FTP_PUT_FILE_PATH(CONFIG_FTP_FILE_PATH), true);
}

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Enable communication with GSM module
 @return void
 ************************************************************************************************************************/
static void _GSM_enable(void)
{
	GPS_PORT |= (1<<GPS_ENABLE_PIN);
	GSM_PORT &= ~(1<<GSM_ENABLE_PIN);
}

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Enable communication with GPS module
 @return void
 ************************************************************************************************************************/
static void _GPS_enable(void)
{
	GPS_PORT &= ~(1<<GPS_ENABLE_PIN);
	GSM_PORT |= (1<<GSM_ENABLE_PIN);
}

static void _wait_for_connection(void) {
	while (EXT_CONNECTION_CREG_FLAG != CREG_REGISTERED && _ack_call_ready_response_flag != SIM908_FLAG_CALL_READY) {
		_delay_ms(CONNECTION_RETRY_DELAY_IN_MS);
	}
}

static bool _wait_response(volatile uint8_t *__flag, uint8_t __ok_def)
{
	while(*__flag == SIM908_FLAG_WAITING) {
		_delay_ms(100);
	}
	if (*__flag == __ok_def) {
		return true;
	}
	return false;
}

/* AT test command echo and expected response:
	AT <CR> <LF> <CR> <LF> OK <CR> <LF>
	0x41 = A
	0x54 = T
	0x0d = <CR>
	0x0a = <LF>
	0x0d = <CR>
	0x0a = <LF>
	0x4f = O
	0x4b = K
	0x0d = <CR>
	0x0a = <LF>
*/
static bool _check_response(const char *defined_response) {
	char c;
	bool ret = true;
	uint8_t i;

	for(i = 0; i < strlen(defined_response) && ret != false; i++) {
	//for(i = 0; i < 2 && ret != false; i++) {
		c = _char_at(i, _rx_buffer_tail, _rx_response_length);
		ret = (c == defined_response[i]) ? true : false;
	}
	return ret;
}

static char _char_at(uint8_t __index, uint8_t __tail, uint8_t __length) {
	volatile uint8_t i = (RX_BUFFER_SIZE + __tail - __length + __index + 1) % RX_BUFFER_SIZE;
	return _rx_buffer[i];
}

/* mode, longitude, latitude, altitude, UTC time, TTFF, Satellite in view, speed over ground, course over ground */
static void _get_GPS_response(void)
{
	// _delay_ms(1000); /* Maybe needed */
	do {
		SIM908_cmd(AT_GPS_GET_LOCATION, false);
		_gps_pull_flag = SIM908_FLAG_GPS_PULL;
	} while (!_wait_response(&_ack_gps_response_flag, SIM908_FLAG_GPS_PULL_OK));
}

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Set the timestamp in MSD structure.
 @param const char array of the timestamp string from GPS response
 @return 4 bytes unsigned in UTC seconds (time in seconds since 1970)
 ************************************************************************************************************************/
static uint32_t _set_UTC_sec(const char *__utc_raw)
{
	char year[5] = {__utc_raw[0],  __utc_raw[1], __utc_raw[2], __utc_raw[3], '\0'};
	char month[3] = {__utc_raw[4],  __utc_raw[5], '\0'};
	char day[3] = {__utc_raw[6],  __utc_raw[7], '\0'};
	char hour[3] = {__utc_raw[8],  __utc_raw[9], '\0'};
	char minute[3] = {__utc_raw[10],  __utc_raw[11], '\0'};
	char second[3] = {__utc_raw[12],  __utc_raw[13], '\0'};

	FIXED_TIME t;
	t.year = atoi(year);
	t.mon = atoi(month);
	t.day = atoi(day);
	t.hour = atoi(hour);
	t.min = atoi(minute);
	t.sec = atoi(second);

	return calc_UTC_seconds(&t);
}

static int32_t _set_lat_long(const char *__lat_long_raw) {
    int8_t lat_long_i = 0;
    uint8_t i;
    char lat_long_deg[3];

    for (i = 0; i < 6; i++) {
        if (__lat_long_raw[i] == '.') {
            lat_long_i = i - 2;
        }
    }

    for (i = 0; i < lat_long_i; i++) {
        lat_long_deg[i] = __lat_long_raw[i];
    }
    lat_long_deg[lat_long_i] = '\0';

    /* gg + (mm.mmmmmm * 60 / 100) / 100 = gg.mmmmmmmm */
    float __decimal_degree = (atoi(lat_long_deg) + atof(&__lat_long_raw[lat_long_i]) / 60);

    /* From decimal degrees to milliarcseconds */
    return (__decimal_degree * 3600000);
}

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Set the direction in MSD structure.
 @param const char array of the course over ground string from GPS response
 @return 1 byte unsigned direction in degrees
 ************************************************************************************************************************/
static uint8_t _set_direction(const char *__direction_raw)
{
	/* (0 <= __direction_raw >= 255) */
	return (255.0*atoi(__direction_raw)/360.0);
}

/**********************************************************************//**
 * @ingroup sim908
 * @brief Set the service provider IPV4 address in MSD structure
 * @param array of 4 bytes
 * @return void
 * @note May also be blank field
 * DDDD  EEEEE L     EEEEE TTTTT EEEEE  ???
 * D   D E     L     E       T   E     ?   ?
 * D   D EEE   L     EEE     T   EEE      ?
 * D   D E     L     E       T   E       ?
 * DDDD  EEEEE LLLLL EEEEE   T   EEEEE   ?
 *************************************************************************/
static void _set_service_provider(uint8_t *__IPV4)
{
	/* ToDo in ISR */
	/* Get IP response: +CGPADDR: 1,"10.132.118.14" */
	do {
		SIM908_cmd(AT_GPRS_PDP_GET_IP, false);
	} while (!_wait_response(&_ack_ip_response_flag, SIM908_FLAG_IP_PULL));

	char *IP;
	/* ToDo - AT command to get IPV4 address */
	uint8_t __SP_response[4] = {100, 0, 100, 0};

	if (__SP_response == NULL)
	{
		__IPV4[0] = 0;
		__IPV4[1] = 0;
		__IPV4[2] = 0;
		__IPV4[3] = 0;
	}

	else
	{
		__IPV4[0] = __SP_response[0];
		__IPV4[1] = __SP_response[1];
		__IPV4[2] = __SP_response[2];
		__IPV4[3] = __SP_response[3];
	}
}

static void _raw_to_array(char **__output) {
	uint8_t i, j = 0, ij = 0;
	for (i = 0; i < _gps_response_length; i++) {
		char c = _char_at(i, _gps_response_tail, _gps_response_length);
		if (c == ',') {
			j++;
			ij = 0;
			} else {
			*(*(__output + j) + ij++) = c;
		}
	}
}

/**********************************************************************//**
 * @ingroup sim908
 * @brief Set the filename of the MSD
 * @param const char array of the timestamp string from GPS response
 * @return void
 * @note Format: 2014-10-12_13.17.34
 *************************************************************************/
static void _set_MSD_filename(const char *__UTC_raw)
{
	uint8_t i = 0;

	for (i = 0; i < 19; i++)
	{
		if (i == 4 || i == 7)
		{
			EXT_MSD_FILENAME[i] = '-';
		}
		else if (i == 10)
		{
			EXT_MSD_FILENAME[i] = '_';
		}
		else if (i == 13 || i == 16)
		{
			EXT_MSD_FILENAME[i] = '.';
		}
		else
		{
			EXT_MSD_FILENAME[i] = *__UTC_raw++;
		}
	}
	EXT_MSD_FILENAME[i] = '\0';
}

void _SIM908_callback(char data)
{
	#ifdef DEBUG_UART_ENABLE
		uart1_send_char(data);																	/* Mirroring communication from sim908 to uart1 */
	#endif

	_rx_response_length++;
	_rx_buffer[_rx_buffer_tail = (_rx_buffer_tail + 1) % RX_BUFFER_SIZE] = data; /* Stores received data in buffer. This technically starts from index '1', but as the buffer is circular, it does not matter */

	if (data == CR) {																			/* Checking and counting for CR and LF */
		_CR_counter++;
	} else if (data == LF) {
		_LF_counter++;
	}

	if (_CR_counter > 0 && _LF_counter > 0) {
		_CR_counter = _LF_counter = 0;
		if (_check_response(SIM908_RESPONSE_CR_LF) || _check_response(SIM908_RESPONSE_LF_CR)) {	/* Skipping empty lines */
		} else if (_check_response(SIM908_RESPONSE_OK)) {										/* OK */
			_ack_response_flag = SIM908_FLAG_OK;
		} else if (_check_response(SIM908_RESPONSE_ERROR)) {									/* Error */
			_ack_response_flag = SIM908_FLAG_ERROR;
		} else if (_check_response(SIM908_RESPONSE_GPS_PULL)) {								/* GPS pull */
			if (_gps_pull_flag == SIM908_FLAG_GPS_PULL) {
				_gps_response_tail = _rx_buffer_tail;
				_gps_response_length = _rx_response_length;
				_ack_gps_response_flag = SIM908_FLAG_GPS_PULL_OK;
				_gps_pull_flag = SIM908_FLAG_WAITING;
			}
		} else if (_check_response(SIM908_RESPONSE_CREG)) {										/* CREG */
			EXT_CONNECTION_CREG_FLAG = _char_at(7, _rx_buffer_tail, _rx_response_length) == '1' ? CREG_REGISTERED : CREG_NOT_REGISTERED;
		} else if (_check_response(SIM908_RESPONSE_GPS_READY)) {								/* GPS Ready */
			_ack_gps_response_flag = SIM908_FLAG_GPS_OK;
		} else if (_check_response(SIM908_RESPONSE_FTP_PUT)) {									/* FTPPUT */
			/* 	FTP PUT OPEN SESSION:	"+FTPPUT:1,1,1260"
				FTP PUT RESPONSE:		"+FTPPUT:2,140"
				FTP PUT CLOSE SESSION:	"+FTPPUT:1,0"		*/
			char c1 = _char_at(8, _rx_buffer_tail, _rx_response_length);
			char c2 = _char_at(10, _rx_buffer_tail, _rx_response_length);
			char c3 = _char_at(11, _rx_buffer_tail, _rx_response_length);
			if (c1 == '1' && c2 == '1' && c3 == ',') {
				_ack_ftp_response_flag = SIM908_FLAG_FTP_PUT_OPEN;
			} else if(c1 == '2' && c2 == '1' && c3 == '4') {
				_ack_ftp_response_flag = SIM908_FLAG_FTP_PUT_SUCCESS;
			} else if(c1 == '1' && c2 == '0') {
				_ack_ftp_response_flag = SIM908_FLAG_FTP_PUT_CLOSE;
			} else {
				_ack_ftp_response_flag = SIM908_FLAG_FTP_PUT_ERROR;
			}
		} else if (_check_response(SIM908_RESPONSE_RDY)) {									/* System ready */
			_system_running_flag = SIM908_FLAG_RUNNING;
		} else if (_check_response(SIM908_RESPONSE_AT)) {									/* Sync AT cmd */
			_rx_response_length = 0;
		} else if (_check_response(SIM908_RESPONSE_CALL_READY)) {
			_ack_call_ready_response_flag = SIM908_RESPONSE_CALL_READY;
		}
		_rx_response_length = 0;
	}
}

#ifdef DEBUG_UART_ENABLE
void _PC_callback(char data)
{
	uart0_send_char(data);
	uart1_send_char(data);
}
#endif
