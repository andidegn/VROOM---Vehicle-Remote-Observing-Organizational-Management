/**********************************************************************//**
 * @file: UART_Driver.h
 *
 * @Created: 12-09-2014 20:34:36
 * @Author: Andi Degn
 * @Version: 0.5
 * @defgroup uart UART Driver
 * @{
	 This is a driver for the UART0-1
	 on the ATMEGA family processors.\n
	 UART0:\n
	 * RX: PE0\n
	 * TX: PE1\n
	 \n
	 UART1:\n
	 * RX: PD2\n
	 * TX: PD3\n
	 @defgroup uart_pub Public
	 @defgroup uart_priv Private
 * @}
 *************************************************************************/


#ifndef UART_H_
#define UART_H_

#include <avr/interrupt.h>

#ifndef F_CPU
	#error: F_CPU must be defined!!!
#endif

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief defines the size of the transmit buffers \n
 *************************************************************************/
#define UART0_TX_BUFFER_SIZE 8
#define UART0_RX_BUFFER_SIZE 8
#define UART1_TX_BUFFER_SIZE 8
#define UART1_RX_BUFFER_SIZE 256

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief define for UART return
 *************************************************************************/
#define UART_NO_DATA 0x100U
#define UART_READ_NOT_ALOWED 0x200U
#define UART_BUFFER_OVERRUN 0x300U

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief defines for the mode of operations \n
 * * UART_MODE_NORMAL : Asynchronous at normal speed \n
 * * UART_MODE_DOUBLE : Asynchronous at double speed \n
 * * UART_MODE_SYNC   : Synchronous (NOT SUPPORTED YET) \n
 * * UART_MODE_MSPIM  : SPI Master (NOT SUPPORTED YET) \n
 *************************************************************************/
typedef enum {
	UART_MODE_NORMAL,
	UART_MODE_DOUBLE
} UART_MODE;

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief defines for the parity bit selection
 *************************************************************************/
typedef enum {
	UART_PARITY_DISABLED = 0,
	UART_PARITY_ENABLED = _BV(4), // UMP00 = UMP10 = 4
	UART_PARITY_EN_EVEN = _BV(5), // UMP01 = UMP11 = 5
	UART_PARITY_EN_ODD = _BV(4) | _BV(5)
} UART_PARITY_MODE;

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief defines for the stop bit selection
 *************************************************************************/
typedef enum {
	UART_ONE_STOP_BIT = 0,
	UART_TWO_STOP_BIT = _BV(3) // USBS0 = USBS1 = 3
} UART_STOP_BIT;

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief defines for the character size
 *************************************************************************/
typedef enum {
	UART_5_BIT,
	UART_6_BIT,
	UART_7_BIT,
	UART_8_BIT,
	UART_9_BIT
} UART_CHAR_SIZE;

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief defines for the different baud rates
 *************************************************************************/
typedef enum {
	UART_BAUD_2400 = 2400,
	UART_BAUD_4800 = 4800,
	UART_BAUD_7200 = 7200,
	UART_BAUD_9600 = 9600,
	UART_BAUD_14K4 = 14400,
	UART_BAUD_19K2 = 19200,
	UART_BAUD_28K8 = 28800,
	UART_BAUD_38K4 = 38400,
	UART_BAUD_57K6 = 57600,
	UART_BAUD_115K2 = 115200,
	UART_BAUD_128K0 = 128000,
	UART_BAUD_230K4 = 230400,
	UART_BAUD_460K8 = 460800,
	UART_BAUD_921K6 = 921600
} UART_BAUD;


/**********************************************************************//**
 * @ingroup uart_pub
 * @brief Sets up the UART0 bus as asynchronous using supplied parameters
 *
 * @param UART_MODE __operational_mode - normal speed or double speed mode
 * @param UART_BAUD __baud_rate - baud rate of operations
 * @param UART_PARITY_MODE __paraty_mode - sets the parity bit mode
 * @param UART_STOP_BIT __stop_bit - one or two stop bits
 * @param UART_CHAR_SIZE __char_size - the character size between 5 and 9 bits
 * @param void *__callback_function_ptr - the pointer to the function which should be called when the SPI interrupt is triggered
 *
 * @note BE SURE TO HAVE SELECTED A CLOCK SPEED FOR THE MCU WITH A LOW ERROR RATE FOR UART USAGE
 *
 * @return uint8_t - a handle for the setup
 *************************************************************************/
void uart0_setup_async(UART_MODE __operational_mode,
						 UART_BAUD __baud_rate,
						 UART_PARITY_MODE __paraty_mode,
						 UART_STOP_BIT __stop_bit,
						 UART_CHAR_SIZE __char_size,
						 void (*__callback_function_ptr)(char __data));

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief Sends 1 (one) char of 'data' on the UART bus
 *
 * @param char __data - the data to be sent
 *
 * @return void
 *************************************************************************/
void uart0_send_char(char __data);


/**********************************************************************//**
 * @ingroup uart_pub
 * @brief Sends a string (x no. of chars) of 'data' on the UART bus
 *
 * @param const char *__data - a sting to be sent
 *
 * @return void
 *************************************************************************/
void uart0_send_string(const char *__data);

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief Sends data (based on length) of 'data' on the UART bus.
 *
 * @param const char *__data - data to be sent
 * @param uint8_t __length - number of bytes
 * @note Does not terminates when zero character is meet
 * @return void
 *************************************************************************/
void uart0_send_data(const char *__data, uint8_t __length);

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief Reads 1 (one) char of 'data' in the UART buffer
 *
 * @param void
 *
 * @return uint16_t - next char(byte) in the buffer
 *************************************************************************/
uint16_t uart0_read_char(void);

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief Sets up the UART1 bus as asynchronous using supplied parameters
 *
 * @param UART_MODE __operational_mode - normal speed or double speed mode
 * @param UART_BAUD __baud_rate - baud rate of operations
 * @param UART_PARITY_MODE __paraty_mode - sets the parity bit mode
 * @param UART_STOP_BIT __stop_bit - one or two stop bits
 * @param UART_CHAR_SIZE __char_size - the character size between 5 and 9 bits
 * @param void *__callback_function_ptr - the pointer to the function which should be called when the SPI interrupt is triggered
 *
 * @note BE SURE TO HAVE SELECTED A CLOCK SPEED FOR THE MCU WITH A LOW ERROR RATE FOR UART USAGE
 *
 * @return uint8_t - a handle for the setup
 *************************************************************************/
void uart1_setup_async(UART_MODE __operational_mode,
						 UART_BAUD __baud_rate,
						 UART_PARITY_MODE __paraty_mode,
						 UART_STOP_BIT __stop_bit,
						 UART_CHAR_SIZE __char_size,
						 void (*__callback_function_ptr)(char __data));

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief Sends 1 (one) char of 'data' on the UART bus
 *
 * @param char __data - the data to be sent
 *
 * @return void
 *************************************************************************/
void uart1_send_char(char __data);


/**********************************************************************//**
 * @ingroup uart_pub
 * @brief Sends a string (x no. of chars) of 'data' on the UART bus
 *
 * @param const char *__data - a sting to be sent
 *
 * @return void
 *************************************************************************/
void uart1_send_string(const char *__data);

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief Sends data (based on length) of 'data' on the UART bus.
 *
 * @param const char *__data - data to be sent
 * @param uint8_t __length - number of bytes
 * @note Does not terminates when zero character is meet
 * @return void
 *************************************************************************/
void uart1_send_data(const char *__data, uint8_t __length);

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief Reads 1 (one) char of 'data' in the UART buffer
 *
 * @param void
 *
 * @return uint16_t - next char(byte) in the buffer
 *************************************************************************/
uint16_t uart1_read_char(void);


#endif /* UART_H_ */