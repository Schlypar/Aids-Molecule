#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int printString(char *text)
{
	if (!text)
	{
		return 1;
	}
	printf("\"");
	printf("%s", text);
	printf("\"");
	printf("\n");
return 0;
}

/*--------------------------------------------------------------------------------------------------------------*/
