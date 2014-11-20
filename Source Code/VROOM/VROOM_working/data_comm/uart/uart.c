/**********************************************************************//**
 * @file: UART0_Driver.c
 *
 * @Created: 12-09-2014 20:34:20
 * @Author: Andi Degn
 * @Version: 0.5
 * @defgroup uart UART Driver
 * @{
	 This is a driver for the UART on the ATMEGA family processors
	 @defgroup uart_priv Private
	 @defgroup uart_pub Public
 * @}
 *************************************************************************/

#include "uart.h"
#include <stdlib.h>
#include <util/delay.h>

/**********************************************************************//**
 * @ingroup uart_priv
 * @brief defines for the factor used to calculate baud rate and UBRR0value
 * @{
 *************************************************************************/
#define UBRR_FACTOR_ASYNC_NORM	16
#define UBRR_FACTOR_ASYNC_DOUBLE 8
/* @} */

/**********************************************************************//**
 * @ingroup uart_priv
 * @brief defines for calculating the UBRR0 value
 * @note 0.5F is added to make sure rounding is done rather than truncating
 * @{
 *************************************************************************/
#define CALC_UBRR(baud_rate, factor) (((F_CPU / (factor * baud_rate)) - 1) + 0.5F)
/* @} */

/* local variables for UART0 */
static void (*_callback_function0_ptr)(char __data);
static volatile char _tx_buffer0[UART0_TX_BUFFER_SIZE];
static volatile uint8_t _tx_buffer0_head = 0U;
static volatile uint8_t _tx_buffer0_tail = 0U;
static volatile char _rx_buffer0[UART0_RX_BUFFER_SIZE];
static volatile uint8_t _rx_buffer0_head = 0U;
static volatile uint8_t _rx_buffer0_tail = 0U;

/* local variables for UART0 */
static void (*_callback_function1_ptr)(char __data);
static volatile char _tx_buffer1[UART1_TX_BUFFER_SIZE];
static volatile uint8_t _tx_buffer1_head = 0U;
static volatile uint8_t _tx_buffer1_tail = 0U;
static volatile char _rx_buffer1[UART1_RX_BUFFER_SIZE];
static volatile uint8_t _rx_buffer1_head = 0U;
static volatile uint8_t _rx_buffer1_tail = 0U;

/************************************************************************/
/* UART0 functions                                                      */
/************************************************************************/
/**********************************************************************//**
 * @ingroup uart_pub
 * Takes the supplied UART parameters and sets up the UART accordingly
 **************************************************************************/
void uart0_setup_async(UART_MODE __operational_mode,
						 UART_BAUD __baud_rate,
						 UART_PARITY_MODE __paraty_mode,
						 UART_STOP_BIT __stop_bit,
						 UART_CHAR_SIZE __char_size,
						 void (*__callback_function_ptr)(char __data)) {
	/* saves the current state of the status register and disables global interrupt */
	uint8_t _sreg = SREG;
	cli();

	/* setting the values for the speed of the UART */
	switch (__operational_mode) {
		case UART_MODE_NORMAL:
			UBRR0 = CALC_UBRR(__baud_rate, UBRR_FACTOR_ASYNC_NORM);
			break;

		case UART_MODE_DOUBLE:
			UBRR0 = CALC_UBRR(__baud_rate, UBRR_FACTOR_ASYNC_DOUBLE);
			UCSR0A = _BV(U2X0);
			break;

		default:
			break;
	}

	/* setting up bits for the character size. It is done here as it was
	not possible to	differentiate between UART0_8_BIT and UART0_9_BIT in an enum */
	uint8_t _char_size = 0U;
	switch (__char_size) {
		case UART_6_BIT:
			_char_size = _BV(UCSZ00);
			break;

		case UART_7_BIT:
			_char_size= _BV(UCSZ01);
			break;

		case UART_8_BIT:
		case UART_9_BIT:
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
	UCSR0B |= _BV(RXCIE0) |
			  _BV(RXEN0) |
			  _BV(TXEN0) |
			  (__char_size == UART_9_BIT ? _BV(UCSZ02) : 0);

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
	UCSR0C |= __paraty_mode |
			  __stop_bit |
			  _char_size;

	//UCSR0C |= _BV(UCSZ00) |
			  //_BV(UCSZ01);

	/* storing callback function */
	_callback_function0_ptr = __callback_function_ptr;
	_tx_buffer0_tail = _tx_buffer0_head = _rx_buffer0_tail = _rx_buffer0_head = 0U;

	/* restore the status register */
	SREG = _sreg;
}

/**********************************************************************//**
 * @ingroup uart_pub
 * Sends one char
 *************************************************************************/
void uart0_send_char(char __data) {
	uint8_t tmp = (_tx_buffer0_head + 1) & (UART0_TX_BUFFER_SIZE - 1);

	/* waiting for free space in buffer */
	while (tmp == _tx_buffer0_tail);

	/* copying data to local buffer */
	_tx_buffer0[tmp] = __data;
	_tx_buffer0_head = tmp;

	/* enable Data Register empty interrupt */
	UCSR0B |= _BV(UDRIE0);
}

/**********************************************************************//**
 * @ingroup uart_pub
 * Loops through the 'data' string stores it in a local buffer, then sets
 * the Data Register Empty interrupt bit.
 *************************************************************************/
void uart0_send_string(const char *__data) {
	while (*__data) {
		uart0_send_char(*__data++);
	};
}

/**********************************************************************//**
 * @ingroup uart_pub
 * Loops through the 'data' string stores it in a local buffer, then sets
 * the Data Register Empty interrupt bit.
 * Does not terminates when zero character is meet
 *************************************************************************/
void uart0_send_data(const char *__data, uint8_t __length) {
	while (__length-- > 0) {
		uart0_send_char(*__data++);
	};
}

/**********************************************************************//**
 * @ingroup uart_pub
 * Reads one char
 *************************************************************************/
uint16_t uart0_read_char(void) {
	uint16_t _result;
	if (_callback_function0_ptr != NULL) {
		_result = UART_READ_NOT_ALOWED;
	} else if (_rx_buffer0_head != _rx_buffer0_tail) {
		_result = _rx_buffer0[_rx_buffer0_tail = (_rx_buffer0_tail + 1) % UART0_RX_BUFFER_SIZE];
	} else {
		_result = UART_NO_DATA;
	}
	return _result;
}

/**********************************************************************//**
 * @ingroup uart_priv
 * Interrupt service routine for the UART transmit.
 **************************************************************************/
ISR(USART0_UDRE_vect, ISR_BLOCK) {
	if (_tx_buffer0_head != _tx_buffer0_tail) {
		UDR0 = _tx_buffer0[_tx_buffer0_tail = (_tx_buffer0_tail + 1) % UART0_TX_BUFFER_SIZE];
	} else {
		UCSR0B &= ~_BV(UDRIE0);
	}
}

/**********************************************************************//**
 * @ingroup uart_priv
 * Interrupt service routine for the UART receive. If a callback function pointer is
 * supplied when setting up the UART, a callback to that function is being performed
 **************************************************************************/
ISR(USART0_RX_vect, ISR_BLOCK) {
	char received_data = UDR0;
	if (_callback_function0_ptr != NULL) {
		_callback_function0_ptr(received_data);
	} else {
		if (_rx_buffer0_head + 1 != _rx_buffer0_tail) {
			_rx_buffer0[_rx_buffer0_head = (_rx_buffer0_head + 1) % UART0_RX_BUFFER_SIZE] = received_data;
		}
	}
}


/************************************************************************/
/* UART1 functions                                                      */
/************************************************************************/

/**********************************************************************//**
 * @ingroup uart_pub
 * Takes the supplied UART parameters and sets up the UART accordingly
 **************************************************************************/
void uart1_setup_async(UART_MODE __operational_mode,
						 UART_BAUD __baud_rate,
						 UART_PARITY_MODE __paraty_mode,
						 UART_STOP_BIT __stop_bit,
						 UART_CHAR_SIZE __char_size,
						 void (*__callback_function_ptr)(char __data)) {
	/* saves the current state of the status register and disables global interrupt */
	uint8_t _sreg = SREG;
	cli();

	/* setting the values for the speed of the UART */
	switch (__operational_mode) {
		case UART_MODE_NORMAL:
			UBRR1 = CALC_UBRR(__baud_rate, UBRR_FACTOR_ASYNC_NORM);
			break;

		case UART_MODE_DOUBLE:
			UBRR1 = CALC_UBRR(__baud_rate, UBRR_FACTOR_ASYNC_DOUBLE);
			UCSR1A = _BV(U2X1);
			break;

		default:
			break;
	}

	/* setting up bits for the character size. It is done here as it was
	not possible to	differentiate between UART1_8_BIT and UART1_9_BIT in an enum */
	uint8_t _char_size = 0U;
	switch (__char_size) {
		case UART_6_BIT:
			_char_size = _BV(UCSZ10);
			break;

		case UART_7_BIT:
			_char_size= _BV(UCSZ11);
			break;

		case UART_8_BIT:
		case UART_9_BIT:
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
	UCSR1B |= _BV(RXCIE1) |
			  _BV(RXEN1) |
			  _BV(TXEN1) |
			  (__char_size == UART_9_BIT ? _BV(UCSZ12) : 0);

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
	UCSR1C |= __paraty_mode |
			  __stop_bit |
			  _char_size;

	/* storing callback function */
	_callback_function1_ptr = __callback_function_ptr;
	_tx_buffer1_tail = _tx_buffer1_head = _rx_buffer1_tail = _rx_buffer1_head = 0U;

	/* restores the status register */
	SREG = _sreg;
}

/**********************************************************************//**
 * @ingroup uart_pub
 * Sends one char
 *************************************************************************/
void uart1_send_char(char __data) {
	uint8_t tmp = (_tx_buffer1_head + 1) % UART1_TX_BUFFER_SIZE;

	/* waiting for free space in buffer */
	while (tmp == _tx_buffer1_tail);

	/* copying data to local buffer */
	_tx_buffer1[tmp] = __data;
	_tx_buffer1_head = tmp;

	/* enable Data Register empty interrupt */
	UCSR1B |= _BV(UDRIE1);
}

/**********************************************************************//**
 * @ingroup uart_pub
 * Loops through the 'data' string stores it in a local buffer, then sets
 * the Data Register Empty interrupt bit.
 *************************************************************************/
void uart1_send_string(const char *__data) {
	while (*__data) {
		uart1_send_char(*__data++);
	}
}

/**********************************************************************//**
 * @ingroup uart_pub
 * Loops through the 'data' string stores it in a local buffer, then sets
 * the Data Register Empty interrupt bit.
 * Does not terminates when zero character is meet
 *************************************************************************/
void uart1_send_data(const char *__data, uint8_t __length) {
	while (__length-- > 0) {
		uart1_send_char(*__data++);
	};
}

/**********************************************************************//**
 * @ingroup uart_pub
 * Reads one char
 *************************************************************************/
uint16_t uart1_read_char(void) {
	uint16_t _result;
	if (_callback_function1_ptr != NULL) {
		_result = UART_READ_NOT_ALOWED;
	} else if (_rx_buffer1_head != _rx_buffer1_tail) {
		_result = _rx_buffer1[_rx_buffer1_tail = (_rx_buffer1_tail + 1) % UART1_RX_BUFFER_SIZE];
	} else {
		_result = UART_NO_DATA;
	}
	return _result;
}

/**********************************************************************//**
 * @ingroup uart_priv
 * Interrupt service routine for the UART transmit.
 **************************************************************************/
ISR(USART1_UDRE_vect, ISR_BLOCK) {
	if (_tx_buffer1_head != _tx_buffer1_tail) {
		UDR1 = _tx_buffer1[_tx_buffer1_tail = (_tx_buffer1_tail + 1) % UART1_TX_BUFFER_SIZE];
	} else {
		UCSR1B &= ~_BV(UDRIE1);
	}
}

/**********************************************************************//**
 * @ingroup uart_priv
 * Interrupt service routine for the UART. If a callback function pointer is
 * supplied when setting up the UART, a callback to that function is being performed
 **************************************************************************/
ISR(USART1_RX_vect, ISR_BLOCK) {
	char received_data = UDR1;
	if (_callback_function1_ptr != NULL) {
		_callback_function1_ptr(received_data);
	} else {
		if (_rx_buffer1_head + 1 != _rx_buffer1_tail) {
			_rx_buffer1[_rx_buffer1_head = (_rx_buffer1_head + 1) % UART1_RX_BUFFER_SIZE] = received_data;
		}
	}
}