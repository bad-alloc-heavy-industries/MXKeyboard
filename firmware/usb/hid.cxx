// SPDX-License-Identifier: BSD-3-Clause
#include "usb/core.hxx"
#include "usb/device.hxx"
#include "usb/hid.hxx"

using usb::hidReportDescriptorCount;
using namespace usb::types;
using namespace usb::descriptors;
using namespace usb::core;
using usb::device::packet;

namespace usb::hid
{
	const descriptors::hid::hidDescriptor_t usbKeyboardDesc
	{
		sizeof(descriptors::hid::hidDescriptor_t) + sizeof(descriptors::hid::reportDescriptor_t),
		usbDescriptor_t::hid,
		0x0111, // USB HID 1.11 in BCD
		descriptors::hid::countryCode_t::english,
		hidReportDescriptorCount
	};
} // namespace usb::hid

static const std::array<uint8_t, 63> usbKeyboardReport
{{
	// Usage Page (Generic Desktop)
	hid::items::global_t::usagePage | hid::descriptorSize(1),
	uint8_t(hid::usagePage_t::genericDesktop),
	// Usage (Keyboard)
	hid::items::local_t::usage | hid::descriptorSize(1),
	uint8_t(hid::systemUsage_t::keyboard),
	// Collection (Application)
	hid::items::main_t::collection | hid::descriptorSize(1),
	uint8_t(hid::collectionType_t::application),
	// For the first byte of the report
	// Usage Page (KeyCodes)
	hid::items::global_t::usagePage | hid::descriptorSize(1),
	uint8_t(hid::usagePage_t::keyboard),
	// Usage Minimum (Minimum Scancode) = 0xE0
	hid::items::local_t::usageMinimum | hid::descriptorSize(1),
	uint8_t(hid::scancode_t::leftControl),
	// Usage Maximum (Maximum Scancode) = 0xE7
	hid::items::local_t::usageMaximum | hid::descriptorSize(1),
	uint8_t(hid::scancode_t::rightMeta),
	// Logical Minumum = 0
	hid::items::global_t::logicalMinimum | hid::descriptorSize(1),
	0,
	// Logical Maximum = 1
	hid::items::global_t::logicalMaximum | hid::descriptorSize(1),
	1,
	// Report Size (1)
	hid::items::global_t::reportSize | hid::descriptorSize(1),
	1,
	// Report Count (8)
	hid::items::global_t::reportCount | hid::descriptorSize(1),
	8,
	// Input (Data | Variable | Absolute) Modifier byte
	hid::items::main_t::input | hid::descriptorSize(1),
	hid::main_t::data | hid::main_t::variable | hid::main_t::absolute,
	// Report Count (1)
	hid::items::global_t::reportCount | hid::descriptorSize(1),
	1,
	// Report Size (8)
	hid::items::global_t::reportSize | hid::descriptorSize(1),
	8,
	hid::items::main_t::input | hid::descriptorSize(1),
	0U | hid::main_t::constant,
	// Report Count (3)
	hid::items::global_t::reportCount | hid::descriptorSize(1),
	3,
	// Report Size (1)
	hid::items::global_t::reportSize | hid::descriptorSize(1),
	1,
	// Usage Page (LEDs)
	hid::items::global_t::usagePage | hid::descriptorSize(1),
	uint8_t(hid::usagePage_t::led),
	// Usage Minimum (Minimum LED) = 0x01
	hid::items::local_t::usageMinimum | hid::descriptorSize(1),
	uint8_t(hid::led_t::numLock),
	// Usage Maximum (Maximum LED) = 0x03
	hid::items::local_t::usageMaximum | hid::descriptorSize(1),
	uint8_t(hid::led_t::scrollLock),
	// Output (Data | Variable | Absolute) LED report
	hid::items::main_t::output | hid::descriptorSize(1),
	hid::main_t::data | hid::main_t::variable | hid::main_t::absolute,
	// Report Count (1)
	hid::items::global_t::reportCount | hid::descriptorSize(1),
	1,
	// Report Size (5)
	hid::items::global_t::reportSize | hid::descriptorSize(1),
	5,
	// Output (Constant)
	hid::items::main_t::output | hid::descriptorSize(1),
	0U | hid::main_t::constant,
	// Report Count (6)
	hid::items::global_t::reportCount | hid::descriptorSize(1),
	6,
	// Report Size (8)
	hid::items::global_t::reportSize | hid::descriptorSize(1),
	8,
	// Usage Page (KeyCodes)
	hid::items::global_t::usagePage | hid::descriptorSize(1),
	uint8_t(hid::usagePage_t::keyboard),
	// Usage Minimum (Minimum Scancode) = 0
	hid::items::local_t::usageMinimum | hid::descriptorSize(1),
	uint8_t(hid::scancode_t::reserved),
	// Usage Maximum (Maximum Scancode) = 0x65
	hid::items::local_t::usageMaximum | hid::descriptorSize(1),
	uint8_t(hid::scancode_t::application),
	// Logical Minumum = 0
	hid::items::global_t::logicalMinimum | hid::descriptorSize(1),
	0,
	// Logical Maximum = 101
	hid::items::global_t::logicalMaximum | hid::descriptorSize(1),
	101,
	// Input (Data | Array) Scancodes
	hid::items::main_t::input | hid::descriptorSize(1),
	hid::main_t::data | hid::main_t::array,
	// End Collection
	hid::items::main_t::endCollection | hid::descriptorSize(0)
}};

namespace usb::hid
{
	const std::array<hid::reportDescriptor_t, hidReportDescriptorCount> usbKeyboardReportDesc
	{{
		{
			usbDescriptor_t::report,
			sizeof(hid::reportDescriptor_t) + usbKeyboardReport.size()
		}
	}};
}

static const std::array<usbMultiPartDesc_t, 2> usbHIDSecs
{{
	{
		sizeof(hid::hidDescriptor_t),
		&usb::hid::usbKeyboardDesc
	},
	{
		sizeof(hid::reportDescriptor_t),
		&usb::hid::usbKeyboardReportDesc
	}
}};

static const flash_t<usbMultiPartTable_t> usbHIDDescriptor{{usbHIDSecs.begin(), usbHIDSecs.end()}};

namespace usb::hid
{
	answer_t handleGetDescriptor() noexcept
	{
		if (packet.requestType.dir() == endpointDir_t::controllerOut)
			return {response_t::unhandled, nullptr, 0, memory_t::sram};
		const auto descriptor = packet.value.asDescriptor();

		switch (descriptor.type)
		{
			case usbDescriptor_t::hid:
			{
				if (descriptor.index >= 1)
					break;
				const auto descriptor{*usbHIDDescriptor};
				epStatusControllerIn[0].isMultiPart(true);
				epStatusControllerIn[0].partNumber = 0;
				epStatusControllerIn[0].partsData = descriptor;
				return {response_t::data, nullptr, descriptor.totalLength(), memory_t::flash};
			}
			case usbDescriptor_t::report:
			{
				if (descriptor.index >= hidReportDescriptorCount)
					break;
				if (descriptor.index == 0)
					return {response_t::data, usbKeyboardReport.data(), usbKeyboardReport.size(), memory_t::flash};
				return {response_t::stall, nullptr, 0, memory_t::sram};
			}
			default:
				break;
		}
		return {response_t::unhandled, nullptr, 0, memory_t::sram};
	}

	answer_t handleHIDRequest() noexcept
	{
		if (packet.request == request_t::getDescriptor)
			return handleGetDescriptor();
	}
} // namespace usb::hid
