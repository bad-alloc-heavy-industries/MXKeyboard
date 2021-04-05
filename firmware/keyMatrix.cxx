// SPDX-License-Identifier: BSD-3-Clause
#include <cstdint>
#include <array>
#include <avr/cpufunc.h>
#include "MXKeyboard.hxx"
#include "interrupts.hxx"
#include "keyMatrix.hxx"
#include "mask.hxx"
#include "indexedIterator.hxx"
#include "led.hxx"

/*!
 * The column signals are on Port A, bits 0-4
 * The row signals are on Port F, bits 0-5
 */

constexpr static const auto columnMask{genMask<std::uint8_t, 0U, 5U>()};
constexpr static const auto rowMask{genMask<std::uint8_t, 0U, 6U>()};

std::array<keyState_t, keyCount> keyStates{};

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

	TCD0.CTRLA = TC_CLKSEL_DIV4_gc; // Use fPER/4
	TCD0.CTRLB = TC_WGMODE_NORMAL_gc;
	TCD0.CTRLE = TC_BYTEM_NORMAL_gc;
	TCD0.INTCTRLA = TC_OVFINTLVL_HI_gc;
	TCD0.CTRLFCLR = 0x0FU;
	TCD0.CTRLFSET = TC_CMD_UPDATE_gc;
	TCD0.PERBUF = 10000; // 1MHz / 1kHz = 5000
	TCD0.CNT = 0;

	// Pull the initial key state information from flash
	for (const auto &[i, keyState] : utility::indexedIterator_t{keyStates})
	{
		const key_t key = keys[i];
		keyState->ledIndex = key.ledIndex;
	}
}

void keyIRQ() noexcept
{
	for (uint8_t column{0}; column < 21; ++column)
	{
		PORTA.OUT = column;
		// This waits for the propergation delays in the 3-to-8 decoders so the PORTF read is valid
		for (volatile uint8_t wait{0}; wait < 2; ++wait)
			continue;
		const auto pressStates = PORTF.IN;
		for (uint8_t row{0}; row < 6; ++row)
		{
			const bool keyPressed = (pressStates >> row) & 1U;
			const auto &key = keyStates[(column * 6) + row];
			if (key.ledIndex == 255)
				continue;

			if (keyPressed)
				ledSetValue(key.ledIndex, 0x00, 0xFF, 0x00);
			else
				ledSetValue(key.ledIndex, 0x1F, 0x1F, 0xFF);
		}
	}
}
