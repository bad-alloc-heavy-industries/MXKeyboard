// SPDX-License-Identifier: BSD-3-Clause
#ifndef USB_CORE__HXX
#define USB_CORE__HXX

#include <avr/io.h>
#include "types.hxx"

namespace usb::core
{
	using usb::types::deviceState_t;
	using usb::types::usbEP_t;
	using usb::types::ctrlState_t;
	using usb::types::usbEPStatus_t;

	extern deviceState_t usbState;
	extern usbEP_t usbPacket;
	extern ctrlState_t usbCtrlState;

	extern std::array<usbEPStatus_t<const void>, usb::endpointCount> epStatusControllerIn;
	extern std::array<usbEPStatus_t<void>, usb::endpointCount> epStatusControllerOut;

	extern std::array<::USB_EP_t, usb::endpointCount> endpoints;
	extern std::array<std::array<uint8_t, usb::epBufferSize>, usb::endpointCount> epBuffer;

	extern const void *sendData(const uint8_t ep, const void *const bufferPtr, const uint8_t length) noexcept;
	extern void *recvData(const uint8_t ep, void *const buffer, const uint8_t length) noexcept;
} // namespace usb::core

#endif /*USB_CORE__HXX*/
