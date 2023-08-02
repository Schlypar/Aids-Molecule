#pragma once

#include "Array.h"
#include "Iterator.h"
#include "Logger.h"
#include "Sequence.h"
#include <functional>

namespace fn
{

struct filter
{
	std::function<int(int)> _filter;

	filter(std::function<int(int)> func)
	    : _filter(func)
	{
	}
};

struct transformer
{
	std::function<int(int)> _transformer;

	transformer(std::function<int(int)> func)
	    : _transformer(func)
	{
	}
};

}

template <typename T>
class ArraySequence : public Sequence<T>
{
	template <Enumerable Container>
	using IteratorWrapper = typename Sequence<T>::template IteratorWrapper<Container>;

	using Iterator = typename Sequence<T>::Iterator;

private:
	Array<T> container;

public:
	ArraySequence()
	    : container()
	{
		container.Realloc(0);
	}

	ArraySequence(const T* data, Size count)
	    : container(data, count)
	{
	}

	ArraySequence(const Sequence<T>& other)
	    : container(other)
	{
	}

	ArraySequence(Sequence<T>&& other)
	    : container(other)
	{
	}

	template <typename... Args>
	ArraySequence(T head, Args... args)
	    : container(head, args...)
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
		AbstractIterator<T>* iter = new IteratorWrapper<Array<T>>(this->container.begin());
		return Iterator(iter);
	}

	Iterator end() override
	{
		AbstractIterator<T>* iter = new IteratorWrapper<Array<T>>(this->container.end());
		return Iterator(iter);
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
		return container.GetLength();
	}

	bool isEmpty() const noexcept override
	{
		return GetLength() == 0;
	}

	ArraySequence<int> operator|(fn::filter filter)
	{
		ArraySequence<T> result;

		for (T& data : *this)
			if (filter._filter(data))
				result.Append(data);

		return result;
	}

	ArraySequence<int> operator|(fn::transformer transformer)
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

		stream << array.container;

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

private:
	void Resize()
	{
		container.Realloc(container.GetLength() * CAPACITY_TO_REAL_SIZE);
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
	container.Realloc(container.GetLength() + 1);
	container[container.GetLength() - 1] = data;

	return this;
}

template <typename T>
Sequence<T>* ArraySequence<T>::Prepend(const T& data)
{
	container.Realloc(GetLength() + 1);

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

	container.Realloc(container.GetLength() + 1);

	for (Index i = GetLength(); i > index; i--)
		container[i] = container[i - 1];

	container[index] = data;
}

template <typename T>
void ArraySequence<T>::Remove(const Index index)
{
	for (Index i = index; i < GetLength() - 1; i++)
		container[i] = container[i + 1];
	container.Realloc(container.GetLength() - 1);
}
