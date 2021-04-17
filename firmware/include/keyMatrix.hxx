// SPDX-License-Identifier: BSD-3-Clause
#ifndef KEY_MATRIX__HXX
#define KEY_MATRIX__HXX

#include <array>
#include "flash.hxx"

namespace mxKeyboard::keyMatrix
{
	constexpr static size_t keyCount{126};

	struct key_t final
	{
		uint8_t ledIndex;
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
	};

	const std::array<flash_t<key_t>, keyCount> keys
	{{
		{{0}},
		{{1}},
		{{2}},
		{{3}},
		{{4}},
		{{5}},

		{{6}},
		{{7}},
		{{8}},
		{{9}},
		{{10}},
		{{11}},

		{{12}},
		{{13}},
		{{14}},
		{{15}},
		{{16}},
		{{17}},

		{{18}},
		{{19}},
		{{20}},
		{{21}},
		{{22}},
		{{255}},

		{{23}},
		{{24}},
		{{25}},
		{{26}},
		{{27}},
		{{255}},

		{{28}},
		{{29}},
		{{30}},
		{{31}},
		{{32}},
		{{255}},

		{{33}},
		{{34}},
		{{35}},
		{{36}},
		{{37}},
		{{38}},

		{{39}},
		{{40}},
		{{41}},
		{{42}},
		{{43}},
		{{255}},

		{{44}},
		{{45}},
		{{46}},
		{{47}},
		{{48}},
		{{255}},

		{{49}},
		{{50}},
		{{51}},
		{{52}},
		{{53}},
		{{54}},

		{{55}},
		{{56}},
		{{57}},
		{{58}},
		{{59}},
		{{60}},

		{{61}},
		{{62}},
		{{63}},
		{{64}},
		{{65}},
		{{66}},

		{{67}},
		{{68}},
		{{69}},
		{{70}},
		{{71}},
		{{72}},

		{{255}},
		{{73}},
		{{255}},
		{{74}},
		{{255}},
		{{255}},

		{{75}},
		{{76}},
		{{77}},
		{{255}},
		{{255}},
		{{78}},

		{{79}},
		{{80}},
		{{81}},
		{{82}},
		{{83}},
		{{84}},

		{{85}},
		{{86}},
		{{87}},
		{{255}},
		{{255}},
		{{88}},

		{{255}},
		{{89}},
		{{90}},
		{{91}},
		{{92}},
		{{93}},

		{{255}},
		{{94}},
		{{95}},
		{{96}},
		{{97}},
		{{255}},

		{{255}},
		{{98}},
		{{99}},
		{{100}},
		{{101}},
		{{102}},

		{{255}},
		{{103}},
		{{255}},
		{{104}},
		{{255}},
		{{105}}
	}};
} // namespace mxKeyboard::keyMatrix

#endif /*KEY_MATRIX__HXX*/
