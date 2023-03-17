#pragma once

//#include "base.h"

typedef struct Base* (*Sum)(struct Base*, struct Base*);
typedef struct Base* (*Mult)(struct Base*, struct Base*);
typedef struct Base* (*Zero)(char*);
typedef struct Base* (*One)(char*);

typedef struct Ring {
	Sum sum;
	Mult mult;
	Zero zero;
	One one;
} Ring;

Ring* createRing();

Ring* constructRing(Sum, Mult);

//Ring* getRing(Base*);

//Base* sumBase(Base*, Base*);
//
//Base* multBase(Base*, Base*);
//
//Base* zeroBase(Base*);
//
//Base* oneBase(Base*);

void initialiseSum(Ring*, Sum);

void initialiseMult(Ring*, Mult);

void initialiseZero(Ring*, Zero);

void initialiseOne(Ring*, One);