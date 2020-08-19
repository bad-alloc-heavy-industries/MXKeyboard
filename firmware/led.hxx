#ifndef LED__HXX
#define LED__HXX

#include <cstdint>
#include <avr/io.h>

extern void ledSetValue(uint8_t led, uint8_t r, uint8_t g, uint8_t b);
extern void ledLatch();

extern void uartInit();
extern void uartWrite(USART_t &uart, uint8_t data);
extern void uartWaitTXComplete(USART_t &uart);

#endif /*LED__HXX*/
