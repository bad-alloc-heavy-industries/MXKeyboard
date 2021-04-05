// SPDX-License-Identifier: BSD-3-Clause
#ifndef INTERRUPTS__HXX
#define INTERRUPTS__HXX

#define INTERRUPT __attribute__((signal)) __attribute__((used))

extern "C"
{
	void oscFailureIRQ() INTERRUPT;
	void dmaChannel0IRQ() INTERRUPT;
	void dmaChannel1IRQ() INTERRUPT;
	void dmaChannel2IRQ() INTERRUPT;
	void tcc0OverflowIRQ() INTERRUPT;
	void usbBusEvtIRQ() noexcept INTERRUPT;
	void usbIOCompIRQ() noexcept INTERRUPT;
	void ps2IRQ() noexcept INTERRUPT;
	void keyIRQ() noexcept INTERRUPT;
}

#endif /*INTERRUPTS__HXX*/
