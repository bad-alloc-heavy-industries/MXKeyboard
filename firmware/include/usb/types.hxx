#ifndef USB_TYPES__HXX
#define USB_TYPES__HXX

#include <cstdint>
#include <tuple>

namespace vals::usb
{
	// Control A register constants
	constexpr static const uint8_t ctrlAUSBEnable{0x80};
	constexpr static const uint8_t ctrlAModeLowSpeed{0x00};
	constexpr static const uint8_t ctrlAModeFullSpeed{0x40};
	constexpr static const uint8_t ctrlAFIFOEnable{0x20};
	constexpr static const uint8_t ctrlAStoreFrameNum{0x10};

	constexpr inline uint8_t ctrlAMaxEP(const uint8_t endpointCount) noexcept
		{ return endpointCount & 0x0FU; }

	// Control B register constants
	constexpr static const uint8_t ctrlBPullUpDuringReset{0x10};
	constexpr static const uint8_t ctrlBRemoteWakeUp{0x04};
	constexpr static const uint8_t ctrlBGlobalNACK{0x02};
	constexpr static const uint8_t ctrlBAttach{0x01};

	// Status register constants
	constexpr static const uint8_t statusReset{0x01};
	constexpr static const uint8_t statusSuspend{0x02};
	constexpr static const uint8_t statusLocalResume{0x04};
	constexpr static const uint8_t statusUpstreamResume{0x08};
	constexpr static const uint8_t statusResume{0x0C};

	// Address register constant
	constexpr static const uint8_t addressMask{0x7F};

	// Interrupt Control A register constants
	constexpr static const uint8_t intCtrlAEnableStall{0x40};
	constexpr static const uint8_t intCtrlAEnableBusError{0x40};
	constexpr static const uint8_t intCtrlAEnableBusEvent{0x40};
	constexpr static const uint8_t intCtrlAEnableSOF{0x80};
	constexpr static const uint8_t intCtrlAClearMask{0x0C};

	// Interrupt Control B register constants
	constexpr static const uint8_t intCtrlBEnableSetupComplete{0x01};
	constexpr static const uint8_t intCtrlBEnableIOComplete{0x02};
	constexpr static const uint8_t intCtrlBClearMask{0xFC};

	// Interrupt Status A register constants
	constexpr static const uint8_t itrStatusStall{0x01};
	constexpr static const uint8_t itrStatusOverflow{0x02};
	constexpr static const uint8_t itrStatusUnderflow{0x04};
	constexpr static const uint8_t itrStatusCRCError{0x08};
	constexpr static const uint8_t itrStatusReset{0x10};
	constexpr static const uint8_t itrStatusResume{0x20};
	constexpr static const uint8_t itrStatusSuspend{0x40};
	constexpr static const uint8_t itrStatusSOF{0x80};

	// Interrupt Status B register constants
	constexpr static const uint8_t itrStatusSetup{0x01};
	constexpr static const uint8_t itrStatusIOComplete{0x02};

	// Endpoint control register constants
	constexpr static const uint8_t usbEPCtrlStall{0x04};
	constexpr static const uint8_t usbEPCtrlItrDisable{0x08};

	// Endpoint status register constants
	constexpr static const uint8_t usbEPStatusDTS{0x01};
	constexpr static const uint8_t usbEPStatusNACK0{0x02};
	constexpr static const uint8_t usbEPStatusNACK1{0x04};
	constexpr static const uint8_t usbEPStatusBank{0x08};
	constexpr static const uint8_t usbEPStatusSetupComplete{0x10};
	constexpr static const uint8_t usbEPStatusIOComplete{0x20};
	constexpr static const uint8_t usbEPStatusStall{0x80};
} // namespace vals::usb

namespace usb
{
	// Reserve space for EP0 In + Out, EP1 In + Out, and EP2 In + Out.
	constexpr static const uint8_t endpointCount{6};
	constexpr static const uint8_t epBufferSize{64};

	constexpr static const uint16_t vid{0x1209};
	constexpr static const uint16_t pid{0xBADA};

	constexpr static const uint8_t configDescriptorCount{1};
	constexpr static const uint8_t interfaceDescriptorCount{1};
	constexpr static const uint8_t endpointDescriptorCount{1};
	constexpr static const uint8_t hidReportDescriptorCount{1};
	constexpr static const uint8_t stringCount{4};
} // namespace usb

namespace usb::descriptors { struct usbMultiPartTable_t; }

namespace usb::types
{
	enum class ctrlState_t
	{
		idle,
		wait,
		dataTX,
		dataRX,
		statusTX,
		statusRX
	};

	enum class deviceState_t
	{
		detached,
		attached,
		powered,
		waiting,
		addressing,
		addressed,
		configured
	};

	enum class response_t
	{
		data,
		zeroLength,
		unhandled,
		stall
	};

	enum class endpointDir_t : uint8_t
	{
		controllerOut = 0x00U,
		controllerIn = 0x80U
	};

	struct usbEP_t final
	{
	private:
		uint8_t value{};

	public:
		usbEP_t() = default;

		void endpoint(const uint8_t num) noexcept
		{
			value &= 0xF0U;
			value |= num & 0x0FU;
		}

		[[nodiscard]] uint8_t endpoint() const noexcept { return value & 0x0FU; }

		void dir(const endpointDir_t dir) noexcept
		{
			value &= 0x7FU;
			value |= uint8_t(dir);
		}

		[[nodiscard]] endpointDir_t dir() const noexcept { return static_cast<endpointDir_t>(value & 0x80U); }
	};

	template<typename buffer_t> struct usbEPStatus_t final
	{
	private:
		uint8_t value{};

	public:
		buffer_t *memBuffer{nullptr};
		usbEP_t ctrl{};
		uint16_t transferCount{};
		// Multi-part fields
		uint8_t partNumber{};
		const usb::descriptors::usbMultiPartTable_t *partsData{nullptr};

		usbEPStatus_t() = default;

		void transferTerminated(const bool terminated) noexcept
		{
			value &= 0xFEU;
			value |= terminated ? 0x01U : 0x00U;
		}

		[[nodiscard]] bool transferTerminated() const noexcept { return value & 0x01U; }

		void needsArming(const bool needed) noexcept
		{
			value &= 0xFDU;
			value |= needed ? 0x02U : 0x00U;
		}

		[[nodiscard]] bool needsArming() const noexcept { return value & 0x02U; }

		void stall(const bool needed) noexcept
		{
			value &= 0xFBU;
			value |= needed ? 0x04U : 0x00U;
		}

		[[nodiscard]] bool stall() const noexcept { return value & 0x04U; }

		void isMultiPart(const bool multiPart) noexcept
		{
			value &= 0xF7U;
			value |= multiPart ? 0x08U : 0x00U;
		}

		[[nodiscard]] bool isMultiPart() const noexcept { return value & 0x08U; }
		void resetStatus() noexcept { value = 0; }
	};

	using answer_t = std::tuple<response_t, const void *, std::size_t>;
} // namespace usb::types

#endif /*USB_TYPES__HXX*/
