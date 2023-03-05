#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "complex.h"
#include "base.h"

Complex* createComplex()
{
	return (Complex*)malloc(sizeof(Complex));
}

Complex* constructComplex(double real, double imaginary)
{
	Complex* complex = createComplex();
	Base* complexBase = (Base*)complex;

	complexBase->typeName = TYPE_NAME_COMPLEX;
	complexBase->ring = constructRing(sumComplex, multComplex);

	setComplexReal(complex, real);
	setComplexImaginary(complex, imaginary);

	initialiseCopy(complex, &copyComplex);
	initialiseFree(complex, &freeComplex);
	initialiseToString(complex, toStringComplex);
	initialisePrint(complex, printComplex);
	initialiseDump(complex, dumpComplex);

	return (Base*)complex;
}

void freeComplex(Base* complex)
{
	free((Base*)complex->ring);
	free(complex);
}

void setComplexReal(Complex* complex, double real)
{
	complex->real = real;
}

void setComplexImaginary(Complex* complex, double imaginary)
{
	complex->imaginary = imaginary;
}

double getRealPart(Complex* complex)
{
	return complex->real;
}

double getImaginaryPart(Complex* complex)
{
	return complex->imaginary;
}

Complex* copyComplex(Complex *complex)
{
	double realPart = getRealPart(complex);
	double imaginaryPart = getImaginaryPart(complex);
	Complex* copy = constructComplex(realPart, imaginaryPart);
	return copy;
}

char* toStringComplex(Complex* complex)
{
	char* typeName = getType(complex);
	return typeName;
}

char* printComplex(Complex* complex)
{
	double realPart = getRealPart(complex);
	double imaginaryPart = getImaginaryPart(complex);

	char* realNumber = calloc(BASE_LENGTH_OF_ONE_PART, 1);
	char* imaginaryNumber = calloc(BASE_LENGTH_OF_ONE_PART, 1);

	char space[] = " ";
	char plus[] = " + ";
	char i[] = "i";

	int overallLengthWithEndAndSpaces = 2 * BASE_LENGTH_OF_COMPLEX_WORD + 1;

	char* result = calloc(overallLengthWithEndAndSpaces, sizeof(char));

	if (snprintf(realNumber, BASE_LENGTH_OF_ONE_PART, "%lf", realPart) == EOF)
	{
		return NULL;
	}

	if (snprintf(imaginaryNumber, BASE_LENGTH_OF_ONE_PART, "%lf", imaginaryPart) == EOF)
	{
		return NULL;
	}

	if (imaginaryPart >= 0)
	{
		strcat(result, realNumber);
		strcat(result, plus);
		strcat(result, imaginaryNumber);
		strcat(result, i);
	}

	if (imaginaryPart < 0)
	{
		strcat(result, realNumber);
		strcat(result, space);
		strcat(result, imaginaryNumber);
		strcat(result, i);
	}

	free(realNumber);
	free(imaginaryNumber);

	return result;
}

char* dumpComplex(Complex* complex)
{
	char* typeName = getType(complex);
	char* number = printComplex(complex);
	char space[] = " ";

	int lengthWithEndAndSpace = strlen(typeName) + strlen(number) + 2;

	char* result = calloc(lengthWithEndAndSpace, 1);
	
	strcat(result, typeName);
	strcat(result, space);
	strcat(result, number);

	free(typeName);
	free(number);

	return result;
}

Complex* sumComplex(Complex* first, Complex* second)
{
	double real = getRealPart(first) + getRealPart(second);
	double imaginary = getImaginaryPart(first) + getImaginaryPart(second);
	return constructComplex(real, imaginary);
}

Complex* multComplex(Complex* first, Complex* second)
{
	double real = getRealPart(first) * getRealPart(second) - getImaginaryPart(first) * getImaginaryPart(second);
	double imaginary = getRealPart(first) * getImaginaryPart(second) + getImaginaryPart(first) * getRealPart(second);

	return constructComplex(real, imaginary);
}