// SPDX-License-Identifier: BSD-3-Clause
#ifndef MXKEYBOARD__HXX
#define MXKEYBOARD__HXX

#include <avr/io.h>

#define DEFAULT_VISIBILITY __attribute__ ((visibility("default")))
#define USED __attribute__ ((__used__))
#define SECTION(name) __attribute__ ((__section__(name)))

extern void run();

extern void oscInit();
extern void ledInit();
extern void timerInit(TC0_t &timer);
extern void dmaInit();
extern void dmaInit(DMA_CH_t &channel, DMA_CH_TRIGSRC_t triggerSource);
extern void usbInit() noexcept;

extern void dmaTransferLength(DMA_CH_t &channel, uint16_t length);
extern void dmaTransferSource(DMA_CH_t &channel, const void *address);
extern void dmaTransferSource(DMA_CH_t &channel, const volatile void *address);
extern void dmaTransferDest(DMA_CH_t &channel, const void *address);
extern void dmaTransferDest(DMA_CH_t &channel, const volatile void *address);
extern void dmaInterruptEnable(DMA_CH_t &channel);
extern void dmaTrigger(DMA_CH_t &channel);

#endif /*MXKEYBOARD__HXX*/
