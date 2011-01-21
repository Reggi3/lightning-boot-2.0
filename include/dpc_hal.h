/* LF1000 Display Controller (DPC) Driver 
 *
 * dpc_hal.h -- DPC hardware abstraction.
 *
 * Andrey Yurovsky <andrey@cozybit.com> */

#ifndef DHC_HAL_H
#define DHC_HAL_H

/* DPC registers as offsets from DPC_BASE */

#define DPCHTOTAL		0x07C
#define DPCHSWIDTH		0x07E
#define DPCHASTART		0x080
#define DPCHAEND		0x082
#define DPCVTOTAL		0x084
#define DPCVSWIDTH		0x086
#define DPCVASTART		0x088
#define DPCVAEND		0x08A
#define DPCCTRL0		0x08C
#define DPCCTRL1		0x08E
#define DPCEVTOTAL		0x090
#define DPCEVSWIDTH		0x092
#define DPCEVASTART		0x094
#define DPCEVAEND		0x096
#define DPCCTRL2		0x098
#ifdef CPU_LF1000
#define DPCVSEOFFSET		0x09A
#define DPCVSSOFFSET		0x09C
#define DPCEVSEOFFSET		0x09E
#define DPCEVSSOFFSET		0x0A0
#define DPCDELAY0		0x0A2
#define DPRNUMGENCON0		0x0AA
#define DPRNUMGENCON1		0x0AC
#define DPRNUMGENCON2		0x0AC
#define DPCFDTADDR		0x0B0
#define DPCFRDITHERVALUE	0x0B2
#define DPCFGFITHERVALUE	0x0B4
#define DPCFBDITHERVALUE	0x0B6
#elif defined CPU_MF2530F
#define DPCVSEOFFSET		0x09E
#define DPCVSSOFFSET		0x0A0
#define DPCEVSEOFFSET		0x0A2
#define DPCEVSSOFFSET		0x0A4
#define DPCDELAY0		0x0A6
#define DPCDELAY1		0x0A8
#endif
#define DPCCLKENB		0x1C0
#define DPCCLKGEN0		0x1C4
#define DPCCLKGEN1		0x1C8

/* DPC registers for internal video encoder as offsets from DPC_BASE */

#define VENCCTRLA		0x002 
#define VENCCTRLB		0x004 
#define VENCSCH 		0x008 
#define VENCHUE 		0x00A 
#define VENCSAT 		0x00C 
#define VENCCRT 		0x00E 
#define VENCBRT 		0x010 
#define VENCFSCADJH		0x012 
#define VENCFSCADJL		0x014 
#define VENCDACSEL		0x020 
#define VENCICNTL		0x040 
#define VENCHSVS0		0x048 
#define VENCHSOS		0x04A 
#define VENCHSOE		0x04C 
#define VENCVSOS		0x04E 
#define VENCVSOE		0x050  
#define DPUPSCALECON0	0x0A4
#define DPUPSCALECON1	0x0A6
#define DPUPSCALECON2	0x0A8

/* DPC CONTROL 0 REGISTER (DPCCTRL0) */
#define DPCENB		15
#define ENCENB		14
#define DACENB		13
#define RGBMODE		12
#define _INTENB		11
#define _INTPEND	10
#define SCANMODE	9
#define SEAVENB		8
#define DELAYRGB	4
#ifdef CPU_MF2530F
#define POLREV		3
#endif
#define POLFIELD	2
#define POLVSYNC	1
#define POLHSYNC	0

/* DPC CONTROL 1 REGISTER (DPCCTRL1) */
#define SWAPRB		15
#define YCRANGE		13
#define FORMAT		8
#define YCORDER		6
#define BDITHER		4
#define GDITHER		2
#define RDITHER		0

/* DPC CONTROL 2 REGISTER (DPCCTRL2) */
#ifdef CPU_LF1000
#define CPCYC		12	/* STN LCD CP (Shift Clock) Cycle, in VCLKs */
#define STNLCDBITWIDTH	9	/* STN LCD bus bit width */
#define LCDTYPE		8	/* 0: TFT or Video Encoder, 1: STN LCD */
#else
#define LBR		13
#define PSENB		12
#define SPRENB		11
#define SPLENB		10
#define LPENB		8
#endif
#define PADCLKSEL	0

/* DPC CLOCK GENERATION ENABLE REGISTER (DPCCLKENB) */
#define _PCLKMODE	3
#define _CLKGENENB	2

/* DPC CLOCK GENERATION CONTROL 0 REGISTER (DPCCLKGEN0) */
#define OUTCLKENB	15
#define OUTCLKDELAY0	12
#define CLKDIV0		4
#define CLKSRCSEL0	1
#define OUTCLKINV0	0

/* DPC CLOCK GENERATION CONTROL 1 REGISTER (DPCCLKGEN1) */
#define OUTCLKDELAY1	12
#define CLKDIV1		4
#define CLKSRCSEL1	1
#define OUTCLKINV1	0

/* DPC SYNC DELAY 0 REGISTER (DPCDELAY0) */
#define DELAYDE		8
#define DELAYVS		4
#define DELAYHS		0

/* DPC SYNC DELAY 1 REGISTER (DPCDELAY1) */
#define DELAYREV	8
#define DELAYSP		4
#define DELAYLP		0

#endif

