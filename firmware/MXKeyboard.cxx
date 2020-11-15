// SPDX-License-Identifier: BSD-3-Clause
#include <avr/builtins.h>
#include "MXKeyboard.hxx"
#include "led.hxx"

void run()
{
	__builtin_avr_cli();
	oscInit();
	//ps2Init();
	dmaInit();
	ledInit();
	PMIC.CTRL = 0x87;
	usbInit();
	__builtin_avr_sei();

	while (true)
		continue;
}
