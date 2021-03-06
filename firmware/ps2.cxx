// SPDX-License-Identifier: BSD-3-Clause
#include "MXKeyboard.hxx"
#include "ps2.hxx"
#include "interrupts.hxx"

/*!
 * PS2_CLK = PE1
 * PS2_RX = PE2
 * PS2_TX = PE3
 *
 * USARTE0 (PE1-3)
 *
 * ClkPer is 16MHz, PS/2's max clock rate is 10kHz.
 * The ratio between these is 1600, which factoring
 * in the in-built div2 gives a BSEL of 800 - 1
 */

constexpr inline uint8_t highByte(uint16_t value) noexcept { return uint8_t(value >> 8U); }
constexpr inline uint8_t lowByte(uint16_t value) noexcept { return uint8_t(value); }

constexpr static uint8_t PORT_INVEN_gc{0x40U};
constexpr static uint8_t USART_SBMODE_1BIT_gc{0x00U};
constexpr static uint8_t USART_SBMODE_2BIT_gc{0x08U};

void ps2Init() noexcept
{
	// Set up clock + data as "Wired AND w/ pull-up" which is OC+pull
	// We also want the clock pin to react to being pulled low.
	PORTE.PIN1CTRL = PORT_OPC_WIREDANDPULL_gc | PORT_ISC_LEVEL_gc;
	PORTE.PIN3CTRL = PORT_OPC_WIREDANDPULL_gc;
	// Make PE1 and 3 high outputs
	PORTE.OUTSET = 0x0AU;
	PORTE.DIRSET = 0x0AU;
	// Configure the USART dealing with PS2 into 8-bit w/ parity mode, and make it clock-synchronous.
	USARTE0.CTRLC = USART_CMODE_SYNCHRONOUS_gc | USART_CHSIZE_8BIT_gc | USART_SBMODE_1BIT_gc | USART_PMODE_ODD_gc;
	USARTE0.BAUDCTRLB = highByte(799);
	USARTE0.BAUDCTRLA = lowByte(799);
	// Enable RX & TX
	USARTE0.CTRLB = 0x18U;
	// Enable PE1 as INT0IF
	PORTE.INT0MASK = 0x02U;
	PORTE.INTCTRL = uint8_t((PORTE.INTCTRL & 0xFCU) | PORT_INT0LVL_MED_gc);
	// TODO: Check with a scope.
}

[[nodiscard]] uint8_t ps2Read() noexcept
{
	// Reconfigure to send just the clock pulses
	PORTE.DIRCLR = 0x08U;
	USARTE0.DATA = 0;
	// Wait for the "transmission" to complete
	while (!(USARTE0.STATUS & 0x40U))
		continue;
	// Clear the flag and re-enable the TX pin
	USARTE0.STATUS |= 0x40U;
	PORTE.DIRSET = 0x08U;
	const auto data = USARTE0.DATA;
	// TODO: Generate ACK pulses.
	// Send the data pin low, wait a half a clock cycle
	PORTE.OUTSET = 0x02U;
	// Send the clock pin low, wait a half a clock cycle
	// Send both pins back high.
	return data;
}

void ps2Write(const uint8_t data) noexcept
{
}

void ps2IRQ() noexcept
{
	PORTE.DIRSET &= ~0x08;
	while (PORTE.IN & 0x02)
		continue;
	ps2Read();
}
