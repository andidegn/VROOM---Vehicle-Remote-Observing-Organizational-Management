/********************************************//**
@file sim908.c
@author: Kenneth René Jensen
@Version: 0.4
@defgroup sim908 Sim908_GSM
@{
	This is the driver for GSM/GPRS/GPS module sim908
@}
@note NOT YET Complies MISRO 2004 standards
************************************************/
#include "sim908.h"
#include "at_commands.h"
#include "../../data_comm/uart/uart.h"
#include "../../accident_data.h"
#include "../../util/time.h"
#include "../../timer.h"

#define PC_CALLBACK
#define RX_BUFFER_SIZE 256

static volatile char _sim908_buffer[50];
static volatile uint8_t _index = 0;
static volatile uint8_t _CR_counter = 0;
static volatile uint8_t _LF_counter = 0;

static volatile char _rx_buffer[RX_BUFFER_SIZE];
static volatile uint8_t _rx_buffer_tail = 0;
static volatile uint8_t _rx_response_length = 0;

static volatile uint8_t _ack_respons = 0; /* 0= waiting, 1 = ok, 2 = error */
static volatile uint8_t _ack_ftp_respons = 0; /* 0= waiting, 1 = ok, 2 = error */
static volatile uint8_t _ack_gps_respons = 0; /* 0= waiting, 1 = ok, 2 = error */

/* External from accident_data */
char MSD_filename[24];

#define CR	0x0D
#define LF	0x0A
//#define CR	'\r'
//#define LF	'\n'

#define DDR(x) (*(&x - 1))
#define PIN(x) (*(&x - 2))

// #define _TIMEOUT() (SIM908_timeout_counter > SIM908_TIMEOUT_VALUE )
#define RETRY_ATTEMPTS 5

//typedef enum {ignore_data, record_data} CALLBACK_STATE;
//CALLBACK_STATE _callback_state = ignore_data;

/* Prototypes */
static void _setup_GSM(void);
static void _setup_GPS(void);
static void _setup_GPRS_FTP(void);
static void _GSM_enable(void);
static void _GPS_enable(void);
static bool _wait_response(uint8_t *__flag, uint8_t __ok_def);
static bool _check_response(const char *defined_response);
static char* _get_GPS_response(void);
static void _set_MSD_filename(char *__UTC_raw);
static char _char_at(uint8_t index);

static uint32_t _set_UTC_sec(char *__utc_raw);
static int32_t _set_lat_long(char *__lat_long_raw);
static uint8_t _set_direction(char *__direction_raw);
static void _set_service_provider(uint8_t *__IPV4);
static void _raw_to_array(char **__output, char *__raw_at_str);
static void _set_MSD_filename(char *__UTC_raw);

void _SIM908_callback(char data);

#ifdef  PC_CALLBACK
void _PC_callback(char data);
#endif

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Initiates the SIM908 module
 @return void
 @note UART0 is used to communicate with the module. Timer3 is used in determining timeouts.
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

	/* Restore interrupt */
	SREG = SREG_cpy;

	/* Setting up UART for internal communication */
 	uart0_setup_async(UART_MODE_DOUBLE, UART_BAUD_115K2, UART_PARITY_DISABLED, UART_ONE_STOP_BIT, UART_8_BIT, _SIM908_callback);

	#ifdef PC_CALLBACK
	/* Setting up UART for external communication */
	uart1_setup_async(UART_MODE_DOUBLE, UART_BAUD_115K2, UART_PARITY_DISABLED, UART_ONE_STOP_BIT, UART_8_BIT, _PC_callback);
	#endif

	/* Waiting for proper startup er måske ikke nødvendigt*/
	_delay_ms(2000);
}

void SIM908_start(void)
{

	/* Synchronizing baud rate */
	SIM908_cmd(AT_DIAG_TEST, true);

	/* Set baud rate to the host baud rate */
	SIM908_cmd(AT_BAUD_115K2, true);

	/* Enable Echo */
	SIM908_cmd(AT_DIAG_ECHO_ENABLE, true);
	SIM908_cmd("AT+CPIN=5130", true);

	_setup_GSM();
	_setup_GPS();
	_GSM_enable();
	_setup_GPRS_FTP();
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
	_ack_respons = _ack_ftp_respons = _ack_gps_respons = SIM908_RESPONSE_WAITING;

	uart0_send_string(__cmd);
	uart0_send_char(CR);
	uart0_send_char(LF);

	return __wait_for_ok ? _wait_response(&_ack_respons, SIM908_RESPONSE_OK) : true;
}

void set_MSD_data(uint32_t *__UTC_sec, int32_t *__latitude, int32_t *__longitude, uint8_t *__course, uint8_t *__IPV4)
{
	char *__GPS_AT_respons = _get_GPS_response();
	/* Split the response into different strings */
	char *output[9];
	/* GPS raw data: <mode>,<longitude>,<latitude>,<altitude>,<UTC time>,<TTFF>,<num>,<speed>,<course> */
	_raw_to_array(output, __GPS_AT_respons);

	// _set_lat_long(output[2], output[1], &__latitude, &__longitude);
	*__longitude = _set_lat_long(output[1]);
	*__latitude = _set_lat_long(output[2]);
	*__UTC_sec = _set_UTC_sec(output[4]);
	*__course = _set_direction(output[8]);
	
	// ToDo - Set IPV4 address (is a uint8_t[4])
	// _set_service_provider(&__IPV4);
	
	/* Set filename for MSD */
	_set_MSD_filename(output[4]);
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
	
	SIM908_cmd(AT_CALL_KENNETH, true);
}

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Sends MSB binary file to FTP server
 @return void
 * --- Following steps needs to be called whenever data transfer is needed ---
 *  5:	Create filename:		AT+FTPPUTNAME="<filename>"
 *  6:	Open bearer				AT+SAPBR=1,1
 *  7:	Open FTP PUT session	AT+FTPPUT=1
 -- Wait for response --
 *	8:  Set write data			AT+FTPPUT=2,140
 *	9:	Write text (140 bytes)
 *	10:	End write session		AT+FTPPUT=2,0
 *	11: Close bearer			AT+SAPBR=0,1
 ************************************************************************************************************************/
void send_MSD(char *__vroom_id)
{
	uint8_t ___retry_ctr = RETRY_ATTEMPTS;
	char *filename[63];
	//char *filename = "AT+FTPPUTNAME=\"test_lat_long.VROOM\"";
	strcat(filename, AT_FTP_PUT_FILE_NAME); // 15 incl \0
	strcat(filename, "\"");					// 2 incl \0
	strcat(filename, MSD_filename);			// 24 incl \0 (2014-10-12_13.17.34.000)
	strcat(filename, "-(");					// 3
	strcat(filename, __vroom_id);			// 9
	strcat(filename, ").vroom\"");			// 9 incl \0

	SIM908_cmd(filename, true);

	while (!SIM908_cmd(AT_FTP_OPEN_BEARER1, true));
	do {
		SIM908_cmd(AT_FTP_PUT_OPEN_SESSION, false);
	} while (!_wait_response(&_ack_ftp_respons, SIM908_RESPONSE_FTP_PUT_OPEN) && ___retry_ctr-- > 0);

	___retry_ctr = RETRY_ATTEMPTS;
	do {
		SIM908_cmd(AT_FTP_PUT_WRITE_140BYTE, false);
	} while (!_wait_response(&_ack_ftp_respons, SIM908_RESPONSE_FTP_PUT_SUCCESS) && ___retry_ctr-- > 0);

	___retry_ctr = RETRY_ATTEMPTS;
	do {
		uart0_send_data(&_msd.control, 1);
		uart0_send_data(&_msd.VIN, 20);
		uart0_send_data(&_msd.time_stamp, 4);
		uart0_send_data(&_msd.latitude, 4);
		uart0_send_data(&_msd.longitude, 4);
		uart0_send_data(&_msd.direction, 1);
		uart0_send_data(&_msd.sp, 4);
		uart0_send_data(&_msd.optional_data, 102);

		uart0_send_char(CR);
		uart0_send_char(LF);

	} while (!_wait_response(&_ack_ftp_respons, SIM908_RESPONSE_FTP_PUT_OPEN) && ___retry_ctr-- > 0);

	___retry_ctr = RETRY_ATTEMPTS;
	do {
		SIM908_cmd(AT_FTP_PUT_CLOSE_SESSION, false);
	} while (!_wait_response(&_ack_ftp_respons, SIM908_RESPONSE_FTP_PUT_CLOSE) && ___retry_ctr-- > 0);

	SIM908_cmd(AT_FTP_CLOSE_BEARER1, true);
}

static void _setup_GSM(void)
{
	/* Setup phone functionality */
	SIM908_cmd(AT_FULL_FUNCTIONALITY, true);

	/* Forbid incoming calls */
	SIM908_cmd(AT_FORBID_INCOMING_CALLS, true);
}

static void _setup_GPS(void)
{
	while(_ack_gps_respons == SIM908_RESPONSE_WAITING);
	/* Enable GPS */
	SIM908_cmd(AT_GPS_POWER_ON, true);

	/* Set GPS reset to autonomous */
	SIM908_cmd(AT_GPS_RST_AUTONOMY, true);
}

/* ********************************** FTP **********************************
 *	States setting up GPRS - FTP:
 *	1:	Set bearer parameter	AT+SAPBR=3,1,"Contype","GPRS"
 *								AT+SAPBR=3,1,"APN","websp"
 *	2:	Use bearer profile		AT+FTPCID=1
 *  3:	FTP login				AT+FTPSERV="ftp.andidegn.dk"
 *								AT+FTPPORT=1404
 *								AT+FTPUN="VROOM"
 *								AT+FTPPW="6198fg(/G6F/&5(!(!8gf87gMF."
 *  4:  Configure put			AT+FTPPUTPATH="/"
 *								AT+FTPTYPE="A"
 *								AT+FTPPUTOPT="STOR"
 ***************************************************************************/
static void _setup_GPRS_FTP(void)
{
	/* Set bearer parameters */
	SIM908_cmd(AT_FTP_BEARER1_CONTYPE_GPS, true);
	SIM908_cmd(AT_FTP_BEARER1_APN_TDC, true);

	/* Use bearer profile 1 */
	SIM908_cmd(AT_FTP_USE_PROFILE1, true);

	/* FTP login */
	SIM908_cmd(AT_FTP_SET_SERVER_ADDRESS, true);
	SIM908_cmd(AT_FTP_SET_CONTROL_PORT, true);
	SIM908_cmd(AT_FTP_SET_USER_NAME_VROOM, true);
	SIM908_cmd(AT_FTP_SET_PASSWORD, true);

	/* Set put information */
	SIM908_cmd(AT_FTP_SET_DATA_TYPE_BINARY, true);
	SIM908_cmd(AT_FTP_PUT_FILE_STORING, true);
	SIM908_cmd(AT_FTP_PUT_FILE_PATH, true);
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

static bool _wait_response(uint8_t *__flag, uint8_t __ok_def)
{
	while(*__flag == SIM908_RESPONSE_WAITING) {
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
	for(int i = 0; i < sizeof(defined_response); i++) {
		c = _char_at(i);
		if (c != defined_response[i]) {
			return false;
		}
	}
	return true;
}

static char _char_at(uint8_t __index) {
	volatile uint8_t i = (RX_BUFFER_SIZE + _rx_buffer_tail - _rx_response_length + __index + 1) % RX_BUFFER_SIZE;
	return _rx_buffer[i];
}

/* mode, longitude, latitude, altitude, UTC time, TTFF, Satelite in view, speed over ground, course over ground */
char* _get_GPS_response(void)
{
	//do {
	//SIM908_cmd(AT_FTP_PUT_CLOSE_SESSION, false);
	//} while (!_wait_response(&_ack_gps_respons, RESPONSE_GPS_READY) && ___retry_ctr-- > 0);
			
	//SIM908_cmd(AT_GPS_GET_LOCATION, true);
	
	// ToDo - Store result somehow
	
	return "0,953.27674,5552.192069,62.171906,20141110152800.007,160422,12,0.000000,294.187958";
}

static uint32_t _set_UTC_sec(char *__utc_raw)
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

//static void _set_lat_long(char *__lat_raw, char *__long_raw) {
	//int lat_i = 0;
	//int long_i = 0;
	//int i;
	//for (i = 0; i < 6; i++) {
		//if (__lat_raw[i] == '.') {
			//lat_i = i - 2;
		//}
		//if (__long_raw[i] == '.') {
			//long_i = i - 2;
		//}
	//}
//
	//char lat_deg[3];
	//char long_deg[3];
//
	//for (i = 0; i < lat_i; i++) {
		//lat_deg[i] = __lat_raw[i];
	//}
	//lat_deg[lat_i] = '\0';
//
	//for (i = 0; i < long_i; i++) {
		//long_deg[i] = __long_raw[i];
	//}
	//long_deg[long_i] = '\0';
//
	///* gg + (mm.mmmmmm * 60 / 100) / 100 = gg.mmmmmmmm */
	//// Needs to be change to milliarcseconds (int32_t)
	//_msd.latitude = atoi(lat_deg) + atof(&__lat_raw[lat_i]) / 60;
	//_msd.longitude = atoi(long_deg) + atof(&__long_raw[long_i]) / 60;
//}

static int32_t _set_lat_long(char *__lat_long_raw) 
{
	uint8_t __lat_long_i = 0;
	uint8_t i;
	char __lat_long_deg[3];
	
	for (i = 0; i < 6; i++) 
	{
		if (__lat_long_raw[i] == '.') 
		{
			__lat_long_i = i - 2;
		}
	}
	
	for (i = 0; i < __lat_long_i; i++) 
	{
		__lat_long_deg[i] = __lat_long_raw[i];
	}
	__lat_long_deg[__lat_long_i] = '\0';

	float __decimal_degree = (atoi(__lat_long_deg) + atof(&__lat_long_raw[__lat_long_i]) / 60);

	/* From decimal degrees to milliarcseconds */
	return __decimal_degree * 3600000;
}

static uint8_t _set_direction(char *__direction_raw)
{
	/* (0 <= __direction_raw >= 255) */
	return 255.0*atoi(__direction_raw)/360.0;
}

/**********************************************************************//**
 * @brief function to set the service provider
 * @return void
 * @param sp - An array of 4 bytes consisting the SP in IPV4 format
 * @note May also be blank field
 *************************************************************************/
static void _set_service_provider(uint8_t *__IPV4)
{	
	/* ToDo - AT command to get IPV4 address */
	uint8_t __SP_response[4] = {100, 100, 100, 100};
	
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

static void _raw_to_array(char **__output, char *__raw_at_str)
{
	__output[0] = strtok(__raw_at_str, ",");
	__output[1] = strtok(NULL, ",");
	__output[2] = strtok(NULL, ",");
	__output[3] = strtok(NULL, ",");
	__output[4] = strtok(NULL, ",");
	__output[5] = strtok(NULL, ",");
	__output[6] = strtok(NULL, ",");
	__output[7] = strtok(NULL, ",");
	__output[8] = strtok(NULL, ",");
}

static void _set_MSD_filename(char *__UTC_raw)
{
	/* Format: 2014-10-12_13.17.34.000 */
	uint8_t i = 0;
	while (*__UTC_raw != '\0')
	{
		if (i == 4 || i == 7)
		{
			MSD_filename[i++] = '-';
		}
		else if (i == 10)
		{
			MSD_filename[i++] = '_';
		}
		else if (i == 13 || i == 16)
		{
			MSD_filename[i++] = '.';
		}
		else
		{
			MSD_filename[i++] = *__UTC_raw++;
		}
	}
	MSD_filename[i] = '\0';
}

void _SIM908_callback(char data)
{
	#ifdef PC_CALLBACK
		uart1_send_char(data);
	#endif

	_rx_response_length++;
	_rx_buffer[_rx_buffer_tail = (_rx_buffer_tail + 1) % RX_BUFFER_SIZE] = data;

	if (data == CR)
		_CR_counter++;
	else if (data == LF)
		_LF_counter++;

	if (_CR_counter > 0 || _LF_counter > 0) {
		_CR_counter = _LF_counter = 0;
		if (_check_response(RESPONSE_OK)) {
			_ack_respons = SIM908_RESPONSE_OK;
		} else if (_check_response(RESPONSE_ERROR)) {
			_ack_respons = SIM908_RESPONSE_ERROR;
		} else if (_check_response(RESPONSE_GPS_READY)) {
			_ack_gps_respons = SIM908_RESPONSE_GPS_OK;
		} else if (_check_response(RESPONSE_CR_LF) || _check_response(RESPONSE_LF_CR)) {
			_rx_response_length = 0;
		} else if (_check_response(RESPONSE_AT)) {
			_rx_response_length = 0;
		} else if (_check_response(RESPONSE_RDY)) {
		} else if (_check_response(RESPONSE_FTP_PUT)) {
			char c1 = _char_at(8);
			char c2 = _char_at(10);
			char c3 = _char_at(11);
			if (c1 == '1' && c2 == '1' && c3 == ',') {
				_ack_ftp_respons = SIM908_RESPONSE_FTP_PUT_OPEN;
			} else if(c1 == '2' && c2 == '1' && c3 == '4') {
				_ack_ftp_respons = SIM908_RESPONSE_FTP_PUT_SUCCESS;
			} else if(c1 == '1' && c2 == '0') {
				_ack_ftp_respons = SIM908_RESPONSE_FTP_PUT_CLOSE;
			} else {
				_ack_ftp_respons = SIM908_RESPONSE_FTP_PUT_ERROR;
			}
		}
		_rx_response_length = 0;
	}
}

#ifdef PC_CALLBACK
void _PC_callback(char data)
{
	uart0_send_char(data);
}
#endif
