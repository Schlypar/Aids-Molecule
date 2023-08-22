#pragma once

#include "Array.h"
#include "Logger.h"
#include "Sequence.h"
#include <functional>

template <typename T>
class ArraySequence : public Sequence<T>
{
	using ContainerIterator = Array<T>::Iterator;

private:
	Array<T> container;
	int length = 0;

	void Resize()
	{
		if (length == container.GetLength())
			container.Realloc(container.GetLength() * 2);
		else if (length > container.GetLength())
			container.Realloc(length * 2);
	}

public:
	using Iterator = Sequence<T>::Iterator;

	ArraySequence()
	    : container()
	    , length(0)
	{
		container.Realloc(0);
	}

	ArraySequence(const T* data, Size count)
	    : container(data, count)
	    , length(count)
	{
	}

	ArraySequence(const Sequence<T>& other)
	    : container(other)
	    , length(other.GetLength())
	{
	}

	ArraySequence(Sequence<T>&& other)
	    : container(other)
	    , length(other.GetLength())
	{
	}

	template <typename... Args>
	ArraySequence(T head, Args... args)
	    : container(head, args...)
	    , length(sizeof...(args) + 1)
	{
	}

	virtual ~ArraySequence()
	{
		Logger::Info("Destroyed ArraySequence<T>");
	}

	Sequence<T>* ArrayAllocator()
	{
		return (Sequence<T>*) new ArraySequence<T>();
	}

	Iterator begin() override
	{
		return Iterator(new ContainerIterator(this->container.begin()));
	}

	Iterator end() override
	{
		return Iterator(new ContainerIterator(this->container.begin() + this->length));
	}

	// Gets an instance of the first element
	T& GetFirst() const override;

	// Gets an instance of the last element
	T& GetLast() const override;

	// Gets an instance of the element at index
	T& Get(const Index index) const override;

	/*
	 * Appends to array a value.
	 * */
	Sequence<T>* Append(const T& data) override;

	/*
	 * Prepends to array a value.
	 * */
	Sequence<T>* Prepend(const T& data) override;

	/*
	 * Inserts a value to array.
	 * */
	void InsertAt(const Index index, const T& data) override;

	/*
	 * Removes a value from a array.
	 * */
	void Remove(const Index index) override;

	Size GetLength() const noexcept override
	{
		return this->length;
	}

	bool isEmpty() const noexcept override
	{
		return GetLength() == 0;
	}

	ArraySequence<T> operator|(fn::filter<T> filter)
	{
		ArraySequence<T> result;

		for (T& data : *this)
			if (filter._filter(data))
				result.Append(data);

		return result;
	}

	ArraySequence<T> operator|(fn::transformer<T> transformer)
	{
		ArraySequence<T> result;

		for (T& data : *this)
			result.Append(transformer._transformer(data));

		return result;
	}

	// Returns abstract class Sequence that underhood is ArraySequence
	Sequence<T>* Create() const override
	{
		return (Sequence<T>*) new ArraySequence<T>();
	}

	// Returns abstract class Sequence that underhood is ArraySequence and copies it
	Sequence<T>* Copy() const override
	{
		return (Sequence<T>*) new ArraySequence<T>(*this);
	}

	// Gets an instance of the element at index
	T& operator[](const Index index)
	{
		return container[index];
	}

	friend std::ostream& operator<<(std::ostream& stream, ArraySequence<T>& array)
	{
		if (array.isEmpty())
		{
			Logger::Trace("At operator<< overload at Array.h");
			logException(EXCEPTION_INDEX_OUT_OF_RANGE);
			return stream;
		}

		// stream << array.container;
		stream << "[ ";
		for (auto e : array)
			stream << e << " ";
		stream << "]";

		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, ArraySequence<T>* array)
	{
		if (array->isEmpty())
		{
			Logger::Trace("At operator<< overload at Array.h");
			logException(EXCEPTION_INDEX_OUT_OF_RANGE);
			return stream;
		}

		stream << array->container;

		return stream;
	}
};

template <typename T>
T& ArraySequence<T>::GetFirst() const
{
	if (isEmpty())
	{
		Logger::Trace("At Get() at ArraySequence.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw EXCEPTION_INDEX_OUT_OF_RANGE;
	}

	return container.Get(0);
}

template <typename T>
T& ArraySequence<T>::GetLast() const
{
	if (isEmpty())
	{
		Logger::Trace("At Get() at ArraySequence.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw EXCEPTION_INDEX_OUT_OF_RANGE;
	}

	return container.Get(GetLength() - 1);
}

template <typename T>
T& ArraySequence<T>::Get(const Index index) const
{
	if (isEmpty() || index > GetLength())
	{
		Logger::Trace("At Get() at ArraySequence.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw EXCEPTION_INDEX_OUT_OF_RANGE;
	}

	return container.Get(index);
}

template <typename T>
Sequence<T>* ArraySequence<T>::Append(const T& data)
{
	if (!container.isEmpty())
	{
		this->length++;
		this->Resize();
		container[length - 1] = data;
	}
	else
	{
		container.Realloc(1);
		container[0] = data;
		this->length++;
	}

	return this;
}

template <typename T>
Sequence<T>* ArraySequence<T>::Prepend(const T& data)
{
	this->length++;
	this->Resize();

	for (Index i = GetLength(); i > 0; i--)
		container[i] = container[i - 1];

	container[0] = data;

	return this;
}

template <typename T>
void ArraySequence<T>::InsertAt(const Index index, const T& data)
{
	if (index >= GetLength())
	{
		Logger::Trace("At InsertAt() at ArraySequence.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw EXCEPTION_INDEX_OUT_OF_RANGE;
	}

	// container.Realloc(container.GetLength() + 1);
	this->length++;
	this->Resize();

	for (Index i = GetLength(); i > index; i--)
		container[i] = container[i - 1];

	container[index] = data;
}

template <typename T>
void ArraySequence<T>::Remove(const Index index)
{
	for (Index i = index; i < GetLength() - 1; i++)
		container[i] = container[i + 1];

	this->length--;
	// container.Realloc(container.GetLength() - 1);
}
