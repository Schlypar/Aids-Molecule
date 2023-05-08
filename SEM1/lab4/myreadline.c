#include "myreadline.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *readline(const char *promt)
{
	if (promt)
		printf("%s", promt);
	char buffer[81] = { 0 };
	char *result = NULL;
	int len = 0, n, chunkLen, strLen;
	do
	{
		n = scanf("%80[^\n]", buffer);
		if (n < 0)
		{
			free(result);
			scanf("%*c");
			return NULL;
		}
		if (n > 0)
		{
			chunkLen = strlen(buffer);
			strLen = len + chunkLen;
			result = (char *) realloc(result, (strLen + 1) * sizeof(char));
			memmove(result + len, buffer, chunkLen);
			len = strLen;
		}
		else
		{
			scanf("%*c");
		}
	} while (n > 0);
	if (len > 0)
	{
		result[len] = '\0';
		return result;
	}
	free(result);
	return NULL;
}