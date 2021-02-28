// SPDX-License-Identifier: BSD-3-Clause
#ifndef BITS_MOVE__HXX
#define BITS_MOVE__HXX

#include <type_traits>

namespace std
{
	template<typename T> constexpr T&& forward(typename std::remove_reference<T>::type &t) noexcept
		{ return static_cast<T &&>(t); }

	template<typename T> constexpr T&& forward(typename std::remove_reference<T>::type &&t) noexcept
	{
		//static_assert(!std::is_lvalue_reference<T>::value, "template argument subtituting T "
		//	"is an lvalue reference type");
		return static_cast<T &&>(t);
	}

	template<typename T> constexpr remove_reference_t<T> &&move(T &&t) noexcept
		{ return static_cast<remove_reference_t<T> &&>(t); }
} // namespace std

#endif /*BITS_MOVE__HXX*/
