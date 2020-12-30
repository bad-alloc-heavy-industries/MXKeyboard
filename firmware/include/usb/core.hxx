// SPDX-License-Identifier: BSD-3-Clause
#ifndef USB_CORE__HXX
#define USB_CORE__HXX

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

	extern const uint8_t *sendData(const uint8_t ep, const uint8_t *const buffer, const uint8_t length) noexcept;
	extern uint8_t *recvData(const uint8_t ep, uint8_t *const buffer, const uint8_t length) noexcept;
} // namespace usb::core

#endif /*USB_CORE__HXX*/
