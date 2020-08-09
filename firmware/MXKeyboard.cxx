// SPDX-License-Identifier: BSD-3-Clause
#include <avr/builtins.h>
#include "MXKeyboard.hxx"
#include "led.hxx"

uint8_t redValue{0};
bool incRed{true};

void run()
{
	__builtin_avr_cli();
	oscInit();
	ledInit();

	while (true)
	{
		if (incRed)
			++redValue;
		else
			--redValue;

		if (redValue == 0)
			incRed = true;
		else if (redValue == 255)
			incRed = false;

		ledSetValue(0, redValue, incRed ? 1 : 0, 255 - redValue);
		ledLatch();
		__builtin_avr_delay_cycles(266660UL);
	}
}
