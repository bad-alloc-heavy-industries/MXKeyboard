// SPDX-License-Identifier: BSD-3-Clause
#ifndef USB_HID__HXX
#define USB_HID__HXX

#include <array>
#include "types.hxx"

namespace usb::hid
{
	using namespace usb::descriptors::hid;

	extern const hidDescriptor_t usbKeyboardDesc;
	extern const std::array<reportDescriptor_t, hidReportDescriptorCount> usbKeyboardReportDesc;

	extern types::answer_t handleHIDRequest() noexcept;
} // namespace usb::hid

#endif /*USB_HID__HXX*/
