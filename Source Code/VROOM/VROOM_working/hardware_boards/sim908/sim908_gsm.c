/********************************************//**
@file sim908_gsm.c
@author: Kenneth René Jensen
@Version: 0.2
@defgroup sim908 Sim908_GSM
@{
	This is the driver for GSM/GPRS/GPS module sim908
@}
@note NOT YET Complies MISRO 2004 standards
************************************************/
#include "sim908_gsm.h"

static char _sim908_buffer[16];
static uint16_t _temp_char;
static uint8_t _index = 0;

#define CR	0x0D
#define LF	0x0A

#define DDR(x) (*(&x - 1))
#define PIN(x) (*(&x - 2))

/* ToDo - Error code p. 235 AT_Commands document*/
const char* GSM_ERR[773] =
{
	"Phone Failure",
	"No Connection to Phone"
};

/* Prototypes */
int8_t _SIM908_check_response(const char *buffer, const char *command, const char *response);
void _SIM908_callback(char data);
void _flush_buffer(void);
void _read_buffer(void);
#ifdef PC_CALLBACK
void _PC_CALLBACK(char data);
#endif

typedef enum {ignore_response, record_response} CALLBACK_STATE;
CALLBACK_STATE _callback_state = ignore_response;

#define _TIMEOUT() (SIM908_TIMEOUT_COUNTER > SIM908_TIMEOUT_VALUE )

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Initiates the SIM908 module
 @return 1 if initiation has succeeded
		-4 if timeout
 @note UART0 is used to communicate with the module. The function runs recursively until it timeout or succeed 
 ************************************************************************************************************************/
uint8_t SIM908_init(void)
{	
	/* Setting up uart for communication with the module */
 	uart0_setup_async(UART_MODE_DOUBLE,
 					  UART_BAUD_115K2,
 					  UART_PARITY_DISABLED,
 					  UART_ONE_STOP_BIT,
 					  UART_8_BIT,
 					  _SIM908_callback);

	#ifdef PC_CALLBACK
	uart1_setup_async(UART_MODE_DOUBLE,
						UART_BAUD_115K2,
						UART_PARITY_DISABLED,
						UART_ONE_STOP_BIT,
						UART_8_BIT,
						_PC_CALLBACK);
	#endif
	/* Set all related pins to output */
	DDR(DRIVER_PORT) |= (1<<CE_PIN);
	DDR(GSM_PORT) |= (1<<GSM_ENABLE_PIN);
	DDR(GPS_PORT) |= (1<<GPS_ENABLE_PIN);
	
	start_timer3();
	SIM908_TIMEOUT_COUNTER = 0;
	
	while (!_TIMEOUT())
	{
		/* Toggle driver pin to start module */
		DRIVER_PORT |= _BV(CE_PIN);
		_delay_ms(1500);
		DRIVER_PORT &= ~_BV(CE_PIN);
		_delay_ms(1500);
		
		/* Enable Echo - Auto resets if no or wrong response */
		if (SIM908_cmd(AT_DIAG_ECHO_ENABLE, OK) == SIM908_OK)
		{
			stop_timer3();
			return SIM908_OK;
		}
	}
	
	return SIM908_TIMEOUT;
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
		*res is the expected response as a string
 @return 1 if valid command and getting expected response
		-1 if invalid command
		-2 if invalid response
		-3 if fail 
 ************************************************************************************************************************/
int8_t SIM908_cmd(const char *cmd, const char *res)
{
	_flush_buffer();
	_callback_state = record_response;
	uart0_send_string(cmd);
	uart0_send_char(CR);
	_delay_ms(1000);
	_callback_state = ignore_response;
	_read_buffer();
	
	return _SIM908_check_response(_sim908_buffer, cmd, res);
}

/********************************************************************************************************************//**
 @ingroup sim908
 @brief Calling Public-safety answering point
 @return void
 @note Pushes the call until it get correct response
 ************************************************************************************************************************/
void call_PSAP(void)
{
	// while (SIM908_cmd(AT_CALL_EMERGENCY, OK) != SIM908_OK);
	while (SIM908_cmd(AT_CALL_KENNETH, OK) != SIM908_OK);
}

void _flush_buffer(void)
{
	memset(&_sim908_buffer[0], 0, sizeof(_sim908_buffer));
	_index = 0;
}

void _read_buffer(void)
{
	while ((_temp_char = uart0_read_char()) != UART_NO_DATA )
	{
		_sim908_buffer[_index++] = _temp_char;
	}
}

int8_t _SIM908_check_response(const char *buffer, const char *command, const char *response)
{
	uint8_t _response_start = strlen(command) + 1;
	uint8_t _response_end = _index - 1;
	uint8_t _response_txt_idx = _response_start + 2;
	uint8_t i = 0;
	
	/* AT <CR> <CR> <LF> OK <CR> <LF>
		0x41 = A	
		0x54 = T 
		0x0d = <CR>
		0x0d = <CR>
		0x0a = <LF>
		0x4f = O
		0x4b = K
		0x0d = <CR>
		0x0a = <LF>
	*/
	
	/* Check command for the letter A/a */
	if(buffer[0] != 0x41 && buffer[0] != 0x61)
		return	SIM908_INVALID_COMMAND;
	
	/* Check command for the letter T/t */
	if(buffer[1] != 'T' && buffer[1] != 0x74)
		return	SIM908_INVALID_COMMAND;

	/* Check leading response <CR><LF> */
	if(buffer[_response_start] != CR || buffer[_response_start+1] != LF)
		return	SIM908_INVALID_RESPONSE;
		
	/* Check trailing response <CR><LF> */
	if(buffer[_response_end-1]!=CR || buffer[_response_end]!=LF)
		return	SIM908_INVALID_RESPONSE;
		
	/* Compare the actual response with the expected response */
	for(i = _response_txt_idx; i < _response_end-1; i++)
	{
		if(buffer[i] != response[i - _response_txt_idx])
			return SIM908_FAIL;
	}
		
	return SIM908_OK;
}

void _SIM908_callback(char data)
{
	switch (_callback_state)
	{
		case ignore_response : break;
		
		case record_response :
			_sim908_buffer[_index++] = data; 
		break;
		
		default: break;
	}
}

#ifdef PC_CALLBACK
void _PC_callback(char data)
{

}
#endif