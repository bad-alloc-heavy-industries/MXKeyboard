// SPDX-License-Identifier: BSD-3-Clause
#ifndef BITS_RANGE_ACCESS_HXX
#define BITS_RANGE_ACCESS_HXX

#include <cstddef>

namespace std
{
	template<typename container_t>
		constexpr inline auto begin(container_t &cont) -> decltype(cont.begin()) { return cont.begin(); }

	template<typename container_t>
		constexpr inline auto begin(const container_t &cont) -> decltype(cont.begin()) { return cont.begin(); }

	template<typename container_t>
		constexpr inline auto end(container_t &cont) -> decltype(cont.end()) { return cont.end(); }

	template<typename container_t>
		constexpr inline auto end(const container_t &cont) -> decltype(cont.end()) { return cont.end(); }

	template<typename T, size_t N> constexpr inline T *begin(T (&arr)[N]) { return arr; }
	template<typename T, size_t N> constexpr inline T *end(T (&arr)[N]) { return arr + N; }

	template<typename container_t> constexpr inline auto cbegin(const container_t &cont)
		noexcept(noexcept(std::begin(cont))) -> decltype(begin(cont)) { return begin(cont); }

	template<typename container_t> constexpr inline auto cend(const container_t &cont)
		noexcept(noexcept(std::end(cont))) -> decltype(end(cont)) { return end(cont); }

	template<typename container_t>
		constexpr inline auto rbegin(container_t &cont) -> decltype(cont.rbegin()) { return cont.rbegin(); }

	template<typename container_t>
		constexpr inline auto rbegin(const container_t &cont) -> decltype(cont.rbegin()) { return cont.rbegin(); }

	template<typename container_t>
		constexpr inline auto rend(container_t &cont) -> decltype(cont.rend()) { return cont.rend(); }

	template<typename container_t>
		constexpr inline auto rend(const container_t &cont) -> decltype(cont.rend()) { return cont.rend(); }

	template<typename container_t> constexpr inline auto crbegin(const container_t &cont)
		noexcept(noexcept(std::rbegin(cont))) -> decltype(rbegin(cont)) { return rbegin(cont); }

	template<typename container_t> constexpr inline auto crend(const container_t &cont)
		noexcept(noexcept(std::rend(cont))) -> decltype(rend(cont)) { return rend(cont); }

	template<typename container_t> constexpr auto size(const container_t &cont) noexcept(noexcept(cont.size())) ->
		decltype(cont.size()) { return cont.size(); }

	template<typename T, size_t N> constexpr size_t size(const T (&)[N]) noexcept { return N; }

	template<typename container_t>
		[[nodiscard]] constexpr auto empty(const container_t &cont)
			noexcept(noexcept(cont.empty())) -> decltype(cont.empty()) { return cont.empty(); }

	template<typename T, size_t N> [[nodiscard]] constexpr bool empty(const T (&)[N]) noexcept { return false; }

	template<typename container_t> constexpr auto data(container_t &cont)
		noexcept(noexcept(cont.data())) -> decltype(cont.data())  { return cont.data(); }

	template<typename T, size_t N> constexpr T *data(T (&array)[N]) noexcept { return array; }
} // namespace std

#endif /*BITS_RANGE_ACCESS_HXX*/
