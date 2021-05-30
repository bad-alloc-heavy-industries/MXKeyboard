// SPDX-License-Identifier: BSD-3-Clause
#ifndef INDEX_SEQUENCE__HXX
#define INDEX_SEQUENCE__HXX

#include <cstdint>
#include <cstddef>

namespace utility
{
	struct indexIterator_t final
	{
	private:
		std::size_t index;

	public:
		constexpr indexIterator_t(const std::size_t value) noexcept : index{value} { }
		[[nodiscard]] constexpr std::size_t operator *() const noexcept { return index; }

		constexpr indexIterator_t &operator ++() noexcept
		{
			++index;
			return *this;
		}
	};

	[[nodiscard]] constexpr inline bool operator ==
			(const indexIterator_t &a, const indexIterator_t &b) noexcept
		{ return *a == *b; }

	[[nodiscard]] constexpr inline bool operator !=
			(const indexIterator_t &a, const indexIterator_t &b) noexcept
		{ return *a != *b; }

	struct indexSequence_t final
	{
	private:
		std::size_t begin_;
		std::size_t end_;

	public:
		constexpr indexSequence_t(const std::size_t end) noexcept : begin_{0}, end_{end} { }
		constexpr indexSequence_t(const std::size_t begin, const std::size_t end) noexcept :
			begin_{begin}, end_{end} { }

		[[nodiscard]] constexpr indexIterator_t begin() const noexcept { return {begin_}; }
		[[nodiscard]] constexpr indexIterator_t end() const noexcept { return {end_}; }
	};
} // namespace utility

#endif /* INDEX_SEQUENCE__HXX */
