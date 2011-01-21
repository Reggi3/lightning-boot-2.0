#ifndef LIGHTNING_BOOT_DISPLAY_H
#define LIGHTNING_BOOT_DISPLAY_H

#ifdef DISPLAY_SUPPORT
void display_init(void);
void display_backlight(int board_id);
#else
#define display_init(...)
#define display_backlight(...)
#endif

#endif /* LIGHTNING_BOOT_DISPLAY_H */
