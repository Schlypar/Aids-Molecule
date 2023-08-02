#pragma once

#include "IContainer.h"
#include "IIterator.h"

#include "Iterator.h"
#include "Logger.h"
#include "Tuple.h"
#include "concepts.h"

template <typename T>
class Sequence
{
protected:
	template <Enumerable Container>
	class IteratorWrapper : public AbstractIterator<T>
	{
		using ContainerIterator = typename Container::Iterator;

	private:
		IteratorWrapper()
		    : iternal()
		{
		}

	protected:
		ContainerIterator iternal;

	public:
		IteratorWrapper(ContainerIterator iter)
		    : iternal(iter)
		{
		}

		AbstractIterator<T>& operator++() override
		{
			iternal++;
			return *this;
		}

		AbstractIterator<T>& operator--() override
		{
			iternal--;
			return *this;
		}

		T& operator*() const override
		{
			return *iternal;
		}

		bool operator==(const IteratorWrapper<Container>& other) const
		{
			return this->iternal == other.iternal;
		}

		bool operator!=(const IteratorWrapper<Container>& other) const
		{
			return !(this->iternal == other.iternal);
		}

		bool equal(const AbstractIterator<T>& other) const override
		{
			return this->iternal.equal(other);
		}

		AbstractIterator<T>* copy() const override
		{
			return this->iternal.copy();
		}
	};

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
	Sequence<T>* Where(bool (*condition)(T&));

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

	for (T e : *this)
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

	for (T e : *this)
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

	for (T e : *this)
		result->Append(e);

	return result;
}

template <typename T>
Sequence<T>* Sequence<T>::Concat(Sequence<T>* other)
{
	if (this->isEmpty() || other->isEmpty())
	{
		Logger::Trace("At Concat() at Sequence.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw(EXCEPTION_INDEX_OUT_OF_RANGE);
	}

	Sequence<T>* result = this->Create();

	for (T e : *this)
		result->Append(e);

	for (T e : *other)
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
	    : iter(new AbstractIterator<T>())
	{
	}

	Iterator(AbstractIterator<T>* iter)
	    : iter(iter)
	{
	}

	~Iterator()
	{
		delete iter;
	}

	Iterator& operator++()
	{
		this->iter->operator++();
		return *this;
	}

	Iterator operator++(int)
	{
		Iterator temp = this->iter->copy();
		this->iter->operator++();
		return temp;
	}

	Iterator& operator--()
	{
		this->iter->operator++();
		return *this;
	}

	Iterator operator--(int)
	{
		Iterator temp = this->iter->copy();
		this->iter->operator--();
		return temp;
	}

	T& operator*() const
	{
		return *(*(this->iter));
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
