#include <stdio.h>
#include <stdlib.h>

#include "mystring.h"
#include "holylib.h"
#include "myreadline.h"
#include "textProcessorPro.h"

#define DELIM " \t"
#define SEPARATOR "---"

char **parser(char *string, int *len) {
	*len = 0;
	char *word = strtok(string, DELIM);
	char **sentence = (char **)calloc(1, sizeof(char *));
	while (word != NULL) {
		if (word[0] == '\0')
		{
			// free(word);
			word = strtok(NULL, DELIM);
			continue;
		}
		sentence = realloc(sentence, ((*len) + 1) * sizeof(char *));
		sentence[*len] = word;
		(*len)++;
		word = strtok(NULL, DELIM);
	}
	return sentence;
}

void sort(char **sentence, int len)
{
	if (len == 1) 
	{
		return;
	}
	int len1, len2, minLen, index;
	char *tmp;
	for (int i = 0; i < len - 1; i++)
	{
		minLen = strlen(sentence[i]);
		len1 = strlen(sentence[i]);
		for (int j = i + 1; j < len; j++)
		{
			len2 = strlen(sentence[j]);
			if (len2 <= minLen) 
			{
				minLen = len2;
				index = j;
			}
		}
			printf("|%-13d %8s %19d|\n|%-13s %8s %19s|\n", minLen, SEPARATOR, len1, sentence[index], SEPARATOR, sentence[i]);
			printf("--------------------------------------------\n");
			tmp = sentence[i];
			sentence[i] = sentence[index];
			sentence[index] = tmp;
	}
}

char *sortWordOrder(char *text)
{
	int len;
	char *string = calloc(strlen(text) + 2, sizeof(char));
	char space[] = " ";
	char **words;
	words = parser(text, &len);
	sort(words, len);
	printf("/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/\n");
	int i;
	for (i = 0; i < len; i++)
	{
		if (strlen(words[i]) == 0) 
		{
			continue;
		}
		memcpy(string + strlen(string), words[i], strlen(words[i]));
		memcpy(string + strlen(string), space, strlen(space) + 1);
	}
	string[strlen(string)-1] = '\0';
	string = realloc(string, (strlen(string) + 1) * sizeof(char));
	free(words);
	string = realloc(string, (strlen(string) + 1) * sizeof(char));
	free(text);
	return string;
}
