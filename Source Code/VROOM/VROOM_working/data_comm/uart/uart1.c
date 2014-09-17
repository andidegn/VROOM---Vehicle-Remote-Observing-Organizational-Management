/*
 * @file: UART1_Driver.c
 *
 * @Created: 12-09-2014 20:34:20
 * @Author: Andi Degn
 * @Version: 0.2
 * @defgroup uart UART Driver
 * @{
	 This is a driver for the UART bus 1
	 on the ATMEGA family processors placed on PORTE.
	 @defgroup uart_priv Private
	 @defgroup uart_pub Public
 * @}
 */

#include "uart1.h"

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief defines for the factor used to calculate baud rate and UBRR1value
 * @{
 *************************************************************************/
#define UBRR1_FACTOR_ASYNC_NORM	16
#define UBRR1_FACTOR_ASYNC_DOUBLE 8
#define UBRR1_FACTOR_SYNC 2
/* @} */

/**********************************************************************//**
 * @ingroup uart_pub
 * @brief defines for calculating baud rate and UBRR1 value
 * @note 0.5F is added to make sure rounding is done rather than truncating
 * @{
 *************************************************************************/
#define CALC_UBRR1(baud_rate, factor) ((F_CPU / ((factor * baud_rate)) - 1) + 0.5F)
/* @} */

/* local variables */
static void (*_callback_function_ptr)(unsigned char cfp);
static char _buffer_send_data[UART1_BUFFER_SIZE];
static uint8_t _buffer_length = 0U;
static uint8_t _buffer_index = 0U;

/**********************************************************************//**
 * @ingroup uart_pub
 * Takes the supplied UART parameters and sets up the UART accordingly
 **************************************************************************/
void uart1_setup_async(UART1_MODE operational_mode,
						 UART1_BAUD baud_rate,
						 UART1_PARITY_MODE paraty_mode,
						 UART1_STOP_BIT stop_bit,
						 UART1_CHAR_SIZE char_size,
						 void (*callback_function_ptr)(unsigned char cfp)) {
	/* saves the current state of the status register and disables global interrupt */
	uint8_t _sreg = SREG;
	cli();

	/* setting the values for the speed of the UART */
	switch (operational_mode) {
		case UART1_MODE_NORMAL:
			UBRR1 = CALC_UBRR1(baud_rate, UBRR1_FACTOR_ASYNC_NORM);
			break;

		case UART1_MODE_DOUBLE:
			UBRR1 = CALC_UBRR1(baud_rate, UBRR1_FACTOR_ASYNC_DOUBLE);
			UCSR1A = _BV(U2X1);
			break;

		default:
			break;
	}

	/* setting up bits for the character size. It is done here as it was
	not possible to	differentiate between UART1_8_BIT and UART1_9_BIT in an enum */
	uint8_t _char_size = 0U;
	switch (char_size) {
		case 1:
			_char_size = _BV(UCSZ10);
			break;

		case 2:
			_char_size= _BV(UCSZ11);
			break;

		case 3:
			_char_size = _BV(UCSZ10) | _BV(UCSZ11);
			break;

		case 4:
			_char_size = _BV(UCSZ10) | _BV(UCSZ11);
			break;

		default:
			break;
	}

	/* sets various setup bits (p.223-226) */
	/*    Bit  |   7  |   6  |   5  |   4  |   3  |   2  |   1  |   0  |
		       |RXCIE1|TXCIE1|UDRIE1| RXEN1| TXEN1|UCSZ12| RXB81| TXB81| UCSR1B
	Read/Write |  R/W |  R/W |  R/W |  R/W |  R/W |  R/W |  R/W |  R/W |

	Bit 7 - RXCIE1: RX Complete Interrupt Enable
	Bit 6 - TXCIE1: TX Complete Interrupt Enable
	Bit 5 - UDRIE1: UART Data Register Empty Interrupt Enable
	Bit 4 - RXEN1: Receiver Enable
	Bit 3 - TXEN1: Transmitter Enable
	Bit 2 - UCSZ12: Character Size
	Bit 1 - RXB81: Receive Data Bit 8
	Bit 0 - TXB81: Transmit Data Bit 8
	*/
	UCSR1B = 0;
	UCSR1B |= (callback_function_ptr != NULL ? _BV(RXCIE1) : 0) |
			  _BV(RXEN1) |
			  _BV(TXEN1) |
			  (char_size == UART1_9_BIT ? _BV(UCSZ12) : 0);

	/*    Bit  |   7  |   6  |   5  |   4  |   3  |   2  |   1  |   0  |
		       |UMSEL11UMSEL10 UPM11| UPM10| USBS1|UCSZ11|UCSZ10|UCPOL1| UCSR1C
	Read/Write |  R/W |  R/W |  R/W |  R/W |  R/W |  R/W |  R/W |  R/W |

	Bits 7:6 - UMSEL11:0 UART Mode Select
	Bits 5:4 - UPM11:0: Parity Mode
	Bit 3    - USBS1: Stop Bit Select
	Bits 2:1 - UCSZ11:0: Character Size
	Bit 0    - UCPOL1: Clock Polarity
	*/
	/* add "operational_mode" here if support of synchronous
	and/or master SPI is to be implemented */
	UCSR1C = 0;
	UCSR1C |= paraty_mode |
			  stop_bit |
			  _char_size;

	/* storing callback function */
	_callback_function_ptr = callback_function_ptr;

	/* restores the status register */
	SREG = _sreg;
}

/**********************************************************************//**
 * @ingroup uart_pub
 * Sends one char
 *************************************************************************/
void uart1_send_char(unsigned char data) {
	uart1_send_string(&data, 1U);
}

/**********************************************************************//**
 * @ingroup uart_pub
 * Loops through the 'data' string stores it in a local buffer, then sets
 * the Data Register Empty interrupt bit.
 * @note If the data would make the local buffer overrun, the function returns
 *************************************************************************/
void uart1_send_string(unsigned char *data, uint8_t length) {
	if (!(_buffer_length + length > 255)) {
		/* saves the current state of the status register and disables global interrupt */
		uint8_t _sreg = SREG;
		cli();

		/* copying data to local buffer */
		for (uint8_t _i = 0; _i < length; _i++) {
			_buffer_send_data[_buffer_length++] = *data++;
		}

		/* enable Data Register empty interrupt */
		UCSR1B |= _BV(UDRIE1);

		/* restore the status register */
		SREG = _sreg;
	}
	return;
}

/**********************************************************************//**
 * @ingroup uart_priv
 * Interrupt service routine for the UART transmit.
 **************************************************************************/
ISR(USART1_UDRE_vect, ISR_BLOCK) {
	if (_buffer_index < _buffer_length) {
		UDR1 = _buffer_send_data[_buffer_index++];
	} else {
		_buffer_index = 0;
		_buffer_length = 0;
		UCSR1B &= ~_BV(UDRIE1);
		/* releasing the scheduler */
		scheduler_release();
	}
}

/**********************************************************************//**
 * @ingroup uart_priv
 * Interrupt service routine for the UART. If a callback function pointer is
 * supplied when setting up the UART, a callback to that function is being performed
 **************************************************************************/
ISR(USART1_RX_vect, ISR_BLOCK) {
	unsigned char received_data = UDR1;
	if (_callback_function_ptr != NULL) {
		_callback_function_ptr(received_data);
	}
}
