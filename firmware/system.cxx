// SPDX-License-Identifier: BSD-3-Clause
#include "MXKeyboard.hxx"
#include "interrupts.hxx"

/*!
 * The main oscillator for the device is 16MHz,
 * We want/have 16MHz ClkPer, ClkPer2 and ClkPer4 to
 * keep everything nice and easy
 */

// 16MHz oscilator is on PR1/XTAL1
void oscInit()
{
	PORTR.DIRCLR = 0x01;
	OSC.XOSCCTRL = OSC_FRQRANGE_12TO16_gc | OSC_XOSCSEL_EXTCLK_gc;
	// Enable the external osc
	OSC.CTRL |= 0x09;
	while (!(OSC.STATUS & 0x08))
		continue;
	CCP = CCP_IOREG_gc;
	OSC.XOSCFAIL = 1;

	CCP = CCP_IOREG_gc;
	CLK.PSCTRL = CLK_PSADIV_1_gc | CLK_PSBCDIV_1_1_gc;
	CCP = CCP_IOREG_gc;
	CLK.CTRL = CLK_SCLKSEL_XOSC_gc;

	// Disable the internal 2MHz RC osc, but also enable the 32MHz one.
	OSC.CTRL = 0x0A;

	// Configure the PLL to take our 16MHz clock and spin it up to 48MHz
	OSC.PLLCTRL = OSC_PLLSRC_XOSC_gc | 3;
	OSC.CTRL |= 0x10;
	// Wait for it to become ready
	while (!(OSC.STATUS & 0x10))
		continue;
	// And enable the USB peripheral's use of it.
	CLK.USBCTRL = CLK_USBPSDIV_1_gc | CLK_USBSRC_PLL_gc | 1;
}

void oscFailureIRQ()
{
	OSC.CTRL = 0x0A;
	CCP = CCP_IOREG_gc;
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc;
	CCP = CCP_IOREG_gc;
	CLK.PSCTRL = CLK_PSADIV_2_gc | CLK_PSBCDIV_2_2_gc;
	OSC.XOSCFAIL = 2;
}
