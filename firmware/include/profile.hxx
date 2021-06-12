// SPDX-License-Identifier: BSD-3-Clause
#ifndef PROFILE___HXX
#define PROFILE___HXX

#include <cstdint>
#include <array>
#include "keyMatrix.hxx"

namespace mxKeyboard::profile
{
	using usbScancode_t = usb::descriptors::hid::scancode_t;
	using mxKeyboard::keyMatrix::keyCount;
	using mxKeyboard::keyMatrix::rgb_t;

	constexpr static uint8_t profileCount{10U};

	constexpr static inline size_t bytesFor(const size_t bits) noexcept
		{ return (bits / 8U) + ((bits & 7U) ? 1U : 0U); }

	struct eepromPart_t final
	{
		uint8_t profileNumber{0xFFU};
		uint8_t debounce{};
		std::array<rgb_t, keyCount> keyColours{};
	};

	struct [[gnu::packed]] key_t final
	{
		uint8_t timePress{0};
		uint8_t timeRelease{0};
		usbScancode_t scancode{};
	};

	struct flashPart_t final
	{
		std::array<key_t, keyCount> keys{};
		std::array<uint8_t, bytesFor(keyCount)> keyTypes{};
	};

	struct profile_t final
	{
	private:
		eepromPart_t eeprom{};
		flashPart_t flash{};

	public:
		profile_t() noexcept = default;
		static profile_t read(uint8_t profileNumber) noexcept;
		void clear() noexcept { *this = {}; }
		void write() noexcept;
		bool valid(const uint8_t expectedNumber) const noexcept
			{ return eeprom.profileNumber == expectedNumber; }

		void number(const uint8_t profileNumber) noexcept { eeprom.profileNumber = profileNumber; }
		uint8_t number() const noexcept { return eeprom.profileNumber; }
		void debounce(const uint8_t debounce) noexcept { eeprom.debounce = debounce; }
		uint8_t debounce() const noexcept { return eeprom.debounce; }
		void keyColour(const uint8_t index, const rgb_t colour) noexcept { eeprom.keyColours[index] = colour; }
		rgb_t keyColour(const uint8_t index) const noexcept { return eeprom.keyColours[index]; }
		void timePress(const uint8_t index, const uint8_t time) noexcept
			{ flash.keys[index].timePress = time; }
		uint8_t timePress(const uint8_t index) const noexcept { return flash.keys[index].timePress; }
		void timeRelease(const uint8_t index, const uint8_t time) noexcept
			{ flash.keys[index].timeRelease = time; }
		uint8_t timeRelease(const uint8_t index) const noexcept { return flash.keys[index].timeRelease; }
		void scancode(const uint8_t index, usbScancode_t scancode) noexcept
			{ flash.keys[index].scancode = scancode; }
		usbScancode_t scancode(const uint8_t index) const noexcept { return flash.keys[index].scancode; }
		void keyType(uint8_t index, keyMatrix::keyType_t type) noexcept;
		bool keyType(const uint8_t index) const noexcept
			{ return (flash.keyTypes[index >> 3U] >> (index & 7U)) & 1U; }
	};
} // namespace mxKeyboard::profile

#endif /*PROFILE__HXX*/
