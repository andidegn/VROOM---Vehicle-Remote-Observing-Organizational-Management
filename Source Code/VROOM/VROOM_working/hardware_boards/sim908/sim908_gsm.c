/********************************************//**
@file sim908_gsm.c
@author: Kenneth René Jensen
@Version: 0.1
@defgroup sim908 Sim908_GSM
@{
	This is the driver for GSM module sim908
@}
@note Complies MISRO 2004 standards
************************************************/

#include "sim908_gsm.h"

/* Prototypes */
void _SIM908_callback(char data);
void _pin_state(uint8_t driver_pin, uint8_t state);

static char _sim908_buffer[128];
static uint16_t _index;

#define GSM_DRIVER_PIN_3	PE5
#define GSM_DRIVER_PIN_4	PG5
#define GSM_DRIVER_PIN_5	PE3

#define CR 0x0D

#define HIGH 1
#define LOW  0

void SIM908_init(void)
{
	// Init the driver pins for GSM function
	DDRE |= _BV(DDE5) | _BV(DDE3);
	DDRG |= _BV(DDG5);
		
	//Output GSM Timing
	//_pin_state(GSM_DRIVER_PIN_5, HIGH);
	_delay_ms(3000);
	PORTE |= _BV(GSM_DRIVER_PIN_5);
	_delay_ms(1500);
	PORTE &= ~_BV(GSM_DRIVER_PIN_5);	
	

	_delay_ms(1500);
	//	enable GSM TX?RX
	PORTE |= _BV(GSM_DRIVER_PIN_4);
	PORTG &= ~_BV(GSM_DRIVER_PIN_3);
	
	//_pin_state(GSM_DRIVER_PIN_3, LOW);
		//disable GPS TX?RX
	//_pin_state(GSM_DRIVER_PIN_4, HIGH);

	

		
	/* Setup UART0 for GSM */
	//uart0_setup_async(	UART_MODE_NORMAL,
						//UART_BAUD_9600,
						//UART_PARITY_DISABLED,
						//UART_ONE_STOP_BIT,
						//UART_8_BIT,
						//NULL);
	
	//_delay_ms(5000);
	//_delay_ms(5000);
	//_delay_ms(5000);
	
//	uart0_send_string("AT\r");
	//uart0_send_char(CR);
//	_delay_ms(2000);
	//uart0_send_string("AT\r\n");
	//_delay_ms(2000);
	//uart0_send_string("AT+CGPSIPR=9600\r\n");
	//_delay_ms(1000);
	//uart0_send_string("AT+CGPSPWR=1");
	//_delay_ms(1000);
	//uart0_send_string("AT+CGPSRST=1");
	//_delay_ms(5000);
	
	
	
	//Send:AT 2. 
	//uart0_send_string("AT");
	//_delay_ms(200);
	//uart0_send_string("AT");
	//_delay_ms(200);
	//uart0_send_string("AT");
	//
	//uart0_send_string("AT+CSMINS?");
}

int8_t SIM908_cmd(const char *cmd)
{
	
}

//void _pin_state(uint8_t driver_pin, uint8_t state)
//{
	//switch(driver_pin)
	//{
		//case GSM_DRIVER_PIN_4 :
			//if (state)
			//PORTG |= (1<<driver_pin);
			//else
			//PORTG &= ~(1<<driver_pin);
		//break;		
		//
		//case GSM_DRIVER_PIN_5 || GSM_DRIVER_PIN_3:
			//if (state)
			//PORTE |= (1<<driver_pin);
			//else
			//PORTE &= ~(1<<driver_pin);
		//break;
		//
		//default: break;
	//}
//}

void _SIM908_callback(char data)
{
	// _sim908_buffer[(_index++%16)] = data;
	//uart0_send_string("TEST\n\r");
//	uart0_send_char(data);
	//uart0_send_string("\n\r");
	//uart0_send_string("\n\r", 2);
}