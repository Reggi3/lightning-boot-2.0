/*
 *  COPYRIGHT (C) 1986 Gary S. Brown.  You may use this program, or
 *  code or tables extracted from it, as desired without restriction.
 *
 */
 
#ifndef CRC32_H
#define CRC32_H

#include <common.h>

extern const u32 crc32_table[256];

/* Return a 32-bit CRC of the contents of the buffer. */

static inline u32 crc32(u32 val, const void *ss, int len)
{
	const unsigned char *s = ss;
	while (--len >= 0)
		val = crc32_table[(val ^ *s++) & 0xff] ^ (val >> 8);
	return val;
}

void make_crc_table();


#endif
