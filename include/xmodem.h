#ifndef __XMODEM_H__
#define __XMODEM_H__
/* =============================================================================

    Copyright (c) 2006 Pieter Conradie <pieterconradie@users.berlios.de>
    All rights reserved.
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    
    * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
    
    * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
    
    * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.
    
    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
    
    Title:          XMODEM-CRC receive module
    Author(s):      Pieter Conradie
    Creation Date:  2007-03-31
    Revision Info:  $Id: xmodem.h 1 2008-12-02 07:02:22Z Pieter.Conradie $

============================================================================= */

/** 
 *  @ingroup AVR_XMODEM_BOOTLOADER
 *  @defgroup XMODEM xmodem.h : XMODEM-CRC receive module
 *
 *  Receive a file via the XMODEM-CRC protocol. 
 *
 *  Files: xmodem.h & xmodem.c
 *
 *  XMODEM is a simple file transfer protocol. The file is broken up into a 
 *  series of 128-byte data packets that are sent to the receiver. The packet 
 *  is prefixed with a 3 byte header containing a SOH character, 
 *  a "block number" from 0-255, and the "inverse" block number (255 minus 
 *  the block number). A 16-bit CRC is appended to the data.
 * 
 *  A single character response is sent by the receiver to control the flow of
 *  packets:
 *  - C   (0x43) : ASCII 'C' to start an XMODEM transfer using CRC
 *  - ACK (0x06) : Acknowledge 
 *  - NAK (0x15) : Not Acknowledge
 *  - EOT (0x04) : End of Transmission 
 * 
 *  A callback function must be specified to handle the received data.
 *  
 *  @see http://en.wikipedia.org/wiki/XMODEM
 *  
 *  @{
 */

/* _____PROJECT INCLUDES_____________________________________________________ */
#include "include/xmodem/common.h"

/* _____TYPE DEFINITIONS_____________________________________________________ */
/// Definition for a pointer to a function that will be called once a frame has been received
typedef void (*xmodem_on_rx_data_t)(u8 *data, u8 bytes_received);

/* _____FUNCTION DECLARATIONS_________________________________________ */
/**
 *  Blocking function that receives a file transferred with the XMODEM-CRC protocol.
 * 
 * @param on_rx_data Pointer to a function that will be called with each 
 *                   correctly received packet.
 * 
 * @retval TRUE     File succesfully received
 * @retval FALSE    Timed out while trying to receive a file
 */
extern bool_t xmodem_rx_file(xmodem_on_rx_data_t on_rx_data);

/**
 * @}
 */

#endif
