// SPDX-License-Identifier: BSD-3-Clause
#include <cstddef>
#include <array>
#include "MXKeyboard.hxx"
#include "led.hxx"
#include "uart.hxx"
#include "flash.hxx"
#include "interrupts.hxx"

constexpr static inline std::byte operator ""_b(const unsigned long long value) noexcept
	{ return static_cast<std::byte>(value); }

constexpr std::size_t toChips(const std::size_t ledCount) { return (ledCount / 24) + (ledCount % 24 ? 1 : 0); }
constexpr std::size_t toNearestWholeChipBytes(const std::size_t ledCount) { return toChips(ledCount) * 36; }
constexpr std::size_t toNearestWholeChipLEDs(const std::size_t ledCount) { return toChips(ledCount) * 24; }
static_assert(toNearestWholeChipBytes(1) == 36);
static_assert(toNearestWholeChipLEDs(1) == 24);

constexpr static std::size_t ledStringLength{toNearestWholeChipBytes(109)};
constexpr static std::size_t ledStringLEDs{toNearestWholeChipLEDs(109)};

struct ledData_t
{
	std::array<uint8_t, ledStringLength> red{};
	std::array<uint8_t, ledStringLength> green{};
	std::array<uint8_t, ledStringLength> blue{};
	bool setup{false};

	void colour(uint8_t led, uint8_t r, uint8_t g, uint8_t b) noexcept;
};

constexpr static const std::array<flash_t<uint16_t>, 256> gammaLUT
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

constexpr static const std::array<flash_t<uint8_t>, ledStringLEDs> ledIndexMap
{
	23, 22, 21, 20, 19, 18, 17, 16,
	15, 14, 13, 12, 11, 10, 9, 8,
	7, 6, 5, 4, 3, 2, 1, 0,
	47, 46, 45, 44, 43, 42, 41, 40,
	39, 38, 37, 36, 35, 34, 33, 32,
	31, 30, 29, 28, 27, 26, 25, 24,
	71, 70, 69, 68, 67, 66, 65, 64,
	63, 62, 61, 60, 59, 58, 57, 56,
	55, 54, 53, 52, 51, 50, 49, 48,
	95, 94, 93, 92, 91, 90, 89, 88,
	87, 86, 85, 84, 83, 82, 81, 80,
	79, 78, 77, 76, 75, 74, 73, 72,
	119, 118, 117, 116, 115, 114, 113, 112,
	111, 110, 109, 108, 107, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255
};

enum class channel_t { red, green, blue };

static ledData_t leds{};

inline USART_t &ledChannelToUART(const channel_t channel)
{
	if (channel == channel_t::red)
		return USARTD0;
	else if (channel == channel_t::green)
		return USARTC0;
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
	PORTE.OUTSET = 0x10;
	PORTE.DIRSET = 0x30;
	uartInit();
	timerInit(TCC0);
	dmaInit(DMA.CH0, DMA_CH_TRIGSRC_USARTD0_DRE_gc);
	dmaInit(DMA.CH1, DMA_CH_TRIGSRC_USARTC0_DRE_gc);
	dmaInit(DMA.CH2, DMA_CH_TRIGSRC_USARTC1_DRE_gc);
	dmaTransferSource(DMA.CH0, leds.red.data());
	dmaTransferLength(DMA.CH0, leds.red.size());
	dmaTransferDest(DMA.CH0, &ledChannelToUART(channel_t::red).DATA);
	dmaTransferSource(DMA.CH1, leds.green.data());
	dmaTransferLength(DMA.CH1, leds.green.size());
	dmaTransferDest(DMA.CH1, &ledChannelToUART(channel_t::green).DATA);
	dmaTransferSource(DMA.CH2, leds.blue.data());
	dmaTransferLength(DMA.CH2, leds.blue.size());
	dmaTransferDest(DMA.CH2, &ledChannelToUART(channel_t::blue).DATA);
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

constexpr void maskAndCombine(uint8_t &value, const uint8_t mask, const uint8_t comb)
{
	value &= mask;
	value |= comb;
}

void ledData_t::colour(const uint8_t led, const uint8_t r, const uint8_t g, const uint8_t b) noexcept
{
	const uint16_t correctedR{gammaLUT[r]};
	const uint16_t correctedG{gammaLUT[g]};
	const uint16_t correctedB{gammaLUT[b]};

	//const auto offsetLed{led >= 96 ? led + 12 : led};
	//const uint8_t offsetLed{ledIndexMap[led]};
	const auto startBit{led * 12U};
	const auto startByte{startBit / 8U};

	if (led & 1U)
	{
		maskAndCombine(red[startByte], 0xF0U, uint8_t(correctedR >> 8U));
		red[startByte + 1] = uint8_t(correctedR);
		maskAndCombine(green[startByte], 0xF0U, uint8_t(correctedG >> 8U));
		green[startByte + 1] = uint8_t(correctedG);
		maskAndCombine(blue[startByte], 0xF0U, uint8_t(correctedB >> 8U));
		blue[startByte + 1] = uint8_t(correctedB);
	}
	else
	{
		red[startByte] = uint8_t(correctedR >> 4U);
		maskAndCombine(red[startByte + 1], 0x0FU, uint8_t(correctedR << 4U));
		green[startByte] = uint8_t(correctedG >> 4U);
		maskAndCombine(green[startByte + 1], 0x0FU, uint8_t(correctedG << 4U));
		blue[startByte] = uint8_t(correctedB >> 4U);
		maskAndCombine(blue[startByte + 1], 0x0FU, uint8_t(correctedB << 4U));
	}
}

void ledSetValue(const std::size_t led, const uint8_t r, const uint8_t g, const uint8_t b)
	{ leds.colour(ledIndexMap[led], r, g, b); }

void ledLatch()
{
	PORTE.OUTSET = 0x30;
	PORTE.OUTCLR = 0x20;
	PORTE.OUTCLR = 0x10;
}

enum class rgbState_t : uint8_t
{
	red, purple, blue, cyan, green, yellow
};
static rgbState_t currentState{rgbState_t::red};
static uint8_t redValue{255};
static uint8_t greenValue{0};
static uint8_t blueValue{0};

void nextRGBValue() noexcept
{
	switch (currentState)
	{
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
			++greenValue;
			if (greenValue == 255U)
				currentState = rgbState_t::cyan;
			break;
		case rgbState_t::cyan:
			--blueValue;
			if (!blueValue)
				currentState = rgbState_t::green;
			break;
		case rgbState_t::green:
			++redValue;
			if (redValue == 255U)
				currentState = rgbState_t::yellow;
			break;
		case rgbState_t::yellow:
			--greenValue;
			if (!greenValue)
				currentState = rgbState_t::red;
			break;
	}
}

//leds.colour(i, 127, 7, 63);

void tcc0OverflowIRQ()
{
	if (leds.setup)
		ledLatch();
	//for (uint8_t i{0}; i < 109; ++i)
	for (uint8_t i{106}; i < 109; ++i)
		ledSetValue(i, redValue, greenValue, blueValue);
	nextRGBValue();

#if 1
	dmaTrigger(DMA.CH0);
	dmaTrigger(DMA.CH1);
	dmaTrigger(DMA.CH2);
#else
	for (uint8_t i{0}; i < ledStringLength; ++i)
	{
		uartWrite(ledChannelToUART(channel_t::red), static_cast<uint8_t>(leds.red[i]));
		uartWrite(ledChannelToUART(channel_t::green), static_cast<uint8_t>(leds.green[i]));
		uartWrite(ledChannelToUART(channel_t::blue), static_cast<uint8_t>(leds.blue[i]));
	}
#endif
	leds.setup = true;
}

void dmaChannel2IRQ() { ledLatch(); }
