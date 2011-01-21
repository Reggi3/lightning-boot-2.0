/* LF1000 NAND Flash Controller Driver
 *
 * asm/arch/nand.h -- hardware abstraction */

#ifndef LF1000_NAND_H
#define LF1000_HAND_H

#include "platform.h"
#include "mem_controller.h"
#include "gpio.h"

/* 
 * location of NAND Controller base for NFDATA, NFCMD, and NFADDR registeres 
 */
#if defined CPU_MF2530F
#define LF1000_NAND_BASE	LF1000_STATIC_BASE+LF1000_STATIC7_BASE
#elif defined CPU_LF1000
#define LF1000_NAND_BASE	LF1000_STATIC_BASE+LF1000_STATIC11_BASE
#endif
#define LF1000_NAND_END		(LF1000_NAND_BASE+0x18)

/* NAND Controller Registers
 *
 * (offsets from LF1000_MCU_S_BASE) */
#define NFCONTROL		0x74
#ifdef CPU_MF2530F
#define NFECC			0x78	/* NOTE: Hardware ECC is only        */
#define NFDATACNT		0x7C	/*       for small-block Flash chips */
#elif defined CPU_LF1000		/*       on both the MF2530F and the */
#define NFECCL			0x78	/*       LF1000.                     */
#define NFECCH			0x7C
#define NFORGECCL		0x80
#define NFORGECCH		0x84
#define NFCNT			0x88
#define NFECCSTATUS		0x8C
#define NFSYNDRONE31		0x90
#define NFSYNDRONE75		0x94
#endif

/* (offsets from LF1000_NAND_BASE) */
#define NFDATA			0x00
#define NFCMD			0x10
#define NFADDR			0x18

/* NAND Flash Control Register (NFCONTROL) */
#define IRQPEND			15
#ifdef CPU_LF1000
#define ECCRST			11
#endif
#define RnB			9
#define IRQENB			8
#define NFBOOTENB		5
#define NFTYPE			3
#define NFBANK			0

/* NFTYPE bits in NFCONTROL */
#define NFTYPE_LBLOCK		4	/* large block Flash */
#define NFTYPE_EADDR		3	/* extra address Flash */

#ifdef CPU_LF1000
/* NAND Flash Data Count Register (NFCNT) */
#define NFWRCNT			16
#define NFRDCNT			0
#endif

#ifdef CPU_LF1000
/* NAND FLASH ECC STATUS REGISTER (NFECCSTATUS) */
#define NFCHECKERROR		2
#define NFECCDECDONE		1	/* (reading done) */
#define NFECCENCDONE		0	/* (writing done) */

/* NAND FLASH ECC SYNDROME VALUE31 REGISTER (NFSYNDRONE31) */
#define SYNDROM3		13
#define SYNDROM1		0

/* NAND FLASH ECC SYNCDROME VALUE75 REGISTER (NFSYNDRONE75) */
#define SYNCDROM7		13
#define SYNCDROM5		0
#endif /* CPU_LF1000 */

/* GPIO settings for Write Protect control */
#define NAND_WP_PORT		GPIO_PORT_ALV
#define NAND_WP_BASE		LF1000_ALIVE_BASE
#define NAND_WP_OUT		GPIOALVOUT
#define NAND_WP_OUT_ENB		GPIOALVOUTENB
#ifdef CONFIG_MACH_ME_MP2530F
#define NAND_WP_PIN		5
#elif defined CONFIG_MACH_ME_LF1000 || defined CONFIG_MACH_LF_LF1000
#define NAND_WP_PIN		1
#else
#define NAND_WP_PIN		6	
#endif
#define NAND_WP_LEVEL		0	/* level for "on" */

/* GPIO settings for cartridge detection */
#define NAND_CART_DETECT_PORT	GPIO_PORT_A
#define NAND_CART_DETECT_PIN	18
#define NAND_CART_DETECT_LEVEL	0	/* level for "detected" */

/*
 * Cartridge Type Detection
 */
#define NAND_CART_TYPE_PORT	GPIO_PORT_B
#define NAND_CART_TYPE_LOW	2
#define NAND_CART_TYPE_HIGH	5
#define NAND_CART_DEVELOPMENT	0xF	/* 1111 : Development = NAND, writable, ECC on */
#define NAND_CART_MANUFACTURING	0xB	/* 1011 */
#define NAND_CART_PRODUCTION	0x7	/* 0111 : Production = OTP, ECC off */
/*
 * The following are defined for non-MTD applications such as uniboot 
 */

/* NAND Flash Commands */
#define CMD_READ                0x00
#define CMD_READ_CONFIRM        0x30    /* (large block Flash chips only) */
#define CMD_READ_STATUS         0x70

/* NAND status codes */
#define NAND_STATUS_ERROR_SH    0
#define NAND_STATUS_READY_SH    6

/* NAND flags */
#define NAND_LARGE_BLOCK_SH     0
#define NAND_EXTRA_ADDRESS_SH   1

#endif
