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
static void _get_GPS_response(uint32_t *__UTC_sec, int32_t *__latitude, int32_t *__longitude, uint8_t *__course);
static void _set_MSD_filename(char *__UTC_raw);
static char _char_at(uint8_t index);

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
	//SIM908_cmd("AT+CPIN=5130");

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
	_get_GPS_response(__UTC_sec, __latitude, __longitude, __course);
	/* ToDo*/
	// Set IPV4 address __msd.sd (is a uint8_t[4])
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
void send_MSD(void)
{
	uint8_t ___retry_ctr = RETRY_ATTEMPTS;
	//char filename[39];
	char *filename = "AT+FTPPUTNAME=\"NUVIRKERDET.VROOM\"";
	//strcat(filename, AT_FTP_PUT_FILE_NAME); // 15
	//strcat(filename, MSD_filename);			  // 24

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
	SIM908_cmd(AT_FTP_BEARER1_APN_CALLME, true);

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
static void _get_GPS_response(uint32_t *__UTC_sec, int32_t *__latitude, int32_t *__longitude, uint8_t *__course)
{
	// ToDo - Impl. is in accident_data.c (commented)
	// also call _set_MSD_filename(<utc raw data>)
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
	MSD_filename[i++] = '\"';
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
