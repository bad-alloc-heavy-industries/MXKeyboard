// SPDX-License-Identifier: BSD-3-Clause
#include "usb/core.hxx"
#include "usb/device.hxx"

using namespace usb::types;
using namespace usb::core;
void usbHandleStatusCtrlEP() noexcept;

namespace usb::device
{
	setupPacket_t packet;

	answer_t handleStandardRequest() noexcept
	{
		const auto &epStatus{epStatusControllerIn[0]};

		switch (packet.request)
		{
			case request_t::setAddress:
				usbState = deviceState_t::addressing;
				return {response_t::zeroLength, nullptr, 0};
			case request_t::getDescriptor:
				return handleGetDescriptor();
		}

		return {response_t::unhandled, nullptr, 0};
	}
}

namespace usb::device
{
	void handleControlPacket() noexcept
	{
	}
} // namespace usb::device
