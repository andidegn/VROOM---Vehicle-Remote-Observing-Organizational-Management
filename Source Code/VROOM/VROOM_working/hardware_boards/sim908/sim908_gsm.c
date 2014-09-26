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

static char _sim908_buffer[128];
static uint16_t _index;

#define CR 0x0D

#define HIGH 1
#define LOW  0

#define DDR(x) (*(&x - 1))
#define PIN(x) (*(&x - 2))

/* Prototypes */
void _SIM908_callback(char data);
void _PC_CALLBACK(char data);

/* ToDo - Error code p. 235 AT_Commands document*/
const char* GSM_ERR[773] = {	"Phone Faliure",
								"No Connection to Phone"

						   };

void SIM908_init(void)
{
	/* setting up uart for communication with the module */
 	uart0_setup_async(UART_MODE_DOUBLE,
 					  UART_BAUD_115K2,
 					  UART_PARITY_DISABLED,
 					  UART_ONE_STOP_BIT,
 					  UART_8_BIT,
 					  _SIM908_callback);

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

void SIM908_cmd(const char *cmd)
{
	uart0_send_string(cmd);
	uart1_send_char(CR);
}

void call_PSAP(void)
{

}

void _SIM908_callback(char data)
{
	_sim908_buffer[_index];
	_index++;
}

void _PC_callback(char data)
{
	_sim908_buffer[_index];
	_index++;
}