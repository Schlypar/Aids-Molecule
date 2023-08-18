#pragma once

#include "Logger.h"
#include "SegmentedList.h"
#include "Sequence.h"
#include <cwchar>

template <typename T>
class ListSequence : public Sequence<T>
{

	using Iterator = Sequence<T>::Iterator;
	using ContainerIterator = SegmentedList<T>::Iterator;

private:
	SegmentedList<T> container;

public:
	ListSequence()
	    : container()
	{
	}

	ListSequence(const T* data, Size count)
	    : container(data, count)
	{
	}

	ListSequence(const ListSequence<T>& other)
	    : container(other.container)
	{
	}

	ListSequence(ListSequence<T>&& other)
	    : container(std::move(other.container))
	{
	}

	ListSequence(const Sequence<T>& other)
	    : container(other)
	{
	}

	ListSequence(Sequence<T>&& other)
	    : container(other)
	{
	}

	template <typename... Args>
	ListSequence(T head, Args... args)
	    : container(head, args...)
	{
	}

	virtual ~ListSequence()
	{
		Logger::Info("Destroyed ListSequence<T>");
	}

	Iterator begin() override
	{
		return Iterator(new ContainerIterator(this->container.begin()));
	}

	Iterator end() override
	{
		return Iterator(new ContainerIterator(this->container.end()));
	}

	// Gets an intance of what is at the head
	T& GetFirst() const override;

	// Gets an intance of what is at the tail
	T& GetLast() const override;

	// Gets an intance of what is at the index
	T& Get(const Index index) const override;

	/*
	 * Appends data to the sequence and returns pointer to itself
	 * */
	Sequence<T>* Append(const T& data) override;

	/*
	 * Prepends data to the sequence and returns pointer to itself
	 * */
	Sequence<T>* Prepend(const T& data) override;

	// Inserts exactly at the index inputed
	void InsertAt(const Index index, const T& data) override
	{
		container.insertAt(index, data);
	}

	// Removes exactly index inputed
	void Remove(const Index index) override
	{
		for (Index i = index; i < GetLength() - 1; i++)
			container[i] = container[i + 1];
		container.SetSize(GetLength() - 1);
	}

	Size GetLength() const noexcept override
	{
		return container.GetLength();
	}

	bool isEmpty() const noexcept override
	{
		return GetLength() == 0;
	}

	T* GetFirstPointer() const override
	{
		return &container.GetFirst();
	}

	T* GetEndPointer() const override
	{
		return nullptr;
	}

	// Returns abstract class Sequence that underhood is ListSequence
	Sequence<T>* Create() const override
	{
		return (Sequence<T>*) new ListSequence<T>();
	}

	// Returns abstract class Sequence that underhood is ListSequence and copies data to it
	Sequence<T>* Copy() const override
	{
		return (Sequence<T>*) new ListSequence<T>(*this);
	}

	// Gets an intance of what is at the index
	T& operator[](const Index index)
	{
		return container[index];
	}

	ListSequence<T>& operator=(const ListSequence<T>& other)
	{
		this->container = other.container;

		return *this;
	}

	ListSequence<T>& operator=(ListSequence<T>&& other)
	{
		this->container = std::move(other.container);

		return *this;
	}

	friend std::ostream& operator<<(std::ostream& stream, const ListSequence<T>& list)
	{
		if (list.isEmpty())
		{
			Logger::Trace("At operator<< overload at Array.h");
			logException(EXCEPTION_INDEX_OUT_OF_RANGE);
			return stream;
		}

		stream << list.container;

		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, ListSequence<T>* list)
	{
		if (list->isEmpty())
		{
			Logger::Trace("At operator<< overload at Array.h");
			logException(EXCEPTION_INDEX_OUT_OF_RANGE);
			return stream;
		}

		stream << list->container;

		return stream;
	}
};

template <typename T>
T& ListSequence<T>::GetFirst() const
{
	if (isEmpty())
	{
		Logger::Trace("At Get() at ArraySequence.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		exit(EXIT_FAILURE);
	}

	return container.GetFirst();
}

template <typename T>
T& ListSequence<T>::GetLast() const
{
	if (isEmpty())
	{
		Logger::Trace("At Get() at ArraySequence.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		exit(EXIT_FAILURE);
	}

	return container.GetLast();
}

template <typename T>
T& ListSequence<T>::Get(const Index index) const
{
	if (isEmpty())
	{
		Logger::Trace("At Get() at ArraySequence.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		exit(EXIT_FAILURE);
	}

	return container.Get(index);
}

template <typename T>
Sequence<T>* ListSequence<T>::Append(const T& data)
{
	container.Append(data);

	return this;
}

template <typename T>

Sequence<T>* ListSequence<T>::Prepend(const T& data)
{
	container.Prepend(data);

	return this;
}
