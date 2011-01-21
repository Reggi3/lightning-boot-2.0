#ifndef LIGHTNING_BOOT_CONFIG_H
#define LIGHTNING_BOOT_CONFIG_H

#include <platform.h>
#include <mlc.h>
/*
 * minimum voltage for booting (with backlight on), in mV
 */
#define BOOT_MIN_MV	4400
#define LOW_BATT_PORT	GPIO_PORT_C
#define LOW_BATT_PIN	GPIO_PIN14

/* 
 * SDRAM settings
 */
#define SDRAM_START	0x00000000
#ifdef CONFIG_MACH_LF_LF1000
#define SDRAM_SIZE	0x01000000 /* 16MB */
#else
#define SDRAM_SIZE	0x02000000 /* 32MB */
#endif

/* 
 * File System Locations (from autoconf.h)
 * - BOOT_FLAGS contains the control file and other bootup data
 * - BOOT0 and BOOT1 contain the kernel image
 */
#define BOOT_FLAGS_ADDR	(CONFIG_NAND_LF1000_P0_SIZE)
#define BOOT_FLAGS_SIZE	(CONFIG_NAND_LF1000_P1_SIZE)
#define BOOT0_ADDR	(CONFIG_NAND_LF1000_P0_SIZE + \
			 CONFIG_NAND_LF1000_P1_SIZE + \
			 CONFIG_NAND_LF1000_P2_SIZE)
#define BOOT1_ADDR	(BOOT0_ADDR + \
			 CONFIG_NAND_LF1000_P3_SIZE + \
			 CONFIG_NAND_LF1000_P4_SIZE)
#define BOOT_SIZE	(CONFIG_NAND_LF1000_P3_SIZE)

/* 
 * Kernel Command Line settings 
 *
 */

/* Base command line */
#if defined CONFIG_MACH_LF_MP2530F 
#define CMDLINE_BASE "mem=32M init=/sbin/init console=ttyS3,115200 "

#elif defined CONFIG_MACH_ME_LF1000
#define CMDLINE_BASE "mem=50M init=/sbin/init console=ttyS0,115200 "

#elif defined CONFIG_MACH_LF_LF1000
#define CMDLINE_BASE "mem=18M init=/sbin/init console=ttyS0,115200 "

#else
#error "MACHINE TYPE NOT SUPPORTED"

#endif

/* command line rootfs components */
#define CMDLINE_RFS0 "root=31:04 ro rootflags=noatime rootfstype=jffs2 "
#define CMDLINE_RFS1 "root=31:06 ro rootflags=noatime rootfstype=jffs2 "
#define CMDLINE_RFS2 "root=179:02 rw rootfstype=ext3 rootdelay=1 "
#define CMDLINE_RFS3 "root=179:02 rw rootfstype=ext3 rootdelay=1 debug=1"
#ifdef NFS_SUPPORT
#define CMDLINE_NFS  "root=/dev/nfs rw rootflags=noatime rootfstype=ext2 nfsroot=192.168.0.113:/home/lfu/nfsroot ether=10,0xf8800000,eth0 cs89x0_media=rj45 ip=192.168.0.111:192.168.0.113:192.168.0.115:255.255.255.0:lf1000:eth0:off "
#endif

/* ubi components */
#define CMDLINE_UBI "ubi.mtd=Brio ubi.mtd=prg_Brio ubi.mtd=Cartridge ubi.mtd=EXT ubi.mtd=prg_EXT "
/* extra arguments for MFG cartridge */
#define CMDLINE_MFG "mtdparts=lf1000-cart:" \
		"0x20000(prg_LF1000_uniboot),"\
		"0xE0000(prg_Atomic_Boot_Flags),"\
		"0x100000(prg_Manufacturing_Data),"\
		"0x200000(prg_Kernel0),"\
		"0xE00000(prg_Linux_RFS0),"\
		"0x200000(prg_Kernel1),"\
		"0xE00000(prg_Linux_RFS1),"\
		"0xDE00000(prg_Brio),"\
		"-(prg_EXT) "

/*
 * Display and boot splash settings
 */
#define X_RESOLUTION		320
#define Y_RESOLUTION		240
#define FRAME_BUFFER_ADDR	RGB_BASE
#define FRAME_BUFFER_SIZE	(X_RESOLUTION*Y_RESOLUTION*3)
#define BOOTSPLASH_NAME		"bootsplash.rgb"

/*
 * Scratchpad Register usage
 */
#define SCRATCH_POWER_POS	0
#define SCRATCH_BOARD_ID_POS	5
#define SCRATCH_CART_ID_POS	10

/*
 * u-boot settings
 */
#ifdef UBOOT_SUPPORT
/* PORTC button masks */
#if defined CONFIG_MACH_ME_LF1000 || defined CONFIG_MACH_LF_LF1000
#define BUTTON_MSK      0x00003F9F

#define BUTTON_A_BIT	8
#define BUTTON_B_BIT	9
#define BUTTON_PWR_BIT	20
#define BUTTON_UP_BIT	2
#define BUTTON_DN_BIT	3
#define BUTTON_LS_BIT	4

#define BUTTON_A_MSK   (1 << BUTTON_A_BIT)
#define BUTTON_B_MSK   (1 << BUTTON_B_BIT)
#define BUTTON_PWR_MSK (1 << BUTTON_PWR_BIT)
#define BUTTON_UP_MSK  (1 << BUTTON_UP_BIT)
#define BUTTON_DN_MSK  (1 << BUTTON_DN_BIT)
#define BUTTON_LS_MSK  (1 << BUTTON_LS_BIT)

#else /* LF_ME2530 and ME_ME2530 boards */
#define BUTTON_MSK      0x1FE00000
#endif
// UBOOT_ADDR is a bit of a misnomer, as it is the load address for uboot, kernel, cmdline 
// and UBOOT_ADDR2 is for lightning-boot.bin, as it needs to not be sitting in UBOOT_ADDR 
// if its asked to load the kernel
#define UBOOT_ADDR	0x01000000
#endif /* UBOOT_SUPPORT */
#define UBOOT_ADDR2	0x00800000
#endif /* LIGHTNING_BOOT_CONFIG_H */
