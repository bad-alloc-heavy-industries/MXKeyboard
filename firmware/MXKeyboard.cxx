// SPDX-License-Identifier: BSD-3-Clause
#include <avr/builtins.h>
#include "MXKeyboard.hxx"

void run()
{
	__builtin_avr_cli();
	oscInit();
	//ps2Init();
	dmaInit();
	ledInit();
	keyInit();
	usbInit();
	PMIC.CTRL = 0x87;
	__builtin_avr_sei();

	while (true)
		continue;
}
