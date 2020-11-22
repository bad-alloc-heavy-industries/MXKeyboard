#ifndef USB_DESCRIPTORS__HXX
#define USB_DESCRIPTORS__HXX

#include <cstdint>
#include <cstddef>

namespace usb::descriptors
{
	enum class usbDescriptor_t : uint8_t
	{
		invalid,
		device,
		configuration,
		string,
		interface,
		endpoint,
		deviceQualifier,
		otherSpeed,
		interfaceSpeed,
		otg,
		debug,
		interfaceAssociation,
		hid = 0x21U,
		report = 0x22U,
		physicalDesc = 0x23U
	};

	enum class usbClass_t : uint8_t
	{
		none = 0x00U,
		audio = 0x01U,
		cdcACM = 0x02U,
		hid = 0x03U,
		physical = 0x05U,
		image = 0x06U,
		printer = 0x07U,
		massStorage = 0x08U,
		hub = 0x09U,
		cdcData = 0x0AU,
		smartCard = 0x0BU,
		contentSecurity = 0x0DU,
		video = 0x0EU,
		healthcare = 0x0FU,
		audioVisual = 0x10U,
		billboard = 0x11U,
		typeCBridge = 0x12U,
		diagnostic = 0xDCU,
		wireless = 0xE0U,
		misc = 0xEFU,
		application = 0xFEU,
		vendor = 0xFFU
	};

	struct usbDeviceDescriptor_t
	{
		uint8_t length;
		usbDescriptor_t descriptorType;
		uint16_t usbVersion;
		usbClass_t deviceClass;
		uint8_t deviceSubClass;
		uint8_t deviceProtocol;
		uint8_t maxPacketSize0;
		uint16_t vendorID;
		uint16_t productID;
		uint16_t deviceVersion;
		uint8_t strMfrIndex;
		uint8_t strProductIndex;
		uint8_t strSerialNoIndex;
		uint8_t numConfigurations;
	};

	enum class usbConfigAttr_t : uint8_t
	{
		defaults = 0x80U,
		selfPowered = 0x40U,
		remoteWakeup = 0x20U,
		hostNegotiationProto = 0x02U,
		sessionRequestProto = 0x01U
	};

	struct usbConfigDescriptor_t
	{
		uint8_t length;
		usbDescriptor_t descriptorType;
		uint16_t totalLength;
		uint8_t numInterfaces;
		uint8_t configurationValue;
		uint8_t strConfigurationIndex;
		usbConfigAttr_t attributes;
		uint8_t maxPower;
	};

	struct usbInterfaceDescriptor_t
	{
		uint8_t length;
		usbDescriptor_t descriptorType;
		uint8_t interfaceNumber;
		uint8_t alternateSetting;
		uint8_t numEndpoints;
		usbClass_t interfaceClass;
		uint8_t interfaceSubClass;
		uint8_t interfaceProtocol;
		uint8_t strInterfaceIdx;
	};

	enum class usbEndpointType_t : uint8_t
	{
		control = 0,
		isochronous = 1,
		bulk = 2,
		interrupt = 3
	};

	enum class usbEndpointDir_t : uint8_t
	{
		controllerOut = 0x00U,
		controllerIn = 0x80U
	};

	constexpr static const uint8_t endpointDirMask{0x7F};
	constexpr inline uint8_t endpointAddress(const usbEndpointDir_t dir, const uint8_t number) noexcept
		{ return uint8_t(dir) | (number & endpointDirMask); }

	struct usbEndpointDescriptor_t
	{
		uint8_t length;
		usbDescriptor_t descriptorType;
		uint8_t endpointAddress;
		usbEndpointType_t endpointType;
		uint16_t maxPacketSize;
		uint8_t interval;
	};

	namespace subclasses
	{
		enum class device_t : uint8_t
		{
			none = 0
		};

		enum class hid_t : uint8_t
		{
			none = 0,
			bootInterface = 1
		};
	} // namespace subclasses

	namespace protocols
	{
		enum class device_t : uint8_t
		{
			none = 0
		};

		enum class hid_t : uint8_t
		{
			none = 0,
			keyboard = 1,
			mouse = 2
		};
	} // namespace protocols

	struct usbStringDescBase_t
	{
		uint8_t length;
		uint8_t descriptorType;
	} usbStringDescBase_t;

	namespace hid
	{
		enum class countryCode_t : uint8_t
		{
			notSupported = 0,
			arabic = 1,
			belgian = 2,
			canadianBi = 3,
			canadianFrench = 4,
			czech = 5,
			danish = 6,
			finnish = 7,
			french = 8,
			german = 9,
			greek = 10,
			hebrew = 11,
			hungary = 12,
			iso = 13,
			italian = 14,
			japanese = 15,
			korean = 16,
			latinAmerican = 17,
			dutch = 18,
			norwegian = 19,
			persian = 20,
			polish = 21,
			portuguese = 22,
			russian = 23,
			slovak = 24,
			spanish = 25,
			swissFrench = 27,
			swissGerman = 28,
			swiss = 29,
			taiwanese = 30,
			turkishQ = 31,
			english = 32,
			american = 33,
			balkan = 34,
			turkishF = 35
		};

		struct hidDescriptor_t
		{
			uint8_t length;
			usbDescriptor_t descriptorType;
			uint16_t hidVersion;
			countryCode_t countryCode;
			uint8_t numDescriptors;
		};

		struct reportDescriptor_t
		{
			usbDescriptor_t descriptorType;
			uint16_t length;
		};
	} // namespace hid

	struct usbMultiPartDesc_t
	{
		uint8_t length;
		const void *descriptor;
	};

	struct usbMultiPartTable_t
	{
		uint8_t numDesc;
		const usbMultiPartDesc_t *descriptors;
	} usbMultiPartTable_t;
} // namespace usb::descriptors

#endif /*USB_DESCRIPTORS__HXX*/
