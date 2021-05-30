// SPDX-License-Identifier: BSD-3-Clause
#include <array>
#include "MXKeyboard.hxx"
#include "interrupts.hxx"
#include "indexedIterator.hxx"
#include "usb/core.hxx"
#include "usb/device.hxx"
#include "usb/hid.hxx"

/*!
 * "USB IN" transfers == Controller In
 * "USB OUT" transfers == Controller Out
 * We are always the Peripheral, so this is unabiguous.
 */

using namespace usb::types;

namespace usb::core
{
	// These are organised EPxOut, EPxIn, etc
	alignas(2) std::array<endpointCtrl_t, endpointCount> endpoints{};
	std::array<std::array<uint8_t, usb::epBufferSize>, usb::endpointCount * 2> epBuffer{};

	deviceState_t usbState;
	usbEP_t usbPacket;
	bool usbSuspended;
	ctrlState_t usbCtrlState;
	uint8_t usbDeferalFlags;

	std::array<usbEPStatus_t<const void>, endpointCount> epStatusControllerIn;
	std::array<usbEPStatus_t<void>, endpointCount> epStatusControllerOut;
}

using namespace usb::core;

void usbInit() noexcept
{
	// Enable the USB peripheral
	USB.CTRLB &= ~vals::usb::ctrlBAttach;
	USB.CTRLA = vals::usb::ctrlAUSBEnable | vals::usb::ctrlAModeFullSpeed | vals::usb::ctrlAMaxEP(2);

	for (auto &[i, endpoint] : utility::indexedIterator_t{endpoints})
	{
		endpoint->controllerOut.DATAPTR = reinterpret_cast<std::uintptr_t>(epBuffer[i << 1U].data());
		endpoint->controllerOut.CNT = 0;
		endpoint->controllerOut.STATUS = vals::usb::usbEPStatusNACK0 | vals::usb::usbEPStatusNACK1;
		endpoint->controllerIn.DATAPTR = reinterpret_cast<std::uintptr_t>(epBuffer[(i << 1U) + 1U].data());
		endpoint->controllerIn.CNT = 0;
		endpoint->controllerIn.STATUS = vals::usb::usbEPStatusNACK0 | vals::usb::usbEPStatusNACK1;
	}

	// Configure EP0Out as our primary control input EP
	endpoints[0].controllerOut.CTRL = USB_EP_TYPE_CONTROL_gc | USB_EP_BUFSIZE_64_gc | vals::usb::usbEPCtrlItrDisable;
	// Configure EP0In as our primary control output endpoint
	endpoints[0].controllerIn.CTRL = USB_EP_TYPE_CONTROL_gc | USB_EP_BUFSIZE_64_gc | vals::usb::usbEPCtrlItrDisable;
	// Configure EP1Out to perminantly stall.
	endpoints[1].controllerOut.CTRL = USB_EP_TYPE_DISABLE_gc | vals::usb::usbEPCtrlStall;
	// Configure EP1In
	endpoints[1].controllerIn.CTRL = USB_EP_TYPE_DISABLE_gc | vals::usb::usbEPCtrlStall;

	USB.EPPTR = reinterpret_cast<std::uintptr_t>(endpoints.data());
	// Reset all USB interrupts
	USB.INTCTRLA &= vals::usb::intCtrlAClearMask;
	USB.INTCTRLB &= vals::usb::intCtrlBClearMask;
	// Ensure the device address is 0
	USB.ADDR &= ~vals::usb::addressMask;

	// Enable the USB reset interrupt
	USB.INTCTRLA |= vals::usb::intCtrlAEnableBusEvent | USB_INTLVL_LO_gc;

	// Initialise the state machine
	usbState = deviceState_t::detached;
	usbCtrlState = ctrlState_t::idle;
	usbDeferalFlags = 0;
	USB.CTRLB = vals::usb::ctrlBAttach;
}

namespace usb::core
{
	void reset()
	{
		resetEPs(epReset_t::all);

		// Once we get done, idle the peripheral
		USB.ADDR &= ~vals::usb::addressMask;
		usbState = deviceState_t::attached;
		USB.INTCTRLA |= vals::usb::intCtrlAEnableBusEvent;
		USB.INTCTRLB |= vals::usb::intCtrlBEnableIOComplete | vals::usb::intCtrlBEnableSetupComplete;
		endpoints[0].controllerOut.CTRL &= ~vals::usb::usbEPCtrlItrDisable;
		endpoints[0].controllerIn.CTRL &= ~vals::usb::usbEPCtrlItrDisable;
		USB.INTFLAGSACLR = vals::usb::itrStatusReset;
	}

	void resetEPs(const epReset_t what) noexcept
	{
		for (auto &[i, endpoint] : utility::indexedIterator_t{endpoints})
		{
			if (what == epReset_t::user && i == 0)
				continue;

			endpoint->controllerOut.CTRL |= vals::usb::usbEPCtrlItrDisable;
			endpoint->controllerOut.CTRL &= ~vals::usb::usbEPCtrlStall;
			if (i != 0)
			{
				endpoint->controllerOut.CTRL &= ~vals::usb::usbEPCtrlTypeMask;
				endpoint->controllerIn.CTRL &= ~vals::usb::usbEPCtrlTypeMask;
				endpoint->controllerOut.STATUS |= vals::usb::usbEPStatusNACK0 | vals::usb::usbEPStatusNACK1;
				endpoint->controllerOut.STATUS &= ~(vals::usb::usbEPStatusNotReady | vals::usb::usbEPStatusStall |
					vals::usb::usbEPStatusIOComplete | vals::usb::usbEPStatusSetupComplete);
			}
			else
				endpoint->controllerOut.STATUS &= ~(vals::usb::usbEPStatusNACK0);

			endpoint->controllerIn.CTRL |= vals::usb::usbEPCtrlItrDisable;
			endpoint->controllerIn.CTRL &= ~vals::usb::usbEPCtrlStall;
			endpoint->controllerIn.STATUS |= vals::usb::usbEPStatusNACK0 | vals::usb::usbEPStatusNACK1;
			endpoint->controllerIn.STATUS &= ~(vals::usb::usbEPStatusNotReady | vals::usb::usbEPStatusStall |
				vals::usb::usbEPStatusIOComplete | vals::usb::usbEPStatusSetupComplete);
		}
	}

	void wakeup()
	{
		usbSuspended = false;
		//USB.CTRLB |= vals::usb::ctrlBRemoteWakeUp;
		USB.INTFLAGSACLR = vals::usb::itrStatusResume;
	}

	void suspend()
	{
		usbSuspended = true;
		USB.INTFLAGSACLR = vals::usb::itrStatusSuspend;
	}

	const void *sendData(const uint8_t ep, const void *const bufferPtr, const uint8_t length,
		const uint8_t offset) noexcept
	{
		auto *const inBuffer{static_cast<const uint8_t *>(bufferPtr)};
		auto *const outBuffer{epBuffer[(ep << 1) + 1].data() + offset};
		// Copy the data to tranmit from the user buffer
		if (epStatusControllerIn[ep].memoryType() == memory_t::sram)
		{
			for (uint8_t i{0}; i < length; ++i)
				outBuffer[i] = inBuffer[i];
		}
		else
		{
			flash_t<char *> flashBuffer{static_cast<const char *>(bufferPtr)};
			for (uint8_t i{0}; i < length; ++i)
			{
				outBuffer[i] = *flashBuffer;
				++flashBuffer;
			}
		}
		return inBuffer + length;
	}

	void *recvData(const uint8_t ep, void *const bufferPtr, const uint8_t length) noexcept
	{
		const auto *const inBuffer{epBuffer[(ep << 1)].data()};
		auto *const outBuffer{static_cast<uint8_t *>(bufferPtr)};
		// Copy the received data to the user buffer
		for (uint8_t i{0}; i < length; ++i)
			outBuffer[i] = inBuffer[i];
		return outBuffer + length;
	}

	/*!
	 * @returns true when the all the data to be read has been retreived,
	 * false if there is more left to fetch.
	 */
	bool readEP(const uint8_t ep) noexcept
	{
		auto &epStatus{epStatusControllerOut[ep]};
		auto &epCtrl{endpoints[ep].controllerOut};
		auto readCount{epCtrl.CNT};
		// Bounds sanity and then adjust how much is left to transfer
		if (readCount > epStatus.transferCount)
			readCount = epStatus.transferCount;
		epStatus.transferCount -= readCount;
		epStatus.memBuffer = recvData(ep, epStatus.memBuffer, readCount);
		// Mark the recv buffer contents as done with
		epCtrl.CNT = 0;
		epCtrl.STATUS = 0;
		return !epStatus.transferCount;
	}

	/*!
	 * @returns true when the data to be transmitted is entirely sent,
	 * false if there is more left to send.
	 */
	bool writeEP(const uint8_t ep) noexcept
	{
		auto &epStatus{epStatusControllerIn[ep]};
		auto &epCtrl{endpoints[ep].controllerIn};
		const auto sendCount{[&]() noexcept -> uint8_t
		{
			// Bounds sanity and then adjust how much is left to transfer
			if (epStatus.transferCount < epBufferSize)
				return epStatus.transferCount;
			return epBufferSize;
		}()};
		epStatus.transferCount -= sendCount;

		if (!epStatus.isMultiPart())
			epStatus.memBuffer = sendData(ep, epStatus.memBuffer, sendCount);
		else
		{
			if (!epStatus.memBuffer)
				epStatus.memBuffer = (*epStatus.partsData.part(0)).descriptor;
			auto sendAmount{sendCount};
			uint8_t sendOffset{0};
			while (sendAmount)
			{
				const auto part{*epStatus.partsData.part(epStatus.partNumber)};
				auto *const begin{static_cast<const uint8_t *>(part.descriptor)};
				const auto partAmount{[&]() -> uint8_t
				{
					auto *const buffer{static_cast<const uint8_t *>(epStatus.memBuffer)};
					const auto amount{part.length - uint8_t(buffer - begin)};
					if (amount > sendAmount)
						return sendAmount;
					return amount;
				}()};
				sendAmount -= partAmount;
				epStatus.memBuffer = sendData(ep, epStatus.memBuffer, partAmount, sendOffset);
				sendOffset += partAmount;
				// Get the buffer back to check if we exhausted it
				auto *const buffer{static_cast<const uint8_t *>(epStatus.memBuffer)};
				if (uint8_t(buffer - begin) == part.length &&
						epStatus.partNumber + 1 < epStatus.partsData.count())
					// We exhausted the chunk's buffer, so grab the next chunk
					epStatus.memBuffer = (*epStatus.partsData.part(++epStatus.partNumber)).descriptor;
			}
			if (!epStatus.transferCount)
				epStatus.isMultiPart(false);
		}
		// Mark the buffer as ready to send
		epCtrl.CNT = sendCount;
		epCtrl.STATUS &= ~(vals::usb::usbEPStatusNotReady | vals::usb::usbEPStatusNACK0);
		return !epStatus.transferCount;
	}

	template<endpointDir_t direction> void handlePacket(const uint8_t endpoint)
	{
		const auto status{[=]()
		{
			if constexpr (direction == endpointDir_t::controllerOut)
				return endpoints[endpoint].controllerOut.STATUS;
			else
				return endpoints[endpoint].controllerIn.STATUS;
		}()};

		if ((status & vals::usb::usbEPStatusIOComplete) ||
			(direction == endpointDir_t::controllerOut &&
			(status & vals::usb::usbEPStatusSetupComplete)))
		{
			usbPacket.endpoint(endpoint);
			usbPacket.dir(direction);

			if (endpoint == 0)
				usb::device::handleControlPacket();
			else if (usb::device::activeConfig == 1 && endpoint == 1 && direction == endpointDir_t::controllerIn)
				usb::hid::handleReport();
		}
	}
} // namespace usb::core

void usbBusEvtIRQ() noexcept
{
	const auto intCtrl{USB.INTCTRLA};
	const auto status{USB.INTFLAGSASET};

	if (usbState == deviceState_t::attached)
		usbState = deviceState_t::powered;

	if ((status & vals::usb::itrStatusResume) && (intCtrl & vals::usb::intCtrlAEnableBusEvent))
		usb::core::wakeup();
	/*else if (usbSuspended)
		return;*/

	if ((status & vals::usb::itrStatusReset) && (intCtrl & vals::usb::intCtrlAEnableBusEvent))
	{
		usb::core::reset();
		usbState = deviceState_t::waiting;
		return;
	}
	else if ((status & vals::usb::itrStatusSuspend) && (intCtrl & vals::usb::intCtrlAEnableBusEvent))
		usb::core::suspend();

#if 0
	if ((status & vals::usb::itrStatusSOF) && (intCtrl & vals::usb::intCtrlAEnableSOF))
	{
		if (usbStatusTimeout)
			--usbStatusTimeout;
		else
			usb::device::handleStatusPhase();
		USB.INTFLAGSACLR = vals::usb::itrStatusSOF;
	}
#endif

	// TODO: Handle other events we care about here.
}

void usbIOCompIRQ() noexcept
{
	USB.INTFLAGSBCLR = vals::usb::itrStatusIOComplete;

	if (usbState == deviceState_t::detached ||
		usbState == deviceState_t::attached ||
		usbState == deviceState_t::powered)
	{
		USB.INTFLAGSBCLR = vals::usb::itrStatusSetup;
		return;
	}

	for (uint8_t endpoint{}; endpoint < /*usb::endpointCount*/1; ++endpoint)
	{
		usb::core::handlePacket<endpointDir_t::controllerIn>(endpoint);
		usb::core::handlePacket<endpointDir_t::controllerOut>(endpoint);
	}
}
