// SPDX-License-Identifier: BSD-3-Clause
#ifndef KEY_MATRIX__HXX
#define KEY_MATRIX__HXX

#include <array>
#include "flash.hxx"
#include "usb/types.hxx"

namespace mxKeyboard::keyMatrix
{
	constexpr static size_t keyCount{126};

	using usbScancode_t = usb::descriptors::hid::scancode_t;

	struct rgb_t final
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
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
			value |= uint8_t(state ? 0x01U : 0x00U);
		}

		bool logicalState() const noexcept { return value & 0x02U; }
		void logicalState(const bool state) noexcept
		{
			value &= 0xFDU;
			value |= uint8_t(state ? 0x02U : 0x00U);
		}

		bool dirty() const noexcept { return value & 0x04U; }
		void dirty(const bool state) noexcept
		{
			value &= 0xFBU;
			value |= uint8_t(state ? 0x04U : 0x00U);
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
		rgb_t ledColour{};
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
		{{14, usbScancode_t::w}},
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
