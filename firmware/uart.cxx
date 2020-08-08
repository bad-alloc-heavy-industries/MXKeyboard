// SPDX-License-Identifier: BSD-3-Clause
#include "MXKeyboard.hxx"

/*!
 * SCLK_GREEN = PC1
 * SCLK_BLUE = PC5
 * SCLK_RED = PD1
 *
 * COPI_GREEN = PC3
 * COPI_BLUE = PC7
 * COPI_RED = PD3
 *
 * USARTC0 (PC1 & 3)
 * USARTC1 (PC5 & 7)
 * USARTD0 (PD1 & 3)
 */

void uartInitOne(USART_t &uart)
{
	// UDORD is MSB first, UCPHA is leading edge
	uart.CTRLC = USART_CMODE_MSPI_gc | USART_PMODE_DISABLED_gc |
		USART_CHSIZE_8BIT_gc;
	// Enable TX
	uart.CTRLB = 0x08;
	// Clear both BSEL and BSCALE to get as close as possible to no clock division
	uart.BAUDCTRLA = 0;
	uart.BAUDCTRLB = 0;
}

void uartInit()
{
	uartInitOne(USARTC0);
	uartInitOne(USARTC1);
	uartInitOne(USARTD0);
	// Make PC1, 3, 5 and 7 outputs
	PORTC.DIRSET = 0xAA;
	// Make PD 1 & 3 ouputs
	PORTD.DIRSET = 0x0A;
}
