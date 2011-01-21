/* mmc.c - Basic polling MMC/SD driver
 *
 * Copyright 2010 LeapFrog Enterprises Inc.
 *
 * Andrey Yurovsky <ayurovsky@leapfrog.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __MMC_H__
#define __MMC_H__

/* Enable the host controller and attempt to find and initialize an SD card.
 * The card must be plugged in prior to calling this function.  Returns 0 if
 * the controller was enabled and a card was found and initialized, otherwise a
 * non-zero error code is returned. */
int mmc_init(void);

int mmc_read_sector(u32 sector, u32 *dest);

#endif /* __MMC_H__ */
