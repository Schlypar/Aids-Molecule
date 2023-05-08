#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "base.h"
#include "complex.h"
#include "container.h"
#include "dynamicArray.h"
#include "real.h"


#define START_SIZE 2

typedef int Bool;

/*
��������� ����
*/

/*
������������ ������ (������� 8)

���������� �����, ������������ �����

map, where, concat, sort
*/

/*-------------------------------������� ������� ���� �� �������� ��������������------------------------------------------*/

void plusTwoToReal(Base* base)
{
	char* typeName = getType(base);

	if (baseIsThisType(typeName, TYPE_NAME_REAL))
		setReal(base, getReal(base) + 2);
	if (baseIsThisType(typeName, TYPE_NAME_COMPLEX))
		setComplexReal(base, getRealPart(base) + 2);
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

	if (baseIsThisType(typeNameFirst, TYPE_NAME_ARRAY))
		return 0;
	if (baseIsThisType(typeNameSecond, TYPE_NAME_ARRAY))
		return 0;

	if (baseIsThisType(typeNameFirst, TYPE_NAME_REAL))
		realOne = getReal(first);
	else if (baseIsThisType(typeNameFirst, TYPE_NAME_COMPLEX))
		realOne = getRealPart(first);

	if (baseIsThisType(typeNameSecond, TYPE_NAME_REAL))
		realTwo = getReal(first);
	else if (baseIsThisType(typeNameSecond, TYPE_NAME_COMPLEX))
		realTwo = getRealPart(first);

	//�� �������� ������� �����
	free(typeNameFirst);
	free(typeNameSecond);

	if (realOne == realTwo && realOne == 0)
		return 0;

	return (realOne - realTwo > 0) ? 1 : -1;
}

/*----------------------------------------------------------------------------------------------------------------------*/

int main()
{
	setlocale(LC_ALL, "Rus");
	char* str;

	Array* array = constructArray(START_SIZE);

	//���������� ���������� � ���������� � ����� ������ array
	Complex* complex = constructComplex(0, -1.0101);
	appendArray(array, complex);

	//����� ���� toStringBase
	printf("����� ����: ");
	str = toStringBase(complex);
	printf("%s\n\n", str);
	free(str);

	// ����� � ��� ��������� �������� � ������
	appendArray(array, constructReal(1));

	//����������� ��� ���� ������ � �������
	printf("����������� ��� ���� ������: ");
	str = toStringBase(array);
	printf("%s\n\n", str);
	free(str);

	//������ ����������� ��� �������� printBase
	printf("������ ����������� ��� ��������: ");
	str = printBase(array);
	printf("%s\n\n", str);
	free(str);

	//����� ���� ���������� ����� (� ������ ������ sum ���� Real*,
	//�� ����� ���� � ������ (��� �������� �������� � ����������), ������� Base*)
	printf("������ ������� ����� ��� ����������� ���� (Real, Real): ");
	Real* firstPart = constructReal(0);
	Real* secondPart = constructReal(2);
	Base* sum = sumBase(firstPart, secondPart);
	str = printBase(sum);
	printf("%s\n", str);
	free(str);
	freeBase(firstPart);
	freeBase(secondPart);

	//������� ��������� �������� � ������������ ������ ������ (����� � ������� �����)
	printf("������ ������� ��������� ��� ������������ ���� (Real, Complex): ");
	Complex* secondExample = constructComplex(-1.56, 1);
	Base* mult = multBase(sum, secondExample);
	str = printBase(mult);
	printf("%s\n\n", str);
	free(str);
	freeBase(secondExample);
	freeBase(mult);
	freeBase(sum);

	//��������� ������� �� ���� ��������� ������� map
	printf("��������� ������� �� ���� ��������� �������: ");
	map(array, plusTwoToReal);

	str = printBase(array);
	printf("%s\n", str);
	free(str);

	//��������� ������� �� ���� ��������� ������� � �������� where
	printf("��������� ������� �� ���� ��������� ������� � ��������: ");
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

	//������� ������ � ������
	printf("������� ������ � ������: ");
	appendArray(array, array2);

	str = dumpBase(array);
	printf("%s\n\n", str);
	free(str);

	// map � where �������� � �� ���������� ���������
	printf("map � where �������� � �� ���������� ���������\n");
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

	//���������� �� �����������, ���������� ������������� (�������� � �� ���������� ���������)
	printf("\n���������� �� �����������, ���������� �������������\n");
	printf("��: ");
	str = printBase(array);
	printf("%s\n", str);
	free(str);

	printf("�����: ");
	setComparator(array, compareReal); //����������� ������������ ����������, ����� ������
	sort(array);
	str = printBase(array);
	printf("%s\n", str);
	free(str);

	printf("\n� �� �������� ���� �� ��������� ��������: ");
	str = toStringBase(array);
	printf("%s\n", str);
	free(str);

	freeBase(array);

	return 0;
}
