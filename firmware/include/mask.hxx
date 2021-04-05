#ifndef MASK__HXX
#define MASK__HXX

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <limits>

template<typename T, std::size_t offset, std::size_t count>
	constexpr std::enable_if_t<std::is_integral_v<T> && !std::is_signed_v<T>  &&
    	offset < std::numeric_limits<T>::digits &&
    	(offset + count) < std::numeric_limits<T>::digits, T>
	genMask() noexcept
{
    T mask_base{};
    for (std::size_t idx{}; idx < count; ++idx)
        mask_base |= T(1U << (offset + idx));
    return mask_base;
}

#endif /*MASK__HXX*/
