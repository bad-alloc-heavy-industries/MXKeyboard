// SPDX-License-Identifier: BSD-3-Clause
#ifndef UART__HXX
#define UART__HXX

#include <cstdint>
#include <string_view>
#include <avr/io.h>

extern void uartInit() noexcept;
extern void uartWrite(USART_t &uart, uint8_t data);
extern void uartWrite(USART_t &uart, std::flash_string_view str);
extern void uartWaitTXComplete(USART_t &uart);

static auto &debugUART{USARTE1};

using namespace std::literals::string_view_literals;

#endif /*UART__HXX*/
