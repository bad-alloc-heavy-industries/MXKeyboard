// SPDX-License-Identifier: BSD-3-Clause
#include <cstdint>
#include "MXKeyboard.hxx"

void dmaInit()
{
	// Enable DMA controller, no double buffering, round robin
	DMA.CTRL = 0x80 | DMA_DBUFMODE_CH01CH23_gc | DMA_PRIMODE_RR0123_gc;
}

void dmaInit(DMA_CH_t &channel, const DMA_CH_TRIGSRC_t triggerSource)
{
	channel.CTRLA = DMA_CH_BURSTLEN_1BYTE_gc;
	channel.ADDRCTRL = DMA_CH_SRCRELOAD_TRANSACTION_gc | DMA_CH_SRCDIR_INC_gc |
		DMA_CH_DESTRELOAD_NONE_gc | DMA_CH_DESTDIR_FIXED_gc;
	channel.TRIGSRC = triggerSource;
	channel.REPCNT = 0;
	static_assert(sizeof(void *) == sizeof(std::uintptr_t));
	static_assert(sizeof(uint32_t) > sizeof(std::uintptr_t));
}

void dmaTransferLength(DMA_CH_t &channel, const uint16_t length)
	{ channel.TRFCNT = length; }

void dmaTransferSource(DMA_CH_t &channel, const std::uint32_t address)
{
	channel.SRCADDR0 = uint8_t(address);
	channel.SRCADDR2 = uint8_t(address >> 16U);
	channel.SRCADDR1 = uint8_t(address >> 8U);
}

void dmaTransferSource(DMA_CH_t &channel, const void *const address)
	{ dmaTransferSource(channel, reinterpret_cast<std::uint32_t>(address)); }
void dmaTransferSource(DMA_CH_t &channel, const volatile void *const address)
	{ dmaTransferSource(channel, reinterpret_cast<std::uint32_t>(address)); }

void dmaTransferDest(DMA_CH_t &channel, std::uint32_t address)
{
	channel.DESTADDR0 = uint8_t(address);
	channel.DESTADDR2 = uint8_t(address >> 16U);
	channel.DESTADDR1 = uint8_t(address >> 8U);
}

void dmaTransferDest(DMA_CH_t &channel, const void *const address)
	{ dmaTransferDest(channel, reinterpret_cast<std::uint32_t>(address)); }
void dmaTransferDest(DMA_CH_t &channel, const volatile void *const address)
	{ dmaTransferDest(channel, reinterpret_cast<std::uint32_t>(address)); }

void dmaInterruptEnable(DMA_CH_t &channel)
	{ channel.CTRLB = DMA_CH_TRNINTLVL_HI_gc; }

void dmaTrigger(DMA_CH_t &channel)
	{ channel.CTRLA |= 0x80; }
