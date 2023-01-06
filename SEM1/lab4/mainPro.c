
#include <stdio.h>
#include <stdlib.h>
#include "myreadline.h"
#include "mystring.h"
#include "holylib.h"
#include "textProcessorPro.h"

#define PROMT "---> "

char *process(char *str)
{	
	if (!str)
	{
		printf("\nПрограмма закончила свое выполнение\n");
		return NULL;
	}
	char *string = strdup(str);
	printf("\"%s\"\n", str);
	free(str);
	printf("=========================================================\n");
	char *result = sortWordOrder(string);
	return result;
}

int main()
{
	int error;
	char *text;
	while(1)
	{
		text = readline(PROMT);
		text = process(text);\
		if (!text) 
		{
			break;
		}
		printString(text);
		free(text);
	}
	if (text)
	{
		free(text);
	}
	return 0;
}
