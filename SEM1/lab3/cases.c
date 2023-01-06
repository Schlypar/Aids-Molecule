#include "cases.h"
#include "holylib.h"
#include <stdio.h>
#include <stdlib.h>

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
    if (*reallen < 2*(*el)) {													//
	    *reallen = 2*(*el);														//
	    (*data) = (double *)realloc((*data), *reallen * sizeof(double));	//
	}																		// Выделение памяти под элементы массива согласно условию 2*N
	if (*reallen > 2*(*el)) {													// Где N - количество элементов, записанных в массив
		*reallen = 2*(*el);														//
		(*data) = (double *)realloc((*data), *reallen * sizeof(double));	//
    }																		//
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
		if (*reallen < 2*(*el)) {													//
	        *reallen = 2*(*el);														//
	        (*data) = (double *)realloc((*data), *reallen * sizeof(double));	//
		}																		// Выделение памяти под элементы массива согласно условию 2*N
		if (*reallen >= 2*(*el)) {													// Где N - количество элементов, записанных в массив
		    *reallen = 2*(*el);														//
		    (*data) = (double *)realloc((*data), *reallen * sizeof(double));	//
		}																		//
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
    if (*reallen < 2*(*el)) {													//
	    *reallen = 2*(*el);														//
	    (*data) = (double *)realloc((*data), *reallen * sizeof(double));		//
	}																			// Выделение памяти под элементы массива согласно условию 2*N
	if (*reallen >= 2*(*el)) {													// Где N - количество элементов, записанных в массив
		*reallen = 2*(*el);														//
		(*data) = (double *)realloc((*data), *reallen * sizeof(double));		//
    }																			//
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
	else {
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
	}
	printf("=====================================================\n");
    if (*reallen < 2*(*el)) {													//
	    *reallen = 2*(*el);														//
	    (*data) = (double *)realloc((*data), *reallen * sizeof(double));	//
	}																		// Выделение памяти под элементы массива согласно условию 2*N
	if (*reallen > 2*(*el)) {													// Где N - количество элементов, записанных в массив
		*reallen = 2*(*el);														//
		(*data) = (double *)realloc((*data), *reallen * sizeof(double));	//
    }																		//
	return error;
}

double *menuTask(int *loop, int *el, int *reallen, double **data, int *newEl) {
	double mean;
	int n, error, index;
	if (*loop == 0) {
		printf("Как же так? Тут пусто, так что попробуйте снова\n\n");
		return NULL;
	}
	mean = 0;
	n = 0;
	for (int i = 0; i < *el; i++) {															// Считаем среднее значение
		mean += (*data)[i];
		n = i + 1;
	}
	mean = mean/n;
    double *newData = (double *)malloc(*reallen * sizeof(int));
	for (int i = 0; i < (*reallen)/2 + 1; i++) {
		newData[i] = (*data)[i] - mean;
	}
	for (int i = 0; i < (*reallen)/2; i++) {
		if ((*data)[i] > mean) {
		    error = removeElementDouble(data, reallen, i);
			if (error) {
				return NULL;
			}
			(*reallen)++;
			(*el)--;
			(*loop)--;
			i--;
		}
	}
	printf("=====================================================\n");
    if (*reallen < 2*(*el)) {													//
	    *reallen = 2*(*el);														//
	    (*data) = (double *)realloc((*data), *reallen * sizeof(double));		//
	}																			// Выделение памяти под элементы массива согласно условию 2*N
	if (*reallen > 2*(*el)) {													// Где N - количество элементов, записанных в массив
		*reallen = 2*(*el);														//
		(*data) = (double *)realloc((*data), *reallen * sizeof(double));		//
    }																			//
	return newData;
}
