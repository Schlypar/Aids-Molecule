#pragma once

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>

#include "Allocator.h"
#include "Logger.h"

template <typename T>
class Vector
{
private:
	Array<T> vector;

public:
	Vector()
	    : vector()
	{
		Logger::Warn("Default constructor of Vector has been invoked");
	}

	Vector(Size dimension, T data)
	    : vector(dimension)
	{
		Logger::Info("Vector<T> of dimension %u and T data has been constructed", dimension);

		for (Index i = 0; i < dimension; i++)
			vector[i] = data;
	}

	Vector(Size dimension, T* data)
	    : vector(dimension)
	{
		Logger::Info("T* data constructor of Vector<T> of dimension &u", dimension);

		for (Index i = 0; i < dimension; i++)
			vector[i] = data[i];
	}

	Vector(Size dimension, const Array<T>& data)
	    : vector(dimension)
	{
		Logger::Info("Array<T> constructor of Vector<T> of dimension &u", dimension);

		for (Index i = 0; i < dimension; i++)
			vector[i] = data[i];
	}

	template <typename... Args>
	Vector(Size dimension, Args... args)
	    : vector(dimension)
	{
		Logger::Info("Parameter Pack constructor of Vector<T> of dimension &u", dimension);

		if (sizeof...(args) < dimension)
			Logger::Warn("Vector wasn't filled fully with values");
		if (sizeof...(args) > dimension)
			Logger::Warn("Vector was overflowed with values and last %u will be lost",
				sizeof...(args) + 1 - dimension);

		T data[sizeof...(args)];
		Index k = 0;
		((data[k++] = args), ...);

		for (Index i = 0; i < dimension; i++)
			vector[i] = data[i];
	}

	Vector(const Vector<T>& other)
	    : vector(other.vector)
	{
		Logger::Info("Copied Vector<T>");
	}

	Vector(Vector<T>&& other)
	    : vector(std::move(other.vector))
	{
		Logger::Info("Copied Vector<T>");
		other.vector = Array<T>();
	}

	~Vector()
	{
		Logger::Info("Destroyed Vector<T> of dimension %u", vector.GetLength());
	}

	void Randomize();

	// Makes instance of vector to have length of one
	void Normalize();

	// Gets coordinate at index
	T& Get(Index i) const;

	// Gets max value from vector coordinates
	T GetMax() const noexcept;

	// Gets number of coordinates that vector has
	Size Dimension() const
	{
		return vector.GetLength();
	}

	// Gets coordinate at index
	T& operator[](Index i) const;

	// Destroys old vector and makes deep copy of other
	Vector<T>& operator=(const Vector<T>& other);

	// Destroys old vector and steals data from other
	Vector<T>& operator=(Vector<T>&& other);

	bool operator==(const Vector<T>& other) const;

	T EuclidianNorm() const;

	template <typename U>
	friend Vector<U> operator+(const Vector<U>& left, const Vector<U>& right);

	template <typename U>
	friend Vector<U> operator+(const Vector<U>& left, const U& right);

	template <typename U>
	friend Vector<U> operator+(const U& left, const Vector<U>& right);

	template <typename U>
	friend Vector<U> operator-(const Vector<U>& left, const Vector<U>& right);

	template <typename U>
	friend Vector<U> operator-(const Vector<U>& left, const U& right);

	template <typename U>
	friend Vector<U> operator-(const U& left, const Vector<U>& right);

	template <typename U>
	friend U operator*(const Vector<U>& left, const Vector<U>& right);

	template <typename U>
	friend Vector<U> operator*(const Vector<U>& left, const U& right);

	template <typename U>
	friend Vector<U> operator*(const U& left, const Vector<U>& right);

	Vector<T>& operator/=(const T& other);

	template <typename U>
	friend Vector<U> operator/(const Vector<U>& left, const U& right);

	friend std::ostream& operator<<(std::ostream& stream, const Vector<T>& vector)
	{
		stream << "(";

		Size dim = vector.Dimension();

		for (Index i = 0; i < dim - 1; i++)
			stream << vector[i] << ", ";

		stream << vector[dim - 1] << ")";

		return stream;
	}
};

template <typename U>
Vector<U> ZeroVector(Size dimension)
{
	return Vector<U>(dimension, U());
}

template <typename U>
Vector<U> operator+(const Vector<U>& left, const Vector<U>& right)
{
	if (left.Dimension() != right.Dimension())
	{
		Logger::Error("Vectors don't have same size, behaviour is undefined");
		logException(EXCEPTION_BAD_CONTAINER);
		throw EXCEPTION_BAD_CONTAINER;
	}

	Size dim = left.Dimension();
	U data[dim];

	for (Index i = 0; i < dim; i++)
		data[i] = left[i] + right[i];

	return Vector<U>(dim, data);
}

template <typename U>
Vector<U> operator+(const Vector<U>& left, const U& right)
{
	if (left.Dimension() != right.Dimension())
	{
		Logger::Error("Vectors don't have same size, behaviour is undefined");
		logException(EXCEPTION_BAD_CONTAINER);
		throw EXCEPTION_BAD_CONTAINER;
	}

	Size dim = left.Dimension();
	U data[dim];

	for (Index i = 0; i < dim; i++)
		data[i] = left[i] + right;

	return Vector<U>(dim, data);
}

template <typename U>
Vector<U> operator+(const U& left, const Vector<U>& right)
{
	if (left.Dimension() != right.Dimension())
	{
		Logger::Error("Vectors don't have same size, behaviour is undefined");
		logException(EXCEPTION_BAD_CONTAINER);
		throw EXCEPTION_BAD_CONTAINER;
	}

	Size dim = left.Dimension();
	U data[dim];

	for (Index i = 0; i < dim; i++)
		data[i] = left + right[i];

	return Vector<U>(dim, data);
}

template <typename U>
Vector<U> operator-(const Vector<U>& left, const Vector<U>& right)
{
	if (left.Dimension() != right.Dimension())
	{
		Logger::Error("Vectors don't have same size, behaviour is undefined");
		logException(EXCEPTION_BAD_CONTAINER);
		throw EXCEPTION_BAD_CONTAINER;
	}

	Size dim = left.Dimension();
	U data[dim];

	for (Index i = 0; i < dim; i++)
		data[i] = left[i] - right[i];

	return Vector<U>(dim, data);
}

template <typename U>
Vector<U> operator-(const Vector<U>& left, const U& right)
{
	if (left.Dimension() != right.Dimension())
	{
		Logger::Error("Vectors don't have same size, behaviour is undefined");
		logException(EXCEPTION_BAD_CONTAINER);
		throw EXCEPTION_BAD_CONTAINER;
	}

	Size dim = left.Dimension();
	U data[dim];

	for (Index i = 0; i < dim; i++)
		data[i] = left[i] - right;

	return Vector<U>(dim, data);
}

template <typename U>
Vector<U> operator-(const U& left, const Vector<U>& right)
{
	if (left.Dimension() != right.Dimension())
	{
		Logger::Error("Vectors don't have same size, behaviour is undefined");
		logException(EXCEPTION_BAD_CONTAINER);
		throw EXCEPTION_BAD_CONTAINER;
	}

	Size dim = left.Dimension();
	U data[dim];

	for (Index i = 0; i < dim; i++)
		data[i] = left - right[i];

	return Vector<U>(dim, data);
}

template <typename U>
U operator*(const Vector<U>& left, const Vector<U>& right)
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
		result += left.Get(i) * right.Get(i);

	return result;
}

template <typename U>
Vector<U> operator*(const Vector<U>& left, const U& right)
{
	Size dim = left.Dimension();
	U data[dim];

	for (Index i = 0; i < dim; i++)
		data[i] = left[i] * right;

	return Vector<U>(dim, data);
}

template <typename U>
Vector<U> operator*(const U& left, const Vector<U>& right)
{
	Size dim = right.Dimension();
	U data[dim];

	for (Index i = 0; i < dim; i++)
		data[i] = left * right[i];

	return Vector<U>(dim, data);
}

template <typename U>
Vector<U> operator/(const Vector<U>& left, const U& right)
{
	Vector<U> result = Vector<U>(left);

	for (Index i = 0; i < left.Dimension(); i++)
		result[i] = result[i] / right;

	return result;
}

template <typename T>
Vector<T>& Vector<T>::operator/=(const T& other)
{
	for (Index i = 0; i < Dimension(); i++)
		vector[i] = vector[i] / other;

	return *this;
}

template <typename T>
void Vector<T>::Randomize()
{
	srand(time(NULL));
	for (Index i = 0; i < Dimension(); i++)
		vector[i] = T(std::rand() % 100 + (std::rand() % 100) / 100);
}

template <typename T>
void Vector<T>::Normalize()
{
	T norm = EuclidianNorm();

	(*this) /= norm;
}

template <typename T>
T& Vector<T>::Get(Index i) const
{
	if (i >= Dimension())
	{
		Logger::Info("At Vector<T> at operator[%u]", i);
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw EXCEPTION_INDEX_OUT_OF_RANGE;
	}

	return vector[i];
}

template <typename T>
T Vector<T>::GetMax() const noexcept
{
	T max = Get(0);

	for (Index i = 1; i < Dimension(); i++)
		if (vector[i] > max)
			max = vector[i];

	return max;
}

template <typename T>
T& Vector<T>::operator[](Index i) const
{
	if (i >= Dimension())
	{
		Logger::Info("At Vector<T> at operator[%u]", i);
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw EXCEPTION_INDEX_OUT_OF_RANGE;
	}

	return vector[i];
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other)
{
	Logger::Info("Used assignment operator= of Vector<T>");

	vector = other.vector;

	return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& other)
{
	Logger::Info("Used moving operator= of Vector<T>");
	this->vector.Clear();

	vector = other.vector;
	other.vector = Array<T>();

	return *this;
}

template <typename T>
bool Vector<T>::operator==(const Vector<T>& other) const
{
	if (this->Dimension() != other.Dimension())
		return false;

	Size dim = this->Dimension();
	for (Index i = 0; i < dim; i++)
		if (vector[i] != other.vector[i])
			return false;

	return true;
}

template <typename T>
T Vector<T>::EuclidianNorm() const
{
	T sum = T();

	for (Index i = 0; i < Dimension(); i++)
		sum += vector[i] * vector[i];

	return std::sqrt(sum);
}
