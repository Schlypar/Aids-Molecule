#include "cases.h"
#include "holylib.h"
#include <stdio.h>
#include <stdlib.h>

void memalloc(double **base, int *el, int *reallen) 
{
	 if (*reallen < 2*(*el)) {													
	    *reallen = 2*(*el);														
	    (*base) = (double *)realloc((*base), *reallen * sizeof(double));	
	}																		
	if (*reallen > 2*(*el)) {													
		*reallen = 2*(*el);														
		(*base) = (double *)realloc((*base), *reallen * sizeof(double));	
    }																		
}

int menuInput(int *loop, int *el,  double **data, int *reallen) {
	double value;
	int error, len;
	if (*loop != 0) {
		printf("Я не думаю, что стоит инициализировать еще 1 массив\n");			
		return -1;
	}
	printf("Введите длину массива, натуральное число: ");
	error = inputInt(&len);
	if (error) {																			//
		printf("Программа закончена\n");													// Просим ввести длину массива
		return error;																				// 
	}																						//
	printf("\n");																			//
	while (len <= 0) {																		//
		printf("Как же так? Попробуйте снова\n");											// И даем его заполнять
		error = inputInt(&len);																//
		if (error) {
			printf("Программа закончена\n");
			return error;
			}
		}
		(*data) = (double *)realloc((*data), len * sizeof(double));
		for (int i = 0;i < len; i++) {
			printf("data[%d] будет ", i);
			error = inputDouble(&value);
			if (error) {
				printf("Программа закончена\n");
				return error;
			}
			(*data)[i] = value;
			printf("\n");
			(*el)++;
			(*loop)++;
		}
	printf("=====================================================\n");
    memalloc(data, el, reallen);						
	return error;
}

int menuInsert(int *loop, int *reallen, int *el, double **data) {
	double value;
	int error, placeHolder, len;
	if (*loop == 0) {
		printf("Некуда вставлять... Попробуйте снова позже, когда будет куда\n\n");
		return -1;
	}
	printf("Введите номер элемента в массиве, натуральное число, начиная от 1: ");			// Просим ввести индекс
	error = inputInt(&len);															
	if (error) {
		printf("Программа закончена\n");
		return error;
	}
	if (len <= 0) {
		printf("Видимо, у Вас не получилось, попробуйте снова\n\n");											
		return -1;
	}
	placeHolder = len - 1;
	printf("\n");

	printf("Введите его значение, вещественное число: ");									// Просим ввести значение
	error = inputDouble(&value);
	if (error) {
		printf("Программа зкончена\n");
		return error;
	}
	printf("\n");

	if (placeHolder >= *el) {
		(*data)[*loop] = value;
		(*el)++;
		printf("=====================================================\n");					// Рассматриваем 2 случая
		(*loop)++;
		memalloc(data, el, reallen);
		return 0;
	}
	else {
		error = insertElementDouble(data, reallen, placeHolder, value);
		if (error) {
			printf("Программа закончена\n");
			return error;
		}
		if (error == -1) {
			return error;
		}
		(*reallen)--;
		(*el)++;
		printf("=====================================================\n");
		(*loop)++;
	}
    memalloc(data, el, reallen);
	return error;
}

int menuRemove(int *loop, int *el, int *reallen, double **data) {
	int placeHolder, len, error;
	double value;
	if (*loop == 0) {
		printf("Как же так? Я же не могу низвести до атомов массив, попробуйте снова\n\n");
		return -1;
	}
	printf("Введите номер элемента, натуральное число: ");
	error = inputInt(&len);
	if (error) {
		printf("Программа закончена\n");									
		return error;																				// Просим ввести его индекс
	}
	while (len <= 0) {
		printf("Как же так? Попробуйте снова\n");
		error = inputInt(&len);
		if (error) {
			printf("Программа закончена\n");
			return error;
		}
	}
	placeHolder = len - 1;

	if (placeHolder > *el) {
		printf("Как мы удалим то чего нет? Попробуйте снова\n\n");								// Рассматриваем 2 случая
		return -1;
	}
	error = removeElementDouble(data, reallen, placeHolder);
	if (error) {
		printf("Программа закончена\n");
		return error;
	}
	if (error == -1) {
		return error;
	}
	(*reallen)++;
	(*el)--;
	(*loop)--;
	printf("=====================================================\n");
    memalloc(data, el, reallen);
	return error;
}

double *menuTask(int *loop, int *el, int *reallen, double **data, int *newEl) {
	double mean = 0;
	int error;

	for (int i = 0; i < *el; i++)
	{
		mean += (*data)[i];
	}
	mean = mean/(*el);

	double *newData = (double *)malloc((*el) * sizeof(double));
	for (int i = 0; i < *el; i++) 
	{
		newData[i] = (*data)[i] - mean;
	}
	*newEl = *el;

	for (int i = 0; i < (*newEl); i++)
	{
		if (newData[i] < 0) removeElementDouble(data, el, i);
	}
	return newData;
}