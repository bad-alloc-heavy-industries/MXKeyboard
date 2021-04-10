#ifndef FLASH__HXX
#define FLASH__HXX

#include <cstring>
#include <array>
#include <avr/io.h>

template<typename T> struct flash_t final
{
private:
	T value_;

public:
	constexpr flash_t() noexcept : value_{} { }
	constexpr flash_t(const T value) noexcept : value_{value} { }

	operator T() const noexcept
	{
		T result{};
		const auto resultAddr{reinterpret_cast<uint32_t>(&result)};
		const auto valueAddr{reinterpret_cast<uint32_t>(&value_)};
		const uint8_t x{RAMPX};
		const uint8_t z{RAMPZ};

		__asm__(R"(
			movw r26, %[result]
			out 0x39, %C[result]
			movw r30, %[value]
			out 0x3B, %C[value]
			ldi r17, %[count]
			cpi r17, 0
loop%=:
			breq loopDone%=
			elpm r16, Z+
			st X+, r16
			dec r17
			rjmp loop%=
loopDone%=:
			)" : : [result] "g" (resultAddr), [value] "g" (valueAddr), [count] "I" (sizeof(T)) :
				"r16", "r17", "r26", "r27", "r30", "r31"
		);

		RAMPZ = z;
		RAMPX = x;
		return result;
	}
};

template<> struct flash_t<uint8_t> final
{
private:
	uint8_t value_;

public:
	constexpr flash_t(const uint8_t value) noexcept : value_{value} { }

	operator uint8_t() const noexcept
	{
		uint8_t result{};
		const auto valueAddr{reinterpret_cast<uint32_t>(&value_)};
		const uint8_t z{RAMPZ};

		__asm__(R"(
			out 0x3B, %C[value]
			movw r30, %[value]
			elpm %[result], Z+
			)" : [result] "=r" (result) : [value] "g" (valueAddr) :
				"r30", "r31"
		);

		RAMPZ = z;
		return result;
	}
};

template<> struct flash_t<char *> final
{
private:
	const char *value_;

public:
	constexpr flash_t(const char *const value) noexcept : value_{value} { }
	constexpr flash_t<char *> operator +(const std::size_t count) const noexcept
		{ return {value_ + count}; }

	char operator *() const noexcept
	{
		char result{};
		const auto valueAddr{reinterpret_cast<uint32_t>(value_)};
		const uint8_t z{RAMPZ};

		__asm__(R"(
			out 0x3B, %C[value]
			movw r30, %[value]
			elpm %[result], Z+
			)" : [result] "=r" (result) : [value] "g" (valueAddr) :
				"r30", "r31"
		);

		RAMPZ = z;
		return result;
	}

	constexpr flash_t<char *> &operator ++() noexcept
	{
		++value_;
		return *this;
	}

	constexpr flash_t<char *> &operator --() noexcept
	{
		--value_;
		return *this;
	}

	char operator [](const std::size_t count) const noexcept
		{ return *operator +(count); }
	constexpr bool operator ==(const flash_t<char *> &other) const noexcept
		{ return value_ == other.value_; }
	constexpr bool operator !=(const flash_t<char *> &other) const noexcept
		{ return !operator ==(other); }
};

template<> struct flash_t<uint16_t> final
{
private:
	uint16_t value_;

public:
	constexpr flash_t(const uint16_t value) noexcept : value_{value} { }
	operator uint16_t() const noexcept
	{
		uint16_t result{};
		const auto valueAddr{reinterpret_cast<uint32_t>(&value_)};
		const uint8_t z{RAMPZ};

		__asm__(R"(
			out 0x3B, %C[value]
			movw r30, %[value]
			elpm %A[result], Z+
			elpm %B[result], Z
			)" : [result] "=r" (result) : [value] "g" (valueAddr) :
				"r30", "r31"
		);

		RAMPZ = z;
		return result;
	}
};

#endif /*FLASH__HXX*/
