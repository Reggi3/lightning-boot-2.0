/* SDcard.c -- SD bit-bang driver for LF1000
 *
 * Copyright status unknown
 *
 * This file was sent to me by one of the contributors to the #didj IRC
 * channel on freenode. I do not know who the author was or what their
 * copyright intention was. Are you the owner of this code? Am I using
 * it in a way that you do not approve? If so, contact me and I'll make
 * it right. Until then, assuming this is GPLv2-friendly code.
 *
 * Contact: Joe Burks (joe@burks-family.us)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

//-------------------------------------------------------------------------
#include "include/common.h"
#include "include/SDcard.h"

BYTE response_R(BYTE);
BYTE send_cmd(BYTE *);
BYTE send_cmd_fast(BYTE *in);

//-------------------------------------------------------------------------
BYTE response_buffer[20];
static BYTE RCA[2];
BYTE cmd_buffer[5];

BYTE cmd0[5] = {0x40,0x00,0x00,0x00,0x00};//Reset SD Card
BYTE cmd55[5] = {0x77,0x00,0x00,0x00,0x00};//Next CMD is ASC
BYTE cmd2[5] = {0x42,0x00,0x00,0x00,0x00};//asks to send the CID numbers
BYTE cmd3[5] = {0x43,0x00,0x00,0x00,0x00};//Send RCA
BYTE cmd7[5] = {0x47,0x00,0x00,0x00,0x00};//Select one card,put it into Transfer State
BYTE cmd8[5] = {0x48,0x00,0x00,0x01,0xAA};//Interface condition command (V2 check)
BYTE cmd9[5] = {0x49,0x00,0x00,0x00,0x00};//Ask send CSD
BYTE cmd16[5] = {0x50,0x00,0x00,0x02,0x00};//Select a block length
BYTE cmd17[5] = {0x51,0x00,0x00,0x00,0x00};//Read a single block
BYTE acmd6[5] = {0x46,0x00,0x00,0x00,0x02};//SET BUS WIDTH
BYTE cmd24[5] = {0x58,0x00,0x00,0x00,0x00};//Write a single block

//BYTE acmd41[5] = {0x69,0x0f,0xf0,0x00,0x00};//Active Card’s ini process
BYTE acmd41[5] = {0x69,0x4f,0xf0,0x00,0x00};//Active Card’s ini process
BYTE acmd42[5] = {0x6A,0x0f,0xf0,0x00,0x00};//Disable pull up on Dat3
BYTE acmd51[5] = {0x73,0x00,0x00,0x00,0x00};//Read SCR(Configuration Reg)
//-------------------------------------------------------------------------

BYTE sdversion;
BYTE sdcapacity;

// LF1000 locations
#define GPIOBOUT 0xC000A040
#define GPIOBOUTENB 0xC000A044
#define GPIOBPAD 0xC000A058
#define GPIOBALTFN0 0xC000A060

#define SDCLK 0
#define SDCMD 1
#define SDDAT0 2
#define SDDAT1 3
#define SDDAT2 4
#define SDDAT3 5

#define DDRB *((u32 *)GPIOBOUTENB)
#define PORTB *((u32 *)GPIOBOUT)
#define PINB *((u32 *)GPIOBPAD)

// SD Card Set I/O Direction

#define SD_CMD_IN 		DDRB &= ~(1<<SDCMD)
#define SD_CMD_OUT 		DDRB |=  (1<<SDCMD)
#define SD_DAT0_IN 		DDRB &= ~(1<<SDDAT0)
#define SD_DAT0_OUT 	DDRB |=  (1<<SDDAT0)
#define SD_DAT1_IN 		DDRB &= ~(1<<SDDAT1)
#define SD_DAT1_OUT 	DDRB |=  (1<<SDDAT1)
#define SD_DAT2_IN 		DDRB &= ~(1<<SDDAT2)
#define SD_DAT2_OUT 	DDRB |=  (1<<SDDAT2)
#define SD_DAT3_IN 		DDRB &= ~(1<<SDDAT3)
#define SD_DAT3_OUT 	DDRB |=  (1<<SDDAT3)
// SD Card Output High/Low
#define SD_CMD_LOW 		PORTB &= ~(1<<SDCMD)
#define SD_CMD_HIGH 	PORTB |=  (1<<SDCMD)
#define SD_DAT0_LOW 	PORTB &= ~(1<<SDDAT0)
#define SD_DAT0_HIGH 	PORTB |=  (1<<SDDAT0)
#define SD_DAT1_LOW 	PORTB &= ~(1<<SDDAT1)
#define SD_DAT1_HIGH 	PORTB |=  (1<<SDDAT1)
#define SD_DAT2_LOW 	PORTB &= ~(1<<SDDAT2)
#define SD_DAT2_HIGH 	PORTB |=  (1<<SDDAT2)
#define SD_DAT3_LOW 	PORTB &= ~(1<<SDDAT3)
#define SD_DAT3_HIGH 	PORTB |=  (1<<SDDAT3)

#define SD_CLK_LOW 	do { PORTB &= ~(1<<SDCLK); clkDelay(); } while(0)
#define SD_CLK_HIGH 	do { clkDelay(); PORTB |=  (1<<SDCLK); } while(0)

#define SD_FAST_CLK_LOW		do { PORTB &= ~(1<<SDCLK); asm volatile ("nop"); } while(0)
#define SD_FAST_CLK_HIGH	do { asm volatile ("nop"); PORTB |=  (1<<SDCLK); } while(0)

// SD Card Input Read
#define SD_TEST_CMD 	(PINB & (1<<SDCMD))
#define SD_TEST_DAT0 	(PINB & (1<<SDDAT0))
#define SD_TEST_DAT1 	(PINB & (1<<SDDAT1))
#define SD_TEST_DAT2 	(PINB & (1<<SDDAT2))
#define SD_TEST_DAT3 	(PINB & (1<<SDDAT3))

void clkDelay(void)
{
	int x;
	for(x=0;x<1;x++) asm volatile ("nop");
}

void NupAndDown(void)
{
  	SD_CLK_LOW; SD_CLK_HIGH;
}

inline void NupAndDownFast(void)
{
  	SD_FAST_CLK_LOW; SD_FAST_CLK_HIGH;
}

void Ncr(void)
{
  int x;
    SD_CMD_IN;
  for (x=0;x<2;x++) {
    NupAndDown();
  }
}

void NcrFast(void)
{
  int x;
    SD_CMD_IN;
  for (x=0;x<2;x++) {
    NupAndDownFast();
  }
}
//-------------------------------------------------------------------------
void Ncc(void)
{
  int x;
  for (x=0;x<8;x++) {
  	NupAndDown();
  }
}
void NccFast(void)
{
  int x;
  for (x=0;x<8;x++) {
  	NupAndDownFast();
  }
}
//-------------------------------------------------------------------------

void doCmd(BYTE *buf)
{
    int x;
    Ncc();
    for(x=0;x<5;x++) cmd_buffer[x]=buf[x];
    send_cmd(cmd_buffer);//Probe SDV2 compatibility
    Ncr();
}

void doCmdRCA(BYTE *buf)
{
    int x;
    Ncc();
    for(x=0;x<5;x++) cmd_buffer[x]=buf[x];
    cmd_buffer[1] = RCA[0];
    cmd_buffer[2] = RCA[1];
    send_cmd(cmd_buffer);//Ask send CSD
    Ncr();
}

BYTE SdCardInit(void)
{
    BYTE x,y;
	DDRB |= (1<<SDCLK);  //Clock as input
	SD_CMD_IN; SD_CMD_LOW;
	SD_DAT0_IN; SD_DAT0_LOW;
	SD_DAT1_IN; SD_DAT1_LOW;
	SD_DAT2_IN; SD_DAT2_LOW;
	SD_DAT3_IN; SD_DAT3_LOW;

    SD_CMD_OUT;
	SD_CLK_HIGH; SD_CMD_HIGH;
	
	for(x=0;x<40;x++) Ncr();
    for(x=0;x<5;x++) cmd_buffer[x]=cmd0[x];
	y = send_cmd(cmd_buffer);//Reset SD Card
    doCmd(cmd8);
    if (response_R(7)>1) {
	sdversion = 1;
    } else {
	sdversion = 2;
    }
    do
      {
            // for(x=0;x<40;x++) asm volatile ("nop");
	    clkDelay();
            doCmd(cmd55);
			if(response_R(1)>1)  {
				return 1;//response too long or crc error
			}
            doCmd(acmd41);
      } while(response_R(3)==1);
    if((response_buffer[1]&0x40) == 0) sdcapacity=1;
    else sdcapacity=2;

    doCmd(cmd2);
    if(response_R(2)>1) return 2;

    doCmd(cmd3);
    if(response_R(6)>1) return 3;

    RCA[0]=response_buffer[1];
    RCA[1]=response_buffer[2];
    doCmdRCA(cmd9);
    if(response_R(2)>1) return 4;
	
    doCmdRCA(cmd7);
    if(response_R(1)>1) return 5;
	
	//change to 4bit bus width
	do
      {
            for(x=0;x<40;x++) asm volatile ("nop");
	    doCmdRCA(cmd55);
	    if(response_R(1)>1) return 6;//response too long or crc error
               
	    doCmd(acmd6);
      } while(response_R(1)==1);
	
	doCmd(cmd16);
	if (response_R(1)>1) return 7;

    return 0;
}
//-------------------------------------------------------------------------
BYTE SdReadBlock(UINT32 Block, BYTE *buff)
{	  
	unsigned int timeout = 0;

	NccFast();
	cmd_buffer[0] = cmd17[0];
	if ( sdcapacity == 1 ) {
		Block <<= 9;
	}
	cmd_buffer[1] = (Block>>24)&0xff;
	cmd_buffer[2] = (Block>>16)&0xff;
	cmd_buffer[3] = (Block>>8)&0xff;
	cmd_buffer[4] = Block&0xFF;
        send_cmd_fast(cmd_buffer);
        NcrFast();

    while(1) //Wait until startbit comes
	  {
		NupAndDownFast();
		if ( ((PINB >> 2) & 0xF) == 0 ) break;
		if (timeout++ > 65534) return 0x10;
	   }
 	int i; for(i=0;i<512;i++) // Read 512 Bytes
       {
			register unsigned char c = 0;
			NupAndDownFast();
		c |= (PINB << 2) & 0xF0;

			NupAndDownFast();
		c |= (PINB >> 2) & 0xF;
            *buff++=c;
        }
	 for (i = 0; i < 16; i++) //Dummy read CRC
	   {
			NupAndDownFast();
	   }
    return 0;
}

//-------------------------------------------------------------------------
BYTE response_R(BYTE s)
{
    BYTE a=0,b=0,c=0,r=0,crc=0;
    BYTE i,j=6,k;
    while(1)
      {
            SD_CLK_LOW;
            SD_CLK_HIGH;
            if(!(SD_TEST_CMD)) break;
            if(crc++ >254) { return 2; }
      }
    crc =0;
    if(s == 2) j = 17;
    for(k=0; k<j; k++)
      {
            c = 0;
            if(k > 0) //for crc culcar
                b = response_buffer[k-1];
            for(i=0; i<8; i++)
              {
                    SD_CLK_LOW;
                    if(a > 0) c <<= 1;
                    else i++;
                    a++;
                    SD_CLK_HIGH;
                    if(SD_TEST_CMD) c |= 0x01;
                    if(k > 0)
                      {
                            crc <<= 1;
                            if((crc ^ b) & 0x80)
                            crc ^= 0x09;
                            b <<= 1;
                            crc &= 0x7f;
                      }
              }
            if(s==3)
              if( k==1 &&(!(c&0x80))) r=1;
            response_buffer[k] = c;
      }
    if(s==1 || s==6)
      if(c != ((crc<<1)+1)) r=2;

    if(s==7)
	if( response_buffer[4] != 0xAA ) r=0;

    return r;
}
//-------------------------------------------------------------------------

BYTE send_cmd(BYTE *in)
{
    int i,j;
    BYTE b,crc=0;
    SD_CMD_OUT;
    for(i=0; i < 5; i++)
      {
            b = in[i];
            for(j=0; j<8; j++)
              {
                    SD_CLK_LOW;
                    if(b&0x80) SD_CMD_HIGH;
                    else SD_CMD_LOW;
                    crc <<= 1;
                    SD_CLK_HIGH;
                    if((crc ^ b) & 0x80) crc ^= 0x09;
                    b<<=1;
              }
            crc &= 0x7f;
      }
    crc =((crc<<1)|0x01);
    b = crc;
    for(j=0; j<8; j++)
      {
            SD_CLK_LOW;
            if(crc&0x80) SD_CMD_HIGH;
            else SD_CMD_LOW;
            SD_CLK_HIGH;
            crc<<=1;
      }
    return b;
}

BYTE send_cmd_fast(BYTE *in)
{
    int i,j;
    BYTE b,crc=0;
    SD_CMD_OUT;
    for(i=0; i < 5; i++)
      {
            b = in[i];
            for(j=0; j<8; j++)
              {
                    SD_FAST_CLK_LOW;
                    if(b&0x80) SD_CMD_HIGH;
                    else SD_CMD_LOW;
                    crc <<= 1;
                    SD_FAST_CLK_HIGH;
                    if((crc ^ b) & 0x80) crc ^= 0x09;
                    b<<=1;
              }
            crc &= 0x7f;
      }
    crc =((crc<<1)|0x01);
    b = crc;
    for(j=0; j<8; j++)
      {
            SD_FAST_CLK_LOW;
            if(crc&0x80) SD_CMD_HIGH;
            else SD_CMD_LOW;
            SD_FAST_CLK_HIGH;
            crc<<=1;
      }
    return b;
}

