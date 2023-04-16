#pragma once

#include "IContainer.h"
#include "IIterator.h"

#include "Tuple.h"
#include "SegmentedList.h"
#include "Array.h"
#include "Logger.h"

template <typename T>
using Func = T (*)(T&);

template <typename T>
using Condition = bool (*)(T&);

template <typename T>
using Reducer = T (*)(T, T);

template <typename T>
class Sequence
{
public:
    virtual ~Sequence() {}
    
    virtual T* GetFirstPointer() = 0; 
    virtual T* GetEndPointer() = 0;

    virtual IIterator<T>* _Begin() = 0;
    virtual IIterator<T>* _End() = 0;

    virtual T& GetFirst() const = 0;
    virtual T& GetLast() const = 0;
    virtual T& Get(const Index index) const = 0;

    Sequence<T>* Map(T (*func)(T&));
    Sequence<T>* Where(bool (*condition)(T&));

    virtual Sequence<T>* GetSubsequence(const Index start, const Index end);
    virtual Sequence<T>* Concat(Sequence<T>* other);
    virtual Sequence<T>* Slice(const Index index, Size size, Sequence<T>* other);

    T Reduce(T (*reducer)(T, T), T base);

    virtual Size GetLength() const noexcept = 0;

    virtual Sequence<T>* Append(const T& data) = 0;
    virtual Sequence<T>* Append(T&& data) = 0;

    virtual Sequence<T>* Prepend(const T& data) = 0;
    virtual Sequence<T>* Prepend(T&& data) = 0;

    virtual void InsertAt(const Index index, const T& data) = 0;
    virtual void Remove(const Index index) = 0;

    virtual bool isEmpty() const noexcept { return GetLength() == 0; }

    virtual Sequence<T>* Create() = 0;
    virtual Sequence<T>* Copy() = 0;

    friend std::ostream& operator<< (std::ostream& stream, Sequence<T>* sequence)
    {
        for (Index i = 0; i < sequence->GetLength(); i++)
            stream << sequence->Get(i) << " ";

        return stream;
    }
};

template <typename T>
Sequence<T>* Sequence<T>::Map(Func<T> func)
{
    if (this->isEmpty())
    {
        Logger::Trace("At GetSubsequence() at Sequence.h");
        logException(EXCEPTION_INDEX_OUT_OF_RANGE);
        throw(EXCEPTION_INDEX_OUT_OF_RANGE);
    }

    Sequence<T>* result = this->Create();

    for (Index i = 0; i < this->GetLength(); i++)
        result->Append(func(this->Get(i)));
    
    return result;
}

template <typename T>
Sequence<T>* Sequence<T>::Where(Condition<T> condition)
{
    if (this->isEmpty())
    {
        Logger::Trace("At GetSubsequence() at Sequence.h");
        logException(EXCEPTION_INDEX_OUT_OF_RANGE);
        throw(EXCEPTION_INDEX_OUT_OF_RANGE);
    }

    Sequence<T>* result = this->Create();

    for (Index i = 0; i < this->GetLength(); i++)
    {
        if (condition(this->Get(i)))
            result->Append(this->Get(i));
    }

    return result;
}

template<typename T>
Sequence<T>* Sequence<T>::GetSubsequence(const Index start, const Index end)
{
    if (this->isEmpty() || start > end || end >= this->GetLength())
    {
        Logger::Trace("At GetSubsequence() at Sequence.h");
        logException(EXCEPTION_INDEX_OUT_OF_RANGE);
        throw(EXCEPTION_INDEX_OUT_OF_RANGE);
    }

    Sequence<T>* result = this->Create();

    for (Index i = start; i <= end; i++)
        result->Append(this->Get(i));
    
    return result;
}

template <typename T>
Sequence<T>* Sequence<T>::Concat(Sequence<T>* other)
{
    if (this->isEmpty() || other->isEmpty())
    {
        Logger::Trace("At GetSubsequence() at Sequence.h");
        logException(EXCEPTION_INDEX_OUT_OF_RANGE);
        throw(EXCEPTION_INDEX_OUT_OF_RANGE);
    }

    Sequence<T>* result = this->Create();

    IIterator<T>* iter = this->_Begin();
    IIterator<T>* end = this->_End();

    for (iter; !(iter->_isEquals(end)); iter->_Next())
        result->Append(iter->_GetCurrent());

    delete iter; delete end;
    
    iter = other->_Begin();
    end = other->_End();

    for (iter; !(iter->_isEquals(end)); iter->_Next())
        result->Append(iter->_GetCurrent());
    
    delete iter; delete end;
    
    return result;
}

template <typename T>
Sequence<T>* Sequence<T>::Slice(const Index index, Size size, Sequence<T>* other)
{
    if (this->isEmpty() || index >= this->GetLength() || size > other->GetLength() || index + size > this->GetLength())
    {
        Logger::Trace("At GetSubsequence() at Sequence.h");
        logException(EXCEPTION_INDEX_OUT_OF_RANGE);
        throw(EXCEPTION_INDEX_OUT_OF_RANGE);
    }

    Sequence<T>* result = this->Copy();

    for (Index i = 0; i < size; i++)
    {
        result->Get(i + index) = other->Get(i);
    }
    
    return result;
}

template <typename T>
T Sequence<T>::Reduce(Reducer<T> reducer, T base)
{
    if (this->isEmpty())
    {
        Logger::Trace("At GetSubsequence() at Sequence.h");
        logException(EXCEPTION_INDEX_OUT_OF_RANGE);
        throw(EXCEPTION_INDEX_OUT_OF_RANGE);
    }

    for (Index i = this->GetLength() - 1; i > 0; i--)
    {
        base = reducer(this->Get(i), base);
    }
    
    base = reducer(this->GetFirst(), base);

    return base;
}
