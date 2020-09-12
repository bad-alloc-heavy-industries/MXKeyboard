// SPDX-License-Identifier: BSD-3-Clause
#include <cstdint>
#include "MXKeyboard.hxx"
#include "interrupts.hxx"

extern const char stackTop;
extern char vectorAddr;
extern const uint8_t dataStart;
extern const uint8_t dataEnd;
extern const uint8_t dataMemory;
extern char bssStart;
extern const char bssEnd;

#define NAKED __attribute__((naked))

extern "C" void vectorTable() USED SECTION(".vectors") NAKED __attribute__((optimize(0)));
extern "C" void init() DEFAULT_VISIBILITY USED SECTION(".startup");
extern "C" void irqEmptyDef() INTERRUPT;

void init()
{
	__asm__("clr r1");
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

void irqEmptyDef()
{
	while (true)
		continue;
}

void vectorTable()
{
	__asm__(R"(
		jmp init
		jmp oscFailureIRQ ; OSC fail vector
		jmp irqEmptyDef ; Port C Int0 vector
		jmp irqEmptyDef ; Port C Int1 vector
		jmp irqEmptyDef ; Port R Int0 vector
		jmp irqEmptyDef ; Port R Int1 vector
		jmp irqEmptyDef ; DMA Channel 0 vector
		jmp irqEmptyDef ; DMA Channel 1 vector
		jmp dmaChannel2IRQ ; DMA Channel 2 vector
		jmp irqEmptyDef ; DMA Channel 3 vector
		jmp irqEmptyDef ; vector 10
		jmp irqEmptyDef ; vector 11
		jmp irqEmptyDef ; vector 12
		jmp irqEmptyDef ; vector 13
		jmp tcc0OverflowIRQ ; vector 14
		jmp irqEmptyDef ; vector 15
		jmp irqEmptyDef ; vector 16
		jmp irqEmptyDef ; vector 17
		jmp irqEmptyDef ; vector 18
		jmp irqEmptyDef ; vector 19
		jmp irqEmptyDef ; vector 20
		jmp irqEmptyDef ; vector 21
		jmp irqEmptyDef ; vector 22
		jmp irqEmptyDef ; vector 23
		jmp irqEmptyDef ; vector 24
		jmp irqEmptyDef ; vector 25
		jmp irqEmptyDef ; vector 26
		jmp irqEmptyDef ; vector 27
		jmp irqEmptyDef ; vector 28
		jmp irqEmptyDef ; vector 29
		jmp irqEmptyDef ; vector 30
		jmp irqEmptyDef ; vector 31
		jmp irqEmptyDef ; vector 32
		jmp irqEmptyDef ; vector 33
		jmp irqEmptyDef ; vector 34
		jmp irqEmptyDef ; vector 35
		jmp irqEmptyDef ; vector 36
		jmp irqEmptyDef ; vector 37
		jmp irqEmptyDef ; vector 38
		jmp irqEmptyDef ; vector 39
		jmp irqEmptyDef ; vector 40
		jmp irqEmptyDef ; vector 41
		jmp irqEmptyDef ; vector 42
		jmp irqEmptyDef ; vector 43
		jmp irqEmptyDef ; vector 44
		jmp irqEmptyDef ; vector 45
		jmp irqEmptyDef ; vector 46
		jmp irqEmptyDef ; vector 47
		jmp irqEmptyDef ; vector 48
		jmp irqEmptyDef ; vector 49
		jmp irqEmptyDef ; vector 50
		jmp irqEmptyDef ; vector 51
		jmp irqEmptyDef ; vector 52
		jmp irqEmptyDef ; vector 53
		jmp irqEmptyDef ; vector 54
		jmp irqEmptyDef ; vector 55
		jmp irqEmptyDef ; vector 56
		jmp irqEmptyDef ; vector 57
		jmp irqEmptyDef ; vector 58
		jmp irqEmptyDef ; vector 59
		jmp irqEmptyDef ; vector 60
		jmp irqEmptyDef ; vector 61
		jmp irqEmptyDef ; vector 62
		jmp irqEmptyDef ; vector 63
		jmp irqEmptyDef ; vector 64
		jmp irqEmptyDef ; vector 65
		jmp irqEmptyDef ; vector 66
		jmp irqEmptyDef ; vector 67
		jmp irqEmptyDef ; vector 68
		jmp irqEmptyDef ; vector 69
		jmp irqEmptyDef ; vector 70
		jmp irqEmptyDef ; vector 71
		jmp irqEmptyDef ; vector 72
		jmp irqEmptyDef ; vector 73
		jmp irqEmptyDef ; vector 74
		jmp irqEmptyDef ; vector 75
		jmp irqEmptyDef ; vector 76
		jmp irqEmptyDef ; vector 77
		jmp irqEmptyDef ; vector 78
		jmp irqEmptyDef ; vector 79
		jmp irqEmptyDef ; vector 80
		jmp irqEmptyDef ; vector 81
		jmp irqEmptyDef ; vector 82
		jmp irqEmptyDef ; vector 83
		jmp irqEmptyDef ; vector 84
		jmp irqEmptyDef ; vector 85
		jmp irqEmptyDef ; vector 86
		jmp irqEmptyDef ; vector 87
		jmp irqEmptyDef ; vector 88
		jmp irqEmptyDef ; vector 89
		jmp irqEmptyDef ; vector 90
		jmp irqEmptyDef ; vector 91
		jmp irqEmptyDef ; vector 92
		jmp irqEmptyDef ; vector 93
		jmp irqEmptyDef ; vector 94
		jmp irqEmptyDef ; vector 95
		jmp irqEmptyDef ; vector 96
		jmp irqEmptyDef ; vector 97
		jmp irqEmptyDef ; vector 98
		jmp irqEmptyDef ; vector 99
		jmp irqEmptyDef ; vector 100
		jmp irqEmptyDef ; vector 101
		jmp irqEmptyDef ; vector 102
		jmp irqEmptyDef ; vector 103
		jmp irqEmptyDef ; vector 104
		jmp irqEmptyDef ; vector 105
		jmp irqEmptyDef ; vector 106
		jmp irqEmptyDef ; vector 107
		jmp irqEmptyDef ; vector 108
		jmp irqEmptyDef ; vector 109
		jmp irqEmptyDef ; vector 110
		jmp irqEmptyDef ; vector 111
		jmp irqEmptyDef ; vector 112
		jmp irqEmptyDef ; vector 113
		jmp irqEmptyDef ; vector 114
		jmp irqEmptyDef ; vector 115
		jmp irqEmptyDef ; vector 116
		jmp irqEmptyDef ; vector 117
		jmp irqEmptyDef ; vector 118
		jmp irqEmptyDef ; vector 119
		jmp irqEmptyDef ; vector 120
		jmp irqEmptyDef ; vector 121
		jmp irqEmptyDef ; vector 122
		jmp irqEmptyDef ; vector 123
		jmp irqEmptyDef ; vector 124
		jmp irqEmptyDef ; vector 125
		jmp irqEmptyDef ; vector 126
		jmp irqEmptyDef ; vector 127
	)");
}
