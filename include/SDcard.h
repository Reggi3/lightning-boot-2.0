/* SDcard.h
 *
 * Copyright 2010 Joe Burks
 *
 * Contact: Joe Burks (joe@burks-family.us)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef SDCARD_H
#define SDCARD_H

/**********************************************************************/

// #include "Utils.h"

//-------------------------------------------------------------------------
#define BYTE unsigned char
#define UINT16 unsigned int
#define UINT32 unsigned long
//-------------------------------------------------------------------------
extern BYTE SdReadBlock(UINT32 Block, BYTE *buff);
extern BYTE SdWriteBlock(UINT32 Block, BYTE *buff);
extern BYTE SdCardInit(void);
extern void clkDelay(void);

//-------------------------------------------------------------------------

#endif //SDCARD_H
