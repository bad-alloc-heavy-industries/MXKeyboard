// SPDX-License-Identifier: BSD-3-Clause
#include <array>
#include "MXKeyboard.hxx"
#include "usb/types.hxx"

/*!
 * "USB IN" transfers == Controller In
 * "USB OUT" transfers == Controller Out
 * We are always the Peripheral, so this is unabiguous.
 */

// These are organised EPxOut, EPxIn, etc
std::array<USB_EP_t, usb::endpointCount> endpoints{};

std::array<std::array<uint8_t, usb::epBufferSize>, usb::endpointCount> epBuffer{};

void usbInit() noexcept
{
	USB.CTRLA = vals::usb::ctrlAUSBEnable | vals::usb::ctrlAModeFullSpeed | vals::usb::ctrlAMaxEP(4); //6
	USB.EPPTR = reinterpret_cast<std::uintptr_t>(endpoints.data());

	[]() noexcept
	{
		uint8_t i{};
		for (auto &endpoint : endpoints)
			endpoint.DATAPTR = reinterpret_cast<std::uintptr_t>(epBuffer[i++].data());
	}();

	// Configure EP0Out as our primary control input EP
	endpoints[0].CTRL = USB_EP_TYPE_CONTROL_gc | USB_EP_BUFSIZE_64_gc | vals::usb::usbEPCtrlStall;
	// Configure EP0In as our primary control output endpoint
	endpoints[1].CTRL = USB_EP_TYPE_CONTROL_gc | USB_EP_BUFSIZE_64_gc | vals::usb::usbEPCtrlStall;
	// Configure EP1Out to perminantly stall.
	endpoints[2].CTRL = USB_EP_TYPE_DISABLE_gc | vals::usb::usbEPCtrlStall;
	// Configure EP1In
	endpoints[3].CTRL = USB_EP_TYPE_BULK_gc | USB_EP_BUFSIZE_64_gc | vals::usb::usbEPCtrlStall;

	USB.CTRLB = vals::usb::ctrlBAttach;
}
