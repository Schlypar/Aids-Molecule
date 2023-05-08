#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "structGen.h"
#include "structReader.h"

#define SPAC " "

char *randomString(size_t len)
{
	char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-";
	char *randomString = NULL;
	if (!len)
		return NULL;
	randomString = malloc((len + 1) * sizeof(char));
	if (!randomString)
		return NULL;
	for (int i = 0; i < len; i++)
	{
		int key = rand() % (int) (sizeof(characters) - 1);
		randomString[i] = characters[key];
	}
	randomString[len] = '\0';
	return randomString;
}

char *fullnameGen(void)
{
	char *family = randomString(rand() % 10 + 15);
	char *name = randomString(rand() % 20 + 20);
	char *patronymic = randomString(rand() % 21 + 24);

	int len = strlen(family) + strlen(name) + strlen(patronymic) + 3;
	char *fullname = calloc(len, sizeof(char));

	memcpy(fullname, family, strlen(family));
	memcpy(fullname + strlen(fullname), SPAC, 1);

	memcpy(fullname + strlen(fullname), name, strlen(name));
	memcpy(fullname + strlen(fullname), SPAC, 1);

	memcpy(fullname + strlen(fullname), patronymic, strlen(patronymic));

	fullname[len - 1] = '\0';

	free(family);
	free(name);
	free(patronymic);

	return fullname;
}

char *idGen(void)
{
	char charset1[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char charset2[] = "0123456789";

	char *result = malloc(8);

	for (int i = 0; i < 3; i++)
	{
		int key = rand() % (int) (sizeof(charset1) - 1);
		result[i] = charset1[key];
	}
	result[3] = '-';

	for (int i = 4; i < 7; i++)
	{
		int key = rand() % (int) (sizeof(charset2) - 1);
		result[i] = charset2[key];
	}
	result[7] = '\0';

	return result;
}

Voter *voterGen(size_t size)
{
	Voter voter;
	Voter *array = malloc(size * sizeof(Voter));
	for (int k = 0; k < size; k++)
	{
		voter.fullName = fullnameGen();
		char *temp = idGen();
		memcpy(voter.idPolling, temp, strlen(temp) + 1);
		voter.age = rand() % 72 + 18;
		array[k] = voter;
	}
	return array;
}