#ifndef MY_STRING_H
#define MY_STRING_H
#include <stddef.h>

char *strdup (const char *src);

size_t strlen (const char* str);

size_t strspn (const char *str, const char *accept);

void *memcpy (void *dest, void *src, size_t num);

void *memmove (void *dest, void *src, size_t count);

char *strcpy (char* dest, const char* src);

char *strdup (const char *src);

char *strtok (char *str, char *delim);

#endif