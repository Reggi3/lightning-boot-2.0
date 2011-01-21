/*
 * linux/include/asm-arm/arch-lf1000/platform.h
 *
 * Copyright LeapFrog Enterprises Inc.
 *
 * Yakov A. Blyakhman <YBlyakhman@leapfrog.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 */

#ifndef _LF1000_PLATFORM_H
#define _LF1000_PLATFORM_H

/* macro to get at IO space when running virtually */
#define	IO_ADDRESS_OFF		0xf0000000
#define IO_ADDRESS(x)		(((x) & 0x00ffffff) + (((x) >> 4) & 0x0ff00000) + IO_ADDRESS_OFF)

/*
 * Platform CPU Selection
 */

#if	defined	CONFIG_MACH_ME_MP2530F
	#define	CPU_MF2530F
#elif	defined	CONFIG_MACH_LF_MP2530F
	#define	CPU_MF2530F
#elif	defined	CONFIG_MACH_ME_LF1000
	#define	CPU_LF1000
#elif	defined	CONFIG_MACH_LF_LF1000
	#define	CPU_LF1000
#else
	#error NO Platform Selected
#endif

/* ------------------------------------------------------------------------
 *  Lf1000 Registers
 * ------------------------------------------------------------------------
 * FIXME: these shoudn't be needed, clean up this file accordingly?
 */
#define LF1000_SYS_IO				0xc0000000
#define LF1000_SYS_BASE				0xc0000000
#if	defined	CONFIG_MACH_LF_MP2530F
	#define LF1000_ETH_BASE			0x84000000 /*XXX: STATIC1 */
#else
	#define LF1000_ETH_BASE			0x88000000 /*XXX: STATIC1 */
#endif

/////////////////////////////////  PLL  //////////////////////////////////////////////

#ifndef	__ASSEMBLY__
struct bpsr {
    volatile unsigned int	reserved:8;
    volatile unsigned int	pdiv:8;
    volatile unsigned int	mdiv:8;
    volatile unsigned int	sdiv:8;
};

union pllsetreg {
    struct bpsr	bits __attribute__((packed));
    unsigned int		word;
};

struct lf1000_clock {
    volatile unsigned int	clkmodereg;
    volatile unsigned int	pllsetreg0;
    volatile unsigned int	pllsetreg1;
//    volatile union pllsetreg	pllsetreg1;
#if	defined	CPU_MF2530F
    volatile unsigned int	pllsetreg2;
#elif	defined	CPU_LF1000
    volatile unsigned int	reserved0;
#endif
    volatile unsigned int	reserved1[0xc];
    volatile unsigned int	gpiowakeupenb;
    volatile unsigned int	rtcwakeupenb;
    volatile unsigned int	gpiowakeupriseenb;
    volatile unsigned int	gpiowakeupfallenb;
    volatile unsigned int	gpiopend;
    volatile unsigned int	reserved2;
    volatile unsigned int	intpendspad;
    volatile unsigned int	pwrrststatus;
    volatile unsigned int	intenb;
    volatile unsigned int	reserved3[0x6];
    volatile unsigned int	pwrmode;
};

#endif

//------------- SET ONLY THIS DEFINITIONS -------------//

#define	CRYSTAL_FREQ_HZ	27000000	/* 27 MHz hardware installed in Hz */

/*
 * Choose CPU speed to match Audio requirements.  Audio base rate is 32KHz
 * times the i2s oversampling rate of 256.  So CPU rate is a multiple of
 * of 32,000 Hz * 256 i2s rate, or 8,192,000 Hz.  Development rates have
 * been 8.192 Mhz * 47 = 385.024 Mhz and 8.192 Mhz * 48 = 393.216 Mhz
 */

#ifdef CPU_LF1000
#define PLL0_OUT_HZ	393216000	/* desired PLL freq in Hz*/
#else
#define	PLL0_OUT_HZ	385024000	/* desired PLL frequency in Hz */
					// Audio uses PLL0 and requires clock
					// to be a multiple of 8.192 Mhz
#endif

#ifdef CPU_LF1000
#define PLL1_OUT_HZ	147000000	/* desired PLL frequency in Hz	*/
#else
#define	PLL1_OUT_HZ	133000000	/* desired PLL frequency in Hz */
#endif

#ifdef CPU_MF2530F
#define	PLL2_OUT_HZ 	84000000	/* desired PLL frequency in Hz */
#endif

//------------- SET ONLY THIS DEFINITIONS -------------//

#define LF1000_CLOCK_BASE	(LF1000_SYS_IO+0xf000)	/* clock controller */


#define	PLL0		0
#define	PLL1		1
#if	defined	CPU_MF2530F
#define	PLL2		2
#endif

#if defined CPU_MF2530F
#define	MAX_PDIV	64
#define	MIN_PDIV	3
#define	PDIV_OFF	(MIN_PDIV-1)
#define	MAX_MDIV	256
#define	MIN_MDIV	9
#define	MDIV_OFF	(MIN_MDIV-1)
#define	MAX_SDIV	3
#define	MIN_SDIV	0
#elif	defined	CPU_LF1000
#define	MAX_PDIV	63
#define	MIN_PDIV	1
#define	MAX_MDIV	1023
#define	MIN_MDIV	56
#define	MAX_SDIV	5
#define	MIN_SDIV	0
#endif

#if	defined	CPU_MF2530F
  #define	PLL_MUL( n)	((n!=PLL2)?2:1)
#elif	defined	CPU_LF1000
  #define	PLL_MUL( n)	( 1)
#endif

#ifdef CPU_MF2530F
#define	MAX_VCOF_HZ(n)	((n!=PLL2)?400000000:300000000)
#elif defined CPU_LF1000
#define  MAX_VCOF_HZ(n)((n==PLL0)?600000000:  \
			 (n==PLL1)?600000000:0)
#endif

// choose PLL post-scaler based on output frequency
#define	SDIV_HZ( n, f)	(((f) > (MAX_VCOF_HZ(n)/ 2))?0:	\
			 ((f) > (MAX_VCOF_HZ(n)/ 4))?1:	\
			 ((f) > (MAX_VCOF_HZ(n)/ 8))?2:	\
			 ((f) > (MAX_VCOF_HZ(n)/16))?3:0)

#define clock_p		((struct lf1000_clock*)(IO_ADDRESS( LF1000_CLOCK_BASE)))

/////////////////////// CPU, SDRAM, BCLK & PCLK //////////////////////////////////////
#define	CPU_PLL		PLL0
#define CPU_DIV		1
#define HCLK_DIV	3
#define	SDRAM_PLL	PLL1
#ifdef CPU_LF1000
#define BCLK_PLL	PLL0
#define BCLK_DIV	3
#else
#define	BCLK_PLL	PLL1
#endif
#define	PCLK_PLL	BCLK_PLL

/*
 * Memory Controller Settings
 */
#define	LF1000_SYS_IO_VIRT		IO_ADDRESS(LF1000_SYS_IO)
#ifdef CPU_MF2530F
#define	LF1000_NAND			7	/* NAND Controller */
#define	LF1000_ETH			1	/* Ethernet */
#elif defined CPU_LF1000
#define LF1000_NAND			11
#define	LF1000_ETH			2
#endif

/* TODO: replace with mem_controller.h */
#define	LF1000_MEMBW_OFF		0x00015800
#define	LF1000_MEMBW_SR1BW	(1<<LF1000_ETH) /*XXX*/
#define	LF1000_MEMTIMEACS_OFF		0x00015804
#define	LF1000_MEMTIMECOS_OFF		0x00015808
#define	LF1000_MEMTIMEACCL_OFF		0x0001580c
#define	LF1000_MEMTIMEACCH_OFF		0x00015810
#define	LF1000_MEMTIMESACCL_OFF		0x00015814
#define	LF1000_MEMTIMECOH_OFF		0x00015824
#define	LF1000_MEMTIMECAH_OFF		0x00015828
#define	LF1000_MEMTIMEBURSTL_OFF	0x0001582c
#define	LF1000_MEMWAIT_OFF		0x00015834

#define	mrs_read(off)		(*((ulong*)(LF1000_SYS_IO_VIRT+(off))))
#define	mrs_write(off, val)	(*((ulong*)(LF1000_SYS_IO_VIRT+(off))) = (val))

#define	MASK_MRS(size)			((1<<size)-1)

// set Memory Register Static
#define	SET_MRS( num, size, off, val)				\
	    mrs_write( off, test = (mrs_read( off) &		\
		(     ~(MASK_MRS( size)<<(size*num)))) |	\
		((val & MASK_MRS( size))<<(size*num)));

/*****************************************************************************
 * LF1000 Devices Memory Map and IRQ Assignments                             *
 *****************************************************************************/

/* DMA Controller */
#define LF1000_DMA_BASE		0xC0000000
#define LF1000_DMA_IRQ		3

/* Interrupt controller */
#define LF1000_IC_BASE		0xC0000800	

/* Timers */
#define LF1000_TIMER0_BASE	0xC0001800
#define LF1000_TIMER0_IRQ	4
#define LF1000_TIMER1_BASE	0xC0001880
#define LF1000_TIMER1_IRQ	11
#define LF1000_TIMER2_BASE	0xC0001900
#define LF1000_TIMER2_IRQ	15
#define LF1000_TIMER3_BASE	0xC0001980
#define LF1000_TIMER3_IRQ	21
#ifdef CPU_LF1000
#define LF1000_TIMER4_BASE	0xC0001A00
#define LF1000_TIMER4_IRQ	43
#endif

/* Display Controller (DPC) */
#define LF1000_DPC_BASE		0xC0003000
#define LF1000_DPC_END		0xC00031C8

/* Multi-Layer Controller (MLC) */
#define LF1000_MLC_BASE		0xC0004000
#define LF1000_MLC_END		0xC00041C0
/*frame buffers' base address*/
#ifdef CONFIG_MACH_LF_LF1000 /* 32MB of RAM, place FB at 18MB */
#define LF1000_MLC_FB_BASE	0x01200000
#else /* 64MB of RAM, place FB at 50MB */
#define LF1000_MLC_FB_BASE	0x03200000
#endif

/* Analog to Digital Converter (ADC) */
#define LF1000_ADC_BASE		0xC0005000
#define LF1000_ADC_END		0xC0005040
#define LF1000_ADC_IRQ		25

/* Serial Peripheral Interface (SPI) */
#define LF1000_SPI0_BASE	0xC0007800
#define LF1000_SPI0_END		0xC0007844
#define LF1000_SPI0_IRQ		12
#define LF1000_SPI1_BASE	0xC0008000
#define LF1000_SPI1_END		0xC0008044
#define LF1000_SPI1_IRQ		39
#define LF1000_SPI2_BASE	0xC0008800
#define LF1000_SPI2_END		0xC0008844
#define LF1000_SPI2_IRQ		40

/* SD/MMC Controller */
#define LF1000_SDIO0_BASE	0xC0009800
#define LF1000_SDIO0_END	0xC0009FC4
#define LF1000_SDIO0_IRQ	14
#ifdef CPU_LF1000
#define LF1000_SDIO1_BASE	0xC000C800
#define LF1000_SDIO1_END	0xC000CFC4
#define LF1000_SDIO1_IRQ	42
#endif

/* GPIO */
#define LF1000_GPIO_BASE	0xC000A000
#define LF1000_GPIO_END		0xC000A0E4
#define LF1000_GPIO_IRQ		13

/* Pulse Width Modulator (PWM) */
#define LF1000_PWM_BASE		0xC000C000
#define LF1000_PWM_END		0xC000C044

/* AC97 and I2S (audio) */
#define LF1000_AUDIO_BASE	0xC000D800
#define LF1000_AUDIO_END	0xC000DBC8
#define LF1000_AUDIO_IRQ	24

/* I2C */
#define LF1000_I2C0_BASE	0xC000E000
#define LF1000_I2C0_END		0xC000E028
#define LF1000_I2C0_IRQ		32
#define LF1000_I2C1_BASE	0xC000E800
#define LF1000_I2C1_END		0xC000E828
#define LF1000_I2C1_IRQ		33

/* CLOCK and POWER */
#define LF1000_CLKPWR_BASE	0xC000F000
#define LF1000_CLKPWR_END	0xC000F07F

/* RTC controller */
#define LF1000_RTC_BASE		0xC000F080
#define LF1000_RTC_END		0xC000F097
#define LF1000_RTC_IRQ		31

#ifdef CPU_MF2530F
/* Alive GPIO on MF2530F */
#define LF1000_ALIVE_BASE	0xC000F0C0
#define LF1000_ALIVE_END	0xC000F0E4
#endif

/* IDCT Macro Block Decoder */
#define LF1000_IDCT_BASE	0xC000F800
#define LF1000_IDCT_END		0xC000F82C

#ifdef CPU_LF1000
/* GPIO drive current control registers */
#define LF1000_GPIOCURRENT_BASE	0xC000F100
#define LF1000_GPIOCURRENT_END	0xC000F117
#else
/* Setting drive current is unsupported on ME2530 */
#define LF1000_GPIOCURRENT_BASE	0
#define LF1000_GPIOCURRENT_END	0
#endif

/* MCU-A Memory Controller */
#define LF1000_MCU_A_BASE	0xC0014800
#define LF1000_MCU_A_END	0xC0014810
#define LF1000_MEMCONTROL	0x0A

/* MCU-S Memory Controller */
#define LF1000_MCU_S_BASE	0xC0015800
#define LF1000_MCU_S_END	0xC001587C

/* UART */
#define LF1000_UART0_BASE	0xC0016000
#define LF1000_UART0_END	0xC0016044
#define LF1000_UART0_IRQ	10
#define LF1000_UART1_BASE	0xC0016080
#define LF1000_UART1_END	0xC00160C4
#define LF1000_UART1_IRQ	34
#define LF1000_UART2_BASE	0xC0016800
#define LF1000_UART2_END	0xC0016844
#define LF1000_UART2_IRQ	35
#define LF1000_UART3_BASE	0xC0016880
#define LF1000_UART3_END	0xC00168C4
#define LF1000_UART3_IRQ	36
#ifdef	CPU_MF2530F
#define LF1000_UART4_BASE	0xC0017000
#define LF1000_UART4_END	0xC0017044
#define LF1000_UART4_IRQ	37
#define LF1000_UART5_BASE	0xC0017080
#define LF1000_UART5_END	0xC00170C4
#define LF1000_UART5_IRQ	38
#endif

/* UDC (USB) */
#define LF1000_UDC_BASE		0xC0018000
#define LF1000_UDC_END		0xC0018880
#define LF1000_UDC_IRQ		20

#ifdef CPU_LF1000
/* Alive GPIO on LF1000 */
#define LF1000_ALIVE_BASE	0xC0019000
#define LF1000_ALIVE_END	0xC0019018
#endif

/* 3D Engine Control (GA3D) */
#define LF1000_GA3D_BASE	0xC001A000
#define LF1000_GA3D_END		0xC001BFFF

/* Processor ID (ECID) */
#define LF1000_ECID_BASE	0xC001F800

#define LF1000_3DGE_BASE			0xe0000000

/* Extint 0-1 */
#define	LF1000_EXTINT0		8
#define	LF1000_EXTINT1		9

///////////////////////////////// TIMER //////////////////////////////////////////////
/* FIXME: this needs to go away, see timer.h */

#ifndef	__ASSEMBLY__
struct lf1000_timer {
    volatile unsigned int	tmrcount;
    volatile unsigned int	tmrmatch;
    volatile unsigned int	tmrcontrol;
    volatile unsigned int	tmrreserved[0x0d];
    volatile unsigned int	tmrclkenb;
    volatile unsigned int	tmrclkgen;
};

struct lf1000_timer_init {
    struct lf1000_timer*	timer_pnt;
    int			 	irq_number;
};

#endif

//------------- SET ONLY THIS DEFINITIONS -------------//

#define	TIMER_PLL		PLL1		/* timer clock source is PLL1 */

#define	LF1000_SYS_TIMER	0		/* system   timer is TIMER0   */
#define LF1000_SYS_TIMER_IRQ	LF1000_TIMER0_IRQ
#define	LF1000_FREE_TIMER	1		/* free run timer is TIMER1   */

#define LF1000_INTERTICK_TIMER	2		/* Time since SYS_TIMER tick  */
						/* reset in SYS_TIMER isr     */

#define	LF1000_INTERVAL_IN_USEC	(USEC_PER_SEC/HZ) /* system timer cycle */

#define	TIMER_FREE_RUN		0xffffffff
#define	CLKDIV			32		/* timer predivider */
#define	SELTCLK			0x03		/* TCLK/1 */

#define	SELTCLK_MASK		0x03 /*XXX: duplicate? */
#define	CLKDIVR			((CLKDIV<1)?0:(CLKDIV>256)?255:(CLKDIV-1))

#define	TIMER_DIV		((1<<((SELTCLK+1)&SELTCLK_MASK))*CLKDIV)
#define TICKS_PER_mSEC		(get_pll_freq(TIMER_PLL)/TIMER_DIV/1000)
#define TICKS_PER_uSEC          (get_pll_freq(TIMER_PLL)/TIMER_DIV/1000000)
#define	TIMER_SYS_TICK	\
	(TICKS_PER_mSEC * (LF1000_INTERVAL_IN_USEC / 1000 /* for mSec */))

#define TIMER_INTERVAL		TIMER_SYS_TICK
#define TIMER_RELOAD		(TIMER_INTERVAL)

#define	TCLK_MAX_HZ		50000000	/* maximum allowed TCLK freq */

#ifdef CPU_MF2530F
#define	TIMER_PLL_OUT_HZ	((TIMER_PLL == PLL0)? PLL0_OUT_HZ:	\
				 (TIMER_PLL == PLL1)? PLL1_OUT_HZ:	\
				 (TIMER_PLL == PLL2)? PLL2_OUT_HZ:	\
							(TCLK_MAX_HZ+1))
#elif defined(CPU_LF1000)
#define	TIMER_PLL_OUT_HZ	((TIMER_PLL == PLL0)? PLL0_OUT_HZ:	\
				 (TIMER_PLL == PLL1)? PLL1_OUT_HZ:	\
							(TCLK_MAX_HZ+1))
#else
#error "No known CPU type defined, can't set TIMER_PLL_OUT"
#endif

#if ((TIMER_PLL_OUT_HZ/TIMER_DIV) > TCLK_MAX_HZ)
    #error	Maximum allowed Timer Frequency is 50 MHz !!!
#endif

#undef TIMER_PLL_OUT_HZ		// used just for checking max freq

/////////////////////////////// FUNCTIONS /////////////////////////////////////

#ifndef	__ASSEMBLY__
// PLL
extern void			lf1000_clock_init( void);
extern int			lf1000_CalcDivider( unsigned int pll_khz,
						   unsigned int desired_hz);
extern unsigned int		get_pll_div( int pll,int fin_hz,int fout_hz);
extern int			get_pll_freq( unsigned int pll);
// CPU
extern unsigned int		get_cpu_freq( void);
extern void			calc_freq_ratio( unsigned long * loops);
// Timer
extern struct lf1000_timer*	get_sys_timer_pnt( void);
extern struct lf1000_timer*	get_timer_pnt( int timer);

extern void			clear_timer_irq( int irq);
extern int			get_timer_irq( int timer);
extern unsigned long		get_timer_cnt( int timer);
extern unsigned long		get_timer_match( int timer);
//extern unsigned long		get_timer_div( int timer);
extern int			get_timer_freq( int timer);

// UART
extern void			set_uart_baud( int uart, int baud);
#endif

#define MAX_TIMER                       2
#define MAX_PERIOD                      699050

#endif

