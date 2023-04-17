#pragma once

#include "Allocator.h"
#include "Logger.h"
#include "Vector.h"

template <typename T>
class Matrix
{
private:
    Array<Array<T>> matrix;
    Size rows;
    Size columns;

    Index makeLinearIndex(Index i, Index j) const
    {
        if (i > rows || rows <= 0 || j > columns || columns <= 0)
        {
            Logger::Trace("At Matrix<T> at makeLinearIndex(Index, Index)");
            logException(EXCEPTION_INDEX_OUT_OF_RANGE);
            throw EXCEPTION_INDEX_OUT_OF_RANGE;
        }

        return (i * columns) + j;
    }

public:
    Matrix(Size dimension, T data)
        : matrix(dimension), rows(dimension), columns(dimension) 
    {
        Logger::Info("Square Matrix<T> constructor");

        for (Index i = 0; i < rows; i++)
        {
            matrix[i] = Array<T>(columns);
            for (Index j = 0; j < columns; j++)
            {
                Set(i, j, data);
            }
        }
    }

    Matrix(Size rows, Size columns, T data)
        : matrix(rows), rows(rows), columns(columns)
    {
        Logger::Info("T data initialiser constructor of Matrix<T>");

        for (Index i = 0; i < rows; i++)
        {
            matrix[i] = Array<T>(columns);
            for (Index j = 0; j < columns; j++)
            {
                Set(i, j, data);
            }
        }
    }

    Matrix(Size rows, Size columns, T* data)
        : matrix(rows), rows(rows), columns(columns)
    {
        Logger::Info("T* data constructor of Matrix<T> with %u rows and %u columns", rows, columns);

        for (Index i = 0; i < rows; i++)
        {
            matrix[i] = Array<T>(columns);
            for (Index j = 0; j < columns; j++)
            {
                Set(i, j, data[makeLinearIndex(i, j)]);
            }
        }
    }

    Matrix(Size rows, Size columns, const Array<T>& data)
        : matrix(rows), rows(rows), columns(columns)
    {
        Logger::Info("T* data constructor of Matrix<T> with %u rows and %u columns", rows, columns);

        for (Index i = 0; i < rows; i++)
        {
            matrix[i] = Array<T>(columns);
            for (Index j = 0; j < columns; j++)
            {
                Set(i, j, data[makeLinearIndex(i, j)]);
            }
        }
    }

    template <typename... Args>
    Matrix(Size rows, Size columns, Args... args)
        : matrix(rows), rows(rows), columns(columns)
    {
        Logger::Info("Parameter Pack constructor of Matrix<T> with %u rows and %u columns", rows, columns);

        Size numberOfElements = rows * columns;

        if (sizeof...(args) < numberOfElements)
            Logger::Warn("Matrix wasn't filled fully with values");
        if (sizeof...(args) > numberOfElements)
            Logger::Warn("Matrix was overflowed with values and last %u will be lost", sizeof...(args) + 1 - numberOfElements);

        T data[sizeof...(args)];
        Index k = 0;
        ((data[k++] = args), ...);

        for (Index i = 0; i < rows; i++)
        {
            matrix[i] = Array<T>(columns);
            for (Index j = 0; j < columns; j++)
            {
                Set(i, j, data[makeLinearIndex(i, j)]);
            }
        }
    }

    Matrix(const Matrix<T>& other)
        : matrix(other.matrix), rows(other.rows), columns(other.columns)
    {
        Logger::Info("Copied Matrix<T>");
    }

    Matrix(Matrix<T>&& other)
        : matrix(std::move(other.matrix)), rows(other.rows), columns(other.columns)
    {
        Logger::Info("Moved Matrix<T>");
        other.matrix = Array<Array<T>>();
        other.rows = 0;
        other.columns = 0;
    }

    ~Matrix()
    {
        Logger::Info("Destroyed Matrix<T> with %u rows and %u columns", rows, columns);
    }


    Array<T>& Get(Index i) const 
    {
        if (i < 0 || i >= rows)
        {
            Logger::Trace("At Matrix<T> at Get(Index, Index)");
            logException(EXCEPTION_INDEX_OUT_OF_RANGE);
            throw EXCEPTION_INDEX_OUT_OF_RANGE;
        }

        return matrix[i];
    }

    T& Get(Index i, Index j) const
    {
        if (i < 0 || j < 0 || i >= rows || j >= columns)
        {
            Logger::Trace("At Matrix<T> at Get(Index, Index)");
            logException(EXCEPTION_INDEX_OUT_OF_RANGE);
            throw EXCEPTION_INDEX_OUT_OF_RANGE;
        }

        return matrix[i][j];
    }

    void Set(Index i, Index j, T value)
    {
        if (i < 0 || j < 0 || i >= rows || j >= columns)
        {
            Logger::Trace("At Matrix<T> at Get(Index, Index)");
            logException(EXCEPTION_INDEX_OUT_OF_RANGE);
            throw EXCEPTION_INDEX_OUT_OF_RANGE;
        }

        matrix[i][j] = value;
    }

    Size GetNumberOfRows() const
    {
        return rows;
    }

    Size GetNumberOfColumns() const
    {
        return columns;
    }

    Vector<T> GetRow(Index i) const
    {
        if (i < 0 || i >= rows)
        {
            Logger::Trace("At Matrix<T> at GetRow(Index)");
            logException(EXCEPTION_INDEX_OUT_OF_RANGE);
            throw EXCEPTION_INDEX_OUT_OF_RANGE;
        }

        return Vector<T>(columns, matrix[i]);
    }

    Vector<T> GetColumn(Index j) const
    {
        if (j < 0 || j >= columns)
        {
            Logger::Trace("At Matrix<T> at GetColumn(Index)");
            logException(EXCEPTION_INDEX_OUT_OF_RANGE);
            throw EXCEPTION_INDEX_OUT_OF_RANGE;
        }

        T data[columns];

        for (Index i = 0; i < rows; i++)
        {
            data[i] = matrix[i][j];
        }

        return Vector<T>(rows, data);
    }

    Matrix<T> Transpose() const
    {
        T data[rows * columns];

        for (Index i = 0; i < rows; i++)
        {
            for (Index j = 0; j < columns; j++)
            {
                data[makeLinearIndex(i, j)] = this->Get(i, j);
            }
        }

        return Matrix<T>(columns, rows, data);
    }

    Matrix<T>& operator= (const Matrix<T>& other)
    {
        Logger::Info("Used assignment operator= of Matrix<T>");

        this->matrix = other.matrix;
        this->rows = other.rows;
        this->columns = other.columns;

        return *this;
    }

    Matrix<T>& operator= (Matrix<T>&& other)
    {
        Logger::Info("Used moving operator= of Matrix<T>");
        
        this->matrix = std::move(other.matrix);
        this->rows = other.rows;
        this->columns = other.columns;

        other.matrix = Array<Array<T>>();
        other.rows = 0;
        other.columns = 0;

        return *this;
    }

    Array<T>& operator[] (Index i) const
    {
        return this->Get(i);
    }

    bool operator== (const Matrix<T>& other) const
    {
        if (this->rows != other.rows || this->columns != other.columns)
            return false;
        
        for (Index i = 0; i < this->rows; i++)
        {
            if (matrix[i] != other.matrix[i])
                return false;
        }

        return true;
    }

    bool operator!= (const Matrix<T>& other) const
    {
        return !(*this == other);
    }

    template <typename U>
    friend Matrix<U> operator+ (const Matrix<U>& left, const Matrix<U>& right)
    {
        if (left.rows != right.rows || left.columns != right.columns)
        {
            Logger::Error("Matrices don't have same size, behaviour is undefined");
            logException(EXCEPTION_BAD_CONTAINER);
            throw EXCEPTION_BAD_CONTAINER;
        }

        Size rows = std::min(left.GetNumberOfRows(), right.GetNumberOfRows());
        Size columns = std::min(left.GetNumberOfColumns(), right.GetNumberOfColumns());
        Size linearSize = rows * columns;

        T data[linearSize];

        for (Index i = 0; i < rows; i++)
        {
            for (Index j = 0; j < columns; j++)
            {
                data[left.makeLinearIndex(i, j)] = left.Get(i,j) + right.Get(i,j);
            }
        }
        
        return Matrix<T>(rows, columns, data);
    }

    template <typename U>
    friend Matrix<U> operator+ (const U& left, const Matrix<U>& right)
    {
        Size rows = right.rows;
        Size columns = right.columns;
        Size linearSize = rows * columns;

        T data[linearSize];

        for (Index i = 0; i < rows; i++)
        {
            for (Index j = 0; j < columns; j++)
            {
                data[right.makeLinearIndex(i, j)] = left + right.Get(i,j);
            }
        }
        
        return Matrix<T>(rows, columns, data);
    }

    template <typename U>
    friend Matrix<U> operator+ (const Matrix<U>& left, const U& right)
    {
        Size rows = left.rows;
        Size columns = left.columns;
        Size linearSize = rows * columns;

        T data[linearSize];

        for (Index i = 0; i < rows; i++)
        {
            for (Index j = 0; j < columns; j++)
            {
                data[left.makeLinearIndex(i, j)] = left.Get(i,j) + right;
            }
        }
        
        return Matrix<T>(rows, columns, data);
    }


    template <typename U>
    friend Matrix<U> operator- (const Matrix<U>& left, const Matrix<U>& right)
    {
        if (left.rows != right.rows || left.columns != right.columns)
        {
            Logger::Error("Matrices don't have same size, behaviour is undefined");
            logException(EXCEPTION_BAD_CONTAINER);
            throw EXCEPTION_BAD_CONTAINER;
        }

        Size rows = std::min(left.GetNumberOfRows(), right.GetNumberOfRows());
        Size columns = std::min(left.GetNumberOfColumns(), right.GetNumberOfColumns());
        Size linearSize = rows * columns;

        T data[linearSize];

        for (Index i = 0; i < rows; i++)
        {
            for (Index j = 0; j < columns; j++)
            {
                data[left.makeLinearIndex(i, j)] = left.Get(i,j) - right.Get(i,j);
            }
        }
        
        return Matrix<T>(rows, columns, data);
    }

    template <typename U>
    friend Matrix<U> operator- (const U& left, const Matrix<U>& right)
    {
        Size rows = right.rows;
        Size columns = right.columns;
        Size linearSize = rows * columns;

        T data[linearSize];

        for (Index i = 0; i < rows; i++)
        {
            for (Index j = 0; j < columns; j++)
            {
                data[right.makeLinearIndex(i, j)] = left - right.Get(i,j);
            }
        }
        
        return Matrix<T>(rows, columns, data);
    }

    template <typename U>
    friend Matrix<U> operator- (const Matrix<U>& left, const U& right)
    {
        Size rows = left.rows;
        Size columns = left.columns;
        Size linearSize = rows * columns;

        T data[linearSize];

        for (Index i = 0; i < rows; i++)
        {
            for (Index j = 0; j < columns; j++)
            {
                data[left.makeLinearIndex(i, j)] = left.Get(i,j) - right;
            }
        }
        
        return Matrix<T>(rows, columns, data);
    }

    template <typename U>
    friend Matrix<U> operator* (const Matrix<U>& left, const Matrix<U>& right)
    {
        if (left.columns != right.rows)
        {
            Logger::Trace("At Matrix<T> operator* overload");
            logException(EXCEPTION_BAD_CONTAINER);
            throw EXCEPTION_BAD_CONTAINER;
        }

        Matrix<T> result = Matrix<T>(left.rows, right.columns, T());

        for (Index i = 0; i < left.rows; i++)
        {
            Vector<T> row = std::move(left.GetRow(i));
            for (Index j = 0; j < right.columns; j++)
            {
                Vector<T> column = right.GetColumn(j);
                T value = row * column;
                result.Set(i, j, value);
            }
        }

        return result;
    }

    // template <typename U>
    // friend Matrix<U> operator* (const U& left, const Matrix<U>& right); 

    // template <typename U>
    // friend Matrix<U> operator* (const Matrix<U>& left, const U& right);

    friend std::ostream& operator<< (std::ostream& stream, Matrix<T>& matrix)
    {
        for (Index i = 0; i < matrix.rows; i++)
        {
            for (Index j = 0; j < matrix.columns -1 ; j++)
            {
                stream << matrix[i][j] << ", ";
            }

            stream << matrix[i][matrix.columns - 1] << "\n";
        }

        return stream;
    }
};


template <typename U>
Matrix<U> ZeroMatrix(Size dimension)
{
    return Matrix<U>(dimension, U());
}

template <typename U>
Matrix<U> IdentityMatrix(Size dimension)
{
    Matrix<U> result = Matrix<U>(dimension, U());

    for (Index i = 0; i < dimension; i++)
        result[i][i] = U(1);
    
    return result;
}