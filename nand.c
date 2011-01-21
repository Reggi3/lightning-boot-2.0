/* nand.c -- LF1000 NAND Controller Driver
 *
 * Copyright 2007 LeapFrog Enterprises Inc.
 *
 * Andrey Yurovsky <andrey@cozybit.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Note: This driver is designed to enable booting from a large-block NAND 
 *       Flash on the LF1000 CPU, using hardware ECC.  Other Flash types and
 *       OOB layouts are not supported.
 *
 * Note: On the MP2530F CPU, the ECC functionality is disabled.
 */

#include "include/autoconf.h"
#include <platform.h>
#include <common.h>
#include <nand.h>

#include "include/nand.h"

/*
 * NAND controller access
 */
#define nand_command(cmd)	(REG8(LF1000_NAND_BASE+NFCMD) = cmd)
#define nand_address(addr)	(REG8(LF1000_NAND_BASE+NFADDR) = addr)
#define NFDATA8()		REG8(LF1000_NAND_BASE+NFDATA)
#define NFDATA16()		REG16(LF1000_NAND_BASE+NFDATA)
#define NFDATA32()		REG32(LF1000_NAND_BASE+NFDATA)

/*
 * internal functions
 */

static void nand_send_address(u32 page, u32 column)
{
	nand_command(CMD_READ);
	nand_address(column>>0);
	nand_address(column>>8);
	nand_address(page>>0);
	nand_address(page>>8);
	nand_address(page>>16);
	nand_command(CMD_READ_CONFIRM);
}

static void nand_wait_for_ready(void)
{
	while(IS_CLR(REG32(LF1000_MCU_S_BASE+NFCONTROL), RnB));
}

/*
 * API functions
 */

/* assume: offset is NAND erase block size aligned */
int nand_check_block(u32 offset)
{
	offset >>= 11;
	int i;

	for(i = 0; i < 2; i++) {
		nand_send_address(offset+i, NAND_PAGE_SIZE);
		nand_wait_for_ready();
		nand_command(CMD_READ);
		if((NFDATA8() & 0xFF) != 0xFF) {
			return 1;
		}
	}

	return 0;
}

/* assume: size is NAND page size aligned */
int nand_read(u32 *buf, u32 offset, u32 size)
{
	u32 page = (offset>>11);
	u32 oob_buf[NAND_OOB_SIZE>>2];
	int i, step, p_count = 0;

	/* use the size for 32-bit chunks */
	size >>=2;

	do { /* read one page at a time */
		/* read the OOB for this page */
		nand_send_address(page, NAND_PAGE_SIZE);
		nand_wait_for_ready();
		nand_command(CMD_READ);
		for(i = 0; i < (NAND_OOB_SIZE>>2); i++)
			oob_buf[i] = NFDATA32();

		if(p_count < 2 && (oob_buf[0] & 0xFF) != 0xFF) { /*bad block*/
			page += (NAND_EB_SIZE>>11) - p_count;
			if(p_count != 0) /* drop previous page from buffer */
				buf -= NAND_PAGE_SIZE;
			p_count = 0;
			continue;
		}

		/* now read the page data in BCH-size chunks */
		for(step = 0; step < NAND_ECC_STEPS; step++) {
#ifdef CPU_LF1000
			/* retrieve the ECC, store in ORGECC for BCH decoder */
			REG32(LF1000_MCU_S_BASE+NFORGECCL) = 0;//NFDATA32();
			REG32(LF1000_MCU_S_BASE+NFORGECCH) = 0;//NFDATA16();
#endif /* CPU_LF1000 */
			/* read data from this chunk */
			nand_send_address(page, step*NAND_BCH_SIZE);
			nand_wait_for_ready();
			nand_command(CMD_READ);
			for(i = 0; i < (NAND_PAGE_SIZE/NAND_ECC_STEPS/4); i++){
				*buf++ = NFDATA32();
				--size;
			}

#ifdef CPU_LF1000
			/* wait for decoder to finish */
			while(IS_CLR(REG32(LF1000_MCU_S_BASE+NFECCSTATUS),
						NFECCDECDONE));

			/* did decoder detect an error? */
			if(IS_SET(REG32(LF1000_MCU_S_BASE+NFECCSTATUS),
						NFCHECKERROR)) {
				//db_putchar('!');
			}
#endif /* CPU_LF1000 */
		}
		page++;
		if(++p_count > 63)
			p_count = 0;
	} while(size > 0);

	return 0;
}
