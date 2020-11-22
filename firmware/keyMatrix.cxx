// SPDX-License-Identifier: BSD-3-Clause
#include <cstdint>
#include "MXKeyboard.hxx"
#include "mask.hxx"

/*!
 * The column signals are on Port A, bits 0-4
 * The row signals are on Port F, bits 0-5
 */

constexpr static const auto columnMask{genMask<std::uint8_t, 0U, 5U>()};

void keyInit() noexcept
{
	// Set up column scan
	PORTA.OUTCLR = columnMask;
	PORTA.DIR = columnMask;

	PORTA.PIN0CTRL = PORT_OPC_TOTEM_gc;
	PORTA.PIN1CTRL = PORT_OPC_TOTEM_gc;
	PORTA.PIN2CTRL = PORT_OPC_TOTEM_gc;
	PORTA.PIN3CTRL = PORT_OPC_TOTEM_gc;
	PORTA.PIN4CTRL = PORT_OPC_TOTEM_gc;

	// Set up row read-back
	PORTF.DIRCLR = 0xFFU;

	//PORTA.OUT = 2;
}
