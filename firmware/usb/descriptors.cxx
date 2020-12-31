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
		0, // one endpoint to the interface
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
	sizeof(hid::hidDescriptor_t),
	usbDescriptor_t::hid,
	0x0111, // USB HID 1.11 in BCD
	hid::countryCode_t::english,
	hidReportDescriptorCount
};

static const  std::array<hid::reportDescriptor_t, hidReportDescriptorCount>usbKeyboardReportDesc
{{
	{
		usbDescriptor_t::report,
		{}
	}
}};

static const std::array<usbMultiPartDesc_t, 5> usbConfigSecs
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
		&usbKeyboardDesc
	},
	{
		sizeof(usbEndpointDescriptor_t),
		&usbEndpointDesc[0]
	}
}};

static const std::array<usbMultiPartTable_t, 1> usbConfigDescriptors
{{
	{usbConfigSecs.begin(), usbConfigSecs.end()}
}};

static const std::array<usbStringDesc_t, stringCount> usbStrings
{{
	{
		0,
		usbDescriptor_t::string,
		u"bad_alloc Heavy Industries"
	},
	{
		0,
		usbDescriptor_t::string,
		u"MXKeyboard"
	},
	{
		0,
		usbDescriptor_t::string,
		u""
	},
	{
		0,
		usbDescriptor_t::string,
		u"HID keyboard interface"
	}
}};

using namespace usb::types;
using namespace usb::core;

namespace usb::device
{
	answer_t handleGetDescriptor() noexcept
	{
		if (packet.requestType.dir() == endpointDir_t::controllerOut)
			return {response_t::unhandled, nullptr, 0};
		const auto descriptor = packet.value.asDescriptor();

		switch (descriptor.type)
		{
			// Handle device descriptor requests
			case usbDescriptor_t::device:
				return {response_t::data, &usbDeviceDesc, usbDeviceDesc.length};
			case usbDescriptor_t::deviceQualifier:
				return {response_t::data, &usbDeviceQualifierDesc, usbDeviceQualifierDesc.length};
			// Handle configuration descriptor requests
			case usbDescriptor_t::configuration:
			{
				if (descriptor.index >= configDescriptorCount)
					break;
				const auto &configDescriptor{usbConfigDescriptors[descriptor.index]};
				epStatusControllerIn[0].isMultiPart(true);
				epStatusControllerIn[0].partNumber = 0;
				epStatusControllerIn[0].partsData = &configDescriptor;
				return {response_t::data, nullptr, configDescriptor.totalLength()};
			}
			// Handle interface descriptor requests
			case usbDescriptor_t::interface:
			{
				if (descriptor.index >= interfaceDescriptorCount)
					break;
				const auto &interfaceDescriptor{usbInterfaceDesc[descriptor.index]};
				return {response_t::data, &interfaceDescriptor, interfaceDescriptor.length};
			}
			// Handle endpoint descriptor requests
			case usbDescriptor_t::endpoint:
			{
				if (descriptor.index >= endpointDescriptorCount)
					break;
				const auto &endpointDescriptor{usbEndpointDesc[descriptor.index]};
				return {response_t::data, &endpointDescriptor, endpointDescriptor.length};
			}
			// Handle string requests
			case usbDescriptor_t::string:
			{
				if (descriptor.index >= stringCount)
					break;
				const auto &string{usbStrings[descriptor.index]};
				return {response_t::data, &string, string.length};
			}
			default:
				break;
		}
		return {response_t::unhandled, nullptr, 0};
	}
} // namespace usb::device
