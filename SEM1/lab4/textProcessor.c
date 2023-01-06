#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "holylib.h"

#define DELIM " ,.\t"
#define SEPARATOR "---"

void strCut(char *str, int index) {
	for (int i = index; i < strlen(str); i++) {
		str[i] = str[i+1];
	}
	str[strlen(str)+1] = '\0';
}

int stringRemove(char *str, int index)
{
	int len = strlen(str);
	if (index < 0)
	{
		printf("Ошипка... Попробуйте снова\n");
		return 1;
	}
	if (index >= len)
	{
		printf("Ошипка... Попробуйте снова\n");
		return 1;
	}
	for (int i = index; i < len; i++)
	{
		str[i] = str[i + 1];
	}
	str[(strlen(str) + 1)] = '\0';
	return 0;
}

char *stringConcatenate(char *str1, char *str2)
{
	const size_t len1 = strlen(str1);
	const size_t len2 = strlen(str2);
	char *string = (char *)malloc((len1 + len2 + 1) * sizeof(char));
	if (!string)
	{
		return NULL;
	}
	memcpy(string, str1, len1);
	memcpy(string + len1, str2, len2 + 1);
	free(str1);
	return string;
}

char **parser(char *str, int *len) {
	*len = 0;
	char *string = strdup(str);
	free(str);
	char *word = strtok(string, DELIM);
	char **sentence = (char **)calloc(1, sizeof(char *));
	while (word != NULL) {
		if (word[0] != '\0') {
			sentence = realloc(sentence, ((*len) + 1) * sizeof(char *));
			sentence[*len] = word;
			(*len)++;
		}
		else 
		{
			free(word);
		}
		word = strtok(NULL, DELIM);
	}
	free(string);
	return sentence;
}

void sort(char **sentence, int len)
{
	if (len == 1) 
	{
		return;
	}
	int len1, len2, minLen, index;
	minLen = 99999999;
	char *tmp;
	for (int i = 0; i < len - 1; i++)
	{
		len1 = strlen(sentence[i]);
		for (int j = i + 1; j < len; j++)
		{
			len2 = strlen(sentence[j]);
			if (len2 < minLen) 
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
			minLen = 999999999;
	}
}

char *sortWordOrder(char *text)
{
	int len;
	char *string = calloc(1, sizeof(char));
	char *tempString;
	char **words;
	words = parser(text, &len);
	sort(words, len);
	printf("/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/\n");
	for (int i = 0; i < len; i++)
	{
		tempString = stringConcatenate(words[i], " ");
		string = stringConcatenate(string, tempString);
		free(tempString);
	}
	string = realloc(string, (strlen(string) + 1) * sizeof(char));
	free(words);
	string[strlen(string) - 1] = '\0';
	string = realloc(string, (strlen(string) + 1) * sizeof(char));
	return string;
}
