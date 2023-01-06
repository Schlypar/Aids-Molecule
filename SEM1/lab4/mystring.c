#include <stddef.h>
#include "mystring.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void * memset (void *s, int c, size_t n) {
    char ch = (char)c;
    char * str_s = (char*)s;
    while (n--) {
        *str_s++ = ch;
    }
    return s;
}

size_t strlen (const char* str) 
{
    char character = str[0];
    size_t num = 0;
    while(character != 0)
    {
        num++;
        character = str[num];
    }
    return num;
}
size_t strspn (const char *str, const char *accept) {
    size_t i, j;
    for (i = 0; str[i] != '\0'; i++) {
        for (j = 0; accept[j] != str[i]; j++) {
            if (accept[j] == '\0')
                return i;
        }
    }
    return i; 
}

void *memcpy (void *dest, void *src, size_t num) {
	assert(dest);
	assert(src);
	char* p = (char *)dest;
	char* q = (char *)src;
	while (num--) {
		*p = *q;
		p++;
		q++;
	}
	return dest;
}

void *memmove (void *dest, void *src, size_t count) {
	void * ret = dest;
	if (dest <= src || (char*)dest >= ((char*)src + count)) {
		while (count--) {
			*(char*)dest = *(char*)src;
			dest = (char*)dest + 1;
			src = (char*)src + 1;
		}
	}
	else {
		dest = (char*)dest + count - 1;
		src = (char*)src + count - 1;
		while (count--) {
			*(char*)dest = *(char*)src;
			dest = (char*)dest - 1;
			src = (char*)src - 1;
		}
	}
	return ret;
}

char *strcpy (char* dest, const char* src) {
    if (dest == NULL) {
        return NULL;
    }
    char *ptr = dest;
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return ptr;
}

char *strdup (const char *src) {
    char *dst = malloc(strlen (src) + 1);
    if (dst == NULL) return NULL;          
    strcpy(dst, src);                      
    return dst;                            
}

char *strtok(char *str, char *delim) {
    static char *input = NULL;
    if (str != NULL)
        input = str;
    if (input == NULL)
        return NULL;
    char *result = (char *)malloc((strlen(input) + 1) * sizeof(char));
    int i = 0;
    for (; input[i] != '\0'; i++) {
		int flag = 0;
		for (int j = 0; j < strlen(delim); j++) {
		    if (input[i] == delim[j]) {
				flag = 1;
				break;
			}
		}
        if (!flag)
            result[i] = input[i];
        else {
            result[i] = '\0';
            input = input + i + 1;
            return result;
        }
    }
    result[i] = '\0';
    input = NULL;
    return result;
}