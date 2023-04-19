#pragma once

#include "Allocator.h"
#include "Logger.h"

template <typename T>
class Vector
{
private:
    Array<T> vector;

public:
    Vector(Size dimension, T data)
        : vector(dimension) 
    {
        Logger::Info("Vector<T> of dimension %u and T data has been constructed", dimension);

        for (Index i = 0; i < dimension; i++)
        {
            vector[i] = data;
        }
    }

    Vector(Size dimension, T* data)
        : vector(dimension)
    {
        Logger::Info("T* data constructor of Vector<T> of dimension &u", dimension);

        for (Index i = 0; i < dimension; i++)
        {
            vector[i] = data[i];
        }
    }

    Vector(Size dimension, const Array<T>& data)
        : vector(dimension)
    {
        Logger::Info("Array<T> constructor of Vector<T> of dimension &u", dimension);

        for (Index i = 0; i < dimension; i++)
        {
            vector[i] = data[i];
        }
    }

    template <typename... Args>
    Vector(Size dimension, Args... args)
        : vector(dimension)
    {
        Logger::Info("Parameter Pack constructor of Vector<T> of dimension &u", dimension);

        if (sizeof...(args) + 1 < dimension)
            Logger::Warn("Vector wasn't filled fully with values");
        if (sizeof...(args) + 1 > dimension)
            Logger::Warn("Vector was overflowed with values and last %u will be lost", sizeof...(args) + 1 - dimension);

        T data[sizeof...(args)];
        Index k = 0;
        ((data[k++] = args), ...);

        for (Index i = 0; i < dimension; i++)
        {
            vector[i] = data[i];
        }
    }

    Vector(const Vector<T>& other)
        : vector(other.vector)
    {
         Logger::Info("Copied Vector<T>");
    }

    Vector(Vector<T>&& other)
        : vector(other.vector)
    {
         Logger::Info("Copied Vector<T>");
         other.vector = Array<T>();
    }

    ~Vector()
    {
        Logger::Info("Destroyed Vector<T> of dimension %u", vector.GetLength());
    }

    T& Get(Index i) const
    {
        return vector[i];
    }

    Size Dimension() const
    {
        return vector.GetLength();
    }

    T& operator[] (Index i) const
    {
        return vector[i];
    }

    Vector<T>& operator= (const Vector<T>& other)
    {
        Logger::Info("Used assignment operator= of Vector<T>");
        this->vector.~Array();

        vector = other.vector;
    }

    Vector<T>& operator= (Vector<T>&& other)
    {
        Logger::Info("Used moving operator= of Vector<T>");
        this->vector.Clear();

        vector = other.vector;
        other.vector = Array<T>();

        return *this;
    }

    bool operator== (const Vector<T>& other) const
    {
        if (this->Dimension() != other.Dimension())
            return false;
        

        Size dim = this->Dimension();
        for (Index i = 0; i < dim; i++)
        {
            if (vector[i] != other.vector[i])
                return false;
        }

        return true;
    }

    template <typename U>
    friend Vector<U> operator+ (const Vector<U>& left, const Vector<U>& right)
    {
        if (left.Dimension() != right.Dimension())
        {
            Logger::Error("Vectors don't have same size, behaviour is undefined");
            logException(EXCEPTION_BAD_CONTAINER);
            throw EXCEPTION_BAD_CONTAINER;
        }


        Size dim = left.Dimension();
        T data[dim];

        for (Index i = 0; i < dim; i++)
            data[i] = left[i] + right[i];

        return Vector<T>(dim, data);
    }

    template <typename U>
    friend Vector<U> operator+ (const Vector<U>& left, const U& right)
    {
        if (left.Dimension() != right.Dimension())
        {
            Logger::Error("Vectors don't have same size, behaviour is undefined");
            logException(EXCEPTION_BAD_CONTAINER);
            throw EXCEPTION_BAD_CONTAINER;
        }


        Size dim = left.Dimension();
        T data[dim];

        for (Index i = 0; i < dim; i++)
            data[i] = left[i] + right;

        return Vector<T>(dim, data);
    }

    template <typename U>
    friend Vector<U> operator+ (const U& left, const Vector<U>& right)
    {
        if (left.Dimension() != right.Dimension())
        {
            Logger::Error("Vectors don't have same size, behaviour is undefined");
            logException(EXCEPTION_BAD_CONTAINER);
            throw EXCEPTION_BAD_CONTAINER;
        }


        Size dim = left.Dimension();
        T data[dim];

        for (Index i = 0; i < dim; i++)
            data[i] = left + right[i];

        return Vector<T>(dim, data);
    }


    template <typename U>
    friend Vector<U> operator- (const Vector<U>& left, const Vector<U>& right)
    {
        if (left.Dimension() != right.Dimension())
        {
            Logger::Error("Vectors don't have same size, behaviour is undefined");
            logException(EXCEPTION_BAD_CONTAINER);
            throw EXCEPTION_BAD_CONTAINER;
        }


        Size dim = left.Dimension();
        T data[dim];

        for (Index i = 0; i < dim; i++)
            data[i] = left[i] - right[i];

        return Vector<T>(dim, data);
    }

    template <typename U>
    friend Vector<U> operator- (const Vector<U>& left, const U& right)
    {
        if (left.Dimension() != right.Dimension())
        {
            Logger::Error("Vectors don't have same size, behaviour is undefined");
            logException(EXCEPTION_BAD_CONTAINER);
            throw EXCEPTION_BAD_CONTAINER;
        }


        Size dim = left.Dimension();
        T data[dim];

        for (Index i = 0; i < dim; i++)
            data[i] = left[i] - right;

        return Vector<T>(dim, data);
    }

    template <typename U>
    friend Vector<U> operator- (const U& left, const Vector<U>& right)
    {
        if (left.Dimension() != right.Dimension())
        {
            Logger::Error("Vectors don't have same size, behaviour is undefined");
            logException(EXCEPTION_BAD_CONTAINER);
            throw EXCEPTION_BAD_CONTAINER;
        }


        Size dim = left.Dimension();
        T data[dim];

        for (Index i = 0; i < dim; i++)
            data[i] = left - right[i];

        return Vector<T>(dim, data);
    }

    template <typename U>
    friend U operator* (const Vector<U>& left, const Vector<U>& right)
    {
        if (left.Dimension() != right.Dimension())
        {
            Logger::Error("Vectors don't have same size, behaviour is undefined");
            logException(EXCEPTION_BAD_CONTAINER);
            throw EXCEPTION_BAD_CONTAINER;
        }
    
        Size dim = left.Dimension();

        Vector<U> finalSecond = ZeroVector<U>(dim);
        
        U result = U();
        
        for (Index i = 0; i < dim; i++)
        {
            result += left.Get(i) * right.Get(i);
        }

        return result;
    }

    template <typename U>
    friend Vector<U> operator* (const Vector<U>& left, const U& right)
    {
        Size dim = left.Dimension();
        T data[dim];

        for (Index i = 0; i < dim; i++)
            data[i] = left[i] * right;

        return Vector<T>(dim, data);
    }

    template <typename U>
    friend Vector<U> operator* (const U& left, const Vector<U>& right)
    {
        Size dim = right.Dimension();
        T data[dim];

        for (Index i = 0; i < dim; i++)
            data[i] = left * right[i];

        return Vector<T>(dim, data);
    }
};

template <typename U>
Vector<U> ZeroVector(Size dimension)
{
    return Vector<U>(dimension, U());
}