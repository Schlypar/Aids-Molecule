#pragma once

#include "Logger.h"

template <typename T>
class IIterator
{
public:
    virtual IIterator<T>* _Next() { return NULL; }
    virtual IIterator<T>* _Prev() { return NULL; }

    virtual T& _GetCurrent() = 0;
    virtual T* _GetPointer() { return NULL; }

    virtual bool _isEquals(IIterator<T>* other) { return true; }
};