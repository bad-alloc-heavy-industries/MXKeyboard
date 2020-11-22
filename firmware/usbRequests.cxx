#include <array>
#include "usbTypes.hxx"
#include "usbDescriptors.hxx"

using namespace usb::descriptors;

static const usbDeviceDescriptor_t usbDeviceDesc
{
	sizeof(usbDeviceDescriptor_t),
	usbDescriptor_t::device,
	0x0200, // this is 2.00 in USB's BCD format
	usbClass_t::none,
	uint8_t(subclasses::device_t::none),
	uint8_t(protocols::device_t::none),
	usb::epBufferSize,
	usb::vid,
	usb::pid,
	0x0001, // BCD encoded device version
	1, // Manufacturer string index
	2, // Product string index
	0, // Temporarily do not support a serial number string
	usb::configDescriptorCount // One configuration only
};

static const std::array<usbConfigDescriptor_t, usb::configDescriptorCount> usbConfigDesc
{
	{
		sizeof(usbConfigDescriptor_t),
		usbDescriptor_t::configuration,
		sizeof(usbConfigDescriptor_t) + sizeof(usbInterfaceDescriptor_t) +
			sizeof(hid::hidDescriptor_t) + sizeof(hid::reportDescriptor_t) +
			sizeof(usbEndpointDescriptor_t),
		usb::interfaceDescriptorCount,
		1, // This config
		3, // Configuration string index
		usbConfigAttr_t::defaults,
		250 // "500mA max", except we need 1.5A
	}
};

static const std::array<usbInterfaceDescriptor_t, usb::interfaceDescriptorCount> usbInterfaceDesc
{
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
};

static const std::array<usbEndpointDescriptor_t, usb::endpointDescriptorCount> usbEndpointDesc
{
	{
		sizeof(usbEndpointDescriptor_t),
		usbDescriptor_t::endpoint,
		endpointAddress(usbEndpointDir_t::controllerIn, 1),
		usbEndpointType_t::interrupt,
		usb::epBufferSize,
		1 // Poll once per frame
	}
};

static const hid::hidDescriptor_t usbKeyboardDesc
{
	sizeof(hid::hidDescriptor_t),
	usbDescriptor_t::hid,
	0x0111, // USB HID 1.11 in BCD
	hid::countryCode_t::english,
	usb::hidReportDescriptorCount
};

static const  std::array<hid::reportDescriptor_t, usb::hidReportDescriptorCount>usbKeyboardReportDesc
{
	{
		usbDescriptor_t::report,
		{}
	}
};

static const usbMultiPartDesc_t usbConfigSecs[]
{
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
};

bool usbHandleStandardRequest() noexcept
{
}
