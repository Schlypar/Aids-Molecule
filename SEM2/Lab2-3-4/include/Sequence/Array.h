#pragma once

#include "IContainer.h"
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
	class Iterator
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

		Iterator operator+(int n)
		{
			return Iterator(current + n);
		}

		Iterator operator-(int n)
		{
			return Iterator(current - n);
		}

		Iterator& operator++()
		{
			current++;
			return *this;
		}

		Iterator operator++(int)
		{
			Iterator iter = *this;
			++(*this);
			return iter;
		}

		Iterator& operator--()
		{
			current--;
			return *this;
		}

		Iterator operator--(int)
		{
			Iterator iter = *this;
			--(*this);
			return iter;
		}

		bool operator!=(const Iterator& other) const
		{
			return this->current != other.current;
		}

		bool operator==(const Iterator& other) const
		{
			return this->current == other.current;
		}

		// add exeption
		T& operator*()
		{
			return *(current);
		}
	};

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

	// copying constructor
	Array(const Array<T>& other)
	    : size(other.size)
	    , data(new T[size])
	{
		Logger::Info("Copied Array<T>");
		for (Index i = 0; i < other.GetLength(); i++)
			data[i] = other.data[i];
	}

	// moving constructor
	Array(Array<T>&& other)
	    : size(other.size)
	    , data(other.data)
	{
		Logger::Info("Moved Array<T>");
		other.size = 0;
		other.data = nullptr;
	}

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

	void Clear() noexcept
	{
		if (isEmpty())
			return;

		if (data != NULL)
			delete[] data;

		data = new T[1];

		size = 0;
	}

	void Realloc(int newSize)
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

	void Set(const Index index, T data)
	{
		if (index < 0 || index >= size)
		{
			Logger::Trace("At Set(%d) at Array.h", index);
			logException(EXCEPTION_INDEX_OUT_OF_RANGE);
			throw EXCEPTION_INDEX_OUT_OF_RANGE;
		}

		this->data[index] = data;
	}

	T& Get(const Index index) const override
	{
		if (index < 0 || index >= size)
		{
			Logger::Trace("At Get() at Array.h");
			logException(EXCEPTION_INDEX_OUT_OF_RANGE);
			throw EXCEPTION_INDEX_OUT_OF_RANGE;
		}

		return data[index];
	}

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

	T& operator[](const Index index) const
	{
		return data[index];
	}

	bool operator==(const Array<T>& other)
	{
		if (this->size != other.size)
			return false;

		for (Index i = 0; i < this->size; i++)
			if (this->data[i] != other.data[i])
				return false;

		return true;
	}

	bool operator!=(const Array<T>& other)
	{
		return !(*this == other);
	}

	Array<T>& operator=(const Array<T>& other)
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

	Array<T>& operator=(Array<T>&& other)
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

	friend std::ostream& operator<<(std::ostream& stream, Array<T>& array)
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
