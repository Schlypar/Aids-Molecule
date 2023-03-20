#pragma once

#include "IContainer.h"
#include "IIterator.h"

#include "Tuple.h"
#include "List.h"
#include "SegmentedList.h"
#include "Array.h"
#include "Logger.h"



// typedef Sequence<T>* (*Allocator)();

template <typename T>
class Sequence
{
public:
    
    virtual T* GetFirstPointer() = 0; 
    virtual T* GetEndPointer() = 0;

    virtual IIterator<T>* _Begin() = 0;
    virtual IIterator<T>* _End() = 0;

    virtual T& GetFirst() const = 0;
    virtual T& GetLast() const = 0;
    virtual T& Get(const Index index) const = 0;

    virtual Sequence<T>* GetSubsequence(const Index start, const Index end);
    virtual Sequence<T>* Concat(Sequence<T>* other) ;

    virtual Size GetLength() const noexcept = 0;

    virtual void Append(const T& data) = 0;
    virtual void Append(T&& data) = 0;

    virtual void Prepend(const T& data) = 0;
    virtual void Prepend(T&& data) = 0;

    virtual void InsertAt(const Index index, const T& data) = 0;

    virtual bool isEmpty() const noexcept { return GetLength() == 0; }

    virtual Sequence<T>* Copy() = 0;
};



template<typename T>
Sequence<T>* Sequence<T>::GetSubsequence(const Index start, const Index end)
{
    Sequence<T>* result = this->Copy();

    if (this->isEmpty() || start > end || end >= this->GetLength())
    {
        Logger::Trace("At GetSubsequence() at Sequence.h");
        logException(EXCEPTION_INDEX_OUT_OF_RANGE);
        return NULL;
    }

    for (Index i = start; i < end; i++)
        result->Append(this->Get(i));
    
    return result;
}

template <typename T>
Sequence<T>* Sequence<T>::Concat(Sequence<T>* other)
{
    Sequence<T>* result = this->Copy();

    if (this->isEmpty() || other->isEmpty())
    {
        Logger::Trace("At GetSubsequence() at Sequence.h");
        logException(EXCEPTION_INDEX_OUT_OF_RANGE);
        return NULL;
    }

    IIterator<T>* iter = this->_Begin();
    IIterator<T>* end = this->_End();

    for (iter; !(iter->_isEquals(end)); iter->_Next())
        result->Append(iter->_GetCurrent());
    
    delete iter; delete end;
    
    return result;
}