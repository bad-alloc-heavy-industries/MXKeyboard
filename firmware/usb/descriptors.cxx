// SPDX-License-Identifier: BSD-3-Clause
#include <array>
#include "usb/core.hxx"
#include "usb/descriptors.hxx"
#include "usb/device.hxx"

using namespace usb;
using namespace usb::descriptors;

static const usbDeviceDescriptor_t usbDeviceDesc
{
	sizeof(usbDeviceDescriptor_t),
	usbDescriptor_t::device,
	0x0200, // this is 2.00 in USB's BCD format
	usbClass_t::none,
	uint8_t(subclasses::device_t::none),
	uint8_t(protocols::device_t::none),
	epBufferSize,
	vid,
	pid,
	0x0001, // BCD encoded device version
	1, // Manufacturer string index
	2, // Product string index
	0, // Temporarily do not support a serial number string
	configDescriptorCount // One configuration only
};

static const usbDeviceQualifierDescriptor_t usbDeviceQualifierDesc
{
	sizeof(usbDeviceQualifierDescriptor_t),
	usbDescriptor_t::deviceQualifier,
	0x0200, // This is 2.00 in USB's BCD format
	usbClass_t::none,
	uint8_t(subclasses::device_t::none),
	uint8_t(protocols::device_t::none),
	epBufferSize,
	0, // No other configurations
	0 // reserved field that must be 0.
};

static const std::array<usbConfigDescriptor_t, configDescriptorCount> usbConfigDesc
{{
	{
		sizeof(usbConfigDescriptor_t),
		usbDescriptor_t::configuration,
		sizeof(usbConfigDescriptor_t) + sizeof(usbInterfaceDescriptor_t) +
			sizeof(hid::hidDescriptor_t) + sizeof(hid::reportDescriptor_t) +
			sizeof(usbEndpointDescriptor_t),
		interfaceDescriptorCount,
		1, // This config
		4, // Configuration string index
		usbConfigAttr_t::defaults,
		250 // "500mA max", except we need 1A
	}
}};

static const std::array<usbInterfaceDescriptor_t, interfaceDescriptorCount> usbInterfaceDesc
{{
	{
		sizeof(usbInterfaceDescriptor_t),
		usbDescriptor_t::interface,
		0, // interface index 0
		0, // alternate 0
		1, // one endpoint to the interface
		usbClass_t::hid,
		uint8_t(subclasses::hid_t::bootInterface),
		uint8_t(protocols::hid_t::keyboard),
		0 // No string to describe this interface (for now)
	}
}};

static const std::array<usbEndpointDescriptor_t, endpointDescriptorCount> usbEndpointDesc
{{
	{
		sizeof(usbEndpointDescriptor_t),
		usbDescriptor_t::endpoint,
		endpointAddress(usbEndpointDir_t::controllerIn, 1),
		usbEndpointType_t::interrupt,
		epBufferSize,
		1 // Poll once per frame
	}
}};

static const hid::hidDescriptor_t usbKeyboardDesc
{
	sizeof(hid::hidDescriptor_t) + sizeof(hid::reportDescriptor_t),
	usbDescriptor_t::hid,
	0x0111, // USB HID 1.11 in BCD
	hid::countryCode_t::english,
	hidReportDescriptorCount
};

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
	// Report Count (5)
	hid::items::global_t::reportCount | hid::descriptorSize(1),
	5,
	// Report Size (3)
	hid::items::global_t::reportSize | hid::descriptorSize(1),
	3,
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
	uint8_t(hid::scancode_t::power),
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

static const std::array<hid::reportDescriptor_t, hidReportDescriptorCount> usbKeyboardReportDesc
{{
	{
		usbDescriptor_t::report,
		sizeof(hid::reportDescriptor_t) + usbKeyboardReport.size()
	}
}};

static const std::array<usbMultiPartDesc_t, 6> usbConfigSecs
{{
	{
		sizeof(usbConfigDescriptor_t),
		&usbConfigDesc[0]
	},
	{
		sizeof(usbInterfaceDescriptor_t),
		&usbInterfaceDesc[0]
	},
	{
		sizeof(hid::hidDescriptor_t),
		&usbKeyboardDesc
	},
	{
		sizeof(hid::reportDescriptor_t),
		&usbKeyboardReportDesc
	},
	{
		sizeof(usbEndpointDescriptor_t),
		&usbEndpointDesc[0]
	}
}};

static const std::array<flash_t<usbMultiPartTable_t>, 1> usbConfigDescriptors
{{
	{{usbConfigSecs.begin(), usbConfigSecs.end()}}
}};

static const std::array<usbMultiPartDesc_t, 2> usbHIDSecs
{{
	{
		sizeof(hid::hidDescriptor_t),
		&usbKeyboardDesc
	},
	{
		sizeof(hid::reportDescriptor_t),
		&usbKeyboardReportDesc
	}
}};

static const flash_t<usbMultiPartTable_t> usbHIDDescriptor{{usbHIDSecs.begin(), usbHIDSecs.end()}};

static const std::array<usbMultiPartDesc_t, 2> usbBootReportSecs
{{
	{
		sizeof(hid::reportDescriptor_t),
		&usbKeyboardReportDesc
	},
	{
		usbKeyboardReport.size(),
		usbKeyboardReport.data()
	},
}};

static const std::array<flash_t<usbMultiPartTable_t>, hidReportDescriptorCount> usbReports
{{
	{{usbBootReportSecs.begin(), usbBootReportSecs.end()}}
}};

static const std::array<usbStringDesc_t, stringCount + 1> usbStringDescs
{{
	{{u"\x0904", 1}},
	{{u"bad_alloc Heavy Industries", 26}},
	{{u"MXKeyboard", 10}},
	{{u"", 0}},
	{{u"HID keyboard interface", 22}}
}};

static const std::array<std::array<usbMultiPartDesc_t, 2>, stringCount + 1> usbStringParts
{{
	usbStringDescs[0].asParts(),
	usbStringDescs[1].asParts(),
	usbStringDescs[2].asParts(),
	usbStringDescs[3].asParts(),
	usbStringDescs[4].asParts()
}};

static const std::array<flash_t<usbMultiPartTable_t>, stringCount + 1> usbStrings
{{
	{{usbStringParts[0].begin(), usbStringParts[0].end()}},
	{{usbStringParts[1].begin(), usbStringParts[1].end()}},
	{{usbStringParts[2].begin(), usbStringParts[2].end()}},
	{{usbStringParts[3].begin(), usbStringParts[3].end()}},
	{{usbStringParts[4].begin(), usbStringParts[4].end()}},
}};

using namespace usb::types;
using namespace usb::core;

namespace usb::device
{
	answer_t handleGetDescriptor() noexcept
	{
		if (packet.requestType.dir() == endpointDir_t::controllerOut)
			return {response_t::unhandled, nullptr, 0, memory_t::sram};
		const auto descriptor = packet.value.asDescriptor();

		switch (descriptor.type)
		{
			// Handle device descriptor requests
			case usbDescriptor_t::device:
				return {response_t::data, &usbDeviceDesc, usbDeviceDesc.length, memory_t::flash};
			case usbDescriptor_t::deviceQualifier:
				static_assert(sizeof(usbDeviceQualifierDescriptor_t) == 10);
				return {response_t::data, &usbDeviceQualifierDesc, usbDeviceQualifierDesc.length, memory_t::flash};
			// Handle configuration descriptor requests
			case usbDescriptor_t::configuration:
			{
				if (descriptor.index >= configDescriptorCount)
					break;
				static_assert(sizeof(usbConfigDescriptor_t) == 9);
				static_assert(sizeof(usbInterfaceDescriptor_t) == 9);
				static_assert(sizeof(usbEndpointDescriptor_t) == 7);
				const auto configDescriptor{*usbConfigDescriptors[descriptor.index]};
				epStatusControllerIn[0].isMultiPart(true);
				epStatusControllerIn[0].partNumber = 0;
				epStatusControllerIn[0].partsData = configDescriptor;
				return {response_t::data, nullptr, configDescriptor.totalLength(), memory_t::flash};
			}
			// Handle interface descriptor requests
			case usbDescriptor_t::interface:
			{
				if (descriptor.index >= interfaceDescriptorCount)
					break;
				const auto &interfaceDescriptor{usbInterfaceDesc[descriptor.index]};
				return {response_t::data, &interfaceDescriptor, interfaceDescriptor.length, memory_t::flash};
			}
			// Handle endpoint descriptor requests
			case usbDescriptor_t::endpoint:
			{
				if (descriptor.index >= endpointDescriptorCount)
					break;
				const auto &endpointDescriptor{usbEndpointDesc[descriptor.index]};
				return {response_t::data, &endpointDescriptor, endpointDescriptor.length, memory_t::flash};
			}
			// Handle string requests
			case usbDescriptor_t::string:
			{
				if (descriptor.index >= stringCount)
					break;
				const auto &string{*usbStrings[descriptor.index]};
				epStatusControllerIn[0].isMultiPart(true);
				epStatusControllerIn[0].partNumber = 0;
				epStatusControllerIn[0].partsData = string;
				return {response_t::data, nullptr, string.totalLength(), memory_t::flash};
			}
			default:
				break;
		}
		return {response_t::unhandled, nullptr, 0, memory_t::sram};
	}
} // namespace usb::device
