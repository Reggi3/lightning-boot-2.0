/* bootstrap.c -- wrapper for nand_read() to enable self-bootstrapping 
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
#include <gpio_hal.h>
#include <clkpwr.h>
#include "include/nand.h"

/* Bootstrap settings: these are determined by the hardware.  The LF1000 CPU's
 *                     NAND Boot state machine loads 0x800 (one Large Block
 *                     NAND page) of data for us.  We need to load the rest,
 *                     which is the boot sector size minus what's already 
 *                     loaded. */
#define BOOTSTRAP_SIZE		0x800
#define BOOT_SECTOR_SIZE	0x20000	/* assuming 128K sector */

void bootstrap(void)
{	
	/* 'latch' the system power so that user may let go of power switch */
#ifdef CPU_LF1000
	/* enable access to Alive GPIO */
	REG32(LF1000_ALIVE_BASE+ALIVEPWRGATEREG) = 1;
	/* pull VDDPWRON high by setting the flip-flop */
	BIT_SET(REG32(LF1000_ALIVE_BASE+ALIVEGPIOSETREG), VDDPWRONSET);
	/* reset flip-flop to latch in */
	REG32(LF1000_ALIVE_BASE+ALIVEGPIOSETREG) = 0;
	REG32(LF1000_ALIVE_BASE+ALIVEGPIORSTREG) = 0;
#endif

	/* Note: Turn off this option if you are testing lightning-boot by
	 *       forcing it into address 0 via u-boot or some other means like
	 *       JTAG. */
	/* load the rest of the 128K boot sector (hardware loaded 2K) */
#ifdef NAND_RELEASE
	nand_read((u32 *)BOOTSTRAP_SIZE, BOOTSTRAP_SIZE, 
			BOOT_SECTOR_SIZE-BOOTSTRAP_SIZE);
#endif
}
