/* LF1000 UART Driver
 *
 * asm/arch/uart.h -- UART hardware abstraction, settings.
 *
 * Andrey Yurovsky <andrey@cozybit.com>
 */
#ifndef LF1000_UART_H
#define LF1000_UART_H

#include "platform.h"	/* for CPU variant */

/*
 * clock source
 */
#ifdef CPU_MF2530F /* PLL2 has to be used on MP2530F (hardware restriction) */
#define UART_PLL	PLL2
#elif defined CPU_LF1000
#define UART_PLL	PLL1
#endif

/*
 * available ports
 */
#define UART0		0
#define UART1		1
#define UART2		2
#define UART3		3
#define UART4		4
#define UART5		5
#define UART_MAX	6

/*
 * Settings:
 *
 * - LF1000_SYS_UART is the UART number, LF1000_SYS_UART_BASE is the base 
 *   address, LF1000_SYS_UART_IRQ is the IRQ number.  These are required.
 * - define LF1000_SYS_UART_TX_PORT or LF1000_SYS_UART_RX_PORT if a mode change
 *   is required, then:
 * - define LF1000_SYS_UART_TX_PIN or LF1000_SYS_UART_RX_PIN as the bit 
 *   position in the ALTFNn register, for ALTFN1 this is (pin*2-32), for ALTFN0
 *   it is (pin*2)
 * - define LF1000_SYS_UART_TX_MODE or LF1000_SYS_UART_RX_MODE to set the mode
 */
#ifdef CPU_MF2530F
	#define LF1000_SYS_UART		UART3
	#define LF1000_SYS_UART_BASE	LF1000_UART3_BASE
	#define LF1000_SYS_UART_IRQ     LF1000_UART3_IRQ
	/* set up pin functions: */
	#define LF1000_SYS_UART_TX_PORT	GPIOBALTFN1 /* change TX pin mode */
	#define LF1000_SYS_UART_TX_PIN	(19*2-32)
	#define LF1000_SYS_UART_TX_MODE	GPIO_ALT1
	/* (nothing needs to be done for RX pin) */
#elif defined CPU_LF1000
	#define LF1000_SYS_UART		UART0
	#define LF1000_SYS_UART_BASE	LF1000_UART0_BASE
	#define LF1000_SYS_UART_IRQ     LF1000_UART0_IRQ
	/* set up pin functions: */
	#define LF1000_SYS_UART_TX_PORT	GPIOAALTFN0 /* change TX pin mode */
	#define LF1000_SYS_UART_TX_PIN	(8*2)
	#define LF1000_SYS_UART_TX_MODE	GPIO_ALT1
	/* nothing needs to be done for RX pin */
#else
#error "No CPU variant defined, can't build UART driver"
#endif
#define LF1000_SYS_UART_BR	115200	/* system baud rate */

/*
 * UART helper macros
 */

/* (for bootloaders, where we don't calculate the divider) */
#ifdef CPU_MF2530F
#define UARTDIV			26
#elif defined CPU_LF1000
#define UARTDIV			40
#endif

/* calculate baud rate from registers */
#define	UART_BRD(pll, brate)	\
	((get_pll_freq(pll)/1000*(500*PLL_MUL(pll))/((brate)*16))-1)

/* 
 * UART registers, offsets from LF1000_UARTn_BASE where n is the channel set
 * by the LF1000_SYS_UART macro.
 */
#define LCON			0x00
#define UCON			0x02
#define FCON			0x04
#define MCON			0x06
#define TRSTATUS		0x08
#define ESTATUS			0x0A
#define FSTATUS			0x0C
#define MSTATUS			0x0E
#define THB			0x10
#define RHB			0x12
#define BRD			0x14
#define TIMEOUTREG		0x16
#define INTSTATUSREG		0x18
#define UARTCLKENB		0x40
#define UARTCLKGEN		0x44

/*
 * UART Register bits
 */

/* LCON Register */
#define SYNC_PENDCLR		7
#define SIR_MODE		6
#define PARITY_MODE		3
#define STOPBIT			2
#define WORD_LEN		0

/* UCON Register */
#define TX_INT			9
#define RX_INT			8
#define RX_TIMEOUT		7
#define RX_ERRSTATUS		6
#define LOOPBACK_MODE		5
#define SEND_ BREAK		4
#define TRANS_MODE		2
#define RECEIVE_MODE		0

/* FCON Register */
#define TX_FIFO_TRIGGER		6
#define RX_FIFO_TRIGGER		4
#define TX_FIFO_RESET		2
#define RX_FIFO_RESET		1
#define FIFO_EN			0

/* MCON Register */
#define HALF_CH_ENB		7
#define SCRXENB			6
#define SCTXENB			5
#define AFC			4
#define DTR_ACTIVE		1
#define RTS_ACTIVE		0

/* FSTATUS Register */
#define RX_FIFO_ERROR		10
#define TX_FIFO_FULL		9
#define RX_FIFO_FULL		8
#define TX_FIFO_COUNT		4
#define RX_FIFO_COUNT		0

/* TRSTATUS */
#define TRANSMITTER_EMPTY	2
#define	TRANSMIT_BUFFER_EMPTY	1
#define RECEIVE_BUFFER_DATA_READY	0

/* UARTCLKENB */
#define UARTCLKGENENB		2

/* UARTCLKGEN */
#define UARTCLKDIV		4
#define UARTCLKSRCSEL		1

#ifndef	__ASSEMBLY__
struct lf1000_uart {
    volatile unsigned short	lcon;
    volatile unsigned short	ucon;
    volatile unsigned short	fcon;
    volatile unsigned short	mcon;
    volatile unsigned short	trstatus;
    volatile unsigned short	estatus;
    volatile unsigned short	fstatus;
    volatile unsigned short	mstatus;
    volatile unsigned short	thb;
    volatile unsigned short	rhb;
    volatile unsigned short	brd;
    volatile unsigned short	timeoutreg;
    volatile unsigned short	intstatusreg;
    volatile unsigned char	reserved1[0x26];
    volatile unsigned int	uartclkenb;
    volatile unsigned int	uartclkgen;
};

struct lf1000_uart_init {
    struct lf1000_uart*		uart_pnt;
    int			 	irq_number;
};
#endif

#endif /* LF1000_UART_H */
