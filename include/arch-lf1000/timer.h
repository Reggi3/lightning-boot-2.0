/* 
 * asm/arch-lf1000/timer.h -- Timer hardware abstraction.
 *
 * Andrey Yurovsky <andrey@cozybit.com>
 */

#ifndef _LF1000_TIMER_H
#define _LF1000_TIMER_H

/* Timer n Registers (offsets from LF1000_TIMERn_BASE) */
#define TMRCOUNT	0x00
#define TMRMATCH	0x04
#define TMRCONTROL	0x08
#define TMRCLKENB	0x40
#define TMRCLKGEN	0x44

/* Timer Control Register n (TMRCONTROLn) */
#define LDCNT		6
#define INTPEND		5
#define INTENB_T	4 /*  added _T to differentiate from INTENB reg! */
#define RUN		3
#define WDENB		2
#define SETCLK		0

/* Timer Clock Generation Enable Register n (TMRCLKENBn) */
#define TCLKMODE	3
#define TCLKGENENB	2 /* added T to differentiate from others */

/* Timer Clock Generation Control Register n (TMRCLKGENn) */
#define TCLKDIV		4 /* added T to differentiate from others */
#define TCLKSRCSEL	1

#endif
