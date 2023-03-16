#pragma once

#include <iostream>
#include <memory>
#include <stddef.h>

#include "Logger.h"

typedef size_t Size;
typedef size_t Index;

template <typename T> 
class IContainer
{
	virtual T Get(const Index index) const = 0;
	virtual Size GetLength() const = 0;

	virtual bool isEmpty() const = 0;
};
