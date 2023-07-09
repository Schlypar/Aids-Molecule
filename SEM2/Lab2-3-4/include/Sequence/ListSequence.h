#pragma once

#include "Logger.h"
#include "SegmentedList.h"
#include "Sequence.h"
#include <cwchar>

template <typename T>
class ListSequence : public Sequence<T>
{
private:
	SegmentedList<T> container;

public:
	class Iterator : IIterator<T>
	{
		Node<T>* current;

	public:
		Iterator()
		    : current(nullptr)
		{
			Logger::Info("Used default constructor of ListSequence<T>::Iterator");
		}

		Iterator(Node<T>* data)
		    : current(data)
		{
			Logger::Info("Used T* constructor of ListSequence<T>::Iterator");
		}

		Iterator(IIterator<T>* other)
		    : current(((Iterator*) other)->current)
		{
			Logger::Info("Used IIterator* constructor of ListSequence<T>::Iterator");
			Logger::Info("Deleted heap pointer of IIterator<T>*");
			delete other;
		}

		Iterator& operator+(int n)
		{
			while (n > 0)
			{
				this->Next();
				n--;
			}

			return *this;
		}

		Iterator& operator-(int n)
		{
			while (n > 0)
			{
				current = current->prev;
				n--;
			}

			return *this;
		}

		Iterator& operator++()
		{
			current = current->next;
			return *this;
		}

		Iterator& operator--()
		{
			current = current->prev;
			return *this;
		}

		bool operator!=(Iterator& other) const
		{
			return this->current != other.current;
		}

		bool operator==(Iterator& other) const
		{
			return this->current == other.current;
		}

		T& operator*() const
		{
			return this->current->data;
		}

		IIterator<T>* _Next() override
		{
			current = current->next;
			return this;
		}

		IIterator<T>* _Prev() override
		{
			current = current->prev;
			return this;
		}

		T& _GetCurrent() override
		{
			return this->current->data;
		}

		T* _GetPointer() override
		{
			return &this->current->data;
		}

		bool _isEquals(IIterator<T>* other) override
		{
			return this->current == ((Iterator*) other)->current;
		}
	};

	IIterator<T>* _Begin() const override
	{
		return (IIterator<T>*) new (Iterator)(container.GetHead());
	}

	IIterator<T>* _End() const override
	{
		return (IIterator<T>*) new (Iterator)();
	}

	Iterator begin() const
	{
		return (Iterator) (this->_Begin());
	}

	Iterator end() const
	{
		return (Iterator) (this->_End());
	}

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
