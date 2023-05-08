#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


#include "mystring.h"

size_t strlen(const char *str)
{
	char character = str[0];
	size_t num = 0;
	while (character != '\0')
	{
		num++;
		character = str[num];
	}
	return num;
}

size_t strspn(const char *str, const char *accept)
{
	size_t i, j;
	for (i = 0; str[i] != '\0'; i++)
	{
		for (j = 0; accept[j] != str[i]; j++)
			if (accept[j] == '\0')
				return i;
	}
	return i;
}

size_t strcspn(const char *s1, const char *s2)
{
	const char *p, *delim;
	char character, sc;

	for (p = s1;;)
	{
		character = *p++;
		delim = s2;
		do
		{
			if ((sc = *delim++) == character)
				return (p - 1 - s1);
		} while (sc != '\0');
	}
}

void *memcpy(void *dest, void *src, size_t num)
{
	assert(dest);
	assert(src);
	char *p = (char *) dest;
	char *q = (char *) src;
	while (num--)
	{
		*p = *q;
		p++;
		q++;
	}
	return dest;
}

char *strcpy(char *dest, const char *src)
{
	if (dest == NULL)
		return NULL;
	char *ptr = dest;
	while (*src != '\0')
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	return ptr;
}

char *strdup(const char *src)
{
	char *dst = malloc(strlen(src) + 1);
	if (dst == NULL)
		return NULL;
	strcpy(dst, src);
	return dst;
}

char *strtok(char *str, const char *delim)
{
	static char *p = NULL;

	if (str)
		p = str;
	else if (!p)
		return 0;

	str = p + strspn(p, delim);
	p = str + strcspn(str, delim);

	if (p == str)
		return p = NULL;

	p = *p ? *p = '\0', p + 1 : NULL;
	return str;
}
