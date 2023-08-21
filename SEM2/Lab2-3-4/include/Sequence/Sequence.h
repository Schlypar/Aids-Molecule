#pragma once

#include "IContainer.h"
#include "IIterator.h"

#include "Array.h"
#include "Iterator.h"
#include "Logger.h"
#include "Pointer.h"
#include "Tuple.h"
#include "concepts.h"

namespace fn
{

template <typename T>
struct filter
{
	std::function<T(T)> _filter;

	filter(std::function<T(T)> func)
	    : _filter(func)
	{
	}
};

template <typename T>
struct transformer
{
	std::function<T(T)> _transformer;

	transformer(std::function<T(T)> func)
	    : _transformer(func)
	{
	}
};

}

template <typename T>
class Sequence
{
public:
	virtual ~Sequence()
	{
	}

	class Iterator;

	virtual Iterator begin() = 0;
	virtual Iterator end() = 0;

	virtual T& GetFirst() const = 0;
	virtual T& GetLast() const = 0;
	virtual T& Get(const Index index) const = 0;

	Sequence<T>* Map(T (*func)(T&));
	Sequence<T>* Where(std::function<bool(T&)> condition);

	virtual Sequence<T>* GetSubsequence(const Index start, const Index end);
	virtual Sequence<T>* Concat(Sequence<T>* other);
	virtual Sequence<T>* Slice(const Index index, Size size, Sequence<T>* other);

	T Reduce(T (*reducer)(T, T), T base);

	virtual Size GetLength() const noexcept = 0;

	virtual Sequence<T>* Append(const T& data) = 0;
	virtual Sequence<T>* Prepend(const T& data) = 0;

	virtual void InsertAt(const Index index, const T& data) = 0;
	virtual void Remove(const Index index) = 0;

	virtual bool isEmpty() const noexcept
	{
		return GetLength() == 0;
	}

	virtual Sequence<T>* Create() const = 0;
	virtual Sequence<T>* Copy() const = 0;

	friend std::ostream& operator<<(std::ostream& stream, Sequence<T>* sequence)
	{
		for (Index i = 0; i < sequence->GetLength(); i++)
			stream << sequence->Get(i) << " ";

		return stream;
	}

	template <typename U>
	friend SharedPtr<Sequence<U>> operator|(SharedPtr<Sequence<U>> sequence, fn::filter<U> filter);
	template <typename U>
	friend SharedPtr<Sequence<U>> operator|(SharedPtr<Sequence<U>> sequence, fn::transformer<U> transformer);
	template <typename U>
	friend UniquePtr<Sequence<U>> operator|(Sequence<U>* sequence, fn::filter<U> filter);
	template <typename U>
	friend UniquePtr<Sequence<U>> operator|(Sequence<U>* sequence, fn::transformer<U> transformer);
};

template <typename T>
Sequence<T>* Sequence<T>::Map(Func<T> func)
{
	if (this->isEmpty())
	{
		Logger::Trace("At Map() at Sequence.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw(EXCEPTION_INDEX_OUT_OF_RANGE);
	}

	Sequence<T>* result = this->Create();

	for (auto e : *this)
		result->Append(func(e));

	return result;
}

template <typename T>
Sequence<T>* Sequence<T>::Where(Condition<T> condition)
{
	if (this->isEmpty())
	{
		Logger::Trace("At Where() at Sequence.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw(EXCEPTION_INDEX_OUT_OF_RANGE);
	}

	Sequence<T>* result = this->Create();

	for (auto e : *this)
		if (condition(e))
			result->Append(e);

	return result;
}

template <typename T>
Sequence<T>* Sequence<T>::GetSubsequence(const Index start, const Index end)
{
	if (this->isEmpty() || end >= this->GetLength())
	{
		Logger::Trace("At GetSubsequence() at Sequence.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw(EXCEPTION_INDEX_OUT_OF_RANGE);
	}

	Sequence<T>* result = this->Create();

	for (Index i = start; i <= end; i++)
		result->Append(this->Get(i));

	return result;
}

template <typename T>
Sequence<T>* Sequence<T>::Concat(Sequence<T>* other)
{
	Sequence<T>* result = this->Create();

	if (this->isEmpty() == false)
		for (auto e : *this)
			result->Append(e);

	if (other->isEmpty() == false)
		for (auto e : *other)
			result->Append(e);

	return result;
}

template <typename T>
Sequence<T>* Sequence<T>::Slice(const Index index, Size size, Sequence<T>* other)
{
	if (this->isEmpty() || index >= this->GetLength())
	{
		Logger::Trace("At Slice(1) at Sequence.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw(EXCEPTION_INDEX_OUT_OF_RANGE);
	}
	if (other != NULL)
	{
		if (size > other->GetLength() || index + size > this->GetLength())
		{
			Logger::Trace("At Slice(2) at Sequence.h");
			logException(EXCEPTION_INDEX_OUT_OF_RANGE);
			throw(EXCEPTION_INDEX_OUT_OF_RANGE);
		}
	}

	Sequence<T>* result = this->Create();

	if (other != NULL)
	{
		for (Index i = 0; i < size; i++)
			result->Get(i + index) = other->Get(i);
	}
	else
	{
		for (Index i = 0; i < index; i++)
			result->Append(this->Get(i));

		for (Index i = index + size; i < this->GetLength(); i++)
			result->Append(this->Get(i));
	}
	return result;
}

template <typename T>
T Sequence<T>::Reduce(Reducer<T> reducer, T base)
{
	if (this->isEmpty())
	{
		Logger::Trace("At Reduce() at Sequence.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw(EXCEPTION_INDEX_OUT_OF_RANGE);
	}

	for (Index i = this->GetLength() - 1; i > 0; i--)
		base = reducer(this->Get(i), base);

	base = reducer(this->GetFirst(), base);

	return base;
}

template <typename T>
class Sequence<T>::Iterator
{
private:
	AbstractIterator<T>* iter;

public:
	Iterator()
	    : iter(nullptr)
	{
	}

	Iterator(AbstractIterator<T>* iter)
	    : iter(iter)
	{
	}

	Iterator(const Iterator& other)
	    : iter(other.iter->copy())
	{
	}

	Iterator& operator++()
	{
		++(*this->iter);
		return *this;
	}

	Iterator operator++(int)
	{
		Iterator tmp = *this;
		++(*iter);
		return tmp;
	}

	Iterator& operator--()
	{
		--(*this->iter);
		return *this;
	}

	Iterator operator--(int)
	{
		Iterator tmp = *this;
		--(*iter);
		return tmp;
	}

	~Iterator()
	{
		delete iter;
	}

	T& operator*()
	{
		return *(*this->iter);
	}

	bool operator==(const Iterator& other) const
	{
		return this->iter->equal(*other.iter);
	}

	bool operator!=(const Iterator& other) const
	{
		return !(this->iter->equal(*other.iter));
	}
};

template <typename U>
SharedPtr<Sequence<U>> operator|(SharedPtr<Sequence<U>> sequence, fn::filter<U> filter)
{
	SharedPtr<Sequence<U>> result = sequence->Create();

	for (U& data : *sequence)
		if (filter._filter(data))
			result->Append(data);

	return result;
}

template <typename U>
SharedPtr<Sequence<U>> operator|(SharedPtr<Sequence<U>> sequence, fn::transformer<U> transformer)
{
	SharedPtr<Sequence<U>> result = sequence->Create();

	for (U& data : *sequence)
		result->Append(transformer._transformer(data));

	return result;
}

template <typename U>
UniquePtr<Sequence<U>> operator|(Sequence<U>* sequence, fn::filter<U> filter)
{
	Sequence<U>* result = sequence->Create();

	for (U& data : *sequence)
		if (filter._filter(data))
			result->Append(data);

	return UniquePtr<Sequence<U>>(result);
}

template <typename U>
UniquePtr<Sequence<U>> operator|(Sequence<U>* sequence, fn::transformer<U> transformer)
{
	Sequence<U>* result = sequence->Create();

	for (U& data : *sequence)
		result->Append(transformer._transformer(data));

	return UniquePtr<Sequence<U>>(result);
}
