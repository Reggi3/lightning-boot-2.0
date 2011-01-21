#ifndef LIGHTNING_BOOT_JFFS2_H
#define LIGHTNING_BOOT_JFFS2_H

/* jffs2_cat -- find file in JFFS2 buffer
 *
 * [in]  data	- JFFS2 data buffer
 * [in]  imglen	- size of data buffer
 * [in]  path	- file path to search for
 *
 * returns: Pointer to the file's starting inode, if the file was found. NULL
 *          otherwise. */
struct jffs2_raw_inode *jffs2_cat(char *data, unsigned int imglen, 
		const char *path);

/* jffs2_retrieve_data -- copy a file's contents to some destination.
 *
 * [in]	data	- JFFS2 data buffer
 * [in] imglen	- size of data buffer
 * [in]	start	- the file's starting inode
 * [in] output	- pointer to destination buffer */
void jffs2_retrieve_data(char *data, unsigned int imglen,
		struct jffs2_raw_inode *start, char *output);

#define JFFS2_MAGIC_BITMASK 0x1985
#define JFFS2_EMPTY_BITMASK 0xffff
#define JFFS2_DIRTY_BITMASK 0x0000

/* Summary node MAGIC marker */
#define JFFS2_SUM_MAGIC	0x02851885

/* We only allow a single char for length, and 0xFF is empty flash so
   we don't want it confused with a real length. Hence max 254.
*/
#define JFFS2_MAX_NAME_LEN 254

/* How small can we sensibly write nodes? */
#define JFFS2_MIN_DATA_LEN 128

#define JFFS2_COMPR_NONE	0x00
#define JFFS2_COMPR_ZERO	0x01
#define JFFS2_COMPR_RTIME	0x02
#define JFFS2_COMPR_RUBINMIPS	0x03
#define JFFS2_COMPR_COPY	0x04
#define JFFS2_COMPR_DYNRUBIN	0x05
#define JFFS2_COMPR_ZLIB	0x06
/* Compatibility flags. */
#define JFFS2_COMPAT_MASK 0xc000      /* What do to if an unknown nodetype is found */
#define JFFS2_NODE_ACCURATE 0x2000
/* INCOMPAT: Fail to mount the filesystem */
#define JFFS2_FEATURE_INCOMPAT 0xc000
/* ROCOMPAT: Mount read-only */
#define JFFS2_FEATURE_ROCOMPAT 0x8000
/* RWCOMPAT_COPY: Mount read/write, and copy the node when it's GC'd */
#define JFFS2_FEATURE_RWCOMPAT_COPY 0x4000
/* RWCOMPAT_DELETE: Mount read/write, and delete the node when it's GC'd */
#define JFFS2_FEATURE_RWCOMPAT_DELETE 0x0000

#define JFFS2_NODETYPE_DIRENT (JFFS2_FEATURE_INCOMPAT | JFFS2_NODE_ACCURATE | 1)
#define JFFS2_NODETYPE_INODE (JFFS2_FEATURE_INCOMPAT | JFFS2_NODE_ACCURATE | 2)
#define JFFS2_NODETYPE_CLEANMARKER (JFFS2_FEATURE_RWCOMPAT_DELETE | JFFS2_NODE_ACCURATE | 3)
#define JFFS2_NODETYPE_PADDING (JFFS2_FEATURE_RWCOMPAT_DELETE | JFFS2_NODE_ACCURATE | 4)

#define JFFS2_NODETYPE_SUMMARY (JFFS2_FEATURE_RWCOMPAT_DELETE | JFFS2_NODE_ACCURATE | 6)

#define JFFS2_NODETYPE_XATTR (JFFS2_FEATURE_INCOMPAT | JFFS2_NODE_ACCURATE | 8)
#define JFFS2_NODETYPE_XREF (JFFS2_FEATURE_INCOMPAT | JFFS2_NODE_ACCURATE | 9)

/* XATTR Related */
#define JFFS2_XPREFIX_USER		1	/* for "user." */
#define JFFS2_XPREFIX_SECURITY		2	/* for "security." */
#define JFFS2_XPREFIX_ACL_ACCESS	3	/* for "system.posix_acl_access" */
#define JFFS2_XPREFIX_ACL_DEFAULT	4	/* for "system.posix_acl_default" */
#define JFFS2_XPREFIX_TRUSTED		5	/* for "trusted.*" */

#define JFFS2_ACL_VERSION		0x0001

// Maybe later...
//#define JFFS2_NODETYPE_CHECKPOINT (JFFS2_FEATURE_RWCOMPAT_DELETE | JFFS2_NODE_ACCURATE | 3)
//#define JFFS2_NODETYPE_OPTIONS (JFFS2_FEATURE_RWCOMPAT_COPY | JFFS2_NODE_ACCURATE | 4)


#define JFFS2_INO_FLAG_PREREAD	  1	/* Do read_inode() for this one at
					   mount time, don't wait for it to
					   happen later */
#define JFFS2_INO_FLAG_USERCOMPR  2	/* User has requested a specific
					   compression type */


/* These can go once we've made sure we've caught all uses without
   byteswapping */

struct jffs2_unknown_node
{
	/* All start like this */
	u16 magic;
	u16 nodetype;
	u32 totlen; /* So we can skip over nodes we don't grok */
	u32 hdr_crc;
};

struct jffs2_raw_dirent
{
	u16 magic;
	u16 nodetype;	/* == JFFS2_NODETYPE_DIRENT */
	u32 totlen;
	u32 hdr_crc;
	u32 pino;
	u32 version;
	u32 ino; /* == zero for unlink */
	u32 mctime;
	u8 nsize;
	u8 type;
	u8 unused[2];
	u32 node_crc;
	u32 name_crc;
	u8 *name;
};

/* The JFFS2 raw inode structure: Used for storage on physical media.  */
/* The uid, gid, atime, mtime and ctime members could be longer, but
   are left like this for space efficiency. If and when people decide
   they really need them extended, it's simple enough to add support for
   a new type of raw node.
*/
struct jffs2_raw_inode
{
	u16 magic;      /* A constant magic number.  */
	u16 nodetype;   /* == JFFS2_NODETYPE_INODE */
	u32 totlen;     /* Total length of this node (inc data, etc.) */
	u32 hdr_crc;
	u32 ino;        /* Inode number.  */
	u32 version;    /* Version number.  */
	u32 mode;       /* The file's type or mode.  */
	u16 uid;        /* The file's owner.  */
	u16 gid;        /* The file's group.  */
	u32 isize;      /* Total resultant size of this inode (used for truncations)  */
	u32 atime;      /* Last access time.  */
	u32 mtime;      /* Last modification time.  */
	u32 ctime;      /* Change time.  */
	u32 offset;     /* Where to begin to write.  */
	u32 csize;      /* (Compressed) data size */
	u32 dsize;	     /* Size of the node's data. (after decompression) */
	u8 compr;       /* Compression algorithm used */
	u8 usercompr;   /* Compression algorithm requested by the user */
	u16 flags;	     /* See JFFS2_INO_FLAG_* */
	u32 data_crc;   /* CRC for the (compressed) data.  */
	u32 node_crc;   /* CRC for the raw inode (excluding data)  */
	u8 *data;
};

struct jffs2_raw_xattr {
	u16 magic;
	u16 nodetype;	/* = JFFS2_NODETYPE_XATTR */
	u32 totlen;
	u32 hdr_crc;
	u32 xid;		/* XATTR identifier number */
	u32 version;
	u8 xprefix;
	u8 name_len;
	u16 value_len;
	u32 data_crc;
	u32 node_crc;
	u8 *data;
} __attribute__((packed));

struct jffs2_raw_xref
{
	u16 magic;
	u16 nodetype;	/* = JFFS2_NODETYPE_XREF */
	u32 totlen;
	u32 hdr_crc;
	u32 ino;		/* inode number */
	u32 xid;		/* XATTR identifier number */
	u32 xseqno;	/* xref sequencial number */
	u32 node_crc;
} __attribute__((packed));

struct jffs2_raw_summary
{
	u16 magic;
	u16 nodetype; 	/* = JFFS2_NODETYPE_SUMMARY */
	u32 totlen;
	u32 hdr_crc;
	u32 sum_num;	/* number of sum entries*/
	u32 cln_mkr;	/* clean marker size, 0 = no cleanmarker */
	u32 padded;	/* sum of the size of padding nodes */
	u32 sum_crc;	/* summary information crc */
	u32 node_crc; 	/* node crc */
	u32 *sum; 	/* inode summary info */
};

union jffs2_node_union
{
	struct jffs2_raw_inode i;
	struct jffs2_raw_dirent d;
	struct jffs2_raw_xattr x;
	struct jffs2_raw_xref r;
	struct jffs2_raw_summary s;
	struct jffs2_unknown_node u;
};

/* Data payload for device nodes. */
union jffs2_device_node {
	u16 old;
	u32 new;
};

#endif
