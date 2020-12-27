#ifndef USB_TYPES__HXX
#define USB_TYPES__HXX

#include <cstdint>
#include <cstddef>

namespace vals
{
	namespace usb
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
	}
}

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

	extern bool usbHandleStandardRequest() noexcept;
}

#endif /*USB_TYPES__HXX*/
