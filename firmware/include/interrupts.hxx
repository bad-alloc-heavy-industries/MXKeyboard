// SPDX-License-Identifier: BSD-3-Clause
#ifndef INTERRUPTS__HXX
#define INTERRUPTS__HXX

#define INTERRUPT __attribute__((signal)) __attribute__((used))

void oscFailureIRQ() INTERRUPT;
void dmaChannel0IRQ() INTERRUPT;
void dmaChannel1IRQ() INTERRUPT;
void dmaChannel2IRQ() INTERRUPT;

#endif /*INTERRUPTS__HXX*/
