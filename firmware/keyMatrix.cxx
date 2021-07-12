// SPDX-License-Identifier: BSD-3-Clause
#include <cstdint>
#include <array>
#include <substrate/indexed_iterator>
#include <substrate/index_sequence>
#include <avr/cpufunc.h>
#include "MXKeyboard.hxx"
#include "interrupts.hxx"
#include "keyMatrix.hxx"
#include "mask.hxx"
#include "led.hxx"
#include "profile.hxx"
#include "usb/hid.hxx"

/*!
 * The column signals are on Port A, bits 0-4
 * The row signals are on Port F, bits 0-5
 */

using namespace mxKeyboard::keyMatrix;
using mxKeyboard::profile::profile_t;

constexpr static const auto columnMask{genMask<std::uint8_t, 0U, 5U>()};
constexpr static const auto rowMask{genMask<std::uint8_t, 0U, 6U>()};

static profile_t profile{};
static std::array<keyState_t, keyCount> keyStates{{}};

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

	// Enable normal lds/sts access to the EEPROM
	NVM.CTRLB |= NVM_EEMAPEN_bm;

	profile = profile_t::read(0);
	if (!profile.valid(0))
	{
		profile.clear();
		profile.number(0);
		profile.debounce(1);

		for (const auto &index : substrate::indexSequence_t{keyCount})
		{
			const auto i{static_cast<uint8_t>(index)};
			const key_t key = keys[i];
			profile.keyColour(i, {0x1FU, 0x1FU, 0xFFU});
			profile.timePress(i, 0);
			profile.timePress(i, 0);
			profile.scancode(i, key.usbScancode);

			if (key.usbScancode == usbScancode_t::numLock || key.usbScancode == usbScancode_t::capsLock ||
				key.usbScancode == usbScancode_t::scrollLock)
				profile.keyType(i, keyType_t::latching);
		}
		profile.write();
	}

	// Pull the initial key state information from flash
	for (const auto &[index, keyState] : substrate::indexedIterator_t{keyStates})
	{
		const auto i{static_cast<uint8_t>(index)};
		const key_t key = keys[i];
		keyState.state = {};
		keyState.debounce = profile.debounce();
		keyState.timePress = profile.timePress(i);
		keyState.timeRelease = profile.timeRelease(i);
		keyState.ledIndex = key.ledIndex;
		keyState.ledColour = profile.keyColour(i);
		keyState.usbScancode = profile.scancode(i);
		keyState.state.keyType(profile.keyType(i) ? keyType_t::latching : keyType_t::momentary);

		if (key.ledIndex != 255)
			ledSetValue(key.ledIndex, keyState.ledColour.r, keyState.ledColour.g, keyState.ledColour.b);
	}
}

namespace mxKeyboard::keyMatrix
{
	void updateKey(keyState_t &key)
	{
		if (key.state.logicalState())
		{
			ledSetValue(key.ledIndex, 0x00, 0xFF, 0x00);
			usb::hid::keyPress(key.usbScancode);
		}
		else
		{
			ledSetValue(key.ledIndex, key.ledColour.r, key.ledColour.g, key.ledColour.b);
			usb::hid::keyRelease(key.usbScancode);
		}
	}
}

void keyIRQ() noexcept
{
	for (uint8_t column{0}; column < 21; ++column)
	{
		PORTA.OUT = column;
		// This waits for the propergation delays in the 3-to-8 decoders so the PORTF read is valid
		for (volatile uint8_t wait{0}; wait < 1; ++wait)
			continue;
		const auto pressStates = PORTF.IN;
		for (uint8_t row{0}; row < 6; ++row)
		{
			const bool switchState{(pressStates >> row) & 1U};
			auto &key{keyStates[(column * 6) + row]};
			if (key.ledIndex == 255)
				continue;

			if (key.state.physicalState() == switchState)
			{
				if (key.state.dirty())
				{
					key.debounce = 0;
					key.timePress = 0;
					key.timeRelease = 0;
					key.state.dirty(false);
					updateKey(key);
				}
			}
			else
			{
				uint8_t timerCount{0};
				key.state.dirty(true);

				if (key.debounce)
				{
					--key.debounce;
					continue;
				}
				else if (key.state.keyType() == keyType_t::momentary)
				{
					if (switchState)
						timerCount = key.timePress--;
					else
						timerCount = key.timeRelease--;
				}
				else if (switchState) // keyType_t::latching
				{
					// If the latching key is currently considered pressed
					if (key.state.logicalState())
						timerCount = key.timeRelease--;
					else
						timerCount = key.timePress--;
				}
				// If the timer for the key expired
				if (!timerCount)
				{
					key.state.physicalState(switchState);
					// If the key is momentary, update it with the current real state
					if (key.state.keyType() == keyType_t::momentary)
						key.state.logicalState(switchState);
					// Else invert the logical state as we are completing a key press
					else if (switchState) // keyType_t::latching
						key.state.logicalState(!key.state.logicalState());
				}
			}
		}
	}
	usb::hid::handleReport();
}
