 /*      Copyright (C) 2007 Kosta Demirev <kdemirev@yahoo.com> */
#ifndef	_DEVELOPMENT_H
#define	_DEVELOPMENT_H
#include "asm/arch/uart.h"

#define	VIRT

#ifdef	VIRT
#define DBG_LF1000_UART_FCR	((( struct lf1000_uart*)IO_ADDRESS( LF1000_SYS_UART_BASE))->fcon)
#define DBG_LF1000_UART_SR	((( struct lf1000_uart*)IO_ADDRESS( LF1000_SYS_UART_BASE))->trstatus)
#define DBG_LF1000_UART_FR	((( struct lf1000_uart*)IO_ADDRESS( LF1000_SYS_UART_BASE))->fstatus)
#define DBG_LF1000_UART_TR	((( struct lf1000_uart*)IO_ADDRESS( LF1000_SYS_UART_BASE))->thb)
#else
#define	DBG_LF1000_UART_FCR	(*(unsigned short*)LF1000_UART_FCR)
#define	DBG_LF1000_UART_FR	(*(unsigned short*)LF1000_UART_FR )
#define	DBG_LF1000_UART_SR	(*(unsigned short*)LF1000_UART_SR )
#define	DBG_LF1000_UART_TR	(*(unsigned short*)LF1000_UART_TR )
#endif

static inline void flush(void)
{
    if( DBG_LF1000_UART_FCR & 0x01) {	// if   FIFO  enabled
	while (  DBG_LF1000_UART_FR & 0x0200 ) barrier();
    } else {				// else FIFO disabled
	while (!(DBG_LF1000_UART_SR & 0x0002)) barrier();
    }
}

static inline void putc(int c)
{
    flush();
    DBG_LF1000_UART_TR = c;
    if( c == '\n') {
	flush();
	DBG_LF1000_UART_TR = '\r';
    }
}

static inline void putstr(const char *ptr)
{
	char c;

	while ((c = *ptr++) != '\0') {
		if (c == '\n')
			putc('\r');
		putc(c);
	}

	flush();
}

static inline void puth( unsigned long val)
{
int i;
char hex[] = {	'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    putc('0');
    putc('x');

    for( i=0; i<8; i++) {
	putc( hex[ (val>>((7-i)*4))&0x0f]);
    }
}

#endif
