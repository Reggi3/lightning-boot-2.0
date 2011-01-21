/* debug.c -- Simple UART driver for debugging.
 *
 * Copyright 2007 LeapFrog Enterprises Inc.
 *
 * Andrey Yurovsky <andrey@cozybit.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "include/autoconf.h"
#include <platform.h>
#include <common.h>
#include <uart.h>
#include <gpio.h>
#include <gpio_hal.h>

#include "include/debug.h"

#define UART8(r)        REG8(LF1000_SYS_UART_BASE+r)
#define UART16(r)       REG16(LF1000_SYS_UART_BASE+r)
#define UART32(r)       REG32(LF1000_SYS_UART_BASE+r)

#if defined DEBUG && defined DEBUG_STOPWATCH
#include "include/timer.h"
const char *db_stopwatch;
#endif

void db_init(void)
{
	u32 tmp;

	/* GPIO setup, as needed */

	#ifdef LF1000_SYS_UART_TX_PORT
	tmp = REG32(LF1000_GPIO_BASE+LF1000_SYS_UART_TX_PORT);
	tmp &= ~(0x3<<(LF1000_SYS_UART_TX_PIN));
	tmp |= (LF1000_SYS_UART_TX_MODE<<(LF1000_SYS_UART_TX_PIN));
	REG32(LF1000_GPIO_BASE+LF1000_SYS_UART_TX_PORT) = tmp;
	#endif

	#ifdef LF1000_SYS_UART_RX_PORT
	tmp = REG32(LF1000_GPIO_BASE+LF1000_SYS_UART_RX_PORT);
	tmp &= ~(0x3<<(LF1000_SYS_UART_RX_PIN));
	tmp |= (LF1000_SYS_UART_RX_MODE<<(LF1000_SYS_UART_RX_PIN));
	REG32(LF1000_GPIO_BASE+LF1000_SYS_UART_RX_PORT) = tmp;
	#endif

	/* clear IRQ pending, set 8 bit word length */
	UART16(LCON) = (1<<SYNC_PENDCLR)|(3<<WORD_LEN);

	/* enable polling/IRQ transmit, leave receive disabled */
	UART16(UCON) = (1<<TRANS_MODE)|(1<<RECEIVE_MODE);

	/* reset the FIFOs */
	REG16(LF1000_SYS_UART_BASE+FCON)=(1<<TX_FIFO_RESET)|(1<<RX_FIFO_RESET);

	/* TODO: do we need this? */
	REG16(LF1000_SYS_UART_BASE+MCON) = (1<<SCRXENB);

	/* set the baud rate */
	UART16(BRD) = 11; /* FIXME (for now "1"  sets 115200 baud , "11" sets 19200 baud) */
	UART16(UARTCLKGEN) = ((UARTDIV-1)<<UARTCLKDIV)|(UART_PLL<<UARTCLKSRCSEL);

#if defined DEBUG && defined DEBUG_STOPWATCH
	timer_init();
#endif
}

#ifdef DEBUG
void db_putchar(char c)
{
	if(c == '\n')
		db_putchar('\r');
	while(IS_CLR(UART16(TRSTATUS), TRANSMIT_BUFFER_EMPTY));
	UART8(THB) = c;
}

int db_tstc (void)
{
	return (REG16(LF1000_SYS_UART_BASE+FSTATUS) & 0xF);
}

int db_getc (void)
{
	unsigned char byte;
	while(!db_tstc());
	byte = UART8(RHB);
	//(REG16(LF1000_SYS_UART_BASE+RHB));
	//return REG16(LF1000_SYS_UART_BASE+RHB);
	//db_putchar (byte);
	return byte;
}

int db_getc_async(void)
{
	unsigned char byte;
	if ( !db_tstc() ) return -1;
	byte = UART8(RHB);
	return byte;
}

#if 1
void db_puts(const char *s)
{
	do {
		db_putchar(*s++);
	} while(*s != 0);
}
#endif

void db_hex(unsigned int val)
{
	val= val & 0xf;
	if (val <10)
		db_putchar('0'+val);
	else
		db_putchar('A'+(val-10));
}

void db_byte(char val)
{
	db_hex(val>>4);
	db_hex(val & 0xF);
}

void db_16(u16 val)
{
	db_puts("0x");
	db_hex(val>>(4*3));
	db_hex(val>>(4*2));
	db_hex(val>>(4*1));
	db_hex(val>>(4*0));
}

void db_int(unsigned int val)
{
	db_puts("0x");
	db_hex(val>>(4*7));
	db_hex(val>>(4*6));
	db_hex(val>>(4*5));
	db_hex(val>>(4*4));
	db_hex(val>>(4*3));
	db_hex(val>>(4*2));
	db_hex(val>>(4*1));
	db_hex(val>>(4*0));
}

/*
 * Timer stopwatch debugging
 */

#ifdef DEBUG_STOPWATCH

void db_stopwatch_start(const char *name)
{
	db_stopwatch = name;
	db_puts("START :: ");
	db_puts(db_stopwatch);
	db_putchar('\n');
	timer_start();
}

void db_stopwatch_stop(void)
{
	db_puts("STOP  :: ");
	db_puts(db_stopwatch);
	db_puts(" :: ");
	db_int(timer_stop());
	db_putchar('\n');
}
#endif /* DEBUG_STOPWATCH */

#endif
