/* jffs2.c -- basic read-only JFFS2 support.
 *
 * Copyright 2007 LeapFrog Enterprises Inc.
 *
 * Andrey Yurovsky <andrey@cozybit.com>
 *
 * This code is heavily based on jffs2dump.c, copyright Thomas Gleixner and 
 * distributed as part of mtd-utils (http://www.linux-mtd.infradead.org/). It
 * also borrows from jffs2reader.c, copyright Jari Kirma and also distributed
 * as part of mtd-utils.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <common.h>
#include "include/jffs2.h"
#include "include/crc32.h"
#include "include/string.h"

#define PAD(x) (((x)+3)&~3)

/* jffs2_resolve_inode -- find an inode */
static struct jffs2_raw_inode *jffs2_resolve_inode(char *data, unsigned int imglen,
		u32 ino)
{
	struct jffs2_raw_inode *inode = NULL;
	union jffs2_node_union *node = (union jffs2_node_union *)data;
	u32 crc;
	char *p;

	do {
		/* find a JFFS2 node */
		while((char *)node < data+imglen && 
			(node->u.magic) != JFFS2_MAGIC_BITMASK) {
			p = (char *)node;
			p += 4;
			node = (union jffs2_node_union *)p;
		}

		/* still at a valid node? */
		if((char *)node < data+imglen) {

			/* node is a type INODE
			 * with matching inode
			 * with better version, if we have one
			 * with valid CRC */
			if((node->u.nodetype) == JFFS2_NODETYPE_INODE &&
				(node->d.pino) == ino &&
				(!inode || inode->version <= node->i.version)){
				/* now check CRC, if valid and this is a better
				 * version, then we have a potential match */
				crc = crc32(0, node, sizeof(struct jffs2_unknown_node)-4);
				if(crc == (node->u.hdr_crc))
					inode = &(node->i);
					/*FIXME*/
					if(inode->dsize < inode->isize)	
						return inode;
			}
		
			/* skip past this one, check the next possible node */
			p = (char *)node;
			p += PAD((node->s.totlen));
			node = (union jffs2_node_union *)p;
		} else { /* we're done, return the best match */
			return inode;
		}
	} while(1);
}

/* jffs2_resolve_dirent -- find a directory entry given a path name */
static struct jffs2_raw_dirent *jffs2_resolve_dirent(char *data, 
		unsigned int imglen, char *path, unsigned int plen, u32 ino)
{
	struct jffs2_raw_dirent *dd = NULL;
	union jffs2_node_union *node = (union jffs2_node_union *)data;
	u32 crc, v, vmax = 0;
	char *p;

	do {
		/* find a JFFS2 node */
		while((char *)node < data+imglen && 
			(node->u.magic) != JFFS2_MAGIC_BITMASK) {
			p = (char *)node;
			p += 4;
			node = (union jffs2_node_union *)p;
		}

		/* still at a valid node? */
		if((char *)node < data+imglen) {

			/* node is a DIRENT
			 * with matching name length
			 * and matching name
			 * with >= version 
			 * with valid CRC */
			if((node->u.nodetype) == JFFS2_NODETYPE_DIRENT &&
				(ino == 0 || (node->d.pino) == ino) &&
				node->d.nsize == plen &&
				!strncmp((char *)&node->d.name, path, node->d.nsize) &&
				(v = (node->d.version)) >= vmax) {
				/* now check CRC, if valid and this is a better
				 * version, then we have a potential match */
				crc = crc32(0, node, sizeof(struct jffs2_unknown_node) - 4);
				if(crc == (node->u.hdr_crc) && vmax <= v) {
					vmax = v;
					dd = &(node->d);
				}
			}
		
			/* skip past this one to check the next possible node */
			p = (char *)node;
			p += PAD((node->s.totlen));
			node = (union jffs2_node_union *)p;
		} else { /* we're done, return the best match */
			return dd;
		}
	} while(1);
}

void jffs2_retrieve_data(char *data, unsigned int imglen, 
		struct jffs2_raw_inode *start, char *output)
{
	union jffs2_node_union *node = (union jffs2_node_union *)start;
	char *p = (char *)start;
	u32 crc;
	u32 version = start->version;
	u32 ino = start->ino;
	int size = start->isize;

	do {
		/* find a JFFS2 node */
		while((char *)node < data+imglen && 
			(node->u.magic) != JFFS2_MAGIC_BITMASK) {
			p = (char *)node;
			p += 4;
			node = (union jffs2_node_union *)p;
		}

		/* still at a valid node? */
		if((char *)node < data+imglen) {

			/* node is an INODE
			 * with matching ino
			 * with version we're looking for
			 * with valid CRC */
			if(node->u.nodetype == JFFS2_NODETYPE_INODE &&
				node->i.ino == ino &&
				node->i.version == version) {
				/* now check CRC, if valid then
				 * we have a potential match */
				crc = crc32(0, node, sizeof(struct jffs2_unknown_node) - 4);
				if(crc == node->u.hdr_crc) {
					/* copy the contents */
					p = (char *)node;
					memcpy(output,
						p+sizeof(struct jffs2_raw_inode) - 4, 
						node->i.csize);
					/* adjust pointers and indexes */
					output += node->i.csize;
					size -= node->i.csize;
					version++;
				}
			}
		
			/* skip past this one to check the next possible node */
			p = (char *)node;
			p += PAD(node->s.totlen);
			node = (union jffs2_node_union *)p;
		} else { /* we're done, return the best match */
			return;
		}
	} while(size > 0);
}

struct jffs2_raw_inode *jffs2_cat(char *data, unsigned int imglen,
		const char *path)
{
	u32 ino = 0;
	char *p = (char *)path;
	char *e;
	struct jffs2_raw_dirent *dir = NULL;

	while(*p != '\0') {
		if(*p == '/') {
			p++;
			continue;
		}
		for(e = p+1; *e != '\0' && *e != '/'; e++);
	
		dir = jffs2_resolve_dirent(data, imglen, p, (unsigned int)(e-p), ino);
		if(dir)
			ino = (dir->ino);
		else
			return 0;

		p = e;
	}

	return jffs2_resolve_inode(data, imglen, ino);
}
