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

	struct [[gnu::packed]] endpointCtrl_t final
	{
		static_assert(sizeof(::USB_EP_t) == 8);

		::USB_EP_t controllerOut;
		::USB_EP_t controllerIn;
	};

	extern deviceState_t usbState;
	extern usbEP_t usbPacket;
	extern ctrlState_t usbCtrlState;

	extern std::array<usbEPStatus_t<const void>, usb::endpointCount> epStatusControllerIn;
	extern std::array<usbEPStatus_t<void>, usb::endpointCount> epStatusControllerOut;

	extern std::array<endpointCtrl_t, usb::endpointCount> endpoints;
	extern std::array<std::array<uint8_t, usb::epBufferSize>, usb::endpointCount * 2> epBuffer;

	extern const void *sendData(uint8_t ep, const void *bufferPtr, uint8_t length, uint8_t offset = 0) noexcept;
	extern void *recvData(uint8_t ep, void *buffer, uint8_t length) noexcept;

	enum class epReset_t : uint8_t
	{
		all,
		user
	};

	extern void resetEPs(epReset_t what) noexcept;

	extern bool readEP(uint8_t ep) noexcept;
	extern bool writeEP(uint8_t ep) noexcept;
	extern void pauseWriteEP(uint8_t ep) noexcept;
} // namespace usb::core

#endif /*USB_CORE__HXX*/
