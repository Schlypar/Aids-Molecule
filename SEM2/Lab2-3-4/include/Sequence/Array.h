#pragma once

#include "IContainer.h"
#include "Iterator.h"
#include "Logger.h"
#include <utility>

#define CAPACITY_TO_REAL_SIZE 2

template <typename T>
class Array : IContainer<T>
{
private:
	Size size;
	T* data = NULL;

public:
	class Iterator;

	Iterator begin()
	{
		return Iterator(data);
	}

	Iterator end()
	{
		return Iterator(data + size);
	}

	Array()
	    : size(1)
	    , data(new T[1])
	{
		Logger::Info("Default constructor of Array<T>");
		data[0] = T();
	}

	Array(const T* other, Size count)
	    : size(count)
	    , data(new T[count])
	{
		Logger::Info("Copied Array<T> from array");
		std::copy(other, other + count, data);
	}

	Array(Size size)
	    : size(size)
	    , data(size ? new T[size]() : nullptr)
	{
		Logger::Info("Allocated Array<T> of size %u", size);
		for (Index i = 0; i < size; i++)
			data[i] = T();
	}

	Array(const Array<T>& other)
	    : size(other.size)
	    , data(new T[size])
	{
		Logger::Info("Copied Array<T>");
		for (Index i = 0; i < other.GetLength(); i++)
			data[i] = other.data[i];
	}

	Array(Array<T>&& other)
	    : size(other.size)
	    , data(other.data)
	{
		Logger::Info("Moved Array<T>");
		other.size = 0;
		other.data = nullptr;
	}

	/*
	 * Constructor for Array that takes any number of arguments
	 *
	 * If we write Array<int> arr = { 1,2,3 } then we get array containing ints and
	 * array will be 1,2,3. Parameters are in order you wrote them
	 * */
	template <typename... Args>
	Array(T head, Args&&... args)
	    : size(sizeof...(args) + 1)
	    , data(new T[sizeof...(args) + 1])
	{
		Logger::Info("Parameter Pack constructor of Array<T> of size %u", size);

		data[0] = head;

		Index i = 1;
		((data[i++] = args), ...);
	}

	~Array()
	{
		Logger::Info("Destroyed Array<T>");
		if (data)
			delete[] data;
	}

	/*
	 * Degrades instance of Array to initial state (default constructor)
	 * */
	void Clear() noexcept;

	/*
	 * Reallocs memory to the new size
	 *
	 * realloc to size that is smaller than initial size before calling Realloc.
	 * Realloc can throw int (Exception) and it is for user to catch it.
	 * Exception can happen because of negative or zero size realloc.
	 * */
	void Realloc(int newSize);

	void Set(const Index index, T data);

	// Gets instance of what is at that index
	T& Get(const Index index) const override;

	Size GetLength() const noexcept override
	{
		return size;
	}

	void SetSize(Size newSize)
	{
		size = newSize;
	}

	bool isEmpty() const noexcept override
	{
		return (size == 0 || !data);
	}

	// Gets instance of what is at that index
	T& operator[](const Index index) const
	{
		return Get(index);
	}

	/*
	 * If two arrays have same size, all elements are the same
	 * and in the same order then this two arrays are equal.
	 * */
	bool operator==(const Array<T>& other);

	bool operator!=(const Array<T>& other)
	{
		return !(*this == other);
	}

	/*
	 * Destroys old array and makes deep copy of another array
	 * */
	Array<T>& operator=(const Array<T>& other);

	/*
	 * Destroys old array and steal pointer to the array from temporary other
	 * */
	Array<T>& operator=(Array<T>&& other);

	friend std::ostream& operator<<(std::ostream& stream, const Array<T>& array)
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

template <typename T>
void Array<T>::Clear() noexcept
{
	if (isEmpty())
		return;

	if (data != NULL)
		delete[] data;

	data = new T[1];

	size = 0;
}

template <typename T>
void Array<T>::Realloc(int newSize)
{
	if (newSize < 0)
	{
		Logger::Trace("At Realloc() at Array.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw EXCEPTION_INDEX_OUT_OF_RANGE;
	}

	T* newBlock = new T[newSize];

	for (Index i = 0; i < newSize; i++)
		newBlock[i] = T();

	if (newSize > 0)
	{
		if (size < newSize)
			for (Index i = 0; i < size; i++)
				newBlock[i] = std::move(data[i]);
		else
			for (Index i = 0; i < newSize; i++)
				newBlock[i] = std::move(data[i]);
	}

	size = newSize;

	if (data)
		delete[] data;

	data = newBlock;
}

template <typename T>
void Array<T>::Set(const Index index, T data)
{
	if (index < 0 || index >= size)
	{
		Logger::Trace("At Set(%d) at Array.h", index);
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw EXCEPTION_INDEX_OUT_OF_RANGE;
	}

	this->data[index] = data;
}

template <typename T>
T& Array<T>::Get(const Index index) const
{
	if (index < 0 || index >= size)
	{
		Logger::Trace("At Get() at Array.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw EXCEPTION_INDEX_OUT_OF_RANGE;
	}

	return data[index];
}

template <typename T>
bool Array<T>::operator==(const Array<T>& other)
{
	if (this->size != other.size)
		return false;

	for (Index i = 0; i < this->size; i++)
		if (this->data[i] != other.data[i])
			return false;

	return true;
}

template <typename T>
Array<T>& Array<T>::operator=(const Array<T>& other)
{
	Logger::Info("Used copying operator = of Array<T>");
	Clear();

	size = other.size;

	delete[] data;
	data = new T[size];

	for (Index i = 0; i < size; i++)
		data[i] = other.Get(i);

	return *this;
}

template <typename T>
Array<T>& Array<T>::operator=(Array<T>&& other)
{
	Logger::Info("Used moving operator = of Array<T>");

	Clear();
	delete[] data;

	data = other.data;
	size = other.size;

	other.data = nullptr;
	other.size = 0;

	return *this;
}

template <typename T>
class Array<T>::Iterator : public AbstractIterator<T>
{
private:
	T* current;

public:
	Iterator(T* data)
	    : current(data)
	{
	}

	Iterator(const Array<T>& other)
	    : current(other.data)
	{
	}

	Iterator(const Iterator& other)
	    : current(other.current)
	{
	}

	~Iterator()
	{
	}

	// Iterator& operator++()
	// {
	// 	current++;
	// 	return *this;
	// }

	// Iterator operator++(int)
	// {
	// 	Iterator iter = *this;
	// 	++(*this);
	// 	return iter;
	// }

	AbstractIterator<T>& operator++() override
	{
		current++;
		return *this;
	}

	Iterator operator++(int)
	{
		Iterator temp = *this;
		current++;
		return temp;
	}

	AbstractIterator<T>& operator--() override
	{
		current--;
		return *this;
	}

	Iterator operator--(int)
	{
		Iterator temp = *this;
		current--;
		return temp;
	}

	bool operator!=(const Iterator& other) const
	{
		return this->current != other.current;
	}

	bool operator==(const Iterator& other) const
	{
		return this->current == other.current;
	}

	bool equal(const AbstractIterator<T>& other) const override
	{
		return this->current == &(*other);
	}

	T& operator*() const override
	{
		return *(current);
	}

	AbstractIterator<T>* copy() const override
	{
		return new Iterator(*this);
	}
};
