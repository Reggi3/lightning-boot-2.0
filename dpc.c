/* dpc.c -- Basic Display Controller (DPC) driver for displaying boot splash
 *          screen.
 * 
 * Copyright 2007 LeapFrog Enterprises Inc.
 *
 * Andrey Yurovsky <andrey@cozybit.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "include/autoconf.h"   /* for partition info */
#include "include/mach-types.h" /* for machine info */
#include <platform.h>
#include <common.h>

#include "include/board.h"
#include "include/dpc_hal.h"

#define DPC16(x)	REG16(LF1000_DPC_BASE+x)
#define DPC32(x)	REG32(LF1000_DPC_BASE+x)

#define DPC_VIDEO_FORMAT	3 /* RGB888 */

#if defined(CONFIG_MACH_ME_LF1000) || defined(CONFIG_MACH_LF_LF1000)
#define DPC_CLK0_SRC		1	/* use PLL1 */
#define DPC_CLK0_DIV		23	/* desired pad clock: 6.4MHz */
#define DPC_CLK0_DELAY		0
#define DPC_CLK0_INV		1	/* invert */
#define DPC_CLK0_OUTENB		0	/* don't connect to pin */
#define DPC_CLK1_SRC		7	/* use clock generator 0 */
#define DPC_CLK1_DIV		0
#define DPC_CLK1_DELAY		0
#define DPC_CLK1_INV		1	/* invert */
#define DPC_PAD_CLK		3	/* inverted VCLK2 */
#define DPC_HSYNC_SWIDTH	2
#define DPC_HSYNC_FRONT_PORCH	50
#define DPC_HSYNC_BACK_PORCH	36
#define DPC_HSYNC_ACTIVEHIGH	0
#define DPC_VSYNC_SWIDTH	1
#define DPC_VSYNC_FRONT_PORCH	4
#define DPC_VSYNC_BACK_PORCH	17
#define DPC_VSYNC_ACTIVEHIGH	0
#else /* CONFIG_MACH_LF_ME2530 */
#define DPC_CLK0_SRC		1	/* use PLL1 */
#define DPC_CLK0_DIV		23	/* FIXME desired pad clock: 6.0MHz */
#define DPC_CLK0_DELAY		0
#define DPC_CLK0_INV		1	/* invert */
#define DPC_CLK0_OUTENB		0	/* don't connect to pin */
#define DPC_CLK1_SRC		7	/* use clock generator 0 */
#define DPC_CLK1_DIV		0
#define DPC_CLK1_DELAY		0
#define DPC_CLK1_INV		1	/* invert */
#define DPC_PAD_CLK		1	/* inverted VCLK2 */
#define DPC_HSYNC_SWIDTH	1
#define DPC_HSYNC_FRONT_PORCH	50
#define DPC_HSYNC_BACK_PORCH	38
#define DPC_HSYNC_ACTIVEHIGH	1
#define DPC_VSYNC_SWIDTH	1
#define DPC_VSYNC_FRONT_PORCH	4
#define DPC_VSYNC_BACK_PORCH	18
#define DPC_VSYNC_ACTIVEHIGH	1
#endif

void dpc_init(void)
{
	u32 tmp;

	/* set PCLK to run only when CPU accesses it */
	BIT_CLR(DPC32(DPCCLKENB),_PCLKMODE);

	/* 
	 * set up DPC Clock 0 
	 */

	tmp = DPC32(DPCCLKGEN0);
	tmp &= ~((7<<CLKSRCSEL0)|(0x3F<<CLKDIV0)|(3<<OUTCLKDELAY0));
	tmp |= (DPC_CLK0_SRC<<CLKSRCSEL0);
	tmp |= ((0x3F & DPC_CLK0_DIV)<<CLKDIV0);
	tmp |= (DPC_CLK0_DELAY<<OUTCLKDELAY0);
	if(DPC_CLK0_INV)
		BIT_SET(tmp, OUTCLKINV0);
	else
		BIT_CLR(tmp, OUTCLKINV0);
	if(DPC_CLK0_OUTENB)
		BIT_SET(tmp, OUTCLKENB);
	else
		BIT_CLR(tmp, OUTCLKENB);
	DPC32(DPCCLKGEN0) = tmp;

	/*
	 * set up DPC Clock 1
	 */

	tmp = DPC32(DPCCLKGEN1);
	tmp &= ~((7<<CLKSRCSEL1)|(0x3F<<CLKDIV1)|(3<<OUTCLKDELAY1));
	tmp |= (DPC_CLK1_SRC<<CLKSRCSEL1);
	tmp |= ((0x3F & DPC_CLK1_DIV)<<CLKDIV1);
	tmp |= (DPC_CLK1_DELAY<<OUTCLKDELAY1);
	if(DPC_CLK1_INV)
		BIT_SET(tmp, OUTCLKINV1);
	else
		BIT_CLR(tmp, OUTCLKINV1);
	DPC32(DPCCLKGEN1) = tmp;

	/* 
	 * enable the clock generator 
	 */

	BIT_SET(DPC32(DPCCLKENB), _CLKGENENB);

	/*
	 * Set up control registers
	 */

	BIT_CLR(DPC16(DPCCTRL0), _INTPEND);
	/* turn off interlace, field invert, and embedded sync */
	DPC16(DPCCTRL0) &= ~((1<<SCANMODE)|(1<<POLFIELD)|(1<<SEAVENB)|
				(0xF<<DELAYRGB));
	/* use RGB mode */
	BIT_SET(DPC16(DPCCTRL0), RGBMODE);
	/* set horizintal sync active high */
	if(DPC_HSYNC_ACTIVEHIGH)
		BIT_SET(DPC16(DPCCTRL0), POLHSYNC);
	else
		BIT_CLR(DPC16(DPCCTRL0), POLHSYNC);
	/* set vertical sync active high */
	if(DPC_VSYNC_ACTIVEHIGH)
		BIT_SET(DPC16(DPCCTRL0), POLVSYNC);
	else
		BIT_CLR(DPC16(DPCCTRL0), POLVSYNC);

	DPC16(DPCCTRL1) &= ~(0xAFFF); /* clear all but reserved bits */
	DPC16(DPCCTRL1) |= (DPC_VIDEO_FORMAT<<FORMAT);

	DPC16(DPCCTRL2) &= ~(3<<PADCLKSEL);
	DPC16(DPCCTRL2) |= (DPC_PAD_CLK<<PADCLKSEL);

	/*
	 * Set up horizonal sync
	 */

	DPC16(DPCHTOTAL) = DPC_HSYNC_SWIDTH + DPC_HSYNC_FRONT_PORCH + 
		DPC_HSYNC_BACK_PORCH + X_RESOLUTION - 1;
	DPC16(DPCHSWIDTH) = DPC_HSYNC_SWIDTH - 1;
	DPC16(DPCHASTART) = DPC_HSYNC_SWIDTH + DPC_HSYNC_BACK_PORCH - 1;
	DPC16(DPCHAEND) = DPC_HSYNC_SWIDTH + DPC_HSYNC_BACK_PORCH + 
		X_RESOLUTION - 1;

	/*
	 * Set up vertical sync
	 */

	DPC16(DPCVTOTAL) = DPC_VSYNC_SWIDTH + DPC_VSYNC_FRONT_PORCH + 
		DPC_VSYNC_BACK_PORCH + Y_RESOLUTION - 1;
	DPC16(DPCVSWIDTH) = DPC_VSYNC_SWIDTH - 1;
	DPC16(DPCVASTART) = DPC_VSYNC_SWIDTH + DPC_VSYNC_BACK_PORCH - 1;
	DPC16(DPCVAEND) = DPC_VSYNC_SWIDTH + DPC_VSYNC_BACK_PORCH +
		Y_RESOLUTION - 1;

	/* 
	 * Set up delays 
	 */

#ifdef CPU_LF1000
	DPC16(DPCDELAY0) = ((7<<DELAYHS)|(7<<DELAYVS)|(7<<DELAYDE));
#elif defined CPU_MF2530F
	DPC16(DPCDELAY0) = ((4<<DELAYHS)|(4<<DELAYVS)|(4<<DELAYDE));
	DPC16(DPCDELAY1) = ((4<<DELAYLP)|(4<<DELAYSP)|(4<<DELAYREV));
#endif

	/*
	 * Set vsync offset
	 */

#ifdef CPU_LF1000
	DPC16(DPCVSEOFFSET) = 1;
	DPC16(DPCVSSOFFSET) = 1;
	DPC16(DPCEVSEOFFSET) = 1;
	DPC16(DPCEVSSOFFSET) = 1;
#elif defined CPU_MF2530F
	DPC16(DPCVSEOFFSET) = 0;
	DPC16(DPCVSSOFFSET) = 0;
	DPC16(DPCEVSEOFFSET) = 0;
	DPC16(DPCEVSSOFFSET) = 0;
#endif

	/*
	 * Finally, turn on the DPC!
	 */

	BIT_SET(DPC16(DPCCTRL0), DPCENB);
}
