#ifndef LIGHTNING_BOOT_STRING_H
#define LIGHTNING_BOOT_STRING_H

unsigned int strlen(const char *s);
char *strcpy(char *dest, const char *src);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, unsigned int n);
char *strcat(char *restrict s1, const char *restrict s2);
char *strncat(char *restrict s1, const char *restrict s2, unsigned int n);
char *memcpy(char *dest, char *src, unsigned int n);

#endif
