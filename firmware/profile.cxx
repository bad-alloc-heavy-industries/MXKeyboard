// SPDX-License-Identifier: BSD-3-Clause
#include "profile.hxx"

using mxKeyboard::keyMatrix::keyType_t;

namespace mxKeyboard::profile
{
	[[gnu::section(".profile")]] const static std::array<flashPart_t, profileCount> flashProfiles{{}};

	profile_t profile_t::read(const uint8_t profileNumber) noexcept
	{
		profile_t profile{};
		const auto *const eeprom{reinterpret_cast<void *>(MAPPED_EEPROM_START +
			(sizeof(eepromPart_t) * profileNumber))};

		std::memcpy(&profile.eeprom, eeprom, sizeof(eepromPart_t));

		/*flash_t<flashPart_t *> flashPart{&flashProfiles[profileNumber]};
		profile.flash = *flashPart;*/

		return profile;
	}

	void profile_t::write() noexcept
	{
		//
	}

	void profile_t::keyType(const uint8_t index, const keyType_t type) noexcept
	{
		if (type == keyType_t::latching)
			flash.keyTypes[index >> 3U] |= uint8_t(1U << (index & 7U));
		else
			flash.keyTypes[index >> 3U] &= uint8_t(~(1U << (index & 7U)));
	}
} // namespace mxKeyboard::profile
