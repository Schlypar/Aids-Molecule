#pragma once

#include "ring.h"

#include"base.h"

#define TYPE_NAME_COMPLEX "Complex"
#define BASE_LENGTH_OF_COMPLEX_WORD 30
#define BASE_LENGTH_OF_ONE_PART 15

typedef struct Complex {
	Base base;
	double real;
	double imaginary;
} Complex;

Complex* createComplex();

Complex* constructComplex(double, double);

void freeComplex(Complex*);

double getRealPart(Complex*);

double getImaginaryPart(Complex*);

Complex* copyComplex(Complex*);

char* toStringComplex(Complex*);

char* printComplex(Complex*);

char* dumpComplex(Complex*);

void setComplexReal(Complex*, double);

void setComplexImaginary(Complex*, double);

Complex* sumComplex(Complex*, Complex*);

Complex* multComplex(Complex*, Complex*);