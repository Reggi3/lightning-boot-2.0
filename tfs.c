/* tfs.c -- access to Table File System, a simple packing format for binaries.
 *
 * Copyright 2007 LeapFrog Enterprises Inc.
 *
 * Andrey Yurovsky <andrey@cozybit.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <common.h>
#include "include/nand.h"
#include "include/debug.h"
#include "include/string.h"

struct tfs_hdr {
	u32 magic;
	u32 num_files;
} __attribute__((__packed__));

struct tfs_sum {
	char file_name[64];
	u32 num_blocks;
} __attribute__((__packed__));

static struct {
	struct tfs_hdr *header;
	struct tfs_sum *summary;
	u32 data_block;
} tfs_info;

/*
 * Load TFS summary.  The summary is stored in the first non-bad block at the
 * given offset.  The size of the summary itself is limited such that it fits 
 * on one block.  To load, we find the first non-bad block and then read that
 * block.  The data we read must be the TFS summary, or we fail. */
int tfs_load_summary(u32 *buf, u32 offset)
{
	/* find the first non-bad block */
	while(nand_check_block(offset) != 0) {
		db_puts("tfs: load summary, skipped bad block\n");
		offset += NAND_EB_SIZE;
	};
	
	/* fetch one block of data */	
	nand_read(buf, offset, NAND_EB_SIZE);

	/* inspect the TFS summary */
	tfs_info.header = (struct tfs_hdr*)buf;
	tfs_info.summary = (struct tfs_sum *)((char *)buf + 
			sizeof(struct tfs_hdr));
	tfs_info.data_block = offset+NAND_EB_SIZE;

	if(tfs_info.header->magic == 0x12345678) {
		db_puts("tfs: loaded summary ");
		db_int(tfs_info.header->num_files); db_puts(" files\n");
		return 0;
	}
	return 1;
}

int tfs_load_file(char *name, u32 *buf)
{
	u32 offset;
	char *p;
	int i;
	struct tfs_sum *s = tfs_info.summary;
	int skip = 0;

	/* find file in the summary */

	for(i = 0; i < tfs_info.header->num_files; i++) {
		if(!strncmp(s->file_name, name, 64))
			break;

		skip += s->num_blocks;
		p = (char *)s + sizeof(struct tfs_sum);
		s = (struct tfs_sum *)p;
	}

	if(i >= tfs_info.header->num_files) {
		db_puts("tfs: failed to find file\n");
		return 1;
	}

	/* skip to file data */
	for(offset = tfs_info.data_block; skip > 0; skip--) {
		do {
			offset += NAND_EB_SIZE;
		} while(nand_check_block(offset) != 0); /* (skip bad blocks) */
	}

	/* read the file out */

	return nand_read(buf, offset, (s->num_blocks+1)*NAND_EB_SIZE);
}
