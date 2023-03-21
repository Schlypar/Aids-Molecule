#pragma once

#include "Logger.h"

template <typename T>
class IIterator
{
public:
    virtual IIterator<T>* _Next() {}
    virtual IIterator<T>* _Prev() {}

    virtual T& _GetCurrent() {}
    virtual T* _GetPointer() {}

    virtual bool _isEquals(IIterator<T>* other) {}
};