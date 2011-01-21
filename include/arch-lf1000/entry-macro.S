/*
 * include/asm-arm/arch-lf1000/entry-macro.S
 *
 * (c) LeapFrog Enterprises Inc.
 *
 * Yakov A. Blyakhman <YBlyakhman@leapfrog.com>
 *
 * Low-level IRQ helper macros for LF1000 platforms
 *
 * This file is licensed under  the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */
#include <asm/hardware.h>
#include <asm/hardware/vic.h>
#include <asm/arch/ic.h>

		.macro	disable_fiq
		.endm

		.macro	get_irqnr_and_base, irqnr, irqstat, base, tmp
		ldr	\base, =IO_ADDRESS(LF1000_IC_BASE)
		ldr	\irqstat, [\base, #INTPENDL]	@ get masked status
		mov	\irqnr, #0
		teq	\irqstat, #0
		beq	1003f

1001:		tst	\irqstat, #15
		bne	1002f
		add	\irqnr, \irqnr, #4
		movs	\irqstat, \irqstat, lsr #4
		bne	1001b
1002:		tst	\irqstat, #1
		bne	2003f
		add	\irqnr, \irqnr, #1
		movs	\irqstat, \irqstat, lsr #1
		bne	1002b
1003:
		ldr	\irqstat, [\base, #INTPENDH]	@ get masked status
		teq	\irqstat, #0
		beq	2003f
		add	\irqnr, #32

2001:		tst	\irqstat, #15
		bne	2002f
		add	\irqnr, \irqnr, #4
		movs	\irqstat, \irqstat, lsr #4
		bne	2001b
2002:		tst	\irqstat, #1
		bne	2003f
		add	\irqnr, \irqnr, #1
		movs	\irqstat, \irqstat, lsr #1
		bne	2002b
2003:
		teq	\irqnr, #0
@		movs	\irqnr, \irqnr

@		clz	\irqnr, \irqstat
@1003:		/* EQ will be set if we reach MAXIRQNUM */
		.endm

