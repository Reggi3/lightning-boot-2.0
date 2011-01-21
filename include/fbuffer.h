/* fbuffer.g
 *
 * Copyright 2010 Joe Burks
 *
 * Contact: Joe Burks (joe@burks-family.us)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef FBUFFER_H
#define FBUFFER_H
#include "common.h"

extern u8 *fbuff_addr;
extern u8 bgcolor;
extern u8 fgcolor;

extern void fbclear();
extern void fbinit();
extern void renderAlNum(int x, int y, char val );
extern void renderHexNyb(int x, int y, int val );
extern void renderString(int x, int y, char *str);
extern void renderHexByte(int x, int y, int val);
extern void renderHexU32(int x, int y, u32 val);
extern void dumpData(u8 *data, int size);
extern void fbscroll();
void fb_showerror(u32 v1, u32 v2);
void fbsetInverse(u8 val);

#endif

