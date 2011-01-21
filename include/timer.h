#ifndef LIGHTNING_BOOT_TIMER_H
#define LIGHTNING_BOOT_TIMER_H

void timer_init(void);
void timer_start(void);
u32 timer_stop(void);
void tmr_poll_start(u16);
u8 tmr_poll_has_expired(void);
void timerPause(u16);

#endif /* LIGHTNING_BOOT_TIMER_H */
