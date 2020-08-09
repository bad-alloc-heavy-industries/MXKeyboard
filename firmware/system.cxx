// SPDX-License-Identifier: BSD-3-Clause
#include "MXKeyboard.hxx"
#include "interrupts.hxx"

// 16MHz oscilator is on PR1/XTAL1
void oscInit()
{
	PORTR.DIRCLR = 0x01;
	OSC.XOSCCTRL = OSC_FRQRANGE_12TO16_gc | OSC_XOSCSEL_EXTCLK_gc;
	// Enable the external osc
	OSC.CTRL = 0x09;
	while (!(OSC.STATUS & 0x08))
		continue;
	CCP = CCP_IOREG_gc;
	OSC.XOSCFAIL = 1;

	CCP = CCP_IOREG_gc;
	CLK.PSCTRL = CLK_PSADIV_1_gc | CLK_PSBCDIV_1_1_gc;
	CCP = CCP_IOREG_gc;
	CLK.CTRL = CLK_SCLKSEL_XOSC_gc;
	CCP = CCP_IOREG_gc;
	CLK.LOCK = 1;

	// Disable the internal 2MHz RC osc, but also enable the 32MHz one.
	OSC.CTRL = 0x0A;
}

void oscFailureIRQ()
{
	CCP = CCP_IOREG_gc;
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc;
	CCP = CCP_IOREG_gc;
	CLK.PSCTRL = CLK_PSADIV_2_gc | CLK_PSBCDIV_2_2_gc;
	OSC.XOSCFAIL = 3;
}
