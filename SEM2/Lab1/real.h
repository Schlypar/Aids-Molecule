#pragma once

#include "ring.h"

#include "base.h"

#define TYPE_NAME_REAL "Real"
#define BASE_LENGTH_OF_REAL_WORD 15


typedef struct Real {
	Base base;
	double real;
} Real;

Real* createReal();

Real* constructReal(double);

void freeReal(Real*);

double getReal(Real*);

Real* copyReal(Real*);

char* toStringReal(Real*);

char* printReal(Real*);

char* dumpReal(Real*);

void setReal(Real*, double);

Real* sumReal(Real*, Real*);

Real* multReal(Real*, Real*);