// SPDX-License-Identifier: BSD-3-Clause
#ifndef INTERRUPTS__HXX
#define INTERRUPTS__HXX

#define INTERRUPT __attribute__((signal)) __attribute__((used))

#define oscFailureIRQ __vector_oscFailureIRQ
#define dmaChannel0IRQ __vector_dmaChannel0IRQ
#define dmaChannel1IRQ __vector_dmaChannel1IRQ
#define dmaChannel2IRQ __vector_dmaChannel2IRQ

void oscFailureIRQ() INTERRUPT;
void dmaChannel0IRQ() INTERRUPT;
void dmaChannel1IRQ() INTERRUPT;
void dmaChannel2IRQ() INTERRUPT;

#endif /*INTERRUPTS__HXX*/
