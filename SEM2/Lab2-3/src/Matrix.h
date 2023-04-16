#pragma once

#include "Allocator.h"

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

    // vector multiplication, Vector is just one case of Matrix
    template <typename U>
    friend U DotProduct(const Matrix<U>& first, const Matrix<U>& second)
    {
        Logger::Debug("Dot Product has started");
        if (first.columns != second.rows)
        {
            if (first.columns != second.columns)
            {
                Logger::Trace("At Matrix<T> at DotProduct(const Matrix<T>&)");
                logException(EXCEPTION_BAD_CONTAINER);
                throw EXCEPTION_BAD_CONTAINER;
            }
        }

        Matrix<U> finalSecond;
        if (first.columns == second.rows)
        {
            Logger::Debug("Here possibly");
            finalSecond = second;
        }
        else
        {
            Logger::Warn("Transoposing Matrix in Dot Product. Check the order");
            finalSecond = second.Transpose();
        }

        U result;
        result = 0;
        for (Index i = 0; i < first.columns; i++)
        {
            result += first.Get(0, i) * finalSecond.Get(i, 0);
        }

        Logger::Debug("Dot Product has finished");
        return result;
    }

public:
    Matrix()
        : matrix(2), rows(2), columns(2)
    {
        Logger::Info("Default constructor of Matrix<T> with %u rows and %u columns", rows, columns);

        matrix[0] = { 1,0 };
        matrix[1] = { 0,1 };
    }

    Matrix(Size rows, Size columns)
        : matrix(rows), rows(rows), columns(columns)
    {
        Logger::Info("Allocated Matrix<T> with %u rows and %u columns", rows, columns);

        T data[columns] = {0};

        for (Index i = 0; i < rows; i++)
        {
            matrix[i] = Array<T>(data, columns);
            matrix[i][i] = 1.0;
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
    Matrix(Size rows, Size columns, T head, Args... args)
        : matrix(rows), rows(rows), columns(columns)
    {
        Logger::Info("Parameter Pack constructor of Matrix<T> with %u rows and %u columns", rows, columns);

        Size numberOfElements = rows * columns;

        if (sizeof...(args) + 1 < numberOfElements)
            Logger::Warn("Matrix wasn't filled fully with values");
        if (sizeof...(args) + 1 > numberOfElements)
            Logger::Warn("Matrix was overflowed with values and last %u will be lost", sizeof...(args) + 1 - numberOfElements);

        Array<T> tempArray{head, args...};

        for (Index i = 0; i < rows; i++)
        {
            matrix[i] = Array<T>(columns);
            for (Index j = 0; j < columns; j++)
            {
                Set(i, j, tempArray[makeLinearIndex(i, j)]);
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

    Matrix<T> GetRow(Index i) const
    {
        if (i < 0 || i >= rows)
        {
            Logger::Trace("At Matrix<T> at GetRow(Index)");
            logException(EXCEPTION_INDEX_OUT_OF_RANGE);
            throw EXCEPTION_INDEX_OUT_OF_RANGE;
        }

        return Matrix<T>(1, columns, matrix[i]);
    }

    Matrix<T> GetColumn(Index j) const
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

        return Matrix<T>(rows, 1, data);
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

        // Logger::Debug("Here possibly");
        // matrix.~Array();
        // Logger::Debug("Here possibly");

        Array<Array<T>> copy = Array<Array<T>>(other.rows);

        for (Index i = 0; i < other.rows; i++)
        {
            copy[i] = Array<T>(other.matrix[i]);
        }
        
        this->matrix = std::move(copy);
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

        other.matrix = Array<T>();

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

    bool operator== (const Matrix<T>* other) const
    {
        if (this->rows != other->rows || this->columns != other->columns)
            return false;
        
        for (Index i = 0; i < this->rows; i++)
        {
            if (matrix[i] != other.matrix[i])
                return false;
        }

        return true;
    }

    template <typename U>
    friend Matrix<U> operator+ (const Matrix<U>& left, const Matrix<U>& right)
    {
        if (left.rows != right.rows || left.columns != right.columns)
        {
            Logger::Warn("Matrices don't have same size, addition is undefined");
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
            Logger::Warn("Matrices don't have same size, addition is undefined");
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

        Matrix<T> result = Matrix<T>(left.rows, right.columns);

        for (Index i = 0; i < left.rows; i++)
        {
            for (Index j = 0; j < right.columns; j++)
            {
                Matrix<T> row = std::move(left.GetRow(i));
                Matrix<T> column = std::move(right.GetColumn(j));
                T value = DotProduct(row, column);
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
            stream << "|";
            for (Index j = 0; j < matrix.columns -1 ; j++)
            {
                stream << matrix[i][j] << ",\t";
            }

            stream << matrix[i][matrix.columns - 1] << "|\n";
        }

        return stream;
    }
};