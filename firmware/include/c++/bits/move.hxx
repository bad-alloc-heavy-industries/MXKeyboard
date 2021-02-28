// SPDX-License-Identifier: BSD-3-Clause
#ifndef BITS_MOVE__HXX
#define BITS_MOVE__HXX

#include <type_traits>

namespace std
{
	namespace impl
	{
		template<typename T> constexpr inline T *addressof(T &r) noexcept { return __builtin_addressof(r); }
	}

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

	template<typename T> constexpr inline T *addressof(T &r) noexcept { return impl::addressof(r); }
	template<typename T> const T *addressof(const T &&) = delete;

	namespace impl
	{
		template<typename T, typename U = T> constexpr inline T exchange(T &obj, U &&newValue)
		{
			const T oldValue{std::move(obj)};
			obj = std::forward<U>(newValue);
			return oldValue;
		}
	} // namespace impl

	template<typename T> constexpr inline
		enable_if_t<!is_tuple_like_v<T> && is_move_constructible_v<T> && is_move_assignable_v<T>>
			swap(T &a, T &b) noexcept(is_nothrow_move_constructible_v<T> && is_nothrow_move_assignable_v<T>)
	{
		T tmp{std::move(a)};
		a = std::move(b);
		b = std::move(tmp);
	}

	template<typename T, size_t N> constexpr inline enable_if_t<is_swappable_v<T>>
		swap(T (&a)[N], T (&b)[N]) noexcept(is_nothrow_swappable_v<T>)
	{
		for (size_t n = 0; n < N; ++n)
			swap(a[n], b[n]);
	}
} // namespace std

#endif /*BITS_MOVE__HXX*/
