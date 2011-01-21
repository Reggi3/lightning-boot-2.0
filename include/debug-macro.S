/* linux/include/asm-arm/arch-lf1000/debug-macro.S
 *
 * Debugging macro include header
 *
 *  Copyright (C) 2007	Kosta Demirev <kdemirev@yahoo.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/

#include	"platform.h"

		.macro	addruart,rx
		mrc	p15, 0, \rx, c1, c0
		tst	\rx, #1			@ MMU enabled?
		moveq	\rx,      #LF1000_SYS_IO
		movne	\rx,      #IO_ADDRESS_OFF	@ virtual base
		orr	\rx, \rx, #(LF1000_SYS_UART_BASE & 0x00ff0000)
		orr	\rx, \rx, #(LF1000_SYS_UART_BASE & 0x0000ff00)
		orr	\rx, \rx, #(LF1000_SYS_UART_BASE & 0x000000ff)
		.endm

#include <asm/hardware/debug-lf1000.S>
