// SPDX-License-Identifier: BSD-3-Clause
#include "usb/core.hxx"
#include "usb/device.hxx"

using namespace usb;
using namespace usb::types;
using namespace usb::core;
void usbHandleStatusCtrlEP() noexcept;

namespace usb::device
{
	setupPacket_t packet;

	answer_t handleStandardRequest() noexcept
	{
		const auto &epStatus{epStatusControllerIn[0]};

		switch (packet.request)
		{
			case request_t::setAddress:
				usbState = deviceState_t::addressing;
				return {response_t::zeroLength, nullptr, 0, memory_t::sram};
			case request_t::getDescriptor:
				return handleGetDescriptor();
		}

		return {response_t::unhandled, nullptr, 0, memory_t::sram};
	}

	/*!
	 * @returns true when the all the data to be read has been retreived,
	 * false if there is more left to fetch.
	 */
	bool readCtrlEP() noexcept
	{
		auto &epStatus{epStatusControllerOut[0]};
		auto &ep0{endpoints[0].controllerOut};
		auto readCount{ep0.CNT};
		// Bounds sanity and then adjust how much is left to transfer
		if (readCount > epStatus.transferCount)
			readCount = epStatus.transferCount;
		epStatus.transferCount -= readCount;
		epStatus.memBuffer = recvData(0, epStatus.memBuffer, readCount);
		// Mark the recv buffer contents as done with
		ep0.CNT = 0;
		ep0.STATUS = 0;
		return !epStatus.transferCount;
	}

	/*!
	 * @returns true when the data to be transmitted is entirely sent,
	 * false if there is more left to send.
	 */
	bool writeCtrlEP() noexcept
	{
		auto &epStatus{epStatusControllerIn[0]};
		auto &ep0{endpoints[0].controllerIn};
		const auto sendCount{[&]() noexcept -> uint8_t
		{
			// Bounds sanity and then adjust how much is left to transfer
			if (epStatus.transferCount < epBufferSize)
				return epStatus.transferCount;
			return epBufferSize;
		}()};
		epStatus.transferCount -= sendCount;

		if (!epStatus.isMultiPart())
			epStatus.memBuffer = sendData(0, epStatus.memBuffer, sendCount);
		else
		{
			if (!epStatus.memBuffer)
				epStatus.memBuffer = epStatus.partsData->part(0).descriptor;
			auto sendAmount{sendCount};
			uint8_t sendOffset{0};
			while (sendAmount)
			{
				const auto &part{epStatus.partsData->part(epStatus.partNumber)};
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
				epStatus.memBuffer = sendData(0, epStatus.memBuffer, partAmount, sendOffset);
				sendOffset += partAmount;
				// Get the buffer back to check if we exhausted it
				auto *const buffer{static_cast<const uint8_t *>(epStatus.memBuffer)};
				if (buffer - begin == part.length &&
						epStatus.partNumber + 1 < epStatus.partsData->count())
					// We exhausted the chunk's buffer, so grab the next chunk
					epStatus.memBuffer = epStatus.partsData->part(++epStatus.partNumber).descriptor;
			}
			if (!epStatus.transferCount)
				epStatus.isMultiPart(false);
		}
		// Mark the buffer as ready to send
		ep0.CNT = sendCount;
		ep0.STATUS &= ~(vals::usb::usbEPStatusNotReady | vals::usb::usbEPStatusNACK0);
		return !epStatus.transferCount;
	}

	void completeSetupPacket() noexcept
	{
		endpoints[0].controllerOut.STATUS &= ~vals::usb::usbEPStatusSetupComplete;
		// If we have no response
		if (!epStatusControllerIn[0].needsArming())
		{
			// But rather need more data
			if (epStatusControllerOut[0].needsArming())
			{
				// <SETUP[0]><OUT[1]><OUT[0]>...<IN[1]>
				usbCtrlState = ctrlState_t::dataRX;
			}
			// We need to stall in answer
			else if (epStatusControllerIn[0].stall())
			{
				auto &ep0{endpoints[0].controllerIn}; // EP0In
				// <SETUP[0]><STALL>
				ep0.CTRL |= vals::usb::usbEPCtrlStall;
				usbCtrlState = ctrlState_t::idle;
			}
		}
		// We have a valid response
		else
		{
			// Is this as part of a multi-part transaction?
			if (packet.requestType.dir() == endpointDir_t::controllerIn)
				// <SETUP[0]><IN[1]><IN[0]>...<OUT[1]>
				usbCtrlState = ctrlState_t::dataTX;
			// Or just a quick answer?
			else
				//  <SETUP[0]><IN[1]>
				usbCtrlState = ctrlState_t::statusTX;
			if (writeCtrlEP())
			{
				if (usbCtrlState == ctrlState_t::dataTX)
					usbCtrlState = ctrlState_t::statusRX;
				else
					usbCtrlState = ctrlState_t::idle;
			}
		}
	}

	void handleSetupPacket() noexcept
	{
		// Read in the new setup packet
		static_assert(sizeof(setupPacket_t) == 8); // Setup packets must be 8 bytes.
		epStatusControllerOut[0].memBuffer = &packet;
		epStatusControllerOut[0].transferCount = sizeof(setupPacket_t);
		if (!readCtrlEP())
		{
			// Truncated transfer.. WTF.
			auto &ep0{endpoints[0].controllerIn}; // Is this stall on EP0 In or Out?
			ep0.CTRL |= vals::usb::usbEPCtrlStall;
			return;
		}

		// Set up EP0 state for a reply of some kind
		//usbDeferalFlags = 0;
		usbCtrlState = ctrlState_t::wait;
		epStatusControllerIn[0].needsArming(false);
		epStatusControllerIn[0].stall(false);
		epStatusControllerIn[0].transferCount = 0;
		epStatusControllerOut[0].needsArming(false);
		epStatusControllerOut[0].stall(false);
		epStatusControllerOut[0].transferCount = 0;

		const auto &[response, data, size, memoryType] = handleStandardRequest();

		epStatusControllerIn[0].stall(response == response_t::stall || response == response_t::unhandled);
		epStatusControllerIn[0].needsArming(response == response_t::data || response == response_t::zeroLength);
		epStatusControllerIn[0].memBuffer = data;
		epStatusControllerIn[0].memoryType(memoryType);
		const uint16_t transferCount = response == response_t::zeroLength ? 0U : size;
		epStatusControllerIn[0].transferCount = std::min(transferCount, packet.length);
		// If the response is whacko, don't do the stupid thing
		if (response == response_t::data && !data && !epStatusControllerIn[0].isMultiPart())
			epStatusControllerIn[0].needsArming(false);
		completeSetupPacket();
		USB.INTFLAGSBCLR = vals::usb::itrStatusSetup;
	}

	void handleControllerOutPacket() noexcept
	{
		// If we're in the data phase
		if (usbCtrlState == ctrlState_t::dataRX)
		{
			if (readCtrlEP())
			{
				// If we now have all the data for the transaction..
				usbCtrlState = ctrlState_t::statusTX;
				// TODO: Handle data and generate status response.
			}
		}
		// If we're in the status phase
		else
			usbCtrlState = ctrlState_t::idle;
		endpoints[0].controllerOut.STATUS &= ~vals::usb::usbEPStatusIOComplete;
	}

	void handleControllerInPacket() noexcept
	{
		if (usbState == deviceState_t::addressing)
		{
			// We just handled an addressing request, and prepared our answer. Before we get a chance
			// to return from the interrupt that caused this chain of events, lets set the device address.
			const auto address{packet.value.asAddress()};

			// Check that the last setup packet was actually a set address request
			if (packet.requestType.type() != setupPacket::request_t::typeStandard ||
				packet.request != request_t::setAddress || address.addrH != 0)
			{
				USB.ADDR &= ~vals::usb::addressMask;
				usbState = deviceState_t::waiting;
			}
			else
			{
				USB.ADDR = address.addrL & vals::usb::addressMask;
				usbState = deviceState_t::addressed;
			}
		}

		// If we're in the data phase
		if (usbCtrlState == ctrlState_t::dataTX)
		{
			if (writeCtrlEP())
			{
				// If we now have all the data for the transaction..
				usbCtrlState = ctrlState_t::statusRX;
				//usbCtrlState = ctrlState_t::idle;
			}
		}
		// Otherwise this was a status phase TX-complete interrupt
		else
			usbCtrlState = ctrlState_t::idle;
		endpoints[0].controllerIn.STATUS &= ~vals::usb::usbEPStatusIOComplete;
	}

	void handleControlPacket() noexcept
	{
		// If we received a packet..
		if (usbPacket.dir() == endpointDir_t::controllerOut)
		{
			const auto status{endpoints[0].controllerOut.STATUS};
			if (status & vals::usb::usbEPStatusSetupComplete)
				handleSetupPacket();
			else
				handleControllerOutPacket();
		}
		else
			handleControllerInPacket();
	}
} // namespace usb::device
