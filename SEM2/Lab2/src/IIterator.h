#pragma once

template <typename T>
class IIterator
{
public:
    // IIterator(IIterator<T>* other);

    virtual IIterator<T>* _Next() {}
    virtual IIterator<T>* _Prev() {}

    virtual T& _GetCurrent() {}
    virtual T* _GetPointer() {}
    // virtual void SetCurrent(const T& data) = 0;

    virtual bool _isEquals(IIterator<T>* other) {}

    // virtual T& operator* () {}

    // virtual ~IIterator() = 0;
};