/* diskio.c -- PetitFS wrapper to SD driver
 *
 * Copyright 2010 Joe Burks and Copyright 2009 by ChaN (see below)
 *
 * Joe Burks <joe@burks-family.us>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
 
/*----------------------------------------------------------------------------/
/  Petit FatFs - FAT file system module  R0.02                 (C)ChaN, 2009
/-----------------------------------------------------------------------------/
/ Petit FatFs module is an open source software to implement FAT file system to
/ small embedded systems. This is a free software and is opened for education,
/ research and commercial developments under license policy of following trems.
/
/  Copyright (C) 2009, ChaN, all right reserved.
/
/ * The Petit FatFs module is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial use UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/-----------------------------------------------------------------------------/

/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for Petit FatFs (C)ChaN, 2009      */
/*-----------------------------------------------------------------------*/

#include "include/diskio.h"
#include "include/SDcard.h"
#include "include/pff.h"

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (void)
{
	DSTATUS stat = RES_OK;
	BYTE r;

	r = SdCardInit();
	if ( r != 0 ) {
		return RES_NOTRDY;
	}

	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Partial Sector                                                   */
/*-----------------------------------------------------------------------*/

DRESULT disk_readp (
	BYTE* dest,			/* Pointer to the destination object */
	DWORD sector,		/* Sector number (LBA) */
	WORD sofs,			/* Offset in the sector */
	WORD count			/* Byte count (bit15:destination) */
)
{
	DRESULT res = RES_OK;
	BYTE buff[512];
	BYTE *src;
	BYTE r;
	WORD i;

	r = SdReadBlock(sector, buff);
	if ( r != 0 ) {
		return RES_ERROR;
	}

	src = &buff[sofs];
	for ( i = 0; i < count; i++ ) {
		*dest++ = *src++;
	}

	return res;
}

/*-----------------------------------------------------------------------*/
/* Write Partial Sector                                                  */
/*-----------------------------------------------------------------------*/

DRESULT disk_writep (
	const BYTE* buff,	/* Pointer to the data to be written, NULL:Initiate/Finalize write operation */
	DWORD sc		/* Sector number (LBA) or Number of bytes to send */
)
{
	return RES_ERROR;
}
