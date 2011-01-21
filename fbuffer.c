/* fbuffer.c - Frame buffer driver for LF1000
 *
 * Copyright 2010 Joe Burks
 *
 * Contact: Joe Burks (joe@burks-family.us)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <string.h>
#include "include/board.h"
#include "include/common.h"
#include "include/font.h"
#include "include/fbuffer.h"

u8 bgcolor;
u8 fgcolor;
u8 inverse;

#define HSTRIDE 1

void fb_showerror(u32 v1, u32 v2)
{
	int i,j;
	u32 *fb_start = (u32 *)FRAME_BUFFER_ADDR;
	bgcolor = 0x1;
	fgcolor = 0x2;

	// Red outer rectangle
	for ( i = 0; i < (320*8*7)/4; i++ )
		*fb_start++ = 0x03030303; // Red #2

	// Black inner rectangle
	fb_start = (u32*)(FRAME_BUFFER_ADDR + (320*8) + 8);
	for ( i = 0; i < 8*5; i++ ) {
		for ( j = 0; j < (320-16)/4; j++ )
			*fb_start++ = 0x01010101; // Black
		fb_start += 4;
	}
	renderString( 2,2,"Software Failure.    Press LS button to continue.");
	renderString(10,4,        "Guru Meditation #");
	renderHexU32(27,4, v1);
	renderAlNum (27+8,4,'.');
	renderHexU32(27+9,4, v2);
}


/**
  Initialize framebuffer internal state.
 */
void fbinit() {
	bgcolor = 0x00;
	fgcolor = 0xFF;
	inverse = 0;
}

void fbsetInverse(u8 val) {
	inverse = val;
}

/**
  Erase the framebuffer using background color.
 */
void fbclear() {
	int i,k;
	u32 bg = 0;
	u32 *fb_addr;
	fb_addr = (u32 *)(FRAME_BUFFER_ADDR);
	for ( i = 0, k = 0; i < (320*240*HSTRIDE)/4; i++ ) {
		*fb_addr++ = bg;
	}
}

/**
  Scroll the display one line.
  Note: this is coupled to the notion of an 8 pixel tall font. This should get fixed at some point.
 */
void fbscroll() {
	int i,k;
	u8 *fb_addr;

	memcpy ((void *)(FRAME_BUFFER_ADDR),(void *)(FRAME_BUFFER_ADDR + 320*8*HSTRIDE),320*(240-8)*HSTRIDE);

	fb_addr = (u8 *)(FRAME_BUFFER_ADDR + 320*(240-8)*HSTRIDE);
	for ( i = 0, k = 0; i < 320*8*HSTRIDE; i++ ) {
		*fb_addr++ = bgcolor;
	}
}

/**
  Render an alphanumeric value (actually any ascii value between 32 and 127 inclusive).
  Note: This function assumes coordinate system for a 6x8 font. This should be fixed at some point.

  \param x The X text coordinate of the letter
  \param y The Y text coordinate of the letter
  \param val The character to render
 */
void renderAlNum(int x, int y, char val ) {
	int l,b;
	int off;
	u8 fg = fgcolor;
	u8 bg = bgcolor;
	off = val - ' ';
	u8* fbaddr = (u8 *)(FRAME_BUFFER_ADDR + y*8*320*HSTRIDE + x*6*HSTRIDE);
	if ( inverse ) { fg = bgcolor; bg = fgcolor; }
	for ( l = 0; l < 8; l++ ) {
		u8 line = font[off*8+l] >> 1;
		for ( b = 0; b < 6; b++ ) {
			if ( (line & 128) != 0 ) { fbaddr[0] = fg; }
			else fbaddr[0] = bg;
			line <<= 1;
			fbaddr+=HSTRIDE;
		}
		fbaddr += 320*HSTRIDE - 6*HSTRIDE;
	}
}

/**
  Render a 4-bit hexadecimal "Nybble" to the display.
  Note: This function assumes coordinate system for a 6x8 font. This should be fixed at some point.

  \param x The X text coordinate of the hex digit.
  \param y The Y text coordinate of the hex digit.
  \param val The value to render. 0 <= val <= 15
 */
void renderHexNyb(int x, int y, int val ) {
	if ( val < 0xA ) renderAlNum(x,y,val+'0');
	else renderAlNum(x,y,(val-0xA)+'A');
}

/**
  Render a null terminate string of characters. No intelligent wrapping is performed.
  Note: This function assumes coordinate system for a 6x8 font. This should be fixed at some point.

  \param x The X text coordinate of the hex digit.
  \param y The Y text coordinate of the hex digit.
  \param str The null-terminated string to render.
 */
void renderString(int x, int y, char *str ) {
	while ( *str != 0 ) {
		renderAlNum(x,y,*str);
		str++;
		x++;
	}
}

/**
  Render a byte value as two hex digits.
  Note: This function assumes coordinate system for a 6x8 font. This should be fixed at some point.

  \param x The X text coordinate of the hex byte
  \param y The Y text coordinate of the hex byte
  \param val The byte to write (only low order 8 bits will be rendered)
 */
void renderHexByte(int x, int y, int val) {
	renderHexNyb(x,y,(val>>4)&0xF);
	renderHexNyb(x+1,y,(val)&0xF);
}

/**
  Render a dword as 8 hex digits.
  Note: This function assumes coordinate system for a 6x8 font. This should be fixed at some point.

  \param x The X text coordinate of the hex byte
  \param y The Y text coordinate of the hex byte
  \param val The dword to write
 */
void renderHexU32(int x, int y, u32 val) {
	renderHexByte(x,y,val>>24);
	renderHexByte(x+2,y,(val>>16)&0xFF);
	renderHexByte(x+4,y,(val>>8)&0xFF);
	renderHexByte(x+6,y,(val>>0)&0xFF);

}
