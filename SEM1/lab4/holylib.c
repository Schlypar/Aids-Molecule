#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*--------------------------------Функции ввода значений в массивы или переменные-------------------------------*/


int inputInt(int *input)
{
	int check = 0;
	while (check != 1)
	{
		check = scanf("%d", input);
		if (check == EOF)
		{
			return 1;
		}
		if (check == 1)
		{
			break;
		}
		printf("Как же так? Попробуйте снова\n");
		scanf("%*[^\n]");
	}
	return 0;
}

int inputFloat(float *input)
{
	int check = 0;
	while (check != 1)
	{
		check = scanf("%f", input);
		if (check == EOF)
		{
			return 1;
		}
		if (check == 1)
		{
			break;
		}
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
		{
			return 1;
		}
		if (check == 1)
		{
			break;
		}
		printf("Как же так? Попробуйте снова\n");
		scanf("%*[^\n]");
	}
	return 0;
}

int inputLongDouble(long double *input)
{
	int check = 0;
	while (check != 1)
	{
		check = scanf("%Lf", input);
		if (check == EOF)
		{
			return 1;
		}
		if (check == 1)
		{
			break;
		}
		printf("Как же так? Попробуйте снова\n");
		scanf("%*[^\n]");
	}
	return 0;
}

int *inputArray(int *len)
{
	printf("Введите количество элементов массива: ");
	int inputTemp, check;
	int *data = (int *)malloc(sizeof(int));
	check = inputInt(len);
	if (check == 1)
	{
		printf("Программа закончена\n");
		data[0] = 0;
		return data;
	}
	printf("\n");
	data = (int *)realloc(data, *len * sizeof(int));
	if (!data)
	{
		printf("Упс... Что-то пошло не так\n");
		return NULL;
	}
	for (int i = 0; i < *len; ++i)
	{
		printf("data[%d] = ", i);
		check = inputInt(&inputTemp);
		if (check == 1)
		{
			printf("Программа закончена\n");
			break;
		}
		data[i] = inputTemp;
	}
	return data;
}

/*-------------------------------------------------------------------------------------------------------------*/

/*---------------------------Функции, которые являются полезными для будущих лаб-------------------------------*/

void swap(double *a, double *b)
{
	*a = *a + *b;
	*b = *a - *b;
	*a = *a - *b;
}

void swapChar(char *a, char *b)
{
	char buffer = *a;
	*a = *b;
	*b = buffer;
}

int insertElementInt(int **data, int *len, int index, int value)
{
	*len = *len + 1;
	*data = (int *)realloc(*data, *len * sizeof(int));
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
	{
		(*data)[i] = (*data)[i - 1];
	}
	(*data)[index] = value;
	return 0;
}

int insertElementFloat(float **data, int *len, int index, float value)
{
	*len = *len + 1;
	*data = (float *)realloc(*data, *len * sizeof(float));
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
	{
		(*data)[i] = (*data)[i - 1];
	}
	(*data)[index] = value;
	return 0;
}

int insertElementDouble(double **data, int *len, int index, double value)
{
	*len = *len + 1;
	*data = (double *)realloc(*data, *len * sizeof(double));
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
	{
		(*data)[i] = (*data)[i - 1];
	}
	(*data)[index] = value;
	return 0;
}

int removeElementInt(int **data, int *len, int index)
{
	if ((index < 0) || (index >= *len))
	{
		printf("Элементов с такими индексами не существует. Попробуйте снова\n");
		return -1;
	}
	*len = *len - 1;
	for (int i = index; i < *len; i++)
	{
		(*data)[i] = (*data)[i + 1];
	}
	(*data)[*len] = 0;
	*data = (int *)realloc(*data, *len * sizeof(int));
	if (!(*data))
	{
		printf("Упс... Что-то пошло не так... Ошипка\n");
		free(*data);
		return 1;
	}
	return 0;
}

int removeElementFloat(float **data, int *len, int index)
{
	if ((index < 0) || (index >= *len))
	{
		printf("Элементов с такими индексами не существует. Попробуйте снова\n");
		return -1;
	}
	*len = *len - 1;
	for (int i = index; i < *len; i++)
	{
		(*data)[i] = (*data)[i + 1];
	}
	(*data)[*len] = 0;
	*data = (float *)realloc(*data, *len * sizeof(float));
	if (!(*data))
	{
		printf("Упс... Что-то пошло не так... Ошипка\n");
		free(*data);
		return 1;
	}
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
	{
		(*data)[i] = (*data)[i + 1];
	}
	(*data)[*len] = 0;
	*data = (double *)realloc(*data, *len * sizeof(double));
	if (!(*data))
	{
		printf("Упс... Что-то пошло не так... Ошипка\n");
		free(*data);
		return 1;
	}
	return 0;
}

int *arrayGen(int *data, int *len, int start, int end)
{
	int check;
	printf("Введите левую границу диапазона генерации случайных чисел: ");
	check = inputInt(&start);
	if (check == 1)
	{
		printf("Значит пусть будет 0\n");
	}
	printf("\n");

	printf("Введите правую границу диапазона генерации случайных чисел: ");
	check = inputInt(&end);
	if (check == 1)
	{
		printf("Значит пусть будет 10\n");
	}
	printf("\n");

	printf("Введите длину массива со случайными значениями: ");
	check = inputInt(len);
	if (check == 1)
	{
		printf("Значит пусть будет 10\n");
	}
	printf("\n");
	data = (int *)malloc(*len * sizeof(int));
	srand(start - end + clock());

	for (int i = 0; i < *len; ++i)
	{
		data[i] = start + rand() % end;
	}
	return data;
}

void printArrayInt(int *data, int len)
{
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
			printf("[%d]\n", data[0]);
			break;
		}
		printf("[");
		for (int i = 0; i < len - 1; i++)
		{
			end = i;
			printf("%d, ", data[i]);
		}
		end++;
		printf("%d]\n", data[end]);
		printf("\n");
		break;
	}
	default:
	{
		printf("Упс... Что-то пошло не так\n");
		return printArrayInt(data, len);
	}
	}
}

void printArrayFloat(float *data, int len)
{
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
			printf("[%f]\n", data[0]);
			break;
		}
		printf("[");
		for (int i = 0; i < len - 1; i++)
		{
			end = i;
			printf("%f, ", data[i]);
		}
		end++;
		printf("%f]\n", data[end]);
		printf("\n");
		break;
	}
	default:
	{
		printf("Упс... Что-то пошло не так\n");
		return printArrayFloat(data, len);
	}
	}
}

void printArrayDouble(double *data, int len)
{
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
