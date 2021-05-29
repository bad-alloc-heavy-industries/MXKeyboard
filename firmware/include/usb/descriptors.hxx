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

		enum struct main_t : uint8_t
		{
			data = 0,
			array = 0,
			absolute = 0,
			constant = 1,
			variable = 2,
			relative = 3,
		};

		constexpr inline uint8_t operator |(const main_t a, const main_t b) noexcept
			{ return uint8_t(a) | uint8_t(b); }
		constexpr inline uint8_t operator |(const uint8_t a, const main_t b) noexcept
			{ return a | uint8_t(b); }

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

		enum struct scancode_t : uint8_t
		{
			reserved = 0x00U,
			errorRollOver = 0x01U,
			errorPOSTFail = 0x02U,
			errorUndefined = 0x03U,
			a = 0x04U, b = 0x05U, c = 0x06U, d = 0x07U,
			e = 0x08U, f = 0x09U, g = 0x0AU, h = 0x0BU,
			i = 0x0CU, j = 0x0DU, k = 0x0EU, l = 0x0FU,
			m = 0x10U, n = 0x11U, o = 0x12U, p = 0x13U,
			q = 0x14U, r = 0x15U, s = 0x16U, t = 0x17U,
			u = 0x18U, v = 0x19U, w = 0x1AU, x = 0x1BU,
			y = 0x1CU, z = 0x1DU, _1 = 0x1EU, _2 = 0x1F,
			_3 = 0x20U, _4 = 0x21U, _5 = 0x22U, _6 = 0x23U,
			_7 = 0x24U, _8 = 0x25U, _9 = 0x26U, _0 = 0x27U,
			enter = 0x28U,
			escape = 0x29U,
			backspace = 0x2AU,
			tab = 0x2BU,
			space = 0x2CU,
			dash = 0x2DU,
			equals = 0x2EU,
			leftBracket = 0x2FU,
			rightBracket = 0x30U,
			usBackSlash = 0x31U,
			hash = 0x32U,
			semiColon = 0x33U,
			singleQuote = 0x34U,
			graveAccent = 0x35U,
			comma = 0x36U,
			fullStop = 0x37U,
			forwardSlash = 0x38U,
			capsLock = 0x39U,
			f1 = 0x3AU, f2 = 0x3BU, f3 = 0x3CU, f4 = 0x3DU,
			f5 = 0x3EU, f6 = 0x3FU, f7 = 0x40U, f8 = 0x41U,
			f9 = 0x42U, f10 = 0x43U, f11 = 0x44U, f12 = 0x45U,
			printScreen = 0x46U,
			scrollLock = 0x47U,
			pause = 0x48U,
			insert = 0x49U,
			home = 0x4AU,
			pageUp = 0x4BU,
			_delete = 0x4CU, //deleteForward?
			end = 0x4DU,
			pageDown = 0x4EU,
			rightArrow = 0x4FU,
			leftArrow = 0x50U,
			downArrow = 0x51U,
			upArrow = 0x52U,
			numLock = 0x53U,
			keypadDivide = 0x54U,
			keypadMultiply = 0x55U,
			keypadSubtract = 0x56U,
			keypadAdd = 0x57U,
			keypadEnter = 0x58U,
			keypad1 = 0x59U, keypad2 = 0x5AU, keypad3 = 0x5BU, keypad4 = 0x5CU,
			keypad5 = 0x5DU, keypad6 = 0x5EU, keypad7 = 0x5FU, keypad8 = 0x60U,
			keypad9 = 0x61U, keypad0 = 0x62U,
			keypadPeriod = 0x63U,
			intlBackSlash = 0x64U,
			application = 0x65U,
			power = 0x66U,
			keypadEquals = 0x67U,
			f13 = 0x68U, f14 = 0x69U, f15 = 0x6AU, f16 = 0x6BU,
			f17 = 0x6CU, f18 = 0x6DU, f19 = 0x6EU, f20 = 0x6FU,
			f21 = 0x70U, f22 = 0x71U, f23 = 0x72U, f24 = 0x73U,
			execute = 0x74U,
			help = 0x75U,
			menu = 0x76U,
			select = 0x77U,
			stop = 0x78U,
			again = 0x79U,
			undo = 0x7AU,
			cut = 0x7BU,
			copy = 0x7CU,
			paste = 0x7DU,
			find = 0x7EU,
			mute = 0x7FU,
			volumeUp = 0x80U,
			volumeDown = 0x81U,
			lockingCapsLock = 0x82U,
			lockingNumLock = 0x83U,
			lockingScrollLock = 0x84U,
			keypadComma = 0x85U,
			//keypadEqualsSign?

			sysReq = 0x9AU,
			cancel = 0x9BU,
			clear = 0x9CU,
			prior = 0x9DU,
			_return = 0x9EU,
			separator = 0x9FU,
			_out = 0xA0U,
			oper = 0xA1U,
			clearAgain = 0xA2U,
			crSel = 0xA3U,
			exSel = 0xA4U,

			keypad00 = 0xB0U,
			keypad000 = 0xB1U,
			thousandsSeparator = 0xB2U,
			decimalSeparator = 0xB3U,
			currencyUnit = 0xB4U,
			currencySubUnit = 0xB5U,
			keypadLeftPeren = 0xB6U,
			keypadRightPeren = 0xB7U,
			keypadLeftBrace = 0xB8U,
			keypadRightBrace = 0xB9U,
			keypadTab = 0xBAU,
			keypadBackspace = 0xBBU,
			keypadA = 0xBCU, keypadB = 0xBDU,
			keypadC = 0xBEU, keypadD = 0xBFU,
			keypadE = 0xC0U, keypadF = 0xC1U,
			keypadXOR = 0xC2U,
			keypadCircumflex = 0xC3U,
			keypadPercent = 0xC4U,
			keypadLessThan = 0xC5U,
			keypadGreaterThan = 0xC6U,
			keypadAmpersand = 0xC7U,
			keypadDoubleAmpersand = 0xC8U,
			keypadPipe = 0xC9U,
			keypadDoublePipe = 0xCAU,
			keypadColon = 0xCBU,
			keypadHash = 0xCCU,
			keypadSpace = 0xCDU,
			keypadAtSign = 0xCEU,
			keypadExclamation = 0xCFU,
			keypadMemoryStore = 0xD0U,
			keypadMemoryRecall = 0xD1U,
			keypadMemoryClear = 0xD2U,
			keypadMemoryAdd = 0xD3U,
			keypadMemorySubtract = 0xD4U,
			keypadMemoryMultiply = 0xD5U,
			keypadMemoryDivide = 0xD6U,
			keypadPlusMinus = 0xD7U,
			keypadClear = 0xD8U,
			keypadClearEntry = 0xD9U,
			keypadBinary = 0xDAU,
			keypadOctal = 0xDBU,
			keypadDecimal = 0xDCU,
			keypadHexadecimal = 0xDDU,
			leftControl = 0xE0U,
			leftShift = 0xE1U,
			leftAlt = 0xE2U,
			leftMeta = 0xE3U, // Spec calls this "GUI"
			rightControl = 0xE4U,
			rightShift = 0xE5U,
			rightAlt = 0xE6U,
			rightMeta = 0xE7U, // Spec call this "GUI"
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

	extern const std::array<flash_t<usbMultiPartTable_t>, usb::configDescriptorCount> usbConfigDescriptors;
} // namespace usb::descriptors

#endif /*USB_DESCRIPTORS__HXX*/
