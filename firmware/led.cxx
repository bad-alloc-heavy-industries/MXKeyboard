// SPDX-License-Identifier: BSD-3-Clause
#include <cstddef>
#include <array>
#include <avr/cpufunc.h>
#include "MXKeyboard.hxx"
#include "led.hxx"
#include "interrupts.hxx"

constexpr static inline std::byte operator ""_b(const unsigned long long value) noexcept
	{ return static_cast<std::byte>(value); }

constexpr std::size_t toNearestWholeChip(const std::size_t ledCount)
{
	const auto chips{(ledCount / 24) + (ledCount % 24 ? 1 : 0)};
	return chips * 36;
}

static_assert(toNearestWholeChip(1) == 36);

struct ledData_t
{
	std::array<std::byte, toNearestWholeChip(109)> red{};
	std::array<std::byte, toNearestWholeChip(109)> green{};
	std::array<std::byte, toNearestWholeChip(109)> blue{};

	void colour(std::size_t led, uint8_t r, uint8_t g, uint8_t b) noexcept;
};

constexpr static const std::array<uint16_t, 256> gammaLUT
{
	0x000, 0x00A, 0x014, 0x01E, 0x028, 0x033, 0x03D, 0x047,
	0x051, 0x05C, 0x066, 0x071, 0x07B, 0x086, 0x090, 0x09B,
	0x0A5, 0x0B0, 0x0BA, 0x0C5, 0x0D0, 0x0DB, 0x0E5, 0x0F0,
	0x0FB, 0x106, 0x111, 0x11C, 0x127, 0x132, 0x13D, 0x148,
	0x154, 0x15F, 0x16A, 0x175, 0x181, 0x18C, 0x198, 0x1A3,
	0x1AF, 0x1BA, 0x1C6, 0x1D1, 0x1DD, 0x1E9, 0x1F4, 0x200,
	0x20C, 0x218, 0x224, 0x230, 0x23C, 0x248, 0x254, 0x260,
	0x26C, 0x278, 0x284, 0x291, 0x29D, 0x2A9, 0x2B6, 0x2C2,
	0x2CF, 0x2DB, 0x2E8, 0x2F4, 0x301, 0x30E, 0x31B, 0x327,
	0x334, 0x341, 0x34E, 0x35B, 0x368, 0x375, 0x382, 0x38F,
	0x39D, 0x3AA, 0x3B7, 0x3C4, 0x3D2, 0x3DF, 0x3ED, 0x3FA,
	0x408, 0x415, 0x423, 0x431, 0x43E, 0x44C, 0x45A, 0x468,
	0x476, 0x484, 0x492, 0x4A0, 0x4AE, 0x4BC, 0x4CB, 0x4D9,
	0x4E7, 0x4F6, 0x504, 0x512, 0x521, 0x530, 0x53E, 0x54D,
	0x55C, 0x56A, 0x579, 0x588, 0x597, 0x5A6, 0x5B5, 0x5C4,
	0x5D3, 0x5E2, 0x5F2, 0x601, 0x610, 0x620, 0x62F, 0x63F,
	0x64E, 0x65E, 0x66D, 0x67D, 0x68D, 0x69D, 0x6AD, 0x6BD,
	0x6CD, 0x6DD, 0x6ED, 0x6FD, 0x70D, 0x71D, 0x72E, 0x73E,
	0x74E, 0x75F, 0x770, 0x780, 0x791, 0x7A2, 0x7B2, 0x7C3,
	0x7D4, 0x7E5, 0x7F6, 0x807, 0x818, 0x829, 0x83B, 0x84C,
	0x85D, 0x86F, 0x880, 0x892, 0x8A3, 0x8B5, 0x8C7, 0x8D9,
	0x8EA, 0x8FC, 0x90E, 0x920, 0x932, 0x945, 0x957, 0x969,
	0x97B, 0x98E, 0x9A0, 0x9B3, 0x9C5, 0x9D8, 0x9EB, 0x9FE,
	0xA10, 0xA23, 0xA36, 0xA49, 0xA5C, 0xA70, 0xA83, 0xA96,
	0xAAA, 0xABD, 0xAD1, 0xAE4, 0xAF8, 0xB0C, 0xB1F, 0xB33,
	0xB47, 0xB5B, 0xB6F, 0xB83, 0xB97, 0xBAC, 0xBC0, 0xBD4,
	0xBE9, 0xBFD, 0xC12, 0xC27, 0xC3C, 0xC50, 0xC65, 0xC7A,
	0xC8F, 0xCA4, 0xCBA, 0xCCF, 0xCE4, 0xCFA, 0xD0F, 0xD25,
	0xD3A, 0xD50, 0xD66, 0xD7C, 0xD92, 0xDA8, 0xDBE, 0xDD4,
	0xDEA, 0xE00, 0xE17, 0xE2D, 0xE44, 0xE5A, 0xE71, 0xE88,
	0xE9F, 0xEB6, 0xECD, 0xEE4, 0xEFB, 0xF12, 0xF29, 0xF41,
	0xF58, 0xF70, 0xF88, 0xF9F, 0xFB7, 0xFCF, 0xFE7, 0xFFF
};

enum class channel_t { red, green, blue };

static ledData_t leds;

USART_t &ledChannelToUART(const channel_t channel)
{
	if (channel == channel_t::red)
		return USARTC0;
	else if (channel == channel_t::green)
		return USARTD0;
	else if (channel == channel_t::blue)
		return USARTC1;
	return USARTD1; // This is invalid but harmless.
}

/*
 * PE4 = Blank
 * PE5 = XLat
 */

void ledInit()
{
	// Set PE4 and 5 to output and get blanking happening ASAP
	PORTE.OUTCLR = 0x20;
	PORTE.DIRSET = 0x30;
	PORTE.OUTSET = 0x10;
	uartInit();
	timerInit(TCC0);
	dmaInit(DMA.CH0, DMA_CH_TRIGSRC_USARTC0_DRE_gc);
	dmaInit(DMA.CH1, DMA_CH_TRIGSRC_USARTD0_DRE_gc);
	dmaInit(DMA.CH2, DMA_CH_TRIGSRC_USARTC1_DRE_gc);
	dmaTransferLength(DMA.CH0, leds.red.size());
	dmaTransferSource(DMA.CH0, leds.red.data());
	dmaTransferDest(DMA.CH0, &ledChannelToUART(channel_t::red).DATA);
	dmaTransferLength(DMA.CH1, leds.green.size());
	dmaTransferSource(DMA.CH1, leds.green.data());
	dmaTransferDest(DMA.CH1, &ledChannelToUART(channel_t::green).DATA);
	dmaTransferLength(DMA.CH2, leds.blue.size());
	dmaTransferSource(DMA.CH2, leds.blue.data());
	dmaTransferDest(DMA.CH2, &ledChannelToUART(channel_t::blue).DATA);
	dmaInterruptEnable(DMA.CH2);
}

/*
 * LED data array:
 * [ 00 ], [ 00 ], [ 00 ]
 *
 * led = 0, correctedR = 0xDA8:
 * [ DA ], [ 80 ], [ 00 ]
 *
 * led = 1, correctedR = 0xDA8:
 * [ 00 ], [ 0D ], [ A8 ]
 */

void ledData_t::colour(const std::size_t led, const uint8_t r, const uint8_t g, const uint8_t b) noexcept
{
	const auto correctedR{gammaLUT[r]};
	const auto correctedG{gammaLUT[g]};
	const auto correctedB{gammaLUT[b]};

	const auto offsetLed{led >= 96 ? led + 12 : led};
	const auto startBit{offsetLed * 12U};
	const auto startByte{startBit / 8U};

	if (led & 1U)
	{
		red[startByte] = (red[startByte] & 0xF0_b) | std::byte((correctedR >> 8U) & 0x0FU);
		green[startByte] = (green[startByte] & 0xF0_b) | std::byte((correctedG >> 8U) & 0x0FU);
		blue[startByte] = (blue[startByte] & 0xF0_b) | std::byte((correctedB >> 8U) & 0x0FU);
		red[startByte + 1] = std::byte(correctedR);
		green[startByte + 1] = std::byte(correctedG);
		blue[startByte + 1] = std::byte(correctedB);
	}
	else
	{
		red[startByte] = std::byte(correctedR >> 4U);
		green[startByte] = std::byte(correctedG >> 4U);
		blue[startByte] = std::byte(correctedB >> 4U);
		red[startByte + 1] = (red[startByte + 1] & 0x0F_b) | std::byte((correctedR << 4U) & 0xF0U);
		green[startByte + 1] = (green[startByte + 1] & 0x0F_b) | std::byte((correctedG << 4U) & 0xF0U);
		blue[startByte + 1] = (blue[startByte + 1] & 0x0F_b) | std::byte((correctedB << 4U) & 0xF0U);
	}
}

void ledSetValue(const uint8_t led, const uint8_t r, const uint8_t g, const uint8_t b)
	{ leds.colour(led, r, g, b); }

void ledLatch()
{
	PORTE.OUTSET = 0x30;
	PORTE.OUTCLR = 0x20;
	PORTE.OUTCLR = 0x10;
}

enum class rgbState_t : uint8_t
{
	none, red, purple, blue
};
static rgbState_t currentState{rgbState_t::none};
static uint8_t redValue{0};
static uint8_t blueValue{0};

void nextRGBValue() noexcept
{
	switch (currentState)
	{
		case rgbState_t::none:
			++redValue;
			if (redValue == 255U)
				currentState = rgbState_t::red;
			break;
		case rgbState_t::red:
			++blueValue;
			if (blueValue == 255U)
				currentState = rgbState_t::purple;
			break;
		case rgbState_t::purple:
			--redValue;
			if (!redValue)
				currentState = rgbState_t::blue;
			break;
		case rgbState_t::blue:
			--blueValue;
			if (!blueValue)
				currentState = rgbState_t::none;
			break;
	}
}

void tcc0OverflowIRQ()
{
	for (uint8_t i{0}; i < 109; ++i)
		//leds.colour(i, 127, 7, 63);
		leds.colour(i, redValue, 7, blueValue);
	nextRGBValue();

#if 0
	dmaTrigger(DMA.CH0);
	dmaTrigger(DMA.CH1);
	dmaTrigger(DMA.CH2);
#else
	for (uint8_t i{0}; i < 180; ++i)
	{
		uartWrite(ledChannelToUART(channel_t::red), static_cast<uint8_t>(leds.red[i]));
		uartWrite(ledChannelToUART(channel_t::green), static_cast<uint8_t>(leds.green[i]));
		uartWrite(ledChannelToUART(channel_t::blue), static_cast<uint8_t>(leds.blue[i]));
	}
	uartWaitTXComplete(ledChannelToUART(channel_t::red));
	uartWaitTXComplete(ledChannelToUART(channel_t::green));
	uartWaitTXComplete(ledChannelToUART(channel_t::blue));
	ledLatch();
#endif
}

void dmaChannel2IRQ() { ledLatch(); }
