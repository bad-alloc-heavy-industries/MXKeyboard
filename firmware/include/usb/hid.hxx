// SPDX-License-Identifier: BSD-3-Clause
#ifndef USB_HID__HXX
#define USB_HID__HXX

#include <array>
#include "types.hxx"

namespace usb::hid
{
	using namespace usb::descriptors::hid;

	struct bootReport_t final
	{
		uint8_t modifier{};
		uint8_t reserved{};
		std::array<scancode_t, 6> keyCodes{{}};
	};

	extern const hidDescriptor_t usbKeyboardDesc;
	extern const std::array<reportDescriptor_t, hidReportDescriptorCount> usbKeyboardReportDesc;

	extern void init() noexcept;
	extern types::answer_t handleHIDRequest() noexcept;
	extern void handleReport() noexcept;

	extern void keyPress(scancode_t key) noexcept;
	extern void keyRelease(scancode_t key) noexcept;
} // namespace usb::hid

#endif /*USB_HID__HXX*/
