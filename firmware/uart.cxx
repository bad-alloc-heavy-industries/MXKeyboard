// SPDX-License-Identifier: BSD-3-Clause
#include <avr/builtins.h>
#include "MXKeyboard.hxx"
#include "uart.hxx"

/*!
 * SCLK_GREEN = PC1
 * SCLK_BLUE = PC5
 * SCLK_RED = PD1
 *
 * COPI_GREEN = PC3
 * COPI_BLUE = PC7
 * COPI_RED = PD3
 *
 * DBG_RX = PE6
 * DBG_TX = PE7
 *
 * USARTC0 (PC1 & 3)
 * USARTC1 (PC5 & 7)
 * USARTD0 (PD1 & 3)
 * USARTE1 (PE6 & 7)
 */

enum class usartMode_t
{
	UART,
	USRT,
	IrDA,
	MSPI
};

template<usartMode_t mode = usartMode_t::MSPI> constexpr void uartInitOne(USART_t &usart) noexcept
{
	if constexpr (mode == usartMode_t::MSPI)
	{
		// UDORD is MSB first, UCPHA is leading edge
		usart.CTRLC = USART_CMODE_MSPI_gc | USART_PMODE_DISABLED_gc |
			USART_CHSIZE_8BIT_gc;
		// Enable TX
		usart.CTRLB = 0x08;
		// Clear both BSEL and BSCALE to get as close as possible to no clock division
		usart.BAUDCTRLA = 0;
		usart.BAUDCTRLB = 0;
	}
	else if (mode == usartMode_t::UART)
	{
		// Set the USART into async mode, 8n1 w/ no parity and 1 stop bit.
		usart.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc |
			USART_CHSIZE_8BIT_gc;
		// Enable TX, RX, and CLX2X
		usart.CTRLB = 0x1C;
		// Clear both BSEL and BSCALE to get as close as possible to no clock division
		// As Fper is 16MHz, this puts us at 2Baud having enabled CLX2X on this peripheral.
		usart.BAUDCTRLA = 0;
		usart.BAUDCTRLB = 0;
	}
}

void uartInit() noexcept
{
	uartInitOne(USARTC0);
	uartInitOne(USARTC1);
	uartInitOne(USARTD0);
	uartInitOne<usartMode_t::UART>(USARTE1);
	// Make PC1, 3, 5 and 7 outputs
	PORTC.DIRSET = 0xAA;
	// Make PD 1 & 3 ouputs
	PORTD.DIRSET = 0x0A;
	// Force PE 6 to be an input
	PORTE.DIRCLR = 0x40;
	// Make PE 7 an output
	PORTE.DIRSET = 0x80;
}

void uartWrite(USART_t &uart, const uint8_t data)
{
	while (!(uart.STATUS & 0x20))
		continue;
	uart.DATA = data;
}

void uartWaitTXComplete(USART_t &uart)
{
	while (!(uart.STATUS & 0x40))
		continue;
}
