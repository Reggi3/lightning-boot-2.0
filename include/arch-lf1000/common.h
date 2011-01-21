/* Common helper macros for the drivers.
 *
 * Andrey Yurovsky <andrey@cozybit.com> */

#ifndef LF1000_COMMON_H
#define LF1000_COMMON_H

/* bit masking */
#define BIT_SET(v,b)	(v |= (1<<(b)))
#define BIT_CLR(v,b)	(v &= ~(1<<(b)))
#define IS_SET(v,b)	(v & (1<<(b)))
#define IS_CLR(v,b)	!(v & (1<<(b)))
#define BIT_MASK(b)	((1<<(b))-1)

/*
 * make sure bootloaders have basic kernel-style types
 */
#ifndef u8
#define u8	unsigned char
#endif
#ifndef u16
#define u16     unsigned short
#endif
#ifndef u32
#define u32     unsigned int
#endif

#ifndef NULL
#define NULL	0
#endif

/* 
 * register helpers for bootloaders and the like
 */
#define REG8(addr)  *((volatile u8 *)(addr))
#define REG16(addr) *((volatile u16 *)(addr))
#define REG32(addr) *((volatile u32 *)(addr))

void fbcopy (int *data);

#endif
