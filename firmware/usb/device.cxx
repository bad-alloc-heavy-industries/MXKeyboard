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
				return {response_t::zeroLength, nullptr, 0};
			case request_t::getDescriptor:
				return handleGetDescriptor();
		}

		return {response_t::unhandled, nullptr, 0};
	}
}

/*!
 * @returns true when the all the data to be read has been retreived,
 * false if there is more left to fetch.
 */
bool usbServiceCtrlEPRead() noexcept
{
	auto &epStatus{epStatusControllerOut[0]};
	auto &ep0{endpoints[0]};
	auto readCount{ep0.CNT};
	// Bounds sanity and then adjust how much is left to transfer
	if (readCount > epStatus.transferCount)
		readCount = epStatus.transferCount;
	epStatus.transferCount -= readCount;
	epStatus.memBuffer = recvData(0, epStatus.memBuffer, readCount);
	// Mark the FIFO contents as done with
	ep0.STATUS &= ~vals::usb::usbEPStatusNACK0;
	return !epStatus.transferCount;
}

/*!
 * @returns true when the data to be transmitted is entirely sent,
 * false if there is more left to send.
 */
bool usbServiceCtrlEPWrite() noexcept
{
	if (epStatusControllerIn[0].transferCount < epBufferSize)
	{
	}

	auto &epStatus{epStatusControllerIn[0]};
	auto &ep0{endpoints[1]};
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
		while (sendAmount)
		{
			const auto &part{epStatus.partsData->part(epStatus.partNumber)};
			auto *const begin{static_cast<const uint8_t *>(part.descriptor)};
			const auto partAmount{[&]() -> uint8_t
			{
				auto *const buffer{static_cast<const uint8_t *>(epStatus.memBuffer)};
				const auto amount{part.length - (buffer - begin)};
				if (amount > sendAmount)
					return sendAmount;
				return amount;
			}()};
			sendAmount -= partAmount;
			epStatus.memBuffer = sendData(0, epStatus.memBuffer, partAmount);
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
	ep0.STATUS ^= vals::usb::usbEPStatusDTS;
	ep0.STATUS &= ~vals::usb::usbEPStatusNACK0;
	return !epStatus.transferCount;
}

namespace usb::device
{
	void handleControlPacket() noexcept
	{
	}
} // namespace usb::device
