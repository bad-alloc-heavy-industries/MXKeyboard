#include <cstdint>
#include "MXKeyboard.hxx"

extern const char stackTop;
extern char vectorAddr;
extern const uint8_t dataStart;
extern const uint8_t dataEnd;
extern uint8_t dataMemory;
extern char bssStart;
extern char bssEnd;

void init();

using irqFunction_t = void (*)();
irqFunction_t vectorTable[] USED SECTION(".vectors")
{
	init
};

extern void init() DEFAULT_VISIBILITY USED SECTION(".startup");
void init()
{
	SREG = 0;

	static_assert(__AVR_XMEGA__);
	const auto stack{reinterpret_cast<uintptr_t>(&stackTop)};
	SPL = uint8_t(stack);
	SPH = uint8_t(stack >> 8U);

	static_assert(__AVR_3_BYTE_PC__);
	__asm__(R"(
		ldi r16, hh8(vectorTable)
		out 0x3C, r16
		)" : : : "r16"
	);

	RAMPD = 0;
	RAMPX = 0;
	RAMPY = 0;
	RAMPZ = 0;

	static_assert(FLASHEND > 0x10000);
	static_assert(__AVR_ENHANCED__);

	while (true)
	{
		__asm__(R"(
			ldi r26, lo8(dataMemory)
			ldi r27, hi8(dataMemory)
			ldi r16, hh8(dataMemory)
			out 0x3B, r16
			ldi r30, lo8(dataStart)
			ldi r31, hi8(dataStart)
			ldi r16, hh8(dataStart)
			out 0x3B, r16
			ldi r16, hi8(dataEnd)
dataCopyLoop:
			cpi r30, lo8(dataEnd)
			cpc r31, r16
			brne dataCopyDone
			elpm r0, Z+
			st X+, r0
			rjmp dataCopyLoop
dataCopyDone:
			)" : : : "r26", "r27", "r30", "r31", "r16", "r0"
		);

		auto *dst{&bssStart};
		while (dst < &bssEnd)
			*dst++ = 0;

		run();
	}
}
