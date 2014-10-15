/********************************************//**
@file sim908.c
@author: Kenneth Ren� Jensen
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

#define _TIMEOUT() (SIM908_TIMEOUT_COUNTER > SIM908_TIMEOUT_VALUE )

typedef enum {ignore_data, record_data} CALLBACK_STATE;
CALLBACK_STATE _callback_state = ignore_data;

/* Prototypes */
void _setup_GSM(void);
void _setup_GPS(void);
void _setup_GPRS_TCPIP(void);
void _setup_GPRS_FTP(void);
int8_t _SIM908_check_response(void);
void _flush_buffer(void);
int8_t _wait_response(void);
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
		
	/* Setting up timer for timeout determination */	
	init_Timer3_CTC(TIMER_PS256, TIMER_10HZ);
		
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
		
	_setup_GSM();
	_setup_GPS();
	GSM_enable();
	// _setup_GPRS_TCPIP();
	// _setup_GPRS_FTP();
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
	SIM908_TIMEOUT_COUNTER = 0;
	
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

/* ********************************** TCP / IP **********************************
 *	States setting up GPRS - TCP:
 *	1:	Attach to network		AT+CGATT=1
 *	2:	Set single mode			AT+CIPMUX=0
 *	3:	Set APN					AT+CSTT="websp"	(AT+CSTT=<APN>,<USERNAME>,<PASSWORD>)
 *  4:	Bring up GPRS			AT+CIICR
 *  5:  Get local IP address	AT+CIFSR						(MUST BE CALLED??)
 *  ------------------------------------------------------------------------------
 *  6:	Open TCP connection		AT+CIPSTART="TCP","andidegn.dk","1404" 
 *  7:	Send data (140 bytes)	AT+CIPSEND=140
 *  8:	Close TCP connection	AT+CIPCLOSE=1
 * *******************************************************************************/
void _setup_GPRS_TCPIP(void)
{
	/* Attach to network */
	SIM908_cmd(AT_GPRS_ATTACHED);
	
	/* Set Single-connection mode */
	SIM908_cmd(AT_TCPIP_SINGLE);
	
	/* Set APN */
	SIM908_cmd(AT_TCP_APN_CALLME);
	
	/* Bring up GPRS */
	SIM908_cmd(AT_GPRS_ACTIVATE);
	
	/* Local IP MUST BE CALLED ??? - different response */
	uart0_send_string(AT_GPRS_GET_LOCAL_IP);
	uart0_send_char(CR);
	uart0_send_char(LF);
	
	/* Open TCP connection */
	SIM908_cmd(AT_OPEN_TCP);
}

/* ********************************** FTP **********************************
 *	States setting up GPRS - FTP:
 *	1:	Set bearer parameter	AT+SAPBR=3,1,"Contype","GPRS"
 *								AT+SAPBR=3,1,"APN","internet.mtelia.dk"
 *	2:	Open bearer				AT+SAPBR=1,1
 *	3:	Use bearer profile		AT+FTPCID=1
 *  4:	FTP login				AT+FTPSERV="ftp.andidegn.dk"
 *								AT+FTPPORT=1404
 *								AT+FTPUN="VROOM"
 *								AT+FTPPW="6198fg(/G6F/&5(!(!8gf87gMF."	
 *  5:  Configure put			AT+FTPPUTNAME="ftp-test.txt"
 *								AT+FTPPUTPATH="/"
 *								AT+FTPTYPE="A"
 *								AT+FTPPUTOPT="APPE"
 * -------------------------------------------------------------------------
 *  6:	Open FTP PUT session	AT+FTPPUT=1
 *	7:  Set write data			AT+FTPPUT=2,140
 *	8:	Write text (140 bytes)
 *	9:	End write session		AT+FTPPUT=2,0
 *	10: Close bearer			AT+SAPBR=0,1
 ***************************************************************************/
void _setup_GPRS_FTP(void)
{
	/* Set bearer parameters */
	SIM908_cmd(AT_FTP_BEARER1_CONTYPE_GPS);
	SIM908_cmd(AT_FTP_BEARER1_APN_CALLME);
	
	/* Open bearer */
	SIM908_cmd(AT_FTP_OPEN_BEARER1);
	
	/* Use bearer profile 1 */
	SIM908_cmd(AT_FTP_USE_PROFILE1);
	
	/* FTP login */
	SIM908_cmd(AT_FTP_SET_SERVER_ADDRESS);
	SIM908_cmd(AT_FTP_SET_CONTROL_PORT);
	SIM908_cmd(AT_FTP_SET_USER_NAME_VROOM);
	SIM908_cmd(AT_FTP_SET_PASSWORD);
	
	/* Set put information */
	SIM908_cmd(AT_FTP_PUT_NAME);
	SIM908_cmd(AT_FTP_PUT_PATH);	
	SIM908_cmd(AT_FTP_SET_DATA_TYPE_ASCII);
	SIM908_cmd(AT_FTP_PUT_FILE_APPENDING);
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
	SIM908_TIMEOUT_COUNTER = 0;
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
