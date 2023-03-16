#pragma once

#include "IContainer.h"
#include "Logger.h"
#include "Tuple.h"

#define CAPACITY_TO_REAL_SIZE 2

template <typename T>
class Array : IContainer<T>
{
private:
    Size size;
    Size capacity;
    T* data;
public:

    class Iterator
    {
        T* current;
    public:
        
        Iterator(T* data)
            : current(data) {}

        Iterator(const Array<T>& other)
            : current(other.data) {}
        
        Iterator operator+ (int n) 
        {
            return Iterator(current + n);
        }

        Iterator operator- (int n) 
        {
            return Iterator(current - n);
        }

        Iterator& operator++ () { current++; return *this; }

        Iterator operator++ (int)
        {
            Iterator iter = *this;
            ++(*this);
            return iter;
        }

        Iterator& operator-- () { current--; return *this; }

        Iterator operator-- (int)
        {
            Iterator iter = *this;
            --(*this);
            return iter;
        }

        bool operator!= (const Iterator& other) const { return this->current != other.current; }
		bool operator== (const Iterator& other) const { return this->current == other.current; }

        T& operator* () { return *(current); }
    };
    Iterator begin() { return Iterator(data); }
    Iterator end() { return Iterator(data + size); }

    Array()
        : size(0), capacity(0), data(nullptr) {}

    Array(T* other, Size count)
        : size(count), capacity(count), data(new T[count])
    {
        std::copy(other, other + count, data);
    }

    Array(Size size)
        : size(size), capacity(size), data(size ? new T[size]() : nullptr) {}

    //copying constructor
    Array(const Array<T>& other)
        : size(other.size)
    {
        Realloc(other.size);

        for (Index i = 0; i < other.GetLength(); i++)
            data[i] = other.data[i];
    }

    //moving constructor
    Array(Array<T>&& other)
        : size(other.size), capacity(other.capacity), data(other.data) {}

    ~Array()
    {
        Logger::Info("Destroyed Array<T>");
        if(data)
            delete[] data;
    }

    void Clear() noexcept 
    {
        if (isEmpty()) return;

        for (Index i = 0; i < size; ++i) 
            data[i].~T();
    }

    void Realloc(Size newCapacity)
    {
        if (newCapacity < 0)
        {
            Logger::Trace("At Realloc() at Array.h");
            logException(EXCEPTION_INDEX_OUT_OF_RANGE);
            return;
        }

        size = std::min(size, newCapacity);
        T* newBlock = new T[newCapacity];

        for (Index i = 0; i < size; i++)
            newBlock[i] = std::move(data[i]);

        capacity = newCapacity;
        
        delete[] data;

        data = newBlock;
    }

    void Set(const Index index, T data)
    {
        if (isEmpty() || index < 0 || index >= size)
        {
            Logger::Trace("At Set() at Array.h");
            logException(EXCEPTION_INDEX_OUT_OF_RANGE);
            return;
        }

        data[index] = data;
    }

    T Get(const Index index) const override
    {
        if (index < 0 || index >= size)
        {
            Logger::Trace("At Get() at Array.h");
            logException(EXCEPTION_INDEX_OUT_OF_RANGE);
            return data[0];
        }
        
        return data[index];
    }
    

    Size GetLength() const noexcept override { return size; }

    bool isEmpty() const noexcept override { return (size == 0 || !data); }

    T& operator[] (const Index index) { return data[index]; }

    Array<T>& operator= (const Array<T>& other)
    {
        Logger::Info("Used copying operator = of Array<T>");
        Clear();

        for (Index i = 0; i < other.GetLength(); i++)
            data[i] = other.Get(i);
        
        size = other.size;
        capacity = other.capacity;

        return *this;
    }

    Array<T>& operator= (Array<T>&& other)
    {
        Logger::Info("Used moving operator = of Array<T>");
        
        data = other.data;
        size = other.size;
        capacity = other.capacity;

        other.data = nullptr;

        return *this;
    }

    friend std::ostream& operator<< (std::ostream& stream, Array<T>& array)
    {
        if (array.isEmpty())
        {
            Logger::Trace("At operator<< overload at Array.h");
            logException(EXCEPTION_INDEX_OUT_OF_RANGE);
            return stream;
        }

        stream << "[ ";
        for (Index i = 0; i < array.GetLength(); i++)
            stream << array[i] << " ";
        stream << "]";

        return stream;
    }
};