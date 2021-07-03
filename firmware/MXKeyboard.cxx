// SPDX-License-Identifier: BSD-3-Clause
#include <avr/builtins.h>
#include <usb/core.hxx>
#include "MXKeyboard.hxx"
#include "interrupts.hxx"
#include "usb/hid.hxx"

void run()
{
	__builtin_avr_cli();
	oscInit();
	//ps2Init();
	dmaInit();
	ledInit();
	keyInit();
	usb::core::init();
	usb::hid::registerHandlers(1, 1, 1);
	usb::core::attach();
	PMIC.CTRL = 0x87;
	__builtin_avr_sei();

	while (true)
		continue;
}

void usbBusEvtIRQ() noexcept { usb::core::handleIRQ(); }
void usbIOCompIRQ() noexcept { usb::core::handleIRQ(); }
