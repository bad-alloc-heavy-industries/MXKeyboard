// SPDX-License-Identifier: BSD-3-Clause
#ifndef LED__HXX
#define LED__HXX

#include <cstdint>
#include <cstddef>

extern void ledSetValue(std::size_t led, uint8_t r, uint8_t g, uint8_t b);
extern void ledLatch();

#endif /*LED__HXX*/
