/* string.c -- Basic <string.h> functionality without standard library 
 *			 (these are just the typical public-domain implemenations)
 *
 * Andrey Yurovsky <andrey@cozybit.com> 
 */

unsigned int strlen(const char *s)
{
	const char *p = s;

	while (*p != '\0') p++;
	return (unsigned int)(p - s);
}

char *strcpy(char *restrict s1, const char *restrict s2)
{
	char *dst = s1;
	const char *src = s2;

	while((*dst++ = *src++) != '\0');
	return s1;
}

int strcmp(const char *s1, const char *s2)
{
	for(; *s1 == *s2; s1++, s2++) {
		if(*s1 == '\0' && *s2 == '\0')
			return 0;
		if(*s1 == '\0')
			return -1;
		if(*s2 == '\0')
			return 1;
	}
	if(*s1 == '\0')
		return -1;
	return 1;
}

int strncmp(const char *s1, const char *s2, unsigned int n)
{
	unsigned char uc1, uc2;

	if(n == 0)
		return 0;
	while(n-- > 0 && *s1 == *s2) {
		if(n == 0 || *s1 == '\0')
			return 0;
		s1++;
		s2++;
	}
	uc1 = (*(unsigned char *)s1);
	uc2 = (*(unsigned char *)s2);
	return ((uc1 < uc2) ? -1 : (uc1 > uc2));
}

char *strcat(char *restrict s1, const char *restrict s2)
{
	char *s = s1;
	/* Move s so that it points to the end of s1.  */
	while(*s != '\0')
		s++;
	/* Copy the contents of s2 into the space at the end of s1.  */
	strcpy(s, s2);
	return s1;
}

char *strncat(char *restrict s1, const char *restrict s2, unsigned int n)
{
	char *s = s1;

	while(*s != '\0')
		s++;
	/* s now points to s1's trailing null character, now copy
	   up to n bytes from s2 into s1 stopping if a null character
	   is encountered in s2.
	   It is not safe to use strncpy here since it copies EXACTLY n
	   characters, NULL padding if necessary.  */
	while(n != 0 && (*s = *s2++) != '\0') {
		n--;
		s++;
	}
	if(*s != '\0')
		*s = '\0';
	return s1;
}

void *memcpy(void * restrict s1, const void * restrict s2, unsigned int n)
{
	char *dst = s1;
	const char *src = s2;

	while(n-- != 0)
		*dst++ = *src++;
	return s1;
}

