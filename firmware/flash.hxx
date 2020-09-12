#ifndef FLASH__HXX
#define FLASH__HXX

#include <cstring>
#include <array>
#include <type_traits>
#include <avr/io.h>

template<typename T> struct flash_t final
{
private:
	T value_;

public:
	constexpr flash_t(const T value) noexcept : value_{value} { }

	constexpr operator T() const noexcept
	{
		if (std::is_constant_evaluated())
			return value_;
		else
		{
			T result{};
			std::array<std::byte, sizeof(T)> buffer{};
			const uint8_t z{RAMPZ};

			__asm__ __volatile__(R"(
				ldi r30, lo8(%[value])
				ldi r31, hi8(%[value])
				ldi r16, hh8(%[value])
				out 0x3B, r16
				)" : : [value] "m" (value_) : "r16", "r30", "r31"
			);

			for (auto &byte : buffer)
			{
				__asm__ __volatile__(R"(
					elpm %[byte], Z+
					)" : [byte] "=r" (byte)
				);
			}

			RAMPZ = z;
			memcpy(&result, buffer.data(), buffer.size());
			return result;
		}
	}
};

#endif /*FLASH__HXX*/
