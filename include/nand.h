#ifndef LIGHTNING_BOOT_NAND_H
#define LIGHTNING_BOOT_NAND_H

/*
 * settings:
 *	the NAND controller BCH handles 512 bytes at a time
 *	64-byte OOB, with ECC stored in the end
 */
#define NAND_EB_SIZE	0x20000	/* 128KiB */
#define NAND_PAGE_SIZE	0x800	/* 2KiB */
#define NAND_OOB_SIZE	64
#define NAND_BCH_SIZE	512
#define NAND_ECC_BYTES	6
#define NAND_ECC_OFFSET	40
#define NAND_ECC_STEPS	(NAND_PAGE_SIZE/NAND_BCH_SIZE)

/* NAND status codes */
#define NAND_STATUS_ERROR		NAND_STATUS_ERROR_SH
#define NAND_STATUS_READY		NAND_STATUS_READY_SH

int nand_check_block(u32 offset);
int nand_read(u32 *buf, u32 offset, u32 size);
#endif

