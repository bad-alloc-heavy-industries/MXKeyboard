// SPDX-License-Identifier: BSD-3-Clause
#ifndef USB_HID__HXX
#define USB_HID__HXX

#include <array>
#include <usb/types.hxx>
#include "constants.hxx"

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

	extern void keyPress(scancode_t key) noexcept;
	extern void keyRelease(scancode_t key) noexcept;
	extern void handleReport() noexcept;

	extern void registerHandlers(uint8_t inEP, uint8_t interface, uint8_t config) noexcept;
} // namespace usb::hid

#endif /*USB_HID__HXX*/
