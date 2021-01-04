// SPDX-License-Identifier: BSD-3-Clause
#include <array>
#include "MXKeyboard.hxx"
#include "interrupts.hxx"
#include "usb/core.hxx"

/*!
 * "USB IN" transfers == Controller In
 * "USB OUT" transfers == Controller Out
 * We are always the Peripheral, so this is unabiguous.
 */

using namespace usb::types;

namespace usb::core
{
	// These are organised EPxOut, EPxIn, etc
	std::array<USB_EP_t, endpointCount> endpoints{};
	std::array<std::array<uint8_t, usb::epBufferSize>, usb::endpointCount> epBuffer{};

	deviceState_t usbState;
	usbEP_t usbPacket;
	bool usbSuspended;
	ctrlState_t usbCtrlState;
	uint8_t usbDeferalFlags;

	std::array<usbEPStatus_t<const void>, endpointCount> epStatusControllerIn;
	std::array<usbEPStatus_t<void>, endpointCount> epStatusControllerOut;
}

using namespace usb::core;

void usbInit() noexcept
{
	// Enable the USB peripheral
	USB.CTRLA = vals::usb::ctrlAUSBEnable | vals::usb::ctrlAModeFullSpeed | vals::usb::ctrlAMaxEP(4); //6
	USB.EPPTR = reinterpret_cast<std::uintptr_t>(endpoints.data());

	[]() noexcept
	{
		uint8_t i{};
		for (auto &endpoint : endpoints)
			endpoint.DATAPTR = reinterpret_cast<std::uintptr_t>(epBuffer[i++].data());
	}();

	// Configure EP0Out as our primary control input EP
	endpoints[0].CTRL = USB_EP_TYPE_CONTROL_gc | USB_EP_BUFSIZE_64_gc;
	// Configure EP0In as our primary control output endpoint
	endpoints[1].CTRL = USB_EP_TYPE_CONTROL_gc | USB_EP_BUFSIZE_64_gc;
	// Configure EP1Out to perminantly stall.
	endpoints[2].CTRL = USB_EP_TYPE_DISABLE_gc | vals::usb::usbEPCtrlStall;
	// Configure EP1In
	endpoints[3].CTRL = USB_EP_TYPE_BULK_gc | USB_EP_BUFSIZE_64_gc | vals::usb::usbEPCtrlStall;

	// Reset all USB interrupts
	USB.INTCTRLA &= vals::usb::intCtrlAClearMask;
	USB.INTCTRLB &= vals::usb::intCtrlBClearMask;
	// Ensure the device address is 0
	USB.ADDR &= ~vals::usb::addressMask;

	// Enable the USB reset interrupt
	USB.INTCTRLA |= vals::usb::intCtrlAEnableBusEvent | USB_INTLVL_MED_gc;

	// Initialise the state machine
	usbState = deviceState_t::detached;
	usbCtrlState = ctrlState_t::idle;
	usbDeferalFlags = 0;
	USB.CTRLB = vals::usb::ctrlBAttach;
}

namespace usb::core
{
	const void *sendData(const uint8_t ep, const void *const bufferPtr, const uint8_t length) noexcept
	{
		auto *const inBuffer{static_cast<const uint8_t *>(bufferPtr)};
		auto *const outBuffer{epBuffer[(ep << 1) + 1].data()};
		// Copy the data to tranmit from the user buffer
		for (uint8_t i{0}; i < length; ++i)
			outBuffer[i] = inBuffer[i];
		return inBuffer + length;
	}

	void *recvData(const uint8_t ep, void *const bufferPtr, const uint8_t length) noexcept
	{
		const auto *const inBuffer{epBuffer[(ep << 1)].data()};
		auto *const outBuffer{static_cast<uint8_t *>(bufferPtr)};
		// Copy the received data to the user buffer
		for (uint8_t i{0}; i < length; ++i)
			outBuffer[i] = inBuffer[i];
		return outBuffer + length;
	}
} // namespace usb::core

void usbBusEvtIRQ() noexcept
{
}

void usbIOCompIRQ() noexcept
{
}
