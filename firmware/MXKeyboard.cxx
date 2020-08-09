// SPDX-License-Identifier: BSD-3-Clause
#include <avr/builtins.h>
#include "MXKeyboard.hxx"

void run()
{
	__builtin_avr_cli();
	oscInit();
	ledInit();

	while (true)
		continue;
}
