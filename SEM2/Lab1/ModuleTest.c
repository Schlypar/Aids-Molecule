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

typedef int Bool;

/*
Модульный тест
*/

/*
Динамический массив (вариант 8)

Коплексные числа, вещественные числа

map, where, concat, sort
*/

/*-------------------------------Функции которые были бы написаны пользвоателдем------------------------------------------*/

void plusTwoToReal(Base* base)
{
	char* typeName = getType(base);

	if (baseIsThisType(typeName, TYPE_NAME_REAL))
	{
		setReal(base, getReal(base) + 2);
	}
	if (baseIsThisType(typeName, TYPE_NAME_COMPLEX))
	{
		setComplexReal(base, getRealPart(base) + 2);
	}
	free(typeName);
}

void minusFiveToReal(Base* base)
{
	setReal(base, getReal(base) - 5);
}

Bool baseIsReal(Base* base)
{
	char* typeName = getType(base);

	if (strcmp(typeName, TYPE_NAME_REAL) == STRING_EQUALS)
	{
		free(typeName);
		return TRUE;
	}
	else
	{
		free(typeName);
		return FALSE;
	}
}

void compareReal(Base* first, Base* second)
{
	char* typeNameFirst = getType(first);
	char* typeNameSecond = getType(second);

	double realOne = 0, realTwo = 0;

	if (baseIsThisType(typeNameFirst, TYPE_NAME_ARRAY)) return 0;
	if (baseIsThisType(typeNameSecond, TYPE_NAME_ARRAY)) return 0;

	if (baseIsThisType(typeNameFirst, TYPE_NAME_REAL)) realOne = getReal(first);
	else if (baseIsThisType(typeNameFirst, TYPE_NAME_COMPLEX)) realOne = getRealPart(first);

	if (baseIsThisType(typeNameSecond, TYPE_NAME_REAL)) realTwo = getReal(first);
	else if (baseIsThisType(typeNameSecond, TYPE_NAME_COMPLEX)) realTwo = getRealPart(first);

	//не забываем очищать имена
	free(typeNameFirst);
	free(typeNameSecond);

	if (realOne == realTwo && realOne == 0) return 0;

	return (realOne - realTwo > 0) ? 1 : -1;
}

/*----------------------------------------------------------------------------------------------------------------------*/

int main()
{
	setlocale(LC_ALL, "Rus");
	char* str;

	Array* array = constructArray(START_SIZE);
	
	//обьявление переменной и добавление в конец массва array
	Complex* complex = constructComplex(0, -1.0101);
	appendArray(array, complex);

	//вывод типа toStringBase
	printf("вывод типа: ");
	str = toStringBase(complex);
	printf("%s\n\n", str);
	free(str);

	// можно и так добавлять элементы в массив
	appendArray(array, constructReal(1));

	//распечатать все типы данных в массиве
	printf("распечатать все типы данных: ");
	str = toStringBase(array);
	printf("%s\n\n", str);
	free(str);

	//просто распечатать все значения printBase
	printf("просто распечатать все значения: ");
	str = printBase(array);
	printf("%s\n\n", str);
	free(str);

	//сумма двух одинаковых типов (в данном случае sum типа Real*, 
	//но может быть и другой (при например сложении с комплесным), поэтому Base*)
	printf("Пример функции суммы для одинакового типа (Real, Real): ");
	Real* firstPart = constructReal(0);
	Real* secondPart = constructReal(2);
	Base* sum = sumBase(firstPart, secondPart);
	str = printBase(sum);
	printf("%s\n", str);
	free(str);
	freeBase(firstPart);
	freeBase(secondPart);

	//функция умножения работает с полиморфными типами данных (также и функция суммы)
	printf("Пример функции умножения для полиморфного типа (Real, Complex): ");
	Complex* secondExample = constructComplex(-1.56, 1);
	Base* mult = multBase(sum, secondExample);
	str = printBase(mult);
	printf("%s\n\n", str);
	free(str);
	freeBase(secondExample);
	freeBase(mult);
	freeBase(sum);

	//применить функцию ко всем элементам массива map
	printf("применить функцию ко всем элементам массива: ");
	map(array, plusTwoToReal);

	str = printBase(array);
	printf("%s\n", str);
	free(str);

	//применить функцию ко всем элементам массива с условием where
	printf("применить функцию ко всем элементам массива с условием: ");
	where(baseIsReal, array, minusFiveToReal);

	str = dumpBase(array);
	printf("%s\n\n", str);
	free(str);

	Array* array2 = constructArray(START_SIZE);

	Complex* complex2 = constructComplex(1.0, -0.69420);
	appendArray(array2, complex2);

	Complex* complex3 = constructComplex(-500.5, 1.2);
	appendArray(array2, complex3);

	Real* real2 = constructReal(1.56);
	appendArray(array2, real2);

	//вложить массив в массив
	printf("вложить массив в массив: ");
	appendArray(array, array2);

	str = dumpBase(array);
	printf("%s\n\n", str);
	free(str);

	//map и where работают и со вложенными массивами
	printf("map и where работают и со вложенными массивами\n");
	map(array, plusTwoToReal);

	str = printBase(array);
	printf("%s\n", str);
	free(str);

	where(baseIsReal, array, minusFiveToReal);

	str = dumpBase(array);
	printf("%s\n", str);
	free(str);

	Complex* complex4 = constructComplex(0, 34);
	appendArray(array, complex4);

	//сортировка по компаратору, написанным пользователем (работает и со вложенными массивами)
	printf("\nсортировка по компаратору, написанным пользователем\n");
	printf("До: ");
	str = printBase(array);
	printf("%s\n", str);
	free(str);

	printf("После: ");
	setComparator(array, compareReal); //устанавлием определенный компаратор, иначе ошибка
	sort(array);
	str = printBase(array);
	printf("%s\n", str);
	free(str);


	printf("\nИ на последок типы во вложенных массивах: ");
	str = toStringBase(array);
	printf("%s\n", str);
	free(str);

	freeBase(array);

	return 0;
}
