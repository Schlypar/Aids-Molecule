#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include "dynamicArray.h"
#include "base.h"
#include "container.h"
#include "real.h"
#include "complex.h"

#define START_SIZE 2
#define NOT_DONE 1
#define ERROR 1
#define EXIT 1
#define REAL_CASE 1
#define COMPLEX_CASE 3
#define DELIM " +-\t"

#define MENU_STRING "Main menu:\n1 - ввести действительное число\n2 - ввести комплексное число\n3 - toString метод\n4 - print метод\n5 - dump метод\n6 - вложить массив\n"
#define PROMT "->"

typedef int Status;

/*
Модульный тест
*/

/*
Динамический массив (вариант 8)

Коплексные числа, вещественные числа

map, where, concat, sort (в модульном тесте)
*/

Status inputInt(int* input) {
    int check = 0;
    while (check != ERROR) {
        check = scanf("%d", input);
        if (check == EOF) {
            return ERROR;
        }
        if (check == ERROR) {
            break;
        }
        printf("Как же так? Попробуйте снова\n");
        scanf("%*[^\n]");
    }
    return EXIT_SUCCESS;
}

Status inputDouble(double* input) {
    int check = 0;
    while (check != ERROR) {
        check = scanf("%lf", input);
        if (check == EOF) {
            return ERROR;
        }
        if (check == ERROR) {
            break;
        }
        printf("Как же так? Попробуйте снова\n");
        scanf("%*[^\n]");
    }
    return EXIT_SUCCESS;
}

Status caseReal(Array* array)
{
    Status status;
    double real;
    printf("Введите действительное число: ");
    status = inputDouble(&real);
    if (status == ERROR)
    {
        printf("\nФункция main() завершила свое выполение.\n");
        if (array) freeBase(array);
        return EXIT_SUCCESS;
    }
    Real* number = constructReal(real);
    appendArray(array, number);

    return EXIT_SUCCESS;
}

Status caseComplex(Array* array)
{
    Status status;
    double real, imaginary;
    char sign; //wont use after scanf
    printf("Введите комплексное число: ");
    status = scanf("%lf %c %lfi", &real, &sign, &imaginary);
    if (status == ERROR)
    {
        printf("\nФункция main() завершила свое выполение.\n");
        if (array) freeBase(array);
        return EXIT_SUCCESS;
    }
    Complex* number = constructComplex(real, imaginary);
    appendArray(array, number);

    return EXIT_SUCCESS;
}

Base* makeBase()
{
    Status status;
    double real = 0, imaginary = 0;
    printf("Введите действительное или комплексное число: ");
    char string[100] = { 0 };
    scanf("%*c");
    status = scanf("%[^\n]s", string);
    //printf("%s", string);
    if (status == EOF)
    {
        return NULL;
    }
    char* end;

    char* word = strtok(string, DELIM);
    real = strtod(word, &end);

    word = strtok(NULL, DELIM);
    if (word) imaginary = strtod(word, &end);

    int menu = (imaginary != 0) ? COMPLEX_CASE : REAL_CASE;

    Base* number;
    while (NOT_DONE)
    {
        switch (menu)
        {
        case REAL_CASE:
            number = constructReal(real);
            return number;
        case COMPLEX_CASE:
            number = constructComplex(real, imaginary);
            return number;
        default:
            printf("Введите снова: ");
            status = scanf("%s", string);
            if (status == EOF)
            {
                return NULL;
            }
            break;
        }
    }
}

void fill(Array* array)
{
    int size = getSize(array);
    if (size == 0) return;

    for (int i = 0; i < size; i++)
    {
        Base* current = makeBase();

        while (!current) makeBase();

        appendArray(array, current);
    }
}

int main()
{
	setlocale(LC_ALL, "Rus");

	int menu;
    Status status = 0;
    char* str;
    Array* array = constructArray(START_SIZE);
	while (NOT_DONE)
	{
        if (status == EXIT) break;

        printf("%s\n", MENU_STRING);

        status = inputInt(&menu);
        if (status == ERROR) goto errorHandling;

        switch (menu)
        {
        case 1:
            status = caseReal(array);
            if (status == ERROR) goto errorHandling;
            break;
        case 2:
            status = caseComplex(array);
            if (status == ERROR) goto errorHandling;
            break;
        case 3:
            str = toStringBase(array);
            printf("%s\n", str);
            free(str);
            break;
        case 4:
            str = printBase(array);
            printf("%s\n", str);
            free(str);
            break;
        case 5:
            str = dumpBase(array);
            printf("%s\n", str);
            free(str);
            break;
        case 6:
            printf("Введите размер массива: ");
            int size;

            status = inputInt(&size);
            if (status == ERROR) goto errorHandling;

            Array* embeddedArray = constructArray(size);
            fill(embeddedArray);
            appendArray(array, embeddedArray);
            break;
        default:
            freeBase(array);
            printf("\nФункция main() завершила свое выполение.\n");
            status = EXIT;
        }
	}

errorHandling:
    return EXIT_SUCCESS;
}