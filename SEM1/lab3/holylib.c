#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int inputInt(int *input)
{
	int check = 0;
	while (check != 1)
	{
		check = scanf("%d", input);
		if (check == EOF)
			return 1;
		if (check == 1)
			break;
		printf("Как же так? Попробуйте снова\n");
		scanf("%*[^\n]");
	}
	return 0;
}

int inputDouble(double *input)
{
	int check = 0;
	while (check != 1)
	{
		check = scanf("%lf", input);
		if (check == EOF)
			return 1;
		if (check == 1)
			break;
		printf("Как же так? Попробуйте снова\n");
		scanf("%*[^\n]");
	}
	return 0;
}

int insertElementDouble(double **data, int *len, int index, double value)
{
	*len = *len + 1;
	*data = (double *) realloc(*data, *len * sizeof(double));
	if (!(*data))
	{
		printf("Упс... Что-то пошло не так... Ошипка\n");
		free(*data);
		return 1;
	}
	if ((index < 0) || (index >= *len))
	{
		printf("Элемента с таким индексом не существует. Попробуйте снова\n");
		return -1;
	}
	for (int i = *len - 2; i > index; --i)
		(*data)[i] = (*data)[i - 1];
	(*data)[index] = value;
	return 0;
}

int removeElementDouble(double **data, int *len, int index)
{
	if ((index < 0) || (index >= *len))
	{
		printf("Элементов с такими индексами не существует. Попробуйте снова\n");
		return -1;
	}
	*len = *len - 1;
	for (int i = index; i < *len; i++)
		(*data)[i] = (*data)[i + 1];
	(*data)[*len] = 0;
	*data = (double *) realloc(*data, *len * sizeof(double));
	if (!(*data))
	{
		printf("Упс... Что-то пошло не так... Ошипка\n");
		free(*data);
		return 1;
	}
	return 0;
}

void printArrayDouble(double *data, int len)
{
	if (!data)
		return;
	printf("Если хотите вывести массив, то введите 1, иначе 0\n");
	int end, menu, check;
	check = inputInt(&menu);
	if (check == 1)
	{
		printf("Так уж и быть, не будем выводить массив\n");
		return;
	}
	switch (menu)
	{
		case 0:
			{
				return;
			}
		case 1:
			{
				printf("\n");
				if (len == 1)
				{
					printf("[%lf]\n", data[0]);
					break;
				}
				printf("[");
				for (int i = 0; i < len - 1; i++)
				{
					end = i;
					printf("%lf, ", data[i]);
				}
				end++;
				printf("%lf]\n", data[end]);
				printf("\n");
				break;
			}
		default:
			{
				printf("Упс... Что-то пошло не так\n");
				return printArrayDouble(data, len);
			}
	}
}