// SPDX-License-Identifier: BSD-3-Clause
#include <cstdint>
#include "MXKeyboard.hxx"
#include "interrupts.hxx"

extern const char stackTop;
extern char vectorAddr;
extern const uint8_t dataStart;
extern const uint8_t dataEnd;
extern uint8_t dataMemory;
extern char bssStart;
extern char bssEnd;

#define NAKED __attribute__((naked))

extern "C" void vectorTable() USED SECTION(".vectors") NAKED;
void init() DEFAULT_VISIBILITY USED SECTION(".startup");
void irqEmptyDef() INTERRUPT;

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

void irqEmptyDef()
{
	while (true)
		continue;
}

void vectorTable()
{
	init();
	oscFailureIRQ(); // OSC fail vector
	irqEmptyDef(); // Port C Int0 vector
	irqEmptyDef(); // Port C Int1 vector
	irqEmptyDef(); // Port R Int0 vector
	irqEmptyDef(); // Port R Int1 vector
	irqEmptyDef(); // DMA Channel 0 vector
	irqEmptyDef(); // DMA Channel 1 vector
	irqEmptyDef(); // DMA Channel 2 vector
	irqEmptyDef(); // DMA Channel 3 vector
	irqEmptyDef(); // vector 10
	irqEmptyDef(); // vector 11
	irqEmptyDef(); // vector 12
	irqEmptyDef(); // vector 13
	irqEmptyDef(); // vector 14
	irqEmptyDef(); // vector 15
	irqEmptyDef(); // vector 16
	irqEmptyDef(); // vector 17
	irqEmptyDef(); // vector 18
	irqEmptyDef(); // vector 19
	irqEmptyDef(); // vector 20
	irqEmptyDef(); // vector 21
	irqEmptyDef(); // vector 22
	irqEmptyDef(); // vector 23
	irqEmptyDef(); // vector 24
	irqEmptyDef(); // vector 25
	irqEmptyDef(); // vector 26
	irqEmptyDef(); // vector 27
	irqEmptyDef(); // vector 28
	irqEmptyDef(); // vector 29
	irqEmptyDef(); // vector 30
	irqEmptyDef(); // vector 31
	irqEmptyDef(); // vector 32
	irqEmptyDef(); // vector 33
	irqEmptyDef(); // vector 34
	irqEmptyDef(); // vector 35
	irqEmptyDef(); // vector 36
	irqEmptyDef(); // vector 37
	irqEmptyDef(); // vector 38
	irqEmptyDef(); // vector 39
	irqEmptyDef(); // vector 40
	irqEmptyDef(); // vector 41
	irqEmptyDef(); // vector 42
	irqEmptyDef(); // vector 43
	irqEmptyDef(); // vector 44
	irqEmptyDef(); // vector 45
	irqEmptyDef(); // vector 46
	irqEmptyDef(); // vector 47
	irqEmptyDef(); // vector 48
	irqEmptyDef(); // vector 49
	irqEmptyDef(); // vector 50
	irqEmptyDef(); // vector 51
	irqEmptyDef(); // vector 52
	irqEmptyDef(); // vector 53
	irqEmptyDef(); // vector 54
	irqEmptyDef(); // vector 55
	irqEmptyDef(); // vector 56
	irqEmptyDef(); // vector 57
	irqEmptyDef(); // vector 58
	irqEmptyDef(); // vector 59
	irqEmptyDef(); // vector 60
	irqEmptyDef(); // vector 61
	irqEmptyDef(); // vector 62
	irqEmptyDef(); // vector 63
	irqEmptyDef(); // vector 64
	irqEmptyDef(); // vector 65
	irqEmptyDef(); // vector 66
	irqEmptyDef(); // vector 67
	irqEmptyDef(); // vector 68
	irqEmptyDef(); // vector 69
	irqEmptyDef(); // vector 70
	irqEmptyDef(); // vector 71
	irqEmptyDef(); // vector 72
	irqEmptyDef(); // vector 73
	irqEmptyDef(); // vector 74
	irqEmptyDef(); // vector 75
	irqEmptyDef(); // vector 76
	irqEmptyDef(); // vector 77
	irqEmptyDef(); // vector 78
	irqEmptyDef(); // vector 79
	irqEmptyDef(); // vector 80
	irqEmptyDef(); // vector 81
	irqEmptyDef(); // vector 82
	irqEmptyDef(); // vector 83
	irqEmptyDef(); // vector 84
	irqEmptyDef(); // vector 85
	irqEmptyDef(); // vector 86
	irqEmptyDef(); // vector 87
	irqEmptyDef(); // vector 88
	irqEmptyDef(); // vector 89
	irqEmptyDef(); // vector 90
	irqEmptyDef(); // vector 91
	irqEmptyDef(); // vector 92
	irqEmptyDef(); // vector 93
	irqEmptyDef(); // vector 94
	irqEmptyDef(); // vector 95
	irqEmptyDef(); // vector 96
	irqEmptyDef(); // vector 97
	irqEmptyDef(); // vector 98
	irqEmptyDef(); // vector 99
	irqEmptyDef(); // vector 100
	irqEmptyDef(); // vector 101
	irqEmptyDef(); // vector 102
	irqEmptyDef(); // vector 103
	irqEmptyDef(); // vector 104
	irqEmptyDef(); // vector 105
	irqEmptyDef(); // vector 106
	irqEmptyDef(); // vector 107
	irqEmptyDef(); // vector 108
	irqEmptyDef(); // vector 109
	irqEmptyDef(); // vector 110
	irqEmptyDef(); // vector 111
	irqEmptyDef(); // vector 112
	irqEmptyDef(); // vector 113
	irqEmptyDef(); // vector 114
	irqEmptyDef(); // vector 115
	irqEmptyDef(); // vector 116
	irqEmptyDef(); // vector 117
	irqEmptyDef(); // vector 118
	irqEmptyDef(); // vector 119
	irqEmptyDef(); // vector 120
	irqEmptyDef(); // vector 121
	irqEmptyDef(); // vector 122
	irqEmptyDef(); // vector 123
	irqEmptyDef(); // vector 124
	irqEmptyDef(); // vector 125
	irqEmptyDef(); // vector 126
	irqEmptyDef(); // vector 127
}
