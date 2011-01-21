#ifndef LIGHTNING_BOOT_UART_H
#define LIGHTNING_BOOT_UART_H

#include <common.h>

void db_init(void);
#ifdef DEBUG
void db_putchar(char c);
//#define db_puts(...)
void db_puts(const char *s);
void db_hex(unsigned int val);
void db_byte(char val);
void db_16(u16 val);
void db_int(unsigned int val);
int db_tstc (void);
int db_getc (void);
int db_getc_async(void);
void db_displaytee(int v);
#else
#define db_putchar(...)
#define db_puts(...)
#define db_hex(...)
#define db_byte(...)
#define db_int(...)
#define db_getc_async(...)
#define db_displaytee(...)
#endif /* DEBUG */

/* timer stopwatch */

#if defined DEBUG && defined DEBUG_STOPWATCH
void db_stopwatch_start(const char *name);
void db_stopwatch_stop(void);
#else
#define db_stopwatch_start(...)
#define db_stopwatch_stop(...)
#endif /* DEBUG && DEBUG_STOPWATCH */

#endif /* LIGHTNING_BOOT_UART_H */
