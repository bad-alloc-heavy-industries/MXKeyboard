// SPDX-License-Identifier: BSD-3-Clause
#ifndef BITS_PAIR__HXX
#define BITS_PAIR__HXX

#include <type_traits>
//#include <bits/rangeAccess.hxx>
#include <bits/move.hxx>

namespace std
{
	struct piecewise_construct_t { explicit piecewise_construct_t() = default; };
	constexpr inline auto piecewise_construct{piecewise_construct_t()};

	template<typename...> struct tuple;

	namespace impl
	{
		template<bool, typename T1, typename T2> struct pairConstraints_t
		{
			template<typename U1, typename U2> constexpr static bool constructiblePair()
				{ return and_v<is_constructible<T1, const U1 &>, is_constructible<T2, const U2 &>>; }

			template<typename U1, typename U2> constexpr static bool implicitlyConvertiblePair()
				{ return and_v<std::is_convertible<const U1 &, T1>, std::is_convertible<const U2 &, T2>>; }

			template<typename U1, typename U2> constexpr static bool moveConstructiblePair()
				{ return and_v<is_constructible<T1, U1 &&>, is_constructible<T2, U2 &&>>; }

			template<typename U1, typename U2> constexpr static bool implicitlyMoveConvertiblePair()
				{ return and_v<std::is_convertible<U1 &&, T1>, std::is_convertible<U2 &&, T2>>; }

			template<bool implicit, typename U1, typename U2> constexpr static bool copyMovePair()
			{
				using isConvertible = __and<std::is_convertible<const U1 &, T1>, std::is_convertible<U2 &&, T2>>;
				using converts = conditional_t<implicit, isConvertible, __not<isConvertible>>;
				return and_v<is_constructible<T1, const U1 &>, is_constructible<T2, U2 &&>, converts>;
			}

			template<bool implicit, typename U1, typename U2> constexpr static bool moveCopyPair()
			{
				using isConvertible = __and<std::is_convertible<U1 &&, T1>, std::is_convertible<const U2 &, T2>>;
				using converts = conditional_t<implicit, isConvertible, __not<isConvertible>>;
				return and_v<is_constructible<T1, U1 &&>, is_constructible<T2, const U2 &&>, converts>;
			}
		};

		template<typename T1, typename T2> struct pairConstraints_t<false, T1, T2>
		{
			template<typename U1, typename U2> constexpr static bool constructiblePair() { return false; }
			template<typename U1, typename U2> constexpr static bool implicitlyConvertiblePair() { return false; }
			template<typename U1, typename U2> constexpr static bool moveConstructiblePair() { return false; }
			template<typename U1, typename U2> constexpr static bool implicitlyMoveConvertiblePair() { return false; }
		};

		template<typename, typename> struct pair_t;
	} // namespace impl

	template<typename T1, typename T2> struct pair : private impl::pair_t<T1, T2>
	{
		using first_type = T1;
		using second_type = T2;

		T1 first;
		T2 second;

		template<typename U1 = T1, typename U2 = T2,
			enable_if_t<impl::and_v<impl::is_implicitly_default_constructible<U1>,
				impl::is_implicitly_default_constructible<U2>>, bool> = true>
			constexpr pair() : first{}, second{} { }

		template<typename U1 = T1, typename U2 = T2,
			enable_if_t<impl::and_v<is_default_constructible<U1>, is_default_constructible<U2>,
				impl::__not<impl::__and<impl::is_implicitly_default_constructible<U1>,
				impl::is_implicitly_default_constructible<U2>>>>, bool> = false>
			constexpr explicit pair() : first{}, second{} { }

		using pccp_t = impl::pairConstraints_t<true, T1, T2>;

		template<typename U1 = T1, typename U2 = T2,
			enable_if_t<pccp_t::template constructiblePair<U1, U2>() &&
				pccp_t::template implicitlyConvertiblePair<U1, U2>(), bool> = true>
			constexpr pair(const T1 &a, const T2 &b) : first{a}, second{b} { }

		template<typename U1 = T1, typename U2 = T2,
			enable_if_t<pccp_t::template constructiblePair<U1, U2>() &&
				!pccp_t::template implicitlyConvertiblePair<U1, U2>(), bool> = false>
			constexpr explicit pair(const T1 &a, const T2 &b) : first{a}, second{b} { }

		template<typename U1, typename U2> using pccfp_t =
			impl::pairConstraints_t<!is_same_v<T1, U2> || !is_same_v<T2, U2>, T1, T2>;

		template<typename U1, typename U2,
			enable_if_t<pccfp_t<U1, U2>::template constructiblePair<U1, U2>() &&
				pccfp_t<U1, U2>::template implicitlyConvertiblePair<U1, U2>(), bool> = true>
			constexpr pair(const pair<U1, U2> &other) : first{other.first}, second{other.second} { }

		template<typename U1, typename U2,
			enable_if_t<pccfp_t<U1, U2>::template constructiblePair<U1, U2>() &&
				!pccfp_t<U1, U2>::template implicitlyConvertiblePair<U1, U2>(), bool> = false>
			constexpr explicit pair(const pair<U1, U2> &other) : first{other.first}, second{other.second} { }

		constexpr pair(const pair &) = default;
		constexpr pair(pair &&) = default;

		template<typename U1, enable_if_t<pccp_t::template moveCopyPair<true, U1, T2>(), bool> = true>
			constexpr pair(U1 &&x, const T2 &y) : first{forward<U1>(x)}, second{y} { }

		template<typename U1, enable_if_t<pccp_t::template moveCopyPair<false, U1, T2>(), bool> = false>
			constexpr explicit pair(U1 &&x, const T2 &y) : first{forward<U1>(x)}, second{y} { }

		template<typename U2, enable_if_t<pccp_t::template copyMovePair<true, T1, U2>(), bool> = true>
			constexpr pair(const T1 &x, U2 &&y) : first{x}, second{forward<U2>(y)} { }

		template<typename U2, enable_if_t<pccp_t::template copyMovePair<false, T1, U2>(), bool> = false>
			constexpr explicit pair(const T1 &x, U2 &&y) : first{x}, second{forward<U2>(y)} { }

		template<typename U1, typename U2,
			enable_if_t<pccp_t::template moveConstructiblePair<U1, U2>() &&
				pccp_t::template implicitlyMoveConvertiblePair<U1, U2>(), bool> = true>
			constexpr pair(U1 &&x, U2 &&y) : first{std::forward<U1>(x)}, second{std::forward<U2>(y)} { }

		template<typename U1, typename U2,
			enable_if_t<pccp_t::template moveConstructiblePair<U1, U2>() &&
				!pccp_t::template implicitlyMoveConvertiblePair<U1, U2>(), bool> = false>
			constexpr explicit pair(U1 &&x, U2 &&y) : first{std::forward<U1>(x)}, second{std::forward<U2>(y)} { }

		template<typename U1, typename U2,
			enable_if_t<pccfp_t<U1, U2>::template moveConstructiblePair<U1, U2>() &&
				pccp_t::template implicitlyMoveConvertiblePair<U1, U2>(), bool> = true>
			constexpr pair(pair<U1, U2> &&other) : first{std::forward<U1>(other.first)},
				second{std::forward<U2>(other.second)} { }

		template<typename U1, typename U2,
			enable_if_t<pccfp_t<U1, U2>::template moveConstructiblePair<U1, U2>() &&
				!pccp_t::template implicitlyMoveConvertiblePair<U1, U2>(), bool> = false>
			constexpr explicit pair(pair<U1, U2> &&other) : first{std::forward<U1>(other.first)},
				second{std::forward<U2>(other.second)} { }

		template<typename... args1_t, typename... args2_t>
			constexpr pair(piecewise_construct_t, tuple<args1_t...>, tuple<args2_t...>);

		constexpr pair &operator =(conditional_t<impl::and_v<is_copy_assignable<T1>, is_copy_assignable<T2>>,
			const pair &, const impl::noneSuch_t &> other)
		{
			first = other.first;
			second = other.second;
			return *this;
		}

		constexpr pair &operator =(conditional_t<impl::and_v<is_move_assignable<T1>,
			is_move_assignable<T2>>, pair &&, impl::noneSuch_t &&> other)
				noexcept(is_nothrow_move_assignable_v<T1> && is_nothrow_move_assignable_v<T2>)
		{
			first = std::forward<first_type>(other.forward);
			second = std::forward<second_type>(other.second);
			return *this;
		}

		template<typename U1, typename U2>
			constexpr enable_if_t<impl::and_v<is_assignable<T1 &, const U1 &>, is_assignable<T2 &, const U2 &>>, pair &>
				operator =(const pair<U1, U2> &other)
		{
			first = other.first;
			second = other.second;
			return *this;
		}

		template<typename U1, typename U2>
			constexpr enable_if_t<impl::and_v<is_assignable<T1 &, U1 &&>, is_assignable<T2 &, U2 &&>>, pair &>
				operator =(pair<U1, U2> &&other)
		{
			first = std::forward<U1>(other.first);
			second = std::forward<U2>(other.second);
			return *this;
		}

		constexpr void swap(pair &other) noexcept(is_nothrow_swappable_v<T1> && is_nothrow_swappable_v<T2>)
		{
			std::swap(first, other.first);
			std::swap(second, other.second);
		}

	private:
		/*template<typename... args1_t, std::size_t... indexes1, typename... args2_t, std::size_t... indexes2>
			constexpr pair(tuple<args1_t...> &, tuple<args2_t...> &,
				impl::indexTuple_t<indexes1...>, impl::indexTuple_t<indexes2...>);*/
	};

	namespace impl
	{
		template<typename, typename> struct pair_t
		{
			template<typename T1, typename T2> friend struct std::pair;
			pair_t() = default;
			~pair_t() = default;
			pair_t(const pair_t &) = default;
			pair_t &operator =(const pair_t &) = delete;
		};
	} // namespace impl

	template<typename T1, typename T2> constexpr inline enable_if_t<is_swappable_v<T1> && is_swappable_v<T2>>
		swap(pair<T1, T2> &x, pair<T1, T2> &y) noexcept(noexcept(x.swap(y))) { x.swap(y); }

	template<typename T1, typename T2> enable_if_t<!(is_swappable_v<T1> && is_swappable_v<T2>)>
		swap(pair<T1, T2> &, pair<T1, T2> &) = delete;

	template<typename T1, typename T2> constexpr pair<impl::decay_and_strip_t<T1>, impl::decay_and_strip_t<T2>>
		make_pair(T1 &&x, T2 &&y)
	{
		using type1_t = impl::decay_and_strip_t<T1>;
		using type2_t = impl::decay_and_strip_t<T2>;
		using pair_type = pair<type1_t, type2_t>;
		return pair_type{std::forward<T1>(x), std::forward<T2>(y)};
	}
} // namespace std

#endif /*BITS_PAIR__HXX*/
