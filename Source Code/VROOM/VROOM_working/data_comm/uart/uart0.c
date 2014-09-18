/*
 * @file: UART0_Driver.c
 *
 * @Created: 12-09-2014 20:34:20
 * @Author: Andi Degn
 * @Version: 0.2
 * @defgroup uart UART Driver
 * @{
	 This is a driver for the UART bus 0
	 on the ATMEGA family processors placed on PORTE.
	 @defgroup uart_priv Private
	 @defgroup uart_pub Public
 * @}
 */

#include "uart0.h"

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief defines for the factor used to calculate baud rate and UBRR0value
 * @{
 *************************************************************************/
#define UBRR0_FACTOR_ASYNC_NORM	16
#define UBRR0_FACTOR_ASYNC_DOUBLE 8
#define UBRR0_FACTOR_SYNC 2
/* @} */

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief defines for calculating the UBRR0 value
 * @note 0.5F is added to make sure rounding is done rather than truncating
 * @{
 *************************************************************************/
#define CALC_UBRR0(baud_rate, factor) (((F_CPU / (factor * baud_rate)) - 1) + 0.5F)
/* @} */

/* local variables */
static void (*_callback_function_ptr)(char cfp);
static char _buffer_send_data[UART0_BUFFER_SIZE];
static uint8_t _buffer_length = 0U;
static uint8_t _buffer_index = 0U;

/**********************************************************************//**
 * @ingroup uart_pub
 * Takes the supplied UART parameters and sets up the UART accordingly
 **************************************************************************/
void uart0_setup_async(UART0_MODE operational_mode,
						 UART0_BAUD baud_rate,
						 UART0_PARITY_MODE paraty_mode,
						 UART0_STOP_BIT stop_bit,
						 UART0_CHAR_SIZE char_size,
						 void (*callback_function_ptr)(char cfp)) {
	/* saves the current state of the status register and disables global interrupt */
	uint8_t _sreg = SREG;
	cli();

	/* setting the values for the speed of the UART */
	switch (operational_mode) {
		case UART0_MODE_NORMAL:
			UBRR0 = CALC_UBRR0(baud_rate, UBRR0_FACTOR_ASYNC_NORM);
			break;

		case UART0_MODE_DOUBLE:
			UBRR0 = CALC_UBRR0(baud_rate, UBRR0_FACTOR_ASYNC_DOUBLE);
			UCSR0A = _BV(U2X0);
			break;

		default:
			break;
	}

	/* setting up bits for the character size. It is done here as it was
	not possible to	differentiate between UART0_8_BIT and UART0_9_BIT in an enum */
	uint8_t _char_size = 0U;
	switch (char_size) {
		case 1:
			_char_size = _BV(UCSZ00);
			break;

		case 2:
			_char_size= _BV(UCSZ01);
			break;

		case 3:
			_char_size = _BV(UCSZ00) | _BV(UCSZ01);
			break;

		case 4:
			_char_size = _BV(UCSZ00) | _BV(UCSZ01);
			break;

		default:
			break;
	}

	/* sets various setup bits (p.223-226) */
	/*    Bit  |   7  |   6  |   5  |   4  |   3  |   2  |   1  |   0  |
		       |RXCIE0|TXCIE0|UDRIE0| RXEN0| TXEN0|UCSZ02| RXB80| TXB80| UCSR0B
	Read/Write |  R/W |  R/W |  R/W |  R/W |  R/W |  R/W |  R/W |  R/W |

	Bit 7 - RXCIE0: RX Complete Interrupt Enable
	Bit 6 - TXCIE0: TX Complete Interrupt Enable
	Bit 5 - UDRIE0: UART Data Register Empty Interrupt Enable
	Bit 4 - RXEN0: Receiver Enable
	Bit 3 - TXEN0: Transmitter Enable
	Bit 2 - UCSZ02: Character Size
	Bit 1 - RXB80: Receive Data Bit 8
	Bit 0 - TXB80: Transmit Data Bit 8
	*/
	UCSR0B = 0;
	UCSR0B |= (callback_function_ptr != NULL ? _BV(RXCIE0) : 0) |
			  _BV(RXEN0) |
			  _BV(TXEN0) |
			  (char_size == UART0_9_BIT ? _BV(UCSZ02) : 0);

	/*    Bit  |   7  |   6  |   5  |   4  |   3  |   2  |   1  |   0  |
		       |UMSEL01UMSEL00 UPM01| UPM00| USBS0|UCSZ01|UCSZ00|UCPOL0| UCSR0C
	Read/Write |  R/W |  R/W |  R/W |  R/W |  R/W |  R/W |  R/W |  R/W |

	Bits 7:6 - UMSEL01:0 UART Mode Select
	Bits 5:4 - UPM01:0: Parity Mode
	Bit 3    - USBS0: Stop Bit Select
	Bits 2:1 - UCSZ01:0: Character Size
	Bit 0    - UCPOL0: Clock Polarity
	*/
	/* add "operational_mode" here if support of synchronous
	and/or master SPI is to be implemented */
	UCSR0C = 0;
	UCSR0C |= paraty_mode |
			  stop_bit |
			  _char_size;

	//UCSR0C |= _BV(UCSZ00) |
			  //_BV(UCSZ01);

	/* storing callback function */
	_callback_function_ptr = callback_function_ptr;

	/* restore the status register */
	SREG = _sreg;
}

/**********************************************************************//**
 * @ingroup uart_pub
 * Sends one char
 *************************************************************************/
void uart0_send_char(char data) {
	uart0_send_string(&data, 1U);
}

/**********************************************************************//**
 * @ingroup uart_pub
 * Loops through the 'data' string stores it in a local buffer, then sets
 * the Data Register Empty interrupt bit.
 * @note If the data would make the local buffer overrun, the function returns
 *************************************************************************/
void uart0_send_string(char *data, uint8_t length) {
	if (!(_buffer_length + length > 255)) {
		/* saves the current state of the status register and disables global interrupt */
		uint8_t _sreg = SREG;
		cli();

		/* copying data to local buffer */
		for (uint8_t _i = 0; _i < length; _i++) {
			_buffer_send_data[_buffer_length++] = *data++;
		}

		/* enable Data Register empty interrupt */
		UCSR0B |= _BV(UDRIE0);

		/* restore the status register */
		SREG = _sreg;
	}
	return;
}

/**********************************************************************//**
 * @ingroup uart_priv
 * Interrupt service routine for the UART transmit.
 **************************************************************************/
ISR(USART0_UDRE_vect, ISR_BLOCK) {
	if (_buffer_index < _buffer_length) {
		UDR0 = _buffer_send_data[_buffer_index++];
	} else {
		_buffer_index = 0;
		_buffer_length = 0;
		UCSR0B &= ~_BV(UDRIE0);
		/* releasing the scheduler */
		scheduler_release();
	}
}

/**********************************************************************//**
 * @ingroup uart_priv
 * Interrupt service routine for the UART receive. If a callback function pointer is
 * supplied when setting up the UART, a callback to that function is being performed
 **************************************************************************/
ISR(USART0_RX_vect, ISR_BLOCK) {
	char received_data = UDR0;
	if (_callback_function_ptr != NULL) {
		_callback_function_ptr(received_data);
	}
}
