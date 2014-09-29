/********************************************//**
@file sim908_gsm.c
@author: Kenneth René Jensen
@Version: 0.1
@defgroup sim908 Sim908_GSM
@{
	This is the driver for GSM module sim908
@}
@note NOT YET Complies MISRO 2004 standards
************************************************/
#include "sim908_gsm.h"
#include "../../data_comm/uart/uart.h"

static char _sim908_buffer[128];
static char tmp_char; 
static uint8_t _index = 0;

#define CR 0x0D
#define LF 0x0A

#define HIGH 1
#define LOW  0

#define DDR(x) (*(&x - 1))
#define PIN(x) (*(&x - 2))

#define _flush_buffer() while (uart0_read_char() != UART_NO_DATA); 
/* Prototypes */
//int8_t _SIM908_check_response(char *check);

int8_t _SIM908_check_response(const char *response,const char *check,uint8_t len);
void _SIM908_callback(char data);
void _PC_CALLBACK(char data);

/* ToDo - Error code p. 235 AT_Commands document*/
const char* GSM_ERR[773] = {	"Phone Faliure",
								"No Connection to Phone"

						   };

int8_t SIM908_init(void)
{
	/* setting up uart for communication with the module */
 	uart0_setup_async(UART_MODE_DOUBLE,
 					  UART_BAUD_115K2,
 					  UART_PARITY_DISABLED,
 					  UART_ONE_STOP_BIT,
 					  UART_8_BIT,
 					  NULL);
	
	

	/* setting up uart for communication with pc for diag */
	//uart1_setup_async(UART_MODE_DOUBLE,
						//UART_BAUD_115K2,
						//UART_PARITY_DISABLED,
						//UART_ONE_STOP_BIT,
						//UART_8_BIT,
						//_PC_CALLBACK);

	/* saves the current state of the status register and disables global interrupt */
	uint8_t SREG_cpy = SREG;
	cli();

	/* Set all related pins to output */
	DDR(DRIVER_PORT) |= (1<<CE_PIN);
	DDR(GSM_PORT) |= (1<<GSM_ENABLE_PIN);
	DDR(GPS_PORT) |= (1<<GPS_ENABLE_PIN);

	/* Toggle driver pin to start module */
	DRIVER_PORT |= _BV(CE_PIN);
	_delay_ms(1500);
	DRIVER_PORT &= ~_BV(CE_PIN);
	_delay_ms(1500);

	/* Restore interrupt */
	SREG = SREG_cpy;
	_delay_ms(5000);

	_delay_ms(5000);
	_delay_ms(5000);
	
	//while (uart0_read_char() != UART_NO_DATA); 
	
	_flush_buffer();
	SIM908_cmd("AT");	//Test command
	_delay_ms(1500);
	//
	//while (uart0_read_char() != UART_NO_DATA); 
	//UReadBuffer(_sim908_buffer,6);
	
//	while (_sim908_buffer[_index++] = uart0_read_char() != UART_NO_DATA);
	//{
		//
	//}
	uint8_t i = 0;
	_index = 0;
		 
	while ((tmp_char = uart0_read_char()) != UART_NO_DATA) 
	{
		_sim908_buffer[_index] = tmp_char;
		_index++;
	} //AT <CR> <CR> <LF> OK <CR> <LF>


	
	//for(i=0;i<10;i++)
	//{
		//((char*)_sim908_buffer)[i]=uart0_read_char();
	//}
		
	return _SIM908_check_response(_sim908_buffer,"OK",_index);
	// return _SIM908_check_response("OK");


	/* Set list of emergency numbers */
	//SIM908_cmd("AT+CEMNL=112");
}

int8_t GSM_enable(void)
{
	GSM_PORT &= ~(1<<GSM_ENABLE_PIN);
	GPS_PORT |= (1<<GPS_ENABLE_PIN);
	_delay_ms(1500);
	
uint16_t i;
for(i=0;i<6;i++)
{
	((char*)_sim908_buffer)[i]=uart0_read_char();
}

return _SIM908_check_response(_sim908_buffer,"OK",6);
	//return _SIM908_check_response("OK");
}

int8_t GPS_enable(void)
{
	GPS_PORT &= ~(1<<GPS_ENABLE_PIN);
	GSM_PORT = (1<<GSM_ENABLE_PIN);
	_delay_ms(1500);
	
	return 1;
	//return _SIM908_check_response("OK");
}

int8_t SIM908_cmd(const char *cmd)
{
	uart0_send_string(cmd);
	uart0_send_char(CR);
}

int8_t call_PSAP(void)
{

}

int8_t _SIM908_check_response(const char *response,const char *check,uint8_t len)
{
		len-=2;
		
		//Check leading CR LF
		if(response[0]!=0x0D | response[1]!=0x0A)
		return	SIM908_INVALID_RESPONSE;
		
		//Check trailing CR LF
		if(response[len]!=0x0D | response[len+1]!=0x0A)
		return	SIM908_INVALID_RESPONSE;
		
		
		//Compare the response
		for(uint8_t i=2;i<len;i++)
		{
			if(response[i]!=check[i-2])
			return SIM908_FAIL;
		}
		
		return SIM908_OK;
	
	
	
	
	
	//_index = 0;
	//
		//while (_sim908_buffer[_index] = uart0_read_char() != CR);
		//{
			//if(_sim908_buffer[_index] != check[_index])
				//return SIM908_FAIL;
			//
			//_index++;
		//}
	//
	///* Check leading CR and LF */
	//if (_sim908_buffer[0] != CR | _sim908_buffer[1] != LF)
		//return SIM908_INVALID_RESPONSE;
	//
	///* Check trailing CR and LF */
	//if (_sim908_buffer[_index-2] != CR | _sim908_buffer[_index-1] != LF)
		//return SIM908_INVALID_RESPONSE;
		//
	//return SIM908_OK;
}

void _SIM908_callback(char data)
{
	//_sim908_buffer[_index];
	//_index++;
}

void _PC_callback(char data)
{
	//_sim908_buffer[_index];
	//_index++;
}