#ifndef LIGHTNING_BOOT_TFS_H
#define LIGHTNING_BOOT_TFS_H

int tfs_load_summary(u32 *buf, u32 offset);
int tfs_load_file(char *name, u32 *buf);

#endif
