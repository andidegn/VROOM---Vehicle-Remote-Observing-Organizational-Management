/*
 * @file: UART1_Driver.h
 *
 * @Created: 12-09-2014 20:34:36
 * @Author: Andi Degn
 * @Version: 0.2
 * @defgroup uart UART Driver
 * @{
	 This is a driver for the UART bus 1
	 on the ATMEGA family processors.\n
	 RX: PD2 \n
	 TX: PD3 \n
	 @defgroup uart_priv Private
	 @defgroup uart_pub Public
 * @}
 */


#ifndef UART1_H_
#define UART1_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>

#include "../../sensors/sensor_scheduler.h"

#ifndef F_CPU
	#error: F_CPU must be defined!!!
#endif

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief defines the size of the transmit buffer \n
 * @{
 *************************************************************************/
#define UART1_BUFFER_SIZE 255
/* @} */

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief defines for the mode of operations \n
 * * UART1_MODE_NORMAL : Asynchronous at normal speed \n
 * * UART1_MODE_DOUBLE : Asynchronous at double speed \n
 * * UART1_MODE_SYNC   : Synchronous (NOT SUPPORTED YET) \n
 * * UART1_MODE_MSPIM  : SPI Master (NOT SUPPORTED YET) \n
 * @{
 *************************************************************************/
typedef enum {
	UART1_MODE_NORMAL,
	UART1_MODE_DOUBLE
	/* not supported */
	//UART1_MODE_SYNC = _BV(UMSEL10),
	//UART1_MODE_MSPIM = _BV(UMSEL10) & _BV(UMSEL11)
} UART1_MODE;
/* @} */

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief defines for the parity bit selection
 * @{
 *************************************************************************/
typedef enum {
	UART1_PARITY_DISABLED = 0,
	UART1_PARITY_ENABLED = _BV(UPM10),
	UART1_PARITY_EN_EVEN = _BV(UPM11),
	UART1_PARITY_EN_ODD = _BV(UPM11) | _BV(UPM11)
} UART1_PARITY_MODE;
/* @} */

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief defines for the stop bit selection
 * @{
 *************************************************************************/
typedef enum {
	UART1_ONE_STOP_BIT = 0,
	UART1_TWO_STOP_BIT = _BV(USBS1)
} UART1_STOP_BIT;
/* @} */

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief defines for the character size
 * @{
 *************************************************************************/
typedef enum {
	UART1_5_BIT,
	UART1_6_BIT,
	UART1_7_BIT,
	UART1_8_BIT,
	UART1_9_BIT
} UART1_CHAR_SIZE;
/* @} */

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief defines for the trigger polarity
 * @note ONLY FOR SYNCHRONOUS MODE OF OPERATION AND NOT CURRENTLY SUPPURTED
 * @{
 *************************************************************************/
typedef enum { /* ONLY FOR SYNCHRONOUS MODE OF OPERATION */
	UART1_CLOCK_POL_RISING_EDGE = 0,
	UART1_CLOCK_POL_FALLING_EDGE = _BV(UCPOL1)
} UART1_CLOCK_POL;
/* @} */

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief defines for the different baud rates
 * @{
 *************************************************************************/
typedef enum {
	UART1_BAUD_2400 = 2400,
	UART1_BAUD_4800 = 4800,
	UART1_BAUD_7200 = 7200,
	UART1_BAUD_9600 = 9600,
	UART1_BAUD_14K4 = 14400,
	UART1_BAUD_19K2 = 19200,
	UART1_BAUD_28K8 = 28800,
	UART1_BAUD_38K4 = 38400,
	UART1_BAUD_57K6 = 57600,
	UART1_BAUD_115K2 = 115200,
	UART1_BAUD_128K0 = 128000,
	UART1_BAUD_230K4 = 230400
} UART1_BAUD;
/* @} */


/**********************************************************************//**
 * @ingroup uart_pub
 * @brief Sets up the UART bus as asynchronous using supplied parameters
 *
 * @param UART1_MODE operational_mode - normal speed or double speed mode
 * @param UART1_BAUD baud_rate - baud rate of operations
 * @param UART1_PARITY_MODE paraty_mode - sets the parity bit mode
 * @param UART1_STOP_BIT stop_bit - one or two stop bits
 * @param UART1_CHAR_SIZE char_size - the character size between 5 and 9 bits
 * @param void *callback_function_ptr - the pointer to the function which should be called when the SPI interrupt is triggered
 *
 * @note BE SURE TO HAVE SELECTED A CLOCK SPEED FOR THE MCU WITH A LOW ERROR RATE FOR UART USAGE
 *
 * @return uint8_t - a handle for the setup
 *************************************************************************/
void uart1_setup_async(UART1_MODE operational_mode,
						 UART1_BAUD baud_rate,
						 UART1_PARITY_MODE paraty_mode,
						 UART1_STOP_BIT stop_bit,
						 UART1_CHAR_SIZE char_size,
						 void (*callback_function_ptr)(unsigned char cfp));

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief Sends 1 (one) char of 'data' on the UART bus
 *
 * @param unsigned char data - the data to be sent
 *
 * @return void
 *************************************************************************/
void uart1_send_char(unsigned char data);


/**********************************************************************//**
 * @ingroup uart_pub
 * @brief Sends a string (x no. of chars) of 'data' on the UART bus
 *
 * @param unsigned char *data - the data to be sent
 * @param uint8_t length - the length of the string
 *
 * @return void
 *************************************************************************/
void uart1_send_string(unsigned char *data, uint8_t length);

#endif /* UART1_H_ */