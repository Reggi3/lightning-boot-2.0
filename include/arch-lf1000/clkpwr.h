/* asm/arch-lf1000/clkpwr.h -- LF1000 Clock & Power Management Hardware 
 *
 * Andrey Yurovsky <andrey@cozybit.com>
 */

#ifndef _LF1000_CLKPWR
#define _LF1000_CLKPWR

#include "platform.h" /* for CPU variant */

#ifdef CPU_LF1000
#define NUM_PLLS	2
#elif defined CPU_MF2530F
#define NUM_PLLS	3
#endif

/* Clock and Power Control Registers (offsets from LF1000_CLKPWR_BASE) */
#define CLKMODEREG		0x000
#define PLLSETREG0		0x004
#define PLLSETREG1		0x008
#define GPIOWAKEUPENB		0x040
#define RTCWAKEUPENB		0x044
#define GPIOWAKEUPRISEENB	0x048
#define GPIOWAKEUPFALLENB	0x04C
#define GPIOPEND		0x050
#define INTPENDSPAD		0x058
#define PWRRSTSTATUS		0x05C
#define INTENB			0x060
#define PWRMODE			0x07C
#ifdef CPU_LF1000
#define PADSTRENGTHGPIOAL	0x100
#define PADSTRENGTHGPIOAH	0x104
#define PADSTRENGTHGPIOBL	0x108
#define PADSTRENGTHGPIOBH	0x10C
#define PADSTRENGTHGPIOCL	0x110
#define PADSTRENGTHGPIOCH	0x114
#define PADSTRENGTHBUS		0x118
#endif

/* Clock Mode Register (CLKMODEREG) */
#ifdef CPU_MF2530F
#define PLLPWDN2	30
#define PLLPWDN1	29
#define CLKSELSDRAM	25
#define CLKDIVSDRAM	21
#define CLKDIV2BCLK	20
#define CLKSELBCLK	18
#define CLKDIV1BCLK	14
#define CLKDIV2CPU1	13
#define CLKSELCPU1	11
#define CLKDIVCPU1	7
#define CLKDIV2CPU0	6
#define CLKSELCPU0	4
#define CLKDIVCPU0	0
#elif defined CPU_LF1000
#define PLLPWDN1	30
#define CLKSELBCLK	24
#define CLKDIV1BCLK	20
#define CLKDIV2CPU0	6
#define CLKSELCPU0	4
#define CLKDIVCPU0	0
#endif

/* PLL0 Setting Register (PLLSETREG0) */
#ifdef CPU_MF2530F		
#define PDIV_0		16
#elif defined CPU_LF1000
#define PDIV_0		18
#endif
#define MDIV_0		8
#define SDIV_0		0

/* PLL1 Setting Register (PLLSETREG1) */
#ifdef CPU_MF2530F		
#define PDIV_1		16
#elif defined CPU_LF1000
#define PDIV_1		18
#endif
#define MDIV_1		8
#define SDIV_1		0

#ifdef CPU_MF2530F
/* PLL2 Setting Register (PLLSETREG2) */
#define PDIV_2		16
#define MDIV_2		8
#define SDIV_2		0
#endif

/* Power Mode Control Register (PWRMODE) */
#define CHGPLL		15
#define GPIOSWRSTENB	13
#define SWRST		12
#define LASTPWRMODE	4
#define CURPWRMODE	0

/* Interrupt Pending & Scratch Pad Register (INTPENDSPAD) */
#ifdef CPU_LF1000
#define BATFW		14
#endif
#define GPIORESETW	13
#define WATCHDOGRSTW	12
#define POWERONRSTW	11

#endif
