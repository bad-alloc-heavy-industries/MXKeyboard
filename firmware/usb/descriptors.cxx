// SPDX-License-Identifier: BSD-3-Clause
#include <array>
#include <usb/descriptors.hxx>
#include "constants.hxx"
#include "usb/hid.hxx"

using namespace std::literals::string_view_literals;

namespace usb::descriptors
{
	const usbDeviceDescriptor_t deviceDescriptor
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
		configsCount // One configuration only
	};

	static const std::array<usbConfigDescriptor_t, configsCount> configDescs
	{{
		{
			sizeof(usbConfigDescriptor_t),
			usbDescriptor_t::configuration,
			sizeof(usbConfigDescriptor_t) + sizeof(usbInterfaceDescriptor_t) +
				sizeof(hid::hidDescriptor_t) + sizeof(hid::reportDescriptor_t) +
				sizeof(usbEndpointDescriptor_t),
			interfaceCount,
			1, // This config
			4, // Configuration string index
			usbConfigAttr_t::defaults,
			250 // "500mA max", except we need 1A
		}
	}};

	const std::array<usbInterfaceDescriptor_t, interfaceDescriptorCount> interfaceDescriptors
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

	const std::array<usbEndpointDescriptor_t, endpointDescriptorCount> endpointDescriptors
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

	static const std::array<usbMultiPartDesc_t, 5> configSecs
	{{
		{
			sizeof(usbConfigDescriptor_t),
			&configDescs[0]
		},
		{
			sizeof(usbInterfaceDescriptor_t),
			&interfaceDescriptors[0]
		},
		{
			sizeof(hid::hidDescriptor_t),
			&usb::hid::usbKeyboardDesc
		},
		{
			sizeof(hid::reportDescriptor_t),
			&usb::hid::usbKeyboardReportDesc
		},
		{
			sizeof(usbEndpointDescriptor_t),
			&endpointDescriptors[0]
		}
	}};

	const std::array<flash_t<usbMultiPartTable_t>, configsCount> configDescriptors
	{{
		{{configSecs.begin(), configSecs.end()}}
	}};

	static const std::array<usbStringDesc_t, stringCount> stringDescs
	{{
		{{u"bad_alloc Heavy Industries", 26}},
		{{u"MXKeyboard", 10}},
		{{u"", 0}},
		{{u"HID keyboard interface", 22}}
	}};

	static const std::array<std::array<usbMultiPartDesc_t, 2>, stringCount> stringParts
	{{
		stringDescs[0].asParts(),
		stringDescs[1].asParts(),
		stringDescs[2].asParts(),
		stringDescs[3].asParts()
	}};

	const std::array<flash_t<usbMultiPartTable_t>, stringCount> strings
	{{
		{{stringParts[0].begin(), stringParts[0].end()}},
		{{stringParts[1].begin(), stringParts[1].end()}},
		{{stringParts[2].begin(), stringParts[2].end()}},
		{{stringParts[3].begin(), stringParts[3].end()}}
	}};
} // namespace usb::descriptors
