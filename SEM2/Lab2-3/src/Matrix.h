#pragma once

#include "Allocator.h"
#include "Logger.h"
#include "Vector.h"
#include <cmath>

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
    friend Matrix<U> operator+ (const Matrix<U>& left, const Matrix<U>& right);

    template <typename U>
    friend Matrix<U> operator+ (const U& left, const Matrix<U>& right);

    template <typename U>
    friend Matrix<U> operator+ (const Matrix<U>& left, const U& right);

    template <typename U>
    friend Matrix<U> operator- (const Matrix<U>& left, const Matrix<U>& right);

    template <typename U>
    friend Matrix<U> operator- (const U& left, const Matrix<U>& right);

    template <typename U>
    friend Matrix<U> operator- (const Matrix<U>& left, const U& right);

    template <typename U>
    friend Matrix<U> operator* (const Matrix<U>& left, const Matrix<U>& right);

    template <typename U>
    friend Matrix<U> operator* (const U& left, const Matrix<U>& right);

    template <typename U>
    friend Matrix<U> operator* (const Matrix<U>& left, const U& right);

    Matrix<T>& RowsLinearCombination(const T& multiplier, Index which, Index other)
    {
        if (which < 0 || other < 0 || which >= rows || other >= rows)
        {
            Logger::Trace("At RowsLinearCombination(const T&, Index, Index) at MAtrix<T>");
            logException(EXCEPTION_INDEX_OUT_OF_RANGE);
            throw EXCEPTION_INDEX_OUT_OF_RANGE;
        }

        Vector<T> otherRow = GetRow(other);

        otherRow = otherRow * multiplier;

        for (Index j = 0; j < columns; j++)
        {
            matrix[which][j] += otherRow[j];
        }

        return *this;
    }

    Matrix<T> RowsLinearCombination(const T& multiplier, Index which, Index other) const
    {
        Matrix<T> result = Matrix<T>(*this);

        return result.RowsLinearCombination(multiplier, which, other);
    }

    Matrix<T> ColumnsLinearCombination(const T& multiplier, Index which, Index other)
    {
        if (which < 0 || other < 0 || which >= rows || other >= rows)
        {
            Logger::Trace("At RowsLinearCombination(const T&, Index, Index) at MAtrix<T>");
            logException(EXCEPTION_INDEX_OUT_OF_RANGE);
            throw EXCEPTION_INDEX_OUT_OF_RANGE;
        }

        Vector<T> otherColumn = GetColumn(other);

        otherColumn = multiplier * otherColumn;

        for (Index i = 0; i < columns; i++)
        {
            matrix[i][which] += otherColumn[i];
        }

        return *this;
    }

    Matrix<T> ColumnsLinearCombination(const T& multiplier, Index which, Index other) const
    {
        Matrix<T> result = Matrix<T>(*this);

        return result.ColumnsLinearCombination(multiplier, which, other);
    }

    T OneNorm()
    {
        T maxAbsRowSum = T();
        T currentRowSum = T();

        for (Index i = 0; i < rows; i++)
        {
            for (Index j = 0; j < columns; j++)
            {
                currentRowSum += std::abs(this->Get(i, j));
            }

            if (currentRowSum > maxAbsRowSum)
                maxAbsRowSum = currentRowSum;
            
            currentRowSum = T();
        }

        return maxAbsRowSum;
    }

    T InfNorm()
    {
        T maxAbsColumnSum = T();
        T currentColumnSum = T();

        for (Index j = 0; j < columns; j++)
        {
            for (Index i = 0; i < rows; i++)
            {
                currentColumnSum += std::abs(this->Get(i, j));
            }

            if (currentColumnSum > maxAbsColumnSum)
                maxAbsColumnSum = currentColumnSum;
            
            currentColumnSum = T();
        }

        return maxAbsColumnSum;
    }

    template <typename ReturnType>
    ReturnType EuclideanNorm()
    {
        T sum = T();
        for (Index i = 0; i < rows; i++)
        {
            for (Index j = 0; j < columns; j++)
            {
                sum += this->Get(i, j) * this->Get(i, j);
            }
        }

        return ReturnType(std::sqrt(sum));
    }

    Matrix<T>& SwapRows(Index first, Index second)
    {
        if (first < 0 || second < 0 || first >= rows || second >= rows)
        {
            Logger::Trace("At SwapRows(Index, Index) at MAtrix<T>");
            logException(EXCEPTION_BAD_CONTAINER);
            throw EXCEPTION_BAD_CONTAINER;
        }

        for (Index j = 0; j < columns; j++)
            std::swap(matrix[first][j], matrix[second][j]);

        return *this;
    }

    Matrix<T> SwapRows(Index first, Index second) const
    {
        Matrix<T> result = Matrix<T>(*this);

        return result.SwapRows(first, second);
    }

    Matrix<T>& AddRow(const Vector<T>& row)
    {
        if (this->columns != row.Dimension())
        {
            Logger::Trace("At AddRow(const Vector<T>&) at MAtrix<T>");
            logException(EXCEPTION_BAD_CONTAINER);
            throw EXCEPTION_BAD_CONTAINER;
        }

        matrix.Realloc(matrix.GetLength() + 1);
        matrix[matrix.GetLength() - 1] = Array<T>(row.GetContainer());

        rows = rows + 1;

        return *this;
    }

    Matrix<T> AddRow(const Vector<T>& row) const
    {
        Matrix<T> result = Matrix<T>(*this);

        return result.AddRow(row);
    }

    Matrix<T>& AddColumn(const Vector<T>& column)
    {
        if (this->rows != column.Dimension())
        {
            Logger::Trace("At AddRow(const Vector<T>&) at MAtrix<T>");
            logException(EXCEPTION_BAD_CONTAINER);
            throw EXCEPTION_BAD_CONTAINER;
        }

        for (Index i = 0; i < rows; i++)
        {
            Array<T> newColumn = Array<T>(matrix[i]);
            newColumn.Realloc(newColumn.GetLength() + 1);
            newColumn[newColumn.GetLength() - 1] = column[i];
            matrix[i] = newColumn;
        }

        columns = columns + 1;

        return *this;
    }

    Matrix<T> AddColumn(const Vector<T>& column) const
    {
        Matrix<T> result = Matrix<T>(*this);

        return result.AddColumn(column);
    }

    Matrix<T>& AddMatrix(const Matrix<T>& other)
    {
        if (this->rows != other.rows)
        {
            Logger::Trace("At AddMatrix(const Matrix<T>&) at MAtrix<T>");
            logException(EXCEPTION_INDEX_OUT_OF_RANGE);
            throw EXCEPTION_INDEX_OUT_OF_RANGE;
        }

        for (Index i = 0; i < rows; i++)
        {
            Array<T> current = matrix[i];
            current.Realloc(current.GetLength() + other.columns);

            for (Index j = 0; j < other.columns; j++)
            {
                current[j + columns] = other[i][j];
            }
            matrix[i] = current;
        }

        columns += other.columns;

        return *this;
    }

    Matrix<T> AddMatrix(const Matrix<T>& other) const
    {
        Matrix<T> result = Matrix<T>(*this);
        
        return result.AddMatrix(other);
    }

    Matrix<T> FirstPartOfMatrix(Size columnsToFirst)
    {
        Matrix<T> first = Matrix<T>(rows, columnsToFirst, T());

        for (Index i = 0; i < columnsToFirst; i++)
        {
            Vector<T> currentRow = this->GetRow(i);

            for (Index j = 0; j < columnsToFirst; j++)
                first[i][j] = currentRow[j];
        }

        return first;
    }

    Matrix<T> SecondPartOfMatrix(Size columnsToFirst)
    {
        Matrix<T> second = Matrix<T>(rows, columns - columnsToFirst, T());

        for (Index i = 0; i < columnsToFirst; i++)
        {
            Vector<T> currentRow = this->GetRow(i);

            for (Index j = 0; j < columns - columnsToFirst; j++)
                second[i][j] = currentRow[j + columnsToFirst];
        }

        return second;
    }

    template <typename ReturnType>
    Matrix<ReturnType> GaussMethod()
    {
        Matrix<ReturnType> result = Matrix<ReturnType>(rows, columns, ReturnType());

        for (Index i = 0; i < rows; i++)
        {
            for (Index j = 0;j < columns; j++)
            {
                result[i][j] = ReturnType(this->Get(i, j));
            }
        }

        for (Index i = 0; i < rows; i++)
        {
            for (Index k = i + 1; k < rows; k++)
            {
                ReturnType ratio = ReturnType(this->Get(k, i)) / ReturnType(this->Get(i, i));
                result.RowsLinearCombination(-ratio, k, i);
            }
        }

        for (Index i = 0; i < rows; i++)
        {
            ReturnType diagonalValue = result[i][i];

            if (diagonalValue == ReturnType())
                continue;
            
            for (Index j = 0; j < columns; j++)
            {
                result[i][j] = ReturnType(this->Get(i, j)) / diagonalValue;
            }
        }

        return result;
    }

    // template <typename ReturnType>
    // Matrix<ReturnType> InverseGauss()
    // {

    // }

    friend std::ostream& operator<< (std::ostream& stream, Matrix<T>& matrix)
    {
        for (Index i = 0; i < matrix.rows; i++)
        {
            for (Index j = 0; j < matrix.columns - 1 ; j++)
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

template <typename U>
Matrix<U> operator+ (const Matrix<U>& left, const Matrix<U>& right)
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

    U data[linearSize];

    for (Index i = 0; i < rows; i++)
    {
        for (Index j = 0; j < columns; j++)
        {
            data[left.makeLinearIndex(i, j)] = left.Get(i,j) + right.Get(i,j);
        }
    }
    
    return Matrix<U>(rows, columns, data);
}

template <typename U>
Matrix<U> operator+ (const U& left, const Matrix<U>& right)
{
    Size rows = right.rows;
    Size columns = right.columns;
    Size linearSize = rows * columns;

    U data[linearSize];

    for (Index i = 0; i < rows; i++)
    {
        for (Index j = 0; j < columns; j++)
        {
            data[right.makeLinearIndex(i, j)] = left + right.Get(i,j);
        }
    }
    
    return Matrix<U>(rows, columns, data);
}

template <typename U>
Matrix<U> operator+ (const Matrix<U>& left, const U& right)
{
    Size rows = left.rows;
    Size columns = left.columns;
    Size linearSize = rows * columns;

    U data[linearSize];

    for (Index i = 0; i < rows; i++)
    {
        for (Index j = 0; j < columns; j++)
        {
            data[left.makeLinearIndex(i, j)] = left.Get(i,j) + right;
        }
    }
    
    return Matrix<U>(rows, columns, data);
}


template <typename U>
Matrix<U> operator- (const Matrix<U>& left, const Matrix<U>& right)
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

    U data[linearSize];

    for (Index i = 0; i < rows; i++)
    {
        for (Index j = 0; j < columns; j++)
        {
            data[left.makeLinearIndex(i, j)] = left.Get(i,j) - right.Get(i,j);
        }
    }
    
    return Matrix<U>(rows, columns, data);
}

template <typename U>
Matrix<U> operator- (const U& left, const Matrix<U>& right)
{
    Size rows = right.rows;
    Size columns = right.columns;
    Size linearSize = rows * columns;

    U data[linearSize];

    for (Index i = 0; i < rows; i++)
    {
        for (Index j = 0; j < columns; j++)
        {
            data[right.makeLinearIndex(i, j)] = left - right.Get(i,j);
        }
    }
    
    return Matrix<U>(rows, columns, data);
}

template <typename U>
Matrix<U> operator- (const Matrix<U>& left, const U& right)
{
    Size rows = left.rows;
    Size columns = left.columns;
    Size linearSize = rows * columns;

    U data[linearSize];

    for (Index i = 0; i < rows; i++)
    {
        for (Index j = 0; j < columns; j++)
        {
            data[left.makeLinearIndex(i, j)] = left.Get(i,j) - right;
        }
    }
    
    return Matrix<U>(rows, columns, data);
}


template <typename U>
Matrix<U> operator* (const Matrix<U>& left, const Matrix<U>& right)
{
    if (left.columns != right.rows)
    {
        Logger::Trace("At Matrix<T> operator* overload");
        logException(EXCEPTION_BAD_CONTAINER);
        throw EXCEPTION_BAD_CONTAINER;
    }

    Matrix<U> result = Matrix<U>(left.rows, right.columns, U());

    for (Index i = 0; i < left.rows; i++)
    {
        Vector<U> row = std::move(left.GetRow(i));
        for (Index j = 0; j < right.columns; j++)
        {
            Vector<U> column = right.GetColumn(j);
            U value = row * column;
            result.Set(i, j, value);
        }
    }

    return result;
}

template <typename U>
Matrix<U> operator* (const U& left, const Matrix<U>& right)
{
    Size rows = right.rows;
    Size columns = right.columns;
    Size linearSize = rows * columns;

    U data[linearSize];

    for (Index i = 0; i < rows; i++)
    {
        for (Index j = 0; j < columns; j++)
        {
            data[right.makeLinearIndex(i, j)] = left * right.Get(i,j);
        }
    }
    
    return Matrix<U>(rows, columns, data);
}

template <typename U>
Matrix<U> operator* (const Matrix<U>& left, const U& right)
{
    Size rows = left.rows;
    Size columns = left.columns;
    Size linearSize = rows * columns;

    U data[linearSize];

    for (Index i = 0; i < rows; i++)
    {
        for (Index j = 0; j < columns; j++)
        {
            data[left.makeLinearIndex(i, j)] = left.Get(i,j) * right;
        }
    }
    
    return Matrix<U>(rows, columns, data);
}