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

#define PC_CALLBACK

static volatile char _sim908_buffer[16];
static volatile uint8_t _index = 0;
static volatile uint8_t _CR_counter = 0;
static volatile uint8_t _LF_counter = 0;

#define CR	0x0D
#define LF	0x0A

#define DDR(x) (*(&x - 1))
#define PIN(x) (*(&x - 2))

#define _TIMEOUT() (SIM908_timeout_counter > SIM908_TIMEOUT_VALUE )

typedef enum {ignore_data, record_data} CALLBACK_STATE;
CALLBACK_STATE _callback_state = ignore_data;

/* Prototypes */
void _setup_GSM(void);
void _setup_GPS(void);
void _setup_GPRS_FTP(void);
int8_t _SIM908_check_response(void);
void _flush_buffer(void);
int8_t _wait_response(void);
char* _get_GPS_response(void);
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
	_delay_ms(1500);
	DRIVER_PORT &= ~_BV(CE_PIN);
	_delay_ms(1500);

	/* Restore interrupt */
	SREG = SREG_cpy;

	/* Setting up UART for internal communication */
 	uart0_setup_async(UART_MODE_DOUBLE, UART_BAUD_115K2, UART_PARITY_DISABLED, UART_ONE_STOP_BIT, UART_8_BIT, _SIM908_callback);

	#ifdef PC_CALLBACK
	/* Setting up UART for external communication */
	uart1_setup_async(UART_MODE_DOUBLE, UART_BAUD_115K2, UART_PARITY_DISABLED, UART_ONE_STOP_BIT, UART_8_BIT, _PC_callback);
	#endif

	/* Waiting for proper startup */
	_delay_ms(5000);
}

void SIM908_start(void)
{
	/* Synchronizing baud rate */
	while (SIM908_cmd(AT_DIAG_TEST) != SIM908_RESPONSE_OK);

	/* Set baud rate to the host baud rate */
	SIM908_cmd(AT_BAUD_115K2);

	/* Enable Echo */
	SIM908_cmd(AT_DIAG_ECHO_ENABLE);
	
	SIM908_cmd("AT+CPIN=5130");

	_setup_GSM();
	_setup_GPS();
	GSM_enable();
	_setup_GPRS_FTP();
}

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Enable communication with GSM module
 @return void
 ************************************************************************************************************************/
void GSM_enable(void)
{
	GSM_PORT &= ~(1<<GSM_ENABLE_PIN);
	GPS_PORT |= (1<<GPS_ENABLE_PIN);
}

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Enable communication with GPS module
 @return void
 ************************************************************************************************************************/
void GPS_enable(void)
{
	GPS_PORT &= ~(1<<GPS_ENABLE_PIN);
	GSM_PORT = (1<<GSM_ENABLE_PIN);
}

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Used for sending AT SET commands.
 @param *cmd is the AT command as a string
 @return 1 if command and response is OK
		 0 if command is OK but response is ERROR
		-1 if invalid command (not starting with AT)
		-2 if command is OK but response is invalid
		-3 if timeout
 ************************************************************************************************************************/
int8_t SIM908_cmd(const char *cmd)
{
	int8_t _cmd_check = 0;
	_flush_buffer();

	_callback_state = record_data;

	uart0_send_string(cmd);
	uart0_send_char(CR);
	uart0_send_char(LF);

	_cmd_check = _wait_response();

	if(_cmd_check == SIM908_OK)
	{
		_callback_state = ignore_data;
		return _SIM908_check_response();
	}

	return _cmd_check;
}

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Calling Public-safety answering point
 @return 1 if call established and response is OK
		-3 if timeout
 @note Pushes the call again if it fails until it times out
 ************************************************************************************************************************/
int8_t call_PSAP(void)
{
	int8_t _call_check = 0;

	start_timer3();
	SIM908_timeout_counter = 0;

	while (!_TIMEOUT())
	{
		/* _call_check = SIM908_cmd(AT_CALL_EMERGENCY); */
		_call_check = SIM908_cmd(AT_CALL_KENNETH);

		if (_call_check == SIM908_RESPONSE_OK)
		{
			stop_timer3();
			return SIM908_OK;
		}
	}

	stop_timer3();
	return SIM908_TIMEOUT;
}

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Sends MSB binary file to FTP server
 @return 1 if call established and response is OK
		-3 if timeout
 ************************************************************************************************************************/
int8_t send_MSD(void)
{
	//char filename[39];
	char *filename = "AT+FTPPUTNAME=\"AA_TEST15.hex\"";
	//strcat(filename, AT_FTP_PUT_FILE_NAME); // 15
	//strcat(filename, UTC_string);			  // 24
	SIM908_cmd(filename);

	SIM908_cmd(AT_FTP_OPEN_BEARER1);
	SIM908_cmd(AT_FTP_PUT_OPEN_SESSION);
	_delay_ms(4000);

	SIM908_cmd("AT+FTPPUT=2,140");
	_delay_ms(1000);
	
	uart0_send_string(&_msd.control);
	uart0_send_string(&_msd.VIN);
	uart0_send_string(&_msd.time_stamp);
	uart0_send_string(&_msd.latitude);
	uart0_send_string(&_msd.longitude);
	uart0_send_string(&_msd.direction);
	uart0_send_string(&_msd.sp);	
	uart0_send_string(&_msd.optional_data);

	uart0_send_char(CR);
	uart0_send_char(LF);

	_delay_ms(1000);
	SIM908_cmd(AT_FTP_PUT_CLOSE_SESSION);
	_delay_ms(8000);
	SIM908_cmd(AT_FTP_CLOSE_BEARER1);
	
	return SIM908_OK;
}

void _setup_GSM(void)
{
	/* Setup phone functionality */
	SIM908_cmd(AT_FULL_FUNCTIONALITY);

	/* Forbid incoming calls */
	SIM908_cmd(AT_FORBID_INCOMING_CALLS);
}

void _setup_GPS(void)
{
	/* Enable GPS */
	SIM908_cmd(AT_GPS_POWER_ON);

	/* Set GPS reset to autonomous */
	SIM908_cmd(AT_GPS_RST_AUTONOMY);
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
 * --- Following steps needs to be called whenever data transfer is needed ---
 *  5:	Create filename:		AT+FTPPUTNAME="<filename>"
	6:	Open bearer				AT+SAPBR=1,1
	6:	Open FTP PUT session	AT+FTPPUT=1
 *	7:  Set write data			AT+FTPPUT=2,140
 *	8:	Write text (140 bytes)
 *	9:	End write session		AT+FTPPUT=2,0
 *	10: Close bearer			AT+SAPBR=0,1
 ***************************************************************************/
void _setup_GPRS_FTP(void)
{
	/* Set bearer parameters */
	SIM908_cmd(AT_FTP_BEARER1_CONTYPE_GPS);
	SIM908_cmd(AT_FTP_BEARER1_APN_TELENOR);

	/* Use bearer profile 1 */
	SIM908_cmd(AT_FTP_USE_PROFILE1);

	/* FTP login */
	SIM908_cmd(AT_FTP_SET_SERVER_ADDRESS);
	SIM908_cmd(AT_FTP_SET_CONTROL_PORT);
	SIM908_cmd(AT_FTP_SET_USER_NAME_VROOM);
	SIM908_cmd(AT_FTP_SET_PASSWORD);

	/* Set put information */
	SIM908_cmd(AT_FTP_SET_DATA_TYPE_BINARY);
	SIM908_cmd(AT_FTP_PUT_FILE_STORING);
	SIM908_cmd(AT_FTP_PUT_FILE_PATH);
}

void _flush_buffer(void)
{
	memset(&_sim908_buffer, 0, sizeof(_sim908_buffer));
	_CR_counter = 0;
	_LF_counter = 0;
	_index = 0;
}

int8_t _wait_response(void)
{
	start_timer3();
	SIM908_timeout_counter = 0;
	while (_CR_counter < 3  || _LF_counter < 3)
	{
		if (_TIMEOUT())
		{
			stop_timer3();
			return SIM908_TIMEOUT;
		}
		_delay_us(10);
	}
	stop_timer3();
	return SIM908_OK;
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
int8_t _SIM908_check_response()
{
	/* Check command for the letter A/a */
	if(_sim908_buffer[0] != 0x41 && _sim908_buffer[0] != 0x61)
		return	SIM908_INVALID_COMMAND;

	/* Check command for the letter T/t */
	if(_sim908_buffer[1] != 0x54 && _sim908_buffer[1] != 0x74)
		return	SIM908_INVALID_COMMAND;

	/* Check if response is OK */
	if (_sim908_buffer[_index-6] == CR && _sim908_buffer[_index-5] == LF
	    && _sim908_buffer[_index-4] == 'O' && _sim908_buffer[_index-3] == 'K'
		&& _sim908_buffer[_index-2] == CR && _sim908_buffer[_index-1] == LF)
			return SIM908_RESPONSE_OK;

	/* Check if response is ERROR */
	if (_sim908_buffer[_index-9] == CR && _sim908_buffer[_index-8] == LF
		&& _sim908_buffer[_index-7] == 'E' && _sim908_buffer[_index-6] == 'R'
		&& _sim908_buffer[_index-5] == 'R' && _sim908_buffer[_index-4] == 'O'
		&& _sim908_buffer[_index-3] == 'R'
		&& _sim908_buffer[_index-2] == CR && _sim908_buffer[_index-1] == LF)
			return SIM908_RESPONSE_ERROR;

	return SIM908_INVALID_RESPONSE;
}

char* _get_GPS_response(void)
{
	// ToDo
	return 1;
}

void _SIM908_callback(char data)
{
	#ifdef PC_CALLBACK
		uart1_send_char(data);
	#endif

	switch (_callback_state)
	{
		case ignore_data : break;

		case record_data :
			_sim908_buffer[_index++] = data;

			if (data == CR)
				_CR_counter++;
			else if (data == LF)
				_LF_counter++;
		break;

		default: break;
	}
}

#ifdef PC_CALLBACK
void _PC_callback(char data)
{
	uart0_send_char(data);
}
#endif
