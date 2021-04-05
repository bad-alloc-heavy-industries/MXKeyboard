// SPDX-License-Identifier: BSD-3-Clause
#include <cstdint>
#include <avr/cpufunc.h>
#include "MXKeyboard.hxx"
#include "interrupts.hxx"
#include "mask.hxx"

/*!
 * The column signals are on Port A, bits 0-4
 * The row signals are on Port F, bits 0-5
 */

constexpr static const auto columnMask{genMask<std::uint8_t, 0U, 5U>()};
constexpr static const auto rowMask{genMask<std::uint8_t, 0U, 6U>()};
uint16_t scanIterations{};

void keyInit() noexcept
{
	// Set up column scan
	PORTCFG.MPCMASK = columnMask;
	// Set all column pins to be totem-poll outputs in one shot.
	PORTA.PIN0CTRL = PORT_OPC_TOTEM_gc;
	PORTA.OUTCLR = columnMask;
	PORTA.DIR = columnMask;

	// Set up row read-back
	PORTCFG.MPCMASK = rowMask;
	// Set all row pins to be totem-poll inputs in one shot
	PORTF.PIN0CTRL = PORT_OPC_TOTEM_gc;
	PORTF.DIRCLR = rowMask;
	PORTF.OUTCLR = ~rowMask;

	TCD0.CTRLA = TC_CLKSEL_DIV1_gc; // Use fPER directly
	TCD0.CTRLB = TC_WGMODE_NORMAL_gc;
	TCD0.CTRLE = TC_BYTEM_NORMAL_gc;
	TCD0.INTCTRLA = TC_OVFINTLVL_HI_gc;
	TCD0.CTRLFCLR = 0x0FU;
	TCD0.CTRLFSET = TC_CMD_UPDATE_gc;
	TCD0.PERBUF = 20000; // 2MHz / 1kHz = 20000
	TCD0.CNT = 0;

	scanIterations = 0;
}

void keyIRQ() noexcept
{
	if (!scanIterations)
	{
		for (uint8_t i{0}; i < 21; ++i)
		{
			PORTA.OUT = i;
			_NOP();
			_NOP();
			_NOP();
			_NOP();
			_NOP();
			_NOP();
			_NOP();
			_NOP();
			_NOP();
			_NOP();
			_NOP();
			_NOP();
			_NOP();
			_NOP();
			_NOP();
			_NOP();
			_NOP();
			_NOP();
			_NOP();
			_NOP();
			_NOP();
			_NOP();
			_NOP();
			_NOP();
			_NOP();
			_NOP();
			_NOP();
			_NOP();
		}
	}
	++scanIterations;
	if (scanIterations >= 1000)
		scanIterations = 0;
}
