// SPDX-License-Identifier: BSD-3-Clause
#include "MXKeyboard.hxx"

void timerInit(TC0_t &timer)
{
	timer.CTRLA = TC_CLKSEL_DIV4_gc;
	timer.CTRLB = TC_WGMODE_NORMAL_gc;
	timer.CTRLE = TC_BYTEM_NORMAL_gc;
	timer.INTCTRLA = TC_OVFINTLVL_MED_gc;
	timer.CTRLFCLR = 0x0FU;
	timer.CTRLFSET = TC_CMD_UPDATE_gc;
	timer.PER = 33333;
	timer.CNT = 0;
}
