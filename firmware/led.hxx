#ifndef LED__HXX
#define LED__HXX

#include <cstdint>
#include <cstddef>
#include <avr/io.h>

extern void ledSetValue(std::size_t led, uint8_t r, uint8_t g, uint8_t b);
extern void ledLatch();

extern void uartInit();
extern void uartWrite(USART_t &uart, uint8_t data);
extern void uartWaitTXComplete(USART_t &uart);

#endif /*LED__HXX*/
