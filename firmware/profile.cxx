// SPDX-License-Identifier: BSD-3-Clause
#include "indexSequence.hxx"
#include "profile.hxx"

using mxKeyboard::keyMatrix::keyType_t;
using mxKeyboard::profile::flashPart_t;

constexpr static uint16_t flashPageSize{512U};
constexpr static auto flashPageMask{flashPageSize - 1U};
constexpr static uint16_t eepromPageSize{32U};
constexpr static auto eepromPageMask{eepromPageSize - 1U};

template<> struct flash_t<flashPart_t> final
{
private:
	const void *value_;

	[[gnu::noinline]]
	static void readToRAM(const uint32_t flashAddr, const uint32_t memoryAddr, const uint16_t count) noexcept
	{
		__asm__(R"(
			movw r26, %[memory]
			out 0x39, %C[memory]
			movw r30, %[flash]
			out 0x3B, %C[flash]
			movw r24, %[count]
			clz
loop%=:
			breq loopDone%=
			elpm r16, Z+
			st X+, r16
			sbiw r24, 1
			rjmp loop%=
loopDone%=:
			)" : : [memory] "r" (memoryAddr), [flash] "r" (flashAddr), [count] "r" (count) :
				"r16", "r24", "r25", "r26", "r27", "r30", "r31"
		);
	}

	static void erasePageBuffer() noexcept
	{
		NVM.CMD = NVM_CMD_ERASE_FLASH_BUFFER_gc;
		CCP = CCP_IOREG_gc;
		NVM.CTRLA |= NVM_CMDEX_bm;
		while (NVM.STATUS & NVM_NVMBUSY_bm)
			continue;
	}

	[[gnu::noinline]]
	static void loadPageBuffer(const uint32_t flashAddr, const std::array<uint8_t, flashPageSize> &buffer) noexcept
	{
		const auto memoryAddr{reinterpret_cast<uint32_t>(buffer.data())};
		NVM.CMD = NVM_CMD_LOAD_FLASH_BUFFER_gc;

		__asm__(R"(
				movw r26, %[memory]
				out 0x39, %C[memory]
				movw r30, %[flash]
				out 0x3B, %C[flash]
				movw r24, %[count]
				clz
loop%=:
				breq loopDone%=
				ld r0, X+
				ld r1, X+
				spm Z+
				sbiw r24, 2
				rjmp loop%=
loopDone%=:
			)" : : [memory] "r" (memoryAddr), [flash] "r" (flashAddr), [count] "r" (flashPageSize) :
				"r0", "r1", "r24", "r25", "r26", "r27", "r30", "r31"
		);
	}

	[[gnu::noinline]]
	static void writePage(const uint32_t pageAddr) noexcept
	{
		NVM.CMD = NVM_CMD_ERASE_WRITE_FLASH_PAGE_gc;

		__asm__(R"(
				movw r30, %[page] ; Load Z with the page to erase + write
				out 0x3B, %C[page]
				ldi r16, 0x9D
				out 0x34, r16 ; Unlock SPM
				spm
			)" : : [page] "r" (pageAddr) : "r16", "r30", "r31"
		);

		while (NVM.STATUS & NVM_FBUSY_bm)
			continue;
	}

public:
	constexpr flash_t() noexcept : value_{nullptr} { }
	constexpr flash_t(const void *const value) noexcept : value_{value} { }

	operator flashPart_t() const noexcept
	{
		flashPart_t result{};
		const auto resultAddr{reinterpret_cast<uint32_t>(&result)};
		const auto valueAddr{reinterpret_cast<uint32_t>(value_)};
		const uint8_t x{RAMPX};
		const uint8_t z{RAMPZ};
		readToRAM(valueAddr, resultAddr, sizeof(flashPart_t));
		RAMPZ = z;
		RAMPX = x;
		return result;
	}

	void operator =(const flashPart_t &source) const noexcept
	{
		std::array<uint8_t, flashPageSize> flashBuffer{};
		const uint8_t x{RAMPX};
		const uint8_t z{RAMPZ};

		const auto *const sourceBuffer{reinterpret_cast<const uint8_t *>(&source)};
		const auto destAddr{reinterpret_cast<uint32_t>(value_)};
		auto pageAddr{destAddr & uint32_t(~(flashPageSize - 1U))};
		auto offset{static_cast<uint16_t>(destAddr - pageAddr)};

		readToRAM(pageAddr, reinterpret_cast<uint32_t>(flashBuffer.data()), offset);
		std::memcpy(flashBuffer.data() + offset, sourceBuffer,
			std::min(size_t(flashPageSize - offset), sizeof(flashPart_t)));
		if (offset + sizeof(flashPart_t) <= flashPageSize)
		{
			offset += sizeof(flashPart_t);
			const auto remainder{static_cast<uint16_t>(flashPageSize - offset)};
			readToRAM(pageAddr + offset, reinterpret_cast<uint32_t>(flashBuffer.data() + offset), remainder);
			erasePageBuffer();
			loadPageBuffer(pageAddr, flashBuffer);
			writePage(pageAddr);
		}
		else
		{
			auto remainder{static_cast<uint16_t>((offset + sizeof(flashPart_t)) - flashPageSize)};
			offset &= flashPageSize - 1U;
			erasePageBuffer();
			loadPageBuffer(pageAddr, flashBuffer);
			writePage(pageAddr);

			pageAddr += flashPageSize;
			std::memcpy(flashBuffer.data(), sourceBuffer + offset, remainder);
			offset = remainder;
			remainder = flashPageSize - remainder;
			readToRAM(pageAddr + offset, reinterpret_cast<uint32_t>(flashBuffer.data() + offset), remainder);
			erasePageBuffer();
			loadPageBuffer(pageAddr, flashBuffer);
			writePage(pageAddr);
		}

		RAMPZ = z;
		RAMPX = x;
	}
};

struct eeprom_t final
{
private:
	[[gnu::noinline]]
	static void writePage(const uint32_t pageAddr) noexcept
	{
		NVM.CMD = NVM_CMD_ERASE_WRITE_EEPROM_PAGE_gc;
		NVM.ADDR0 = pageAddr & 0xFFU;
		NVM.ADDR1 = (pageAddr >> 8U) & 0xFFU;
		NVM.ADDR2 = (pageAddr >> 16U) & 0xFFU;
		CCP = CCP_IOREG_gc;
		NVM.CTRLA = NVM_CMDEX_bm;
		while (NVM.STATUS & NVM_NVMBUSY_bm)
			continue;
	}

public:
	template<typename T> static void write(uint16_t destAddr, const T &source) noexcept
	{
		auto *eeprom{reinterpret_cast<uint8_t *>(MAPPED_EEPROM_START)};
		const auto *const sourceBuffer{reinterpret_cast<const uint8_t *>(&source)};
		auto pageAddr{destAddr & uint16_t(~eepromPageMask)};

		eeprom += pageAddr;
		for (const auto &i : utility::indexSequence_t{destAddr - pageAddr})
		{
			// Load the EEPROM data
			volatile const auto value{eeprom[i]};
			// Load the EEPROM Page Buffer with that data
			eeprom[i] = value;
		}

		auto offset{static_cast<uint16_t>(destAddr - pageAddr)};
		for (const auto &i : utility::indexSequence_t{sizeof(T)})
		{
			// If we are on a page boundary and that is not the start of the 0th page
			if (offset + i && ((offset + i) & eepromPageMask) == 0)
			{
				// Perform an atomic erase + write cycle
				writePage(pageAddr);
				pageAddr += eepromPageSize;
			}
			// Load the EEPROM Page Buffer with data
			eeprom[offset + i] = sourceBuffer[i];
		}

		offset += sizeof(T);
		const auto remainder{offset & eepromPageMask};
		offset &= uint16_t(~eepromPageMask);
		if (remainder)
		{
			for (const auto &i : utility::indexSequence_t{remainder, eepromPageSize})
			{
				// Load the EEPROM data
				volatile const auto value{eeprom[offset + i]};
				// Load the EEPROM Page Buffer with that data
				eeprom[offset + i] = value;
			}
		}
		// Flush the final page to EEPROM
		writePage(pageAddr);

		NVM.CMD = NVM_CMD_NO_OPERATION_gc;
	}
};

namespace mxKeyboard::profile
{
	[[gnu::section(".profile")]] const static std::array<flashPart_t, profileCount> flashProfiles{{}};

	profile_t profile_t::read(const uint8_t profileNumber) noexcept
	{
		profile_t profile{};
		const auto *const eeprom{reinterpret_cast<void *>(MAPPED_EEPROM_START +
			(sizeof(eepromPart_t) * profileNumber))};

		std::memcpy(&profile.eeprom, eeprom, sizeof(eepromPart_t));
		profile.flash = flash_t<flashPart_t>{&flashProfiles[profileNumber]};
		return profile;
	}

	void profile_t::write() noexcept
	{
		flash_t<flashPart_t> flashPart{&flashProfiles[eeprom.profileNumber]};
		flashPart = flash;
		eeprom_t::write(sizeof(eepromPart_t) * eeprom.profileNumber, eeprom);
	}

	void profile_t::keyType(const uint8_t index, const keyType_t type) noexcept
	{
		if (type == keyType_t::latching)
			flash.keyTypes[index >> 3U] |= uint8_t(1U << (index & 7U));
		else
			flash.keyTypes[index >> 3U] &= uint8_t(~(1U << (index & 7U)));
	}
} // namespace mxKeyboard::profile
