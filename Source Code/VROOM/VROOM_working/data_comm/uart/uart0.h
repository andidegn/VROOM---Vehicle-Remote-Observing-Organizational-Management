/*
 * @file: UART0_Driver.h
 *
 * @Created: 12-09-2014 20:34:36
 * @Author: Andi Degn
 * @Version: 0.2
 * @defgroup uart UART Driver
 * @{
	 This is a driver for the UART bus 0
	 on the ATMEGA family processors.\n
	 RX: PE0 \n
	 TX: PE1 \n
	 @defgroup uart_priv Private
	 @defgroup uart_pub Public
 * @}
 */


#ifndef UART0_H_
#define UART0_H_

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
#define UART0_BUFFER_SIZE 255
/* @} */

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief defines for the mode of operations \n
 * * UART0_MODE_NORMAL : Asynchronous at normal speed \n
 * * UART0_MODE_DOUBLE : Asynchronous at double speed \n
 * * UART0_MODE_SYNC   : Synchronous (NOT SUPPORTED YET) \n
 * * UART0_MODE_MSPIM  : SPI Master (NOT SUPPORTED YET) \n
 * @{
 *************************************************************************/
typedef enum {
	UART0_MODE_NORMAL,
	UART0_MODE_DOUBLE
	/* not supported */
	//UART0_MODE_SYNC = _BV(UMSEL00),
	//UART0_MODE_MSPIM = _BV(UMSEL00) & _BV(UMSEL01)
} UART0_MODE;
/* @} */

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief defines for the parity bit selection
 * @{
 *************************************************************************/
typedef enum {
	UART0_PARITY_DISABLED = 0,
	UART0_PARITY_ENABLED = _BV(UPM00),
	UART0_PARITY_EN_EVEN = _BV(UPM01),
	UART0_PARITY_EN_ODD = _BV(UPM01) | _BV(UPM01)
} UART0_PARITY_MODE;
/* @} */

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief defines for the stop bit selection
 * @{
 *************************************************************************/
typedef enum {
	UART0_ONE_STOP_BIT = 0,
	UART0_TWO_STOP_BIT = _BV(USBS0)
} UART0_STOP_BIT;
/* @} */

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief defines for the character size
 * @{
 *************************************************************************/
typedef enum {
	UART0_5_BIT,
	UART0_6_BIT,
	UART0_7_BIT,
	UART0_8_BIT,
	UART0_9_BIT
} UART0_CHAR_SIZE;
/* @} */

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief defines for the trigger polarity
 * @note ONLY FOR SYNCHRONOUS MODE OF OPERATION AND NOT CURRENTLY SUPPURTED
 * @{
 *************************************************************************/
typedef enum { /* ONLY FOR SYNCHRONOUS MODE OF OPERATION */
	UART0_CLOCK_POL_RISING_EDGE = 0,
	UART0_CLOCK_POL_FALLING_EDGE = _BV(UCPOL0)
} UART0_CLOCK_POL;
/* @} */

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief defines for the different baud rates
 * @{
 *************************************************************************/
typedef enum {
	UART0_BAUD_2400 = 2400,
	UART0_BAUD_4800 = 4800,
	UART0_BAUD_7200 = 7200,
	UART0_BAUD_9600 = 9600,
	UART0_BAUD_14K4 = 14400,
	UART0_BAUD_19K2 = 19200,
	UART0_BAUD_28K8 = 28800,
	UART0_BAUD_38K4 = 38400,
	UART0_BAUD_57K6 = 57600,
	UART0_BAUD_115K2 = 115200,
	UART0_BAUD_128K0 = 128000,
	UART0_BAUD_230K4 = 230400
} UART0_BAUD;
/* @} */


/**********************************************************************//**
 * @ingroup uart_pub
 * @brief Sets up the UART bus as asynchronous using supplied parameters
 *
 * @param UART0_MODE operational_mode - normal speed or double speed mode
 * @param UART0_BAUD baud_rate - baud rate of operations
 * @param UART0_PARITY_MODE paraty_mode - sets the parity bit mode
 * @param UART0_STOP_BIT stop_bit - one or two stop bits
 * @param UART0_CHAR_SIZE char_size - the character size between 5 and 9 bits
 * @param void *callback_function_ptr - the pointer to the function which should be called when the SPI interrupt is triggered
 *
 * @note BE SURE TO HAVE SELECTED A CLOCK SPEED FOR THE MCU WITH A LOW ERROR RATE FOR UART USAGE
 *
 * @return uint8_t - a handle for the setup
 *************************************************************************/
void uart0_setup_async(UART0_MODE operational_mode,
						 UART0_BAUD baud_rate,
						 UART0_PARITY_MODE paraty_mode,
						 UART0_STOP_BIT stop_bit,
						 UART0_CHAR_SIZE char_size,
						 void (*callback_function_ptr)(char cfp));

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief Sends 1 (one) char of 'data' on the UART bus
 *
 * @param char data - the data to be sent
 *
 * @return void
 *************************************************************************/
void uart0_send_char(char data);


/**********************************************************************//**
 * @ingroup uart_pub
 * @brief Sends a string (x no. of chars) of 'data' on the UART bus
 *
 * @param char *data - the data to be sent
 * @param uint8_t length - the length of the string
 *
 * @return void
 *************************************************************************/
void uart0_send_string(char *data, uint8_t length);

#endif /* UART0_H_ */