#ifndef USB_DESCRIPTORS__HXX
#define USB_DESCRIPTORS__HXX

#include <cstdint>
#include <cstddef>
#include <string_view>
#include "types.hxx"
#include "../flash.hxx"

namespace usb::descriptors
{
	enum class usbDescriptor_t : uint8_t
	{
		invalid = 0x00U,
		device = 0x01U,
		configuration = 0x02U,
		string = 0x03U,
		interface = 0x04U,
		endpoint = 0x05U,
		deviceQualifier = 0x06U,
		otherSpeed = 0x07U,
		interfacePower = 0x08U, // Speed, Power?
		otg = 0x09U,
		debug = 0x0AU,
		interfaceAssociation = 0x0BU,
		security = 0x0CU,
		key = 0x0DU,
		encryptionType = 0x0EU,
		deviceCapability = 0x10U,
		wirelessEndpoint = 0x11U,
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

	struct usbDeviceDescriptor_t final
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

	struct usbDeviceQualifierDescriptor_t final
	{
		uint8_t length;
		usbDescriptor_t descriptorType;
		uint16_t usbVersion;
		usbClass_t deviceClass;
		uint8_t deviceSubClass;
		uint8_t deviceProtocol;
		uint8_t maxPacketSize0;
		uint8_t numOtherConfigurations;
		uint8_t reserved;
	};

	enum class usbConfigAttr_t : uint8_t
	{
		defaults = 0x80U,
		selfPowered = 0x40U,
		remoteWakeup = 0x20U,
		hostNegotiationProto = 0x02U,
		sessionRequestProto = 0x01U
	};

	struct [[gnu::packed]] usbConfigDescriptor_t final
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

	struct usbInterfaceDescriptor_t final
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

	using usbEndpointDir_t = usb::types::endpointDir_t;

	constexpr static const uint8_t endpointDirMask{0x7F};
	constexpr inline uint8_t endpointAddress(const usbEndpointDir_t dir, const uint8_t number) noexcept
		{ return uint8_t(dir) | (number & endpointDirMask); }

	struct [[gnu::packed]] usbEndpointDescriptor_t final
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

		enum class vendor_t : uint8_t
		{
			none = 0
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

		struct [[gnu::packed]] hidDescriptor_t final
		{
			uint8_t length;
			usbDescriptor_t descriptorType;
			uint16_t hidVersion;
			countryCode_t countryCode;
			uint8_t numDescriptors;
		};

		struct [[gnu::packed]] reportDescriptor_t final
		{
			usbDescriptor_t descriptorType;
			uint16_t length;
		};

		enum struct descriptorType_t : uint8_t
		{
			main = 0x00U,
			global = 0x04U,
			local = 0x08U,
			reserved = 0x0CU
		};

		namespace items
		{
			enum struct global_t : uint8_t
			{
				usagePage = 0x00U,
				logicalMinimum = 0x10U,
				logicalMaximum = 0x20U,
				physicalMinimum = 0x30U,
				physicalMaximum = 0x40U,
				unitExponent = 0x50U,
				unit = 0x60U,
				reportSize = 0x70U,
				reportID = 0x80U,
				reportCount = 0x90U,
				push = 0xA0U,
				pop = 0xB0U
			};

			enum struct local_t : uint8_t
			{
				usage = 0x00U,
				usageMinimum = 0x10U,
				usageMaximum = 0x20U,
			};

			enum struct main_t : uint8_t
			{
				input = 0x80U,
				output = 0x90U,
				collection = 0xA0U,
				feaure = 0xB0U,
				endCollection = 0xC0U,
			};

			constexpr inline uint8_t operator |(const global_t desc, const uint8_t size) noexcept
				{ return uint8_t(desc) | uint8_t(descriptorType_t::global) | size; }
			constexpr inline uint8_t operator |(const local_t desc, const uint8_t size) noexcept
				{ return uint8_t(desc) | uint8_t(descriptorType_t::local) | size; }
			constexpr inline uint8_t operator |(const main_t desc, const uint8_t size) noexcept
				{ return uint8_t(desc) | uint8_t(descriptorType_t::main) | size; }
		} // namespace items

		enum struct collectionType_t : uint8_t
		{
			physical = 0,
			application = 1,
			logical = 2,
			report = 3,
			namedArray = 4,
			usageSwitch = 5,
			usageModifier = 6
		};

		enum struct usagePage_t : uint8_t
		{
			genericDesktop = 1,
			keyboard = 7,
			led = 8
		};

		enum struct systemUsage_t : uint8_t
		{
			keyboard = 6
		};

		enum struct led_t : uint8_t
		{
			undefined = 0,
			numLock = 1,
			capsLock = 2,
			scrollLock = 3,
			compose = 4,
			kana = 5,
			power = 6,
			shift = 7
		};

		enum struct input_t : uint8_t
		{
			data = 0,
			array = 0,
			absolute = 0,
			constant = 1,
			variable = 2,
			relative = 3,
		};

		constexpr inline uint8_t operator |(const input_t a, const input_t b) noexcept
			{ return uint8_t(a) | uint8_t(b); }
		constexpr inline uint8_t operator |(const uint8_t a, const input_t b) noexcept
			{ return a | uint8_t(b); }

		constexpr inline uint8_t log2(uint8_t value) noexcept
		{
			uint8_t result = 0;
			if (value <= 0x0FU)
				result += 4, value <<= 4;
			if (value <= 0x3FU)
				result += 2, value <<= 2;
			if (value <= 0x7FU)
				++result;
			return uint8_t(sizeof(uint8_t) * 8U) - result;
		}

		constexpr inline uint8_t descriptorSize(const uint8_t size) noexcept
		{
			if (!size)
				return size;
			else if (size > 4)
				return 0x03U; // BAD..
			return log2(size);
		}
	} // namespace hid

	struct usbMultiPartDesc_t final
	{
		uint8_t length;
		const void *descriptor;
	};
} // namespace usb::descriptors

template<> struct flash_t<usb::descriptors::usbMultiPartDesc_t> final
{
private:
	using T = usb::descriptors::usbMultiPartDesc_t;
	const T *value_;

public:
	constexpr flash_t(const T *const value) noexcept : value_{value} { }
	constexpr flash_t(const flash_t &) noexcept = default;
	constexpr flash_t(flash_t &&) noexcept = default;
	constexpr flash_t &operator =(const flash_t &) noexcept = default;
	constexpr flash_t &operator =(flash_t &&) noexcept = default;

	T operator *() const noexcept
	{
		T result{};
		const auto resultAddr{reinterpret_cast<uint32_t>(&result)};
		const auto valueAddr{reinterpret_cast<uint32_t>(value_)};
		const uint8_t x{RAMPX};
		const uint8_t z{RAMPZ};

		static_assert(sizeof(T) == 3);

		__asm__(R"(
			movw r26, %[result]
			out 0x39, %C[result]
			movw r30, %[value]
			out 0x3B, %C[value]
			elpm r16, Z+
			st X+, r16
			elpm r16, Z+
			st X+, r16
			elpm r16, Z
			st X+, r16
			)" : : [result] "g" (resultAddr), [value] "g" (valueAddr) :
				"r16", "r26", "r27", "r30", "r31"
		);

		RAMPZ = z;
		RAMPX = x;
		return result;
	}

	constexpr std::ptrdiff_t operator -(const flash_t &other) const noexcept
		{ return value_ - other.value_; }

	constexpr flash_t operator +(const size_t offset) const noexcept
		{ return {value_ + offset}; }

	constexpr flash_t &operator ++() noexcept
	{
		++value_;
		return *this;
	}

	T operator[](const size_t offset) const noexcept
		{ return *(*this + offset); }

	constexpr bool operator ==(const flash_t &other) const noexcept
		{ return value_ == other.value_; }
	constexpr bool operator !=(const flash_t &other) const noexcept
		{ return value_ != other.value_; }

	constexpr bool operator >=(const flash_t &other) const noexcept
		{ return value_ >= other.value_; }

	constexpr const T *pointer() const noexcept { return value_; }
};

namespace usb::descriptors
{
	struct usbMultiPartTable_t final
	{
	private:
		flash_t<usbMultiPartDesc_t> _begin;
		flash_t<usbMultiPartDesc_t> _end;

	public:
		constexpr usbMultiPartTable_t() noexcept : _begin{nullptr}, _end{nullptr} { }
		constexpr usbMultiPartTable_t(const usbMultiPartDesc_t *const begin,
			const usbMultiPartDesc_t *const end) noexcept : _begin{begin}, _end{end} { }
		constexpr auto &begin() const noexcept { return _begin; }
		constexpr auto &end() const noexcept { return _end; }
		constexpr auto count() const noexcept { return _end - _begin; }

		auto part(const std::size_t index) const noexcept
		{
			if (_begin + index >= _end)
				return _end;
			return _begin + index;
		}
		auto operator [](const std::size_t index) const noexcept { return part(index); }

		[[nodiscard]] auto totalLength() const noexcept
		{
			// TODO: Convert to std::accumulate() later.
			std::size_t count{};
			for (const auto descriptor : *this)
				count += descriptor.length;
			return count;
		}

		constexpr usbMultiPartTable_t &operator =(const usbMultiPartTable_t &) noexcept = default;
	};
} // namespace usb::descriptors

template<> struct flash_t<usb::descriptors::usbMultiPartTable_t> final
{
private:
	using T = usb::descriptors::usbMultiPartTable_t;
	T value_;

public:
	constexpr flash_t(const T value) noexcept : value_{value} { }

	T operator *() const noexcept
	{
		T result{};
		const auto resultAddr{reinterpret_cast<uint32_t>(&result)};
		const auto valueAddr{reinterpret_cast<uint32_t>(&value_)};
		const uint8_t x{RAMPX};
		const uint8_t z{RAMPZ};

		static_assert(sizeof(T) == 4);

		__asm__(R"(
			movw r26, %[result]
			out 0x39, %C[result]
			movw r30, %[value]
			out 0x3B, %C[value]
			elpm r16, Z+
			st X+, r16
			elpm r16, Z+
			st X+, r16
			elpm r16, Z+
			st X+, r16
			elpm r16, Z
			st X+, r16
			)" : : [result] "g" (resultAddr), [value] "g" (valueAddr) :
				"r16", "r26", "r27", "r30", "r31"
		);

		RAMPZ = z;
		RAMPX = x;
		return result;
	}
};

namespace usb::descriptors
{
	struct [[gnu::packed]] usbStringDesc_t
	{
		uint8_t length;
		usbDescriptor_t descriptorType;
		const char16_t *const string;

		constexpr usbStringDesc_t(const std::u16string_view data) :
			length{uint8_t(baseLength() + (data.length() * 2))},
			descriptorType{usbDescriptor_t::string}, string{data.data()} { }

		constexpr uint8_t baseLength() const noexcept { return sizeof(usbStringDesc_t) - sizeof(char16_t *); }
		constexpr uint8_t stringLength() const noexcept { return length - baseLength(); }

		constexpr auto asParts() const noexcept
		{
			const std::array<usbMultiPartDesc_t, 2> parts
			{{
				{
					baseLength(),
					this
				},
				{
					stringLength(),
					string
				}
			}};
			return parts;
		}
	};
} // namespace usb::descriptors

#endif /*USB_DESCRIPTORS__HXX*/
