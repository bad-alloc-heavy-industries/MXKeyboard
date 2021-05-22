// SPDX-License-Identifier: BSD-3-Clause
#ifndef KEY_MATRIX__HXX
#define KEY_MATRIX__HXX

#include <array>
#include "flash.hxx"

namespace mxKeyboard::keyMatrix
{
	constexpr static size_t keyCount{126};

	enum struct usbScancode_t : uint8_t
	{
		reserved = 0x00U,
		errorRollOver = 0x01U,
		errorPOSTFail = 0x02U,
		errorUndefined = 0x03U,
		a = 0x04U, b = 0x05U, c = 0x06U, d = 0x07U,
		e = 0x08U, f = 0x09U, g = 0x0AU, h = 0x0BU,
		i = 0x0CU, j = 0x0DU, k = 0x0EU, l = 0x0FU,
		m = 0x10U, n = 0x11U, o = 0x12U, p = 0x13U,
		q = 0x14U, r = 0x15U, s = 0x16U, t = 0x17U,
		u = 0x18U, v = 0x19U, w = 0x1AU, x = 0x1BU,
		y = 0x1CU, z = 0x1DU, _1 = 0x1EU, _2 = 0x1F,
		_3 = 0x20U, _4 = 0x21U, _5 = 0x22U, _6 = 0x23U,
		_7 = 0x24U, _8 = 0x25U, _9 = 0x26U, _0 = 0x27U,
		enter = 0x28U,
		escape = 0x29U,
		backspace = 0x2AU,
		tab = 0x2BU,
		space = 0x2CU,
		dash = 0x2DU,
		equals = 0x2EU,
		leftBracket = 0x2FU,
		rightBracket = 0x30U,
		usBackSlash = 0x31U,
		hash = 0x32U,
		semiColon = 0x33U,
		singleQuote = 0x34U,
		graveAccent = 0x35U,
		comma = 0x36U,
		fullStop = 0x37U,
		forwardSlash = 0x38U,
		capsLock = 0x39U,
		f1 = 0x3AU, f2 = 0x3BU, f3 = 0x3CU, f4 = 0x3DU,
		f5 = 0x3EU, f6 = 0x3FU, f7 = 0x40U, f8 = 0x41U,
		f9 = 0x42U, f10 = 0x43U, f11 = 0x44U, f12 = 0x45U,
		printScreen = 0x46U,
		scrollLock = 0x47U,
		pause = 0x48U,
		insert = 0x49U,
		home = 0x4AU,
		pageUp = 0x4BU,
		_delete = 0x4CU, //deleteForward?
		end = 0x4DU,
		pageDown = 0x4EU,
		rightArrow = 0x4FU,
		leftArrow = 0x50U,
		downArrow = 0x51U,
		upArrow = 0x52U,
		numLock = 0x53U,
		keypadDivide = 0x54U,
		keypadMultiply = 0x55U,
		keypadSubtract = 0x56U,
		keypadAdd = 0x57U,
		keypadEnter = 0x58U,
		keypad1 = 0x59U, keypad2 = 0x5AU, keypad3 = 0x5BU, keypad4 = 0x5CU,
		keypad5 = 0x5DU, keypad6 = 0x5EU, keypad7 = 0x5FU, keypad8 = 0x60U,
		keypad9 = 0x61U, keypad0 = 0x62U,
		keypadPeriod = 0x63U,
		intlBackSlash = 0x64U,
		application = 0x65U,
		power = 0x66U,
		keypadEquals = 0x67U,
		f13 = 0x68U, f14 = 0x69U, f15 = 0x6AU, f16 = 0x6BU,
		f17 = 0x6CU, f18 = 0x6DU, f19 = 0x6EU, f20 = 0x6FU,
		f21 = 0x70U, f22 = 0x71U, f23 = 0x72U, f24 = 0x73U,
		execute = 0x74U,
		help = 0x75U,
		menu = 0x76U,
		select = 0x77U,
		stop = 0x78U,
		again = 0x79U,
		undo = 0x7AU,
		cut = 0x7BU,
		copy = 0x7CU,
		paste = 0x7DU,
		find = 0x7EU,
		mute = 0x7FU,
		volumeUp = 0x80U,
		volumeDown = 0x81U,
		lockingCapsLock = 0x82U,
		lockingNumLock = 0x83U,
		lockingScrollLock = 0x84U,
		keypadComma = 0x85U,
		//keypadEqualsSign?

		sysReq = 0x9AU,
		cancel = 0x9BU,
		clear = 0x9CU,
		prior = 0x9DU,
		_return = 0x9EU,
		separator = 0x9FU,
		_out = 0xA0U,
		oper = 0xA1U,
		clearAgain = 0xA2U,
		crSel = 0xA3U,
		exSel = 0xA4U,

		keypad00 = 0xB0U,
		keypad000 = 0xB1U,
		thousandsSeparator = 0xB2U,
		decimalSeparator = 0xB3U,
		currencyUnit = 0xB4U,
		currencySubUnit = 0xB5U,
		keypadLeftPeren = 0xB6U,
		keypadRightPeren = 0xB7U,
		keypadLeftBrace = 0xB8U,
		keypadRightBrace = 0xB9U,
		keypadTab = 0xBAU,
		keypadBackspace = 0xBBU,
		keypadA = 0xBCU, keypadB = 0xBDU,
		keypadC = 0xBEU, keypadD = 0xBFU,
		keypadE = 0xC0U, keypadF = 0xC1U,
		keypadXOR = 0xC2U,
		keypadCircumflex = 0xC3U,
		keypadPercent = 0xC4U,
		keypadLessThan = 0xC5U,
		keypadGreaterThan = 0xC6U,
		keypadAmpersand = 0xC7U,
		keypadDoubleAmpersand = 0xC8U,
		keypadPipe = 0xC9U,
		keypadDoublePipe = 0xCAU,
		keypadColon = 0xCBU,
		keypadHash = 0xCCU,
		keypadSpace = 0xCDU,
		keypadAtSign = 0xCEU,
		keypadExclamation = 0xCFU,
		keypadMemoryStore = 0xD0U,
		keypadMemoryRecall = 0xD1U,
		keypadMemoryClear = 0xD2U,
		keypadMemoryAdd = 0xD3U,
		keypadMemorySubtract = 0xD4U,
		keypadMemoryMultiply = 0xD5U,
		keypadMemoryDivide = 0xD6U,
		keypadPlusMinus = 0xD7U,
		keypadClear = 0xD8U,
		keypadClearEntry = 0xD9U,
		keypadBinary = 0xDAU,
		keypadOctal = 0xDBU,
		keypadDecimal = 0xDCU,
		keypadHexadecimal = 0xDDU,
		leftControl = 0xE0U,
		leftShift = 0xE1U,
		leftAlt = 0xE2U,
		leftMeta = 0xE3U, // Spec calls this "GUI"
		rightControl = 0xE4U,
		rightShift = 0xE5U,
		rightAlt = 0xE6U,
		rightMeta = 0xE7U, // Spec call this "GUI"
	};

	struct key_t final
	{
		uint8_t ledIndex;
		usbScancode_t usbScancode;
	};

	enum struct keyType_t : uint8_t
	{
		momentary = 0x00U,
		latching = 0x08U
	};

	struct state_t final
	{
	private:
		uint8_t value{0};

	public:
		constexpr state_t() noexcept = default;

		bool physicalState() const noexcept { return value & 0x01U; }
		void physicalState(const bool state) noexcept
		{
			value &= 0xFEU;
			value |= state ? 0x01U : 0x00U;
		}

		bool logicalState() const noexcept { return value & 0x02U; }
		void logicalState(const bool state) noexcept
		{
			value &= 0xFDU;
			value |= state ? 0x02U : 0x00U;
		}

		bool dirty() const noexcept { return value & 0x04U; }
		void dirty(const bool state) noexcept
		{
			value &= 0xFBU;
			value |= state ? 0x04U : 0x00U;
		}

		keyType_t keyType() const noexcept { return static_cast<keyType_t>(value & 0x08U); }
		void keyType(const keyType_t type) noexcept
		{
			value &= 0xF7;
			value |= static_cast<uint8_t>(type);
		}
	};

	struct keyState_t final
	{
		state_t state{};
		uint8_t debounce{0};
		uint8_t timePress{0};
		uint8_t timeRelease{0};
		uint8_t ledIndex{255};
		usbScancode_t usbScancode{0};
	};

	const std::array<flash_t<key_t>, keyCount> keys
	{{
		{{0, usbScancode_t::escape}},
		{{1, usbScancode_t::graveAccent}}, // Actually backtick..
		{{2, usbScancode_t::tab}},
		{{3, usbScancode_t::capsLock}},
		{{4, usbScancode_t::leftShift}},
		{{5, usbScancode_t::leftControl}},

		{{6, usbScancode_t::f1}},
		{{7, usbScancode_t::_1}},
		{{8, usbScancode_t::q}},
		{{9, usbScancode_t::a}},
		{{10, usbScancode_t::intlBackSlash}},
		{{11, usbScancode_t::leftMeta}},

		{{12, usbScancode_t::f2}},
		{{13, usbScancode_t::_2}},
		{{14, usbScancode_t::u}},
		{{15, usbScancode_t::s}},
		{{16, usbScancode_t::z}},
		{{17, usbScancode_t::leftAlt}},

		{{18, usbScancode_t::f3}},
		{{19, usbScancode_t::_3}},
		{{20, usbScancode_t::e}},
		{{21, usbScancode_t::d}},
		{{22, usbScancode_t::x}},
		{{255, usbScancode_t::reserved}},

		{{23, usbScancode_t::f4}},
		{{24, usbScancode_t::_4}},
		{{25, usbScancode_t::r}},
		{{26, usbScancode_t::f}},
		{{27, usbScancode_t::c}},
		{{255, usbScancode_t::reserved}},

		{{28, usbScancode_t::f5}},
		{{29, usbScancode_t::_5}},
		{{30, usbScancode_t::t}},
		{{31, usbScancode_t::g}},
		{{32, usbScancode_t::v}},
		{{255, usbScancode_t::reserved}},

		{{33, usbScancode_t::f6}},
		{{34, usbScancode_t::_6}},
		{{35, usbScancode_t::y}},
		{{36, usbScancode_t::h}},
		{{37, usbScancode_t::b}},
		{{38, usbScancode_t::space}},

		{{39, usbScancode_t::f7}},
		{{40, usbScancode_t::_7}},
		{{41, usbScancode_t::u}},
		{{42, usbScancode_t::j}},
		{{43, usbScancode_t::n}},
		{{255, usbScancode_t::reserved}},

		{{44, usbScancode_t::f8}},
		{{45, usbScancode_t::_8}},
		{{46, usbScancode_t::i}},
		{{47, usbScancode_t::k}},
		{{48, usbScancode_t::m}},
		{{255, usbScancode_t::reserved}},

		{{49, usbScancode_t::f9}},
		{{50, usbScancode_t::_9}},
		{{51, usbScancode_t::o}},
		{{52, usbScancode_t::l}},
		{{53, usbScancode_t::comma}},
		{{54, usbScancode_t::rightAlt}},

		{{55, usbScancode_t::f10}},
		{{56, usbScancode_t::_0}},
		{{57, usbScancode_t::p}},
		{{58, usbScancode_t::semiColon}},
		{{59, usbScancode_t::fullStop}},
		{{60, usbScancode_t::rightMeta}},

		{{61, usbScancode_t::f11}},
		{{62, usbScancode_t::dash}},
		{{63, usbScancode_t::leftBracket}},
		{{64, usbScancode_t::singleQuote}},
		{{65, usbScancode_t::forwardSlash}},
		{{66, usbScancode_t::menu}},

		{{67, usbScancode_t::f12}},
		{{68, usbScancode_t::equals}},
		{{69, usbScancode_t::rightBracket}},
		{{70, usbScancode_t::hash}},
		{{71, usbScancode_t::rightShift}},
		{{72, usbScancode_t::rightControl}},

		{{255, usbScancode_t::reserved}},
		{{73, usbScancode_t::backspace}},
		{{255, usbScancode_t::reserved}},
		{{74, usbScancode_t::enter}},
		{{255, usbScancode_t::reserved}},
		{{255, usbScancode_t::reserved}},

		{{75, usbScancode_t::printScreen}},
		{{76, usbScancode_t::insert}},
		{{77, usbScancode_t::_delete}},
		{{255, usbScancode_t::reserved}},
		{{255, usbScancode_t::reserved}},
		{{78, usbScancode_t::leftArrow}},

		{{79, usbScancode_t::scrollLock}},
		{{80, usbScancode_t::home}},
		{{81, usbScancode_t::end}},
		{{82, usbScancode_t::reserved}},
		{{83, usbScancode_t::upArrow}},
		{{84, usbScancode_t::downArrow}},

		{{85, usbScancode_t::pause}},
		{{86, usbScancode_t::pageUp}},
		{{87, usbScancode_t::pageDown}},
		{{255, usbScancode_t::reserved}},
		{{255, usbScancode_t::reserved}},
		{{88, usbScancode_t::rightArrow}},

		{{255, usbScancode_t::reserved}},
		{{89, usbScancode_t::numLock}},
		{{90, usbScancode_t::keypad7}},
		{{91, usbScancode_t::keypad4}},
		{{92, usbScancode_t::keypad1}},
		{{93, usbScancode_t::keypad0}},

		{{255, usbScancode_t::reserved}},
		{{94, usbScancode_t::keypadDivide}},
		{{95, usbScancode_t::keypad8}},
		{{96, usbScancode_t::keypad5}},
		{{97, usbScancode_t::keypad2}},
		{{255, usbScancode_t::reserved}},

		{{255, usbScancode_t::reserved}},
		{{98, usbScancode_t::keypadMultiply}},
		{{99, usbScancode_t::keypad9}},
		{{100, usbScancode_t::keypad6}},
		{{101, usbScancode_t::keypad3}},
		{{102, usbScancode_t::keypadPeriod}},

		{{255, usbScancode_t::reserved}},
		{{103, usbScancode_t::keypadSubtract}},
		{{255, usbScancode_t::reserved}},
		{{104, usbScancode_t::keypadAdd}},
		{{255, usbScancode_t::reserved}},
		{{105, usbScancode_t::keypadEnter}}
	}};
} // namespace mxKeyboard::keyMatrix

#endif /*KEY_MATRIX__HXX*/
