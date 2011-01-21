/*
 *  linux/include/asm-arm/arch-lf1000/uncompress.h
 *
 *  Copyright (C) 2007	Kosta Demirev <kdemirev@yahoo.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <asm/io.h>
#include "uart.h"

static void flush(void)
{
	if(readw(LF1000_SYS_UART_BASE+FCON) & (1<<FIFO_EN)) { 
		/* FIFO enabled */
		while(readw(LF1000_SYS_UART_BASE+FSTATUS) & 
				(1<<TX_FIFO_FULL)) {
			barrier();
		}
	} else { /* FIFO disabled */
		while(!(readw(LF1000_SYS_UART_BASE+TRSTATUS) &
				(1<<TRANSMITTER_EMPTY))) {
			barrier();
		}
	}
}

static void putc(int c)
{
	flush();
	writeb(c, LF1000_SYS_UART_BASE+THB);
}

/*
 * nothing to do
 */
#define arch_decomp_setup()
#define arch_decomp_wdog()
