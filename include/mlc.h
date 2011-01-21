#ifndef LIGHTNING_BOOT_MLC_H
#define LIGHTNING_BOOT_MLC_H

#define B8G8R8 0xC653

void mlc_init(void);
void mlc_set_palette_entry(u8 p, u8 r, u8 g, u8 b);
void mlc_set_video_mode(void);

#endif /* LIGHTNING_BOOT_MLC_H */
