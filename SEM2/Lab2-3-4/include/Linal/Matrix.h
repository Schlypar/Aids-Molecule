#pragma once

#include "../Sequence/IContainer.h"
#include "Allocator.h"
#include "Logger.h"
#include "Vector.h"
#include "complex.h"
#include <cmath>

#define EPSILON 1e-7

template <typename T>
T makeFloatZero(T& data)
{
	if (data < EPSILON)
		return T();

	return data;
}

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
	template <typename U>
	friend Matrix<U> ZeroMatrix(Size dimension);

	template <typename U>
	static Matrix<U> IdentityMatrix(Size dimension)
	{
		Matrix<U> result = Matrix<U>(dimension, U());

		for (Index i = 0; i < dimension; i++)
			result[i][i] = U(1);

		return result;
	}

	template <typename U>
	friend Matrix<U> OuterProduct(const Vector<U>& left, const Vector<U>& right);

	Matrix()
	    : matrix(2)
	    , rows(2)
	    , columns(2)
	{
		Logger::Warn("Default constructor of Matrix<T> was invoked");
	}

	Matrix(Size dimension, T data)
	    : matrix(dimension)
	    , rows(dimension)
	    , columns(dimension)
	{
		Logger::Info("Square Matrix<T> constructor");

		for (Index i = 0; i < rows; i++)
		{
			matrix[i] = Array<T>(columns);
			for (Index j = 0; j < columns; j++)
				Set(i, j, data);
		}
	}

	Matrix(Size rows, Size columns, T data)
	    : matrix(rows)
	    , rows(rows)
	    , columns(columns)
	{
		Logger::Info("T data initialiser constructor of Matrix<T>");

		for (Index i = 0; i < rows; i++)
		{
			matrix[i] = Array<T>(columns);
			for (Index j = 0; j < columns; j++)
				Set(i, j, data);
		}
	}

	Matrix(Size rows, Size columns, T* data)
	    : matrix(rows)
	    , rows(rows)
	    , columns(columns)
	{
		Logger::Info("T* data constructor of Matrix<T> with %u rows and %u columns", rows, columns);

		for (Index i = 0; i < rows; i++)
		{
			matrix[i] = Array<T>(columns);
			for (Index j = 0; j < columns; j++)
				Set(i, j, data[makeLinearIndex(i, j)]);
		}
	}

	Matrix(Size rows, Size columns, const Array<T>& data)
	    : matrix(rows)
	    , rows(rows)
	    , columns(columns)
	{
		Logger::Info("T* data constructor of Matrix<T> with %u rows and %u columns", rows, columns);

		for (Index i = 0; i < rows; i++)
		{
			matrix[i] = Array<T>(columns);
			for (Index j = 0; j < columns; j++)
				Set(i, j, data[makeLinearIndex(i, j)]);
		}
	}

	template <typename... Args>
	Matrix(Size rows, Size columns, Args... args)
	    : matrix(rows)
	    , rows(rows)
	    , columns(columns)
	{
		Logger::Info("Parameter Pack constructor of Matrix<T> with %u rows and %u columns", rows, columns);

		Size numberOfElements = rows * columns;

		if (sizeof...(args) < numberOfElements)
			Logger::Warn("Matrix wasn't filled fully with values");
		if (sizeof...(args) > numberOfElements)
			Logger::Warn("Matrix was overflowed with values and last %u will be lost",
				sizeof...(args) + 1 - numberOfElements);

		T data[sizeof...(args)];
		Index k = 0;
		((data[k++] = args), ...);

		for (Index i = sizeof...(args); i < rows * columns; i++)
			data[i] = T();

		for (Index i = 0; i < rows; i++)
		{
			matrix[i] = Array<T>(columns);
			for (Index j = 0; j < columns; j++)
				Set(i, j, data[makeLinearIndex(i, j)]);
		}
	}

	Matrix(const Matrix<T>& other)
	    : matrix(other.matrix)
	    , rows(other.rows)
	    , columns(other.columns)
	{
		Logger::Info("Copied Matrix<T>");
	}

	Matrix(Matrix<T>&& other)
	    : matrix(std::move(other.matrix))
	    , rows(other.rows)
	    , columns(other.columns)
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

	// Gets an instance of a row at index i
	Array<T>& Get(Index i) const;

	// Gets an element at index i, j
	T& Get(Index j, Index i) const;

	void Set(Index i, Index j, T value);

	Size GetNumberOfRows() const
	{
		return rows;
	}

	Size GetNumberOfColumns() const
	{
		return columns;
	}

	// Gets copy of a row
	Vector<T> GetRow(Index i) const;

	// Gets copy of a column
	Vector<T> GetColumn(Index j) const;

	void SetRow(Index j, const Vector<T>& row);

	void SetColumn(Index i, const Vector<T>& column);

	// Creates minor of a matrix at given indecies
	Matrix<T> GetMinor(Index i, Index j) const;

	// Creates new matrix that is transposed of the original
	Matrix<T> Transpose() const noexcept;

	bool isEmpty() const noexcept
	{
		return (rows == 0 || columns == 0);
	}

	bool isSquare() const noexcept
	{
		return rows == columns;
	}

	Matrix<T>& operator=(const Matrix<T>& other);

	Matrix<T>& operator=(Matrix<T>&& other);

	Array<T>& operator[](Index i) const
	{
		return this->Get(i);
	}

	bool operator==(const Matrix<T>& other) const;

	bool operator!=(const Matrix<T>& other) const
	{
		return !(*this == other);
	}

	template <typename U>
	friend U determinant(const Matrix<U>& matrix);

	template <typename U>
	friend Matrix<U> operator+(const Matrix<U>& left, const Matrix<U>& right);

	template <typename U>
	friend Matrix<U> operator+(const U& left, const Matrix<U>& right);

	template <typename U>
	friend Matrix<U> operator+(const Matrix<U>& left, const U& right);

	template <typename U>
	friend Matrix<U> operator-(const Matrix<U>& left, const Matrix<U>& right);

	template <typename U>
	friend Matrix<U> operator-(const U& left, const Matrix<U>& right);

	template <typename U>
	friend Matrix<U> operator-(const Matrix<U>& left, const U& right);

	template <typename U>
	friend Matrix<U> operator*(const Matrix<U>& left, const Matrix<U>& right);

	template <typename U>
	friend Matrix<U> operator*(const U& left, const Matrix<U>& right);

	template <typename U>
	friend Matrix<U> operator*(const Matrix<U>& left, const U& right);

	template <typename U>
	friend Vector<U> operator*(const Matrix<U>& left, const Vector<U>& right);

	Matrix<T>& RowsLinearCombination(const T& multiplier, Index which, Index other);

	Matrix<T> RowsLinearCombination(const T& multiplier, Index which, Index other) const
	{
		Matrix<T> result = Matrix<T>(*this);

		return result.RowsLinearCombination(multiplier, which, other);
	}

	Matrix<T> ColumnsLinearCombination(const T& multiplier, Index which, Index other);

	Matrix<T> ColumnsLinearCombination(const T& multiplier, Index which, Index other) const
	{
		Matrix<T> result = Matrix<T>(*this);

		return result.ColumnsLinearCombination(multiplier, which, other);
	}

	T OneNorm();

	T InfNorm();

	template <typename ReturnType>
	ReturnType EuclideanNorm()
	{
		T sum = T();
		for (Index i = 0; i < rows; i++)
			for (Index j = 0; j < columns; j++)
				sum += this->Get(i, j) * this->Get(i, j);

		return ReturnType(std::sqrt(sum));
	}

	Matrix<T>& SwapRows(Index first, Index second);

	Matrix<T> SwapRows(Index first, Index second) const
	{
		Matrix<T> result = Matrix<T>(*this);

		return result.SwapRows(first, second);
	}

	Matrix<T>& AddRow(const Vector<T>& row);

	Matrix<T> AddRow(const Vector<T>& row) const
	{
		Matrix<T> result = Matrix<T>(*this);

		return result.AddRow(row);
	}

	Matrix<T>& AddColumn(const Vector<T>& column);

	Matrix<T> AddColumn(const Vector<T>& column) const
	{
		Matrix<T> result = Matrix<T>(*this);

		return result.AddColumn(column);
	}

	// Connect two matrices if they have same rows
	Matrix<T>& AddMatrix(const Matrix<T>& other);

	Matrix<T> AddMatrix(const Matrix<T>& other) const
	{
		Matrix<T> result = Matrix<T>(*this);

		return result.AddMatrix(other);
	}

	// Divides matrix and gives inputed number of columns
	// to left part. Return first part
	Matrix<T> FirstPartOfMatrix(Size columnsToFirst);

	// Divides matrix and gives inputed number of columns
	// to left part. Return second part
	Matrix<T> SecondPartOfMatrix(Size columnsToFirst);

	// Creates triangular matrix that is ~ to initial
	Matrix<T>& Triangular();

	Matrix<T> Triangular() const
	{
		Matrix<T> result = Matrix<T>(*this);

		return result.Triangular();
	}

	template <typename ReturnType>
	Matrix<ReturnType> Triangular() const
	{
		Matrix<ReturnType> result = Matrix<ReturnType>(rows, columns, ReturnType());

		for (Index i = 0; i < rows; i++)
			for (Index j = 0; j < columns; j++)
				result[i][j] = ReturnType(this->Get(i, j));

		return result.Matrix<ReturnType>::Triangular();
	}

	// Applies inverse gauss method to copy and returns result
	Matrix<T> InverseGauss() const;

	template <typename ReturnType>
	Matrix<ReturnType> InverseGauss() const
	{
		if (!isSquare())
		{
			Logger::Trace("At Matrix<T> at InverseGauss()");
			logException(EXCEPTION_BAD_CONTAINER);
			throw EXCEPTION_BAD_CONTAINER;
		}

		Matrix<ReturnType> result = Matrix<ReturnType>(rows, columns, ReturnType());

		for (Index i = 0; i < rows; i++)
			for (Index j = 0; j < columns; j++)
				result[i][j] = ReturnType(this->Get(i, j));

		return result.Matrix<ReturnType>::InverseGauss();
	}

	// Calculates inverse of a matrix
	Matrix<T> Inverse() const;

	// Gram-Schmidt algorithm
	Array<Matrix<T>> QRDecomposition() const;

	template <typename ReturnType>
	Array<Matrix<ReturnType>> QRDecomposition() const
	{
		Matrix<ReturnType> result = Matrix<ReturnType>(rows, columns, ReturnType());

		for (Index i = 0; i < rows; i++)
			for (Index j = 0; j < columns; j++)
				result[i][j] = ReturnType(this->Get(i, j));

		return result.Matrix<ReturnType>::QRDecomposition();
	}

	// Calculates eigenvalue and eigenvector for matrix
	Tuple<T, Vector<T>> Eigenpair();

	template <typename ReturnType>
	Matrix<ReturnType> Inverse() const
	{
		Matrix<ReturnType> result = Matrix<ReturnType>(rows, columns, ReturnType());

		for (Index i = 0; i < rows; i++)
			for (Index j = 0; j < columns; j++)
				result[i][j] = ReturnType(this->Get(i, j));

		return result.Matrix<ReturnType>::Inverse();
	}

	friend std::ostream& operator<<(std::ostream& stream, const Matrix<T>& matrix)
	{
		for (Index i = 0; i < matrix.rows; i++)
		{
			for (Index j = 0; j < matrix.columns - 1; j++)
				stream << matrix[i][j] << ", ";

			stream << matrix[i][matrix.columns - 1] << "\n";
		}

		return stream;
	}

private:
	Index IndexOfRowWithMostZeroes() const
	{
		Size counter = 0;
		Index result = 0;

		for (Index i = 0; i < rows; i++)
		{
			for (Index j = 0; j < columns; j++)
				if (matrix[i][j] == T())
					counter++;

			if (counter > result)
				result = i;

			counter = 0;
		}

		return result;
	}

	void Filter()
	{
		for (Index i = 0; i < rows; i++)
			for (Index j = 0; j < columns; j++)
				this->Get(i, j) = makeFloatZero(this->Get(i, j));
	}
};

template <typename U>
Matrix<U> ZeroMatrix(Size dimension)
{
	return Matrix<U>(dimension, U());
}

template <typename U>
U determinant(const Matrix<U>& matrix)
{
	if (!matrix.isSquare())
	{
		Logger::Trace("At determinant(const Matrix<U>&)");
		logException(EXCEPTION_BAD_CONTAINER);
		return EXCEPTION_BAD_CONTAINER;
	}

	if (matrix.rows == 2)
		return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];

	U det = U();

	Index i = matrix.IndexOfRowWithMostZeroes();

	for (Index j = 0; j < matrix.columns; j++)
		if (matrix[i][j] != U())
			det += std::pow(-1, i + 1 + j + 1) * determinant(matrix.GetMinor(i, j));

	return det;
}

template <typename U>
Matrix<U> operator+(const Matrix<U>& left, const Matrix<U>& right)
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
		for (Index j = 0; j < columns; j++)
			data[left.makeLinearIndex(i, j)] = left.Get(i, j) + right.Get(i, j);

	return Matrix<U>(rows, columns, data);
}

template <typename U>
Matrix<U> operator+(const U& left, const Matrix<U>& right)
{
	Size rows = right.rows;
	Size columns = right.columns;
	Size linearSize = rows * columns;

	U data[linearSize];

	for (Index i = 0; i < rows; i++)
		for (Index j = 0; j < columns; j++)
			data[right.makeLinearIndex(i, j)] = left + right.Get(i, j);

	return Matrix<U>(rows, columns, data);
}

template <typename U>
Matrix<U> operator+(const Matrix<U>& left, const U& right)
{
	Size rows = left.rows;
	Size columns = left.columns;
	Size linearSize = rows * columns;

	U data[linearSize];

	for (Index i = 0; i < rows; i++)
		for (Index j = 0; j < columns; j++)
			data[left.makeLinearIndex(i, j)] = left.Get(i, j) + right;

	return Matrix<U>(rows, columns, data);
}

template <typename U>
Matrix<U> operator-(const Matrix<U>& left, const Matrix<U>& right)
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
		for (Index j = 0; j < columns; j++)
			data[left.makeLinearIndex(i, j)] = left.Get(i, j) - right.Get(i, j);

	return Matrix<U>(rows, columns, data);
}

template <typename U>
Matrix<U> operator-(const U& left, const Matrix<U>& right)
{
	Size rows = right.rows;
	Size columns = right.columns;
	Size linearSize = rows * columns;

	U data[linearSize];

	for (Index i = 0; i < rows; i++)
		for (Index j = 0; j < columns; j++)
			data[right.makeLinearIndex(i, j)] = left - right.Get(i, j);

	return Matrix<U>(rows, columns, data);
}

template <typename U>
Matrix<U> operator-(const Matrix<U>& left, const U& right)
{
	Size rows = left.rows;
	Size columns = left.columns;
	Size linearSize = rows * columns;

	U data[linearSize];

	for (Index i = 0; i < rows; i++)
		for (Index j = 0; j < columns; j++)
			data[left.makeLinearIndex(i, j)] = left.Get(i, j) - right;

	return Matrix<U>(rows, columns, data);
}

template <typename U>
Matrix<U> operator*(const Matrix<U>& left, const Matrix<U>& right)
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

	result.Filter();

	return result;
}

template <typename U>
Matrix<U> operator*(const U& left, const Matrix<U>& right)
{
	Size rows = right.rows;
	Size columns = right.columns;
	Size linearSize = rows * columns;

	U data[linearSize];

	for (Index i = 0; i < rows; i++)
		for (Index j = 0; j < columns; j++)
			data[right.makeLinearIndex(i, j)] = left * right.Get(i, j);

	return Matrix<U>(rows, columns, data);
}

template <typename U>
Matrix<U> operator*(const Matrix<U>& left, const U& right)
{
	Size rows = left.rows;
	Size columns = left.columns;
	Size linearSize = rows * columns;

	U data[linearSize];

	for (Index i = 0; i < rows; i++)
		for (Index j = 0; j < columns; j++)
			data[left.makeLinearIndex(i, j)] = left.Get(i, j) * right;

	return Matrix<U>(rows, columns, data);
}

template <typename U>
Vector<U> operator*(const Matrix<U>& left, const Vector<U>& right)
{
	if (left.columns != right.Dimension())
	{
		Logger::Trace("At operator*(const Matrix<U>& left, const Vector<U>& right)");
		logException(EXCEPTION_BAD_CONTAINER);
		throw EXCEPTION_BAD_CONTAINER;
	}

	Vector<U> result = Vector<U>(right.Dimension());
	for (Index i = 0; i < left.rows; i++)
		result[i] = left.GetRow(i) * right;

	return result;
}

template <typename U>
Matrix<U> OuterProduct(const Vector<U>& left, const Vector<U>& right)
{
	Matrix<U> result = Matrix<U>(left.Dimension(), right.Dimension(), U());

	for (Index i = 0; i < result.rows; i++)
	{
		U currentLeftValue = left[i];

		for (Index j = 0; j < result.columns; j++)
			result[i][j] = currentLeftValue * right[j];
	}

	return result;
}

template <typename T>
Array<T>& Matrix<T>::Get(Index i) const
{
	if (i < 0 || i >= rows)
	{
		Logger::Trace("At Matrix<T> at Get(%d)", i);
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw EXCEPTION_INDEX_OUT_OF_RANGE;
	}

	return matrix[i];
}

template <typename T>
T& Matrix<T>::Get(Index j, Index i) const
{
	if (i < 0 || j < 0 || j >= rows || i >= columns)
	{
		Logger::Trace("At Matrix<T> at Get(%d, %d)", j, i);
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw EXCEPTION_INDEX_OUT_OF_RANGE;
	}

	return matrix[j][i];
}

template <typename T>
void Matrix<T>::Set(Index j, Index i, T value)
{
	if (i < 0 || j < 0 || j >= rows || i >= columns)
	{
		Logger::Trace("At Matrix<T> at Set(%d, %d)", j, i);
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw EXCEPTION_INDEX_OUT_OF_RANGE;
	}

	matrix[j][i] = value;
}

template <typename T>
Vector<T> Matrix<T>::GetRow(Index i) const
{
	if (i < 0 || i >= rows)
	{
		Logger::Trace("At Matrix<T> at GetRow(Index)");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw EXCEPTION_INDEX_OUT_OF_RANGE;
	}

	return Vector<T>(columns, matrix[i]);
}

template <typename T>
Vector<T> Matrix<T>::GetColumn(Index j) const
{
	if (j < 0 || j >= columns)
	{
		Logger::Trace("At Matrix<T> at GetColumn(Index)");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw EXCEPTION_INDEX_OUT_OF_RANGE;
	}

	T data[columns];

	for (Index i = 0; i < rows; i++)
		data[i] = matrix[i][j];

	return Vector<T>(rows, data);
}

template <typename T>
void Matrix<T>::SetRow(Index j, const Vector<T>& row)
{
	if (row.Dimension() != columns)
	{
		Logger::Error("At SetRow() at Matrix<T>");
		logException(EXCEPTION_BAD_CONTAINER);
		throw EXCEPTION_BAD_CONTAINER;
	}

	for (Index i = 0; i < columns; i++)
		matrix[j][i] = row[i];
}

template <typename T>
void Matrix<T>::SetColumn(Index i, const Vector<T>& column)
{
	if (column.Dimension() != rows)
	{
		Logger::Error("At SetRow() at Matrix<T>");
		logException(EXCEPTION_BAD_CONTAINER);
		throw EXCEPTION_BAD_CONTAINER;
	}

	for (Index j = 0; j < rows; j++)
		matrix[j][i] = column[j];
}

template <typename T>
Matrix<T> Matrix<T>::GetMinor(Index i, Index j) const
{
	Matrix<T> result = Matrix<T>(rows - 1, columns - 1, T());

	for (Index k = 0; k < rows; k++)
	{
		for (Index h = 0; h < columns; h++)
		{
			if (k != i || h != j)
			{
				if (k < i && h < j)
					result[k][h] = matrix[k][h];
				else if (k < i && h > j)
					result[k][h - 1] = matrix[k][h];
				else if (k > i && h < j)
					result[k - 1][h] = matrix[k][h];
				else if (k > i && h > j)
					result[k - 1][h - 1] = matrix[k][h];
			}
		}
	}

	return result;
}

template <typename T>
Matrix<T> Matrix<T>::Transpose() const noexcept
{
	T data[rows * columns];

	for (Index i = 0; i < rows; i++)
		for (Index j = 0; j < columns; j++)
			data[makeLinearIndex(i, j)] = this->Get(i, j);

	return Matrix<T>(columns, rows, data);
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other)
{
	Logger::Info("Used assignment operator= of Matrix<T>");

	this->matrix = other.matrix;
	this->rows = other.rows;
	this->columns = other.columns;

	return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T>&& other)
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

template <typename T>
bool Matrix<T>::operator==(const Matrix<T>& other) const
{
	if (this->rows != other.rows || this->columns != other.columns)
		return false;

	for (Index i = 0; i < this->rows; i++)
		if (matrix[i] != other.matrix[i])
			return false;

	return true;
}

template <typename T>
Matrix<T>& Matrix<T>::RowsLinearCombination(const T& multiplier, Index which, Index other)
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
		matrix[which][j] += otherRow[j];

	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::ColumnsLinearCombination(const T& multiplier, Index which, Index other)
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
		matrix[i][which] += otherColumn[i];

	return *this;
}

template <typename T>
T Matrix<T>::OneNorm()
{
	T maxAbsRowSum = T();
	T currentRowSum = T();

	for (Index i = 0; i < rows; i++)
	{
		for (Index j = 0; j < columns; j++)
			currentRowSum += std::abs(this->Get(i, j));

		if (currentRowSum > maxAbsRowSum)
			maxAbsRowSum = currentRowSum;

		currentRowSum = T();
	}

	return maxAbsRowSum;
}

template <typename T>
T Matrix<T>::InfNorm()
{
	T maxAbsColumnSum = T();
	T currentColumnSum = T();

	for (Index j = 0; j < columns; j++)
	{
		for (Index i = 0; i < rows; i++)
			currentColumnSum += std::abs(this->Get(i, j));

		if (currentColumnSum > maxAbsColumnSum)
			maxAbsColumnSum = currentColumnSum;

		currentColumnSum = T();
	}

	return maxAbsColumnSum;
}

template <typename T>
Matrix<T>& Matrix<T>::SwapRows(Index first, Index second)
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

template <typename T>
Matrix<T>& Matrix<T>::AddRow(const Vector<T>& row)
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

template <typename T>
Matrix<T>& Matrix<T>::AddColumn(const Vector<T>& column)
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

template <typename T>
Matrix<T>& Matrix<T>::AddMatrix(const Matrix<T>& other)
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
			current[j + columns] = other[i][j];
		matrix[i] = current;
	}

	columns += other.columns;

	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::FirstPartOfMatrix(Size columnsToFirst)
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

template <typename T>
Matrix<T> Matrix<T>::SecondPartOfMatrix(Size columnsToFirst)
{
	Matrix<T> second = Matrix<T>(rows, columns - columnsToFirst, T());

	for (Index i = 0; i < columnsToFirst; i++)
	{
		Vector<T> currentRow = this->GetRow(i);

		for (Index j = 0; j < columns - columnsToFirst; j++)
			second[i][j] = currentRow[j + columnsToFirst];
	}
}

template <typename T>
Matrix<T>& Matrix<T>::Triangular()
{
	for (Index i = 0; i < rows; i++)
	{
		for (Index k = i + 1; k < rows; k++)
		{
			T ratio = this->Get(k, i) / this->Get(i, i);
			RowsLinearCombination(T() - ratio, k, i);
		}
	}

	for (Index i = 0; i < rows; i++)
	{
		T diagonalValue = matrix[i][i];

		if (diagonalValue == T())
			continue;

		for (Index j = 0; j < columns; j++)
			matrix[i][j] = matrix[i][j] / diagonalValue;
	}

	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::InverseGauss() const
{
	Matrix<T> result = Matrix<T>(*this);

	for (Index i = rows - 1; i > 0; i--)
	{
		for (Index k = 0; k < i; k++)
		{
			T ratio = result.Get(k, i) / result.Get(i, i);
			result.RowsLinearCombination(T() - ratio, k, i);
		}
	}

	return result;
}

template <typename T>
Matrix<T> Matrix<T>::Inverse() const
{
	if (!isSquare())
	{
		Logger::Trace("At Matrix<T> at InverseGauss()");
		logException(EXCEPTION_BAD_CONTAINER);
		throw EXCEPTION_BAD_CONTAINER;
	}

	Matrix<T> augmented = this->AddMatrix(Matrix<T>::IdentityMatrix<T>(rows));

	Matrix<T> result = ((augmented.Triangular()).InverseGauss()).SecondPartOfMatrix(columns);

	return result;
}

template <typename T>
Array<Matrix<T>> Matrix<T>::QRDecomposition() const
{
	Size n = columns;
	Matrix<T> Q = Matrix<T>(rows, columns, T());
	Matrix<T> R = Matrix<T>(columns, columns, T());
	for (Index j = 0; j < n; j++)
	{
		Vector<T> q(rows, T());
		for (Index i = 0; i < rows; i++)
			q[i] = matrix[i][j];

		for (Index k = 0; k < j; k++)
		{
			T dotProduct = q * Q.GetColumn(k);

			for (Index i = 0; i < rows; i++)
				q[i] -= dotProduct * Q.matrix[i][k];

			R.Set(k, j, dotProduct);
		}

		T norm = q.EuclidianNorm();

		// if (norm == 0)
		//     throw std::runtime_error("Matrix is not full rank");

		for (Size i = 0; i < rows; i++)
			Q.Set(i, j, q[i] / norm);

		R.Set(j, j, norm);
	}

	return Array<Matrix<T>>(Q, R);
}

template <typename T>
Tuple<T, Vector<T>> Matrix<T>::Eigenpair()
{
	if (!isSquare())
	{
		Logger::Trace("At Eigenpair() at Matrix<T>");
		logException(EXCEPTION_BAD_CONTAINER);
		throw EXCEPTION_BAD_CONTAINER;
	}

	T lambdaOld = 1;
	T lambdaNew = T();

	Vector<T> temp = { columns, T() };
	temp.Randomize();

	for (Index i = 0; i < 15; i++)
	{
		temp = (*this) * temp;
		temp /= temp.EuclidianNorm();
	}

	Vector<T> eigenvector = (*this) * temp;
	T eigenvalue = eigenvector.EuclidianNorm() / temp.EuclidianNorm();

	return Tuple<T, Vector<T>>(eigenvalue, eigenvector);
}
