/* timer.c -- simple LF1000 timer driver for profiling
 *
 * Copyright 2007 LeapFrog Enterprises Inc.
 *
 * Andrey Yurovsky <andrey@cozybit.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Note: this driver is designed to utilize the LF1000 CPU's timer hardware to
 *       enable simple profiling operations for optimizing bootloader code.  As
 *       such, its routines work as a 'stopwatch'.
 */

#include "include/autoconf.h"
#include <platform.h>
#include <common.h>
#include <timer.h>
//#include "include/timer.h"
#include "include/debug.h"

#define TIMER32(r)	REG32(TIMER_BASE+r)

/* 
 * settings 
 */
#define TIMER_BASE	LF1000_TIMER0_BASE
#define TIMER_CLK_SRC	1
#define TIMER_PRES	17			/* divide by 18 */
#define TIMER_CLK	2			/* divide by 8 */

/*
 * Timer Clock Rate:
 *
 * TIMER_SRC_RATE/(TIMER_PRES+1)/(TIMER_CLK+1) =
 *
 * 147461538/18/8 = 1024038 Hz, ~ 1.024MHz, ~1us tick
 * 147461538/18   = 8192304 Hz, ~ 8.192MHz, ~122nS tick
 */

u16 value;

#ifdef DEBUG_STOPWATCH

void timer_init(void)
{
	// commented out due to bug!?
	/* make sure the timer is stopped */
	//BIT_CLR(TIMER32(TMRCONTROL), RUN);
	
	/* set up the clock */
	//TIMER32(TMRCONTROL) &= ~(3<<SETCLK);
	//TIMER32(TMRCONTROL) |= (TIMER_CLK<<SETCLK);
	
	
	TIMER32(TMRCLKGEN) = (TIMER_PRES<<TCLKDIV)|(TIMER_CLK_SRC<<TCLKSRCSEL);
	BIT_SET(TIMER32(TMRCLKENB), TCLKGENENB);
}

/* start the stopwatch */
void timer_start(void)
{
	/* make sure the timer is stopped */
	BIT_CLR(TIMER32(TMRCONTROL), RUN);

	/* zero out the timer */
	TIMER32(TMRCOUNT) = 0;

	/* run the timer */
	BIT_SET(TIMER32(TMRCONTROL), RUN);
}

/* stop the stopwatch, and return the time */
u32 timer_stop(void)
{
	/* stop the timer */
	BIT_CLR(TIMER32(TMRCONTROL), RUN);
	/* get access to counter */
	BIT_SET(TIMER32(TMRCONTROL), LDCNT);

	return TIMER32(TMRCOUNT);
}

void tmr_poll_start(u16 val)
{
	/* make sure the timer is stopped */
	BIT_CLR(TIMER32(TMRCONTROL), RUN);

	/* zero out the timer */
	TIMER32(TMRCOUNT) = 0;

	/* run the timer */
	BIT_SET(TIMER32(TMRCONTROL), RUN);
	
	value = val;
}

u8 tmr_poll_has_expired(void)
{
   BIT_CLR(TIMER32(TMRCONTROL), RUN);
   BIT_SET(TIMER32(TMRCONTROL), LDCNT);
   u32 timer = TIMER32(TMRCOUNT);
   //db_puts("TIMER:");db_int (timer);db_puts(" ");
   //db_puts("VALUE:");db_int (value * 8197);db_puts("\n");
   BIT_SET(TIMER32(TMRCONTROL), RUN);
   
	if (timer >= (value * 8197))
		return 1;
		
	else
		return 0;
		
}

void timerPause(u16 val)
{
	tmr_poll_start(val);
	while (!(tmr_poll_has_expired()));
}
#endif /* DEBUG_STOPWATCH */
