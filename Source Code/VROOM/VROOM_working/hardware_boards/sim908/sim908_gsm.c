/********************************************//**
@file sim908_gsm.c
@author: Kenneth René Jensen
@Version: 0.1
@defgroup sim908 Sim908_GSM
@{
	This is the driver for GSM/GPRS/GPS module sim908
@}
@note NOT YET Complies MISRO 2004 standards
************************************************/
#include "sim908_gsm.h"
#include "../../data_comm/uart/uart.h"

//#include "../../hardware_boards/lcd_board/lcd/lcd.h"

static char _sim908_buffer[128];
static uint16_t _temp_char;
static uint8_t _index = 0;

#define CR	0x0D
#define LF	0x0A

#define DDR(x) (*(&x - 1))
#define PIN(x) (*(&x - 2))

/* Prototypes */
int8_t _SIM908_check_response(const char *buffer, const char *command, const char *response);
void _SIM908_callback(char data);
void _PC_CALLBACK(char data);

/* ToDo - Error code p. 235 AT_Commands document*/
const char* GSM_ERR[773] = {	"Phone Failure",
								"No Connection to Phone"

						   };

typedef enum {ignore_response, record_response} CALLBACK_STATE;
CALLBACK_STATE _callback_state = ignore_response;
	
int8_t SIM908_init(void)
{
	//lcd_init(LCD_DISP_ON);
	//lcd_clrscr();
	
	/* setting up uart for communication with the module */
 	uart0_setup_async(UART_MODE_DOUBLE,
 					  UART_BAUD_115K2,
 					  UART_PARITY_DISABLED,
 					  UART_ONE_STOP_BIT,
 					  UART_8_BIT,
 					  NULL);

	/* setting up uart for communication with pc */
	//uart1_setup_async(UART_MODE_DOUBLE,
						//UART_BAUD_115K2,
						//UART_PARITY_DISABLED,
						//UART_ONE_STOP_BIT,
						//UART_8_BIT,
						//_PC_CALLBACK);

	/* Set all related pins to output */
	DDR(DRIVER_PORT) |= (1<<CE_PIN);
	DDR(GSM_PORT) |= (1<<GSM_ENABLE_PIN);
	DDR(GPS_PORT) |= (1<<GPS_ENABLE_PIN);

	/* Toggle driver pin to start module */
	DRIVER_PORT |= _BV(CE_PIN);
	_delay_ms(1500);
	DRIVER_PORT &= ~_BV(CE_PIN);

	_delay_ms(5000);
	_delay_ms(5000);
	_delay_ms(5000);
	
	/* Auto reset - MAYBE NOT THE BEST SOLUTION */
	if (SIM908_cmd(AT_TEST, OK) != SIM908_OK)
		SIM908_init();
	
	return SIM908_OK;

	/* Set list of emergency numbers */
	//SIM908_cmd("AT+CEMNL=112");
}

void GSM_enable(void)
{
	GSM_PORT &= ~(1<<GSM_ENABLE_PIN);
	GPS_PORT |= (1<<GPS_ENABLE_PIN);
	_delay_ms(1500);
}

void GPS_enable(void)
{
	GPS_PORT &= ~(1<<GPS_ENABLE_PIN);
	GSM_PORT = (1<<GSM_ENABLE_PIN);
	_delay_ms(1500);
}

int8_t SIM908_cmd(const char *cmd, const char *res)
{
	_flush_buffer();
	uart0_send_string(cmd);
	uart0_send_char(CR);
	// _callback_state = record_response;
	_delay_ms(1500);
	while ((_temp_char = uart0_read_char()) != UART_NO_DATA )
	{
		_sim908_buffer[_index++] = _temp_char;
	}
	
	// _callback_state = ignore_response;
	return _SIM908_check_response(_sim908_buffer, cmd, res);
}

int8_t call_PSAP(void)
{
	// return SIM908_cmd(AT_CALL_EMERGENCY, OK);
	return SIM908_cmd(AT_CALL_KENNETH, OK);
}

void _flush_buffer()
{
	memset(&_sim908_buffer[0], 0, sizeof(_sim908_buffer));
	_index = 0;
	while (uart0_read_char() != UART_NO_DATA );
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
	
	if(buffer[0] != 0x41 && buffer[0] != 0x61)
		return	SIM908_INVALID_COMMAND;
	
	if(buffer[1] != 'T' && buffer[1] != 0x74)
		return	SIM908_INVALID_COMMAND;
	///* Check command for the letter A/a */
	//if(command[0] != 0x54 || command[0] != 0x61 )
		//return	SIM908_INVALID_COMMAND;
		//
	///* Check command for the letter T/t */
	//if(command[1] != 0x54 || command[1] != 0x74)
		//return	SIM908_INVALID_COMMAND;
	
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
			_sim908_buffer[_index] = data; 
			_index++;
		break;
		
		default: break;
	}
}

//void _PC_callback(char data)
//{
//
//}