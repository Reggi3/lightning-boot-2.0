/* mlc.c -- Basic Multi-Layer Controller (MLC) Driver for displaying boot
 *          splash screen.
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
#include "include/mlc_hal.h"

#define MLC32(x)	REG32(LF1000_MLC_BASE+x)
#define R5G6B5(r,g,b)   (((r&0x1F)<<11) | ((g&0x3F)<<5) | (b&0x1F))

#define MLC_LOCK_SIZE	8
#ifdef CPU_LF1000
#define MLC_PRIORITY	0
#else /* CPU_MF2530F */
#define MLC_PRIORITY	3
#endif

/*
 * Basic MLC initialization: our goal is just to bring up Layer 0 so that it 
 * can be used to display an RGB splash screen.
 */
void mlc_init(void)
{
	/* dynamic bus clock, PCLK on CPU access */
	MLC32(MLCCLKENB) &= ~(0xF);
	MLC32(MLCCLKENB) |= (2<<BCLKMODE); 

	/* set resolution */
	MLC32(MLCSCREENSIZE) = (((X_RESOLUTION-1)<<SCREENWIDTH)|
				((Y_RESOLUTION-1)<<SCREENHEIGHT));

	/* set up layer priority, turn off field */	
	MLC32(MLCCONTROLT) &= ~((0x3<<PRIORITY)|(1<<FIELDENB));
	MLC32(MLCCONTROLT) |= (MLC_PRIORITY<<PRIORITY);

	/* make the background white */
	MLC32(MLCBGCOLOR) = 0xFFFFFF;

	/* 
	 * Set up Layer 0 
	 */
#if 1
	MLC32(MLCADDRESS0) = FRAME_BUFFER_ADDR;	
	MLC32(MLCCONTROL0) &= ~(3<<LOCKSIZE);
	MLC32(MLCCONTROL0) |= ((MLC_LOCK_SIZE/8)<<LOCKSIZE);
	MLC32(MLCHSTRIDE0) = 1; /* 1 bytes per pixel */
	MLC32(MLCVSTRIDE0) = 1*X_RESOLUTION;
#ifdef CPU_LF1000
	MLC32(MLCLEFTRIGHT0) = ((0<<LEFT)|((X_RESOLUTION-1)<<RIGHT));
	MLC32(MLCTOPBOTTOM0) = ((0<<TOP)|((Y_RESOLUTION-1)<<BOTTOM));
#elif defined CPU_MF2530F
	MLC32(MLCLEFTOP0) = ((0<<TOP)|(0<<LEFT));
	MLC32(MLCRIGHTBOTTOM0) = (((X_RESOLUTION-1)<<RIGHT)|
				((Y_RESOLUTION-1)<<BOTTOM));
#endif
	MLC32(MLCCONTROL0) &= ~(0xFFFF<<FORMAT);
	// MLC32(MLCCONTROL0) |= (0xC653<<FORMAT); /* B8G8R8 */
	MLC32(MLCCONTROL0) |= (0x443a<<FORMAT); /* PTR5G6B5 */
	MLC32(MLCCONTROL0) |= (0x3<<PALETTESLD);

	MLC32(MLCPALETTE0) = R5G6B5(0,0,0x1F);
	MLC32(MLCPALETTE0) = (0xFF<<24) | R5G6B5(0x1F,0x3F,0x1F);
	MLC32(MLCPALETTE0) = (0x1<<24) | R5G6B5(0x0,0x0,0x0);
	MLC32(MLCPALETTE0) = (0x2<<24) | R5G6B5(0x1F,0x0,0x0);
	MLC32(MLCPALETTE0) = (0x3<<24) | R5G6B5(0x1F,0x0,0x0);

	MLC32(MLCCONTROL0) |= (1<<DIRTYFLAG);

	/* 
	 * Turn on Layer 0 
	 */

#ifdef CPU_LF1000
	BIT_SET(MLC32(MLCCONTROL0), PALETTEPWD); /* power up */
	BIT_SET(MLC32(MLCCONTROL0), PALETTESLD); /* unsleep */
#endif
	BIT_SET(MLC32(MLCCONTROL0), LAYERENB);	/* enable */
	BIT_SET(MLC32(MLCCONTROL0), DIRTYFLAG);	/* apply */
#else
	mlc_set_video_mode();
#endif
	/*
	 * Finally, turn on the MLC!
	 */

#ifdef CPU_LF1000
	BIT_SET(MLC32(MLCCONTROLT), PIXELBUFFER_PWD); /* power up */
	BIT_SET(MLC32(MLCCONTROLT), PIXELBUFFER_SLD); /* unsleep */
#endif
	BIT_SET(MLC32(MLCCONTROLT), MLCENB);	/* enable */
	BIT_SET(MLC32(MLCCONTROLT), DITTYFLAG); /* apply */
}

void mlc_set_palette_entry(u8 p, u8 r, u8 g, u8 b)
{
	u32 idx = p;
	MLC32(MLCPALETTE0) = (idx << 24) | R5G6B5(r,g,b);
	MLC32(MLCCONTROL0) |= (1<<DIRTYFLAG);
}

void mlc_set_video_mode(void) {
	MLC32(MLCADDRESS0) = FRAME_BUFFER_ADDR;	
	MLC32(MLCCONTROL0) &= ~(3<<LOCKSIZE);
	MLC32(MLCCONTROL0) |= ((MLC_LOCK_SIZE/8)<<LOCKSIZE);
	MLC32(MLCHSTRIDE0) = 3; /* 3 bytes per pixel */
	MLC32(MLCVSTRIDE0) = 3*X_RESOLUTION;

	MLC32(MLCCONTROL0) &= ~(0xFFFF<<FORMAT);
	MLC32(MLCCONTROL0) |= (0xC653<<FORMAT); /* B8G8R8 */
	MLC32(MLCCONTROL0) |= (1<<DIRTYFLAG);
}

