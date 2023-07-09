#pragma once

#include "Array.h"
#include "Logger.h"
#include "Sequence.h"

template <typename T>
class ArraySequence : public Sequence<T>
{
private:
	Array<T> container;

public:
	class Iterator : IIterator<T>
	{
		T* current;

	public:
		Iterator()
		{
			Logger::Info("Used default constructor of ArraySequence<T>::Iterator");
		}

		Iterator(T* data)
		    : current(data)
		{
			Logger::Info("Used T* constructor of ArraySequence<T>::Iterator");
		}

		Iterator(IIterator<T>* other)
		    : current(((Iterator*) other)->current)
		{
			Logger::Info("Used IIterator* constructor of ArraySequence<T>::Iterator");
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
				this->Prev();
				n--;
			}

			return *this;
		}

		Iterator operator++()
		{
			this->_Next();
			return *this;
		}

		Iterator operator--()
		{
			this->_Prev();
			return *this;
		}

		bool operator!=(const Iterator& other) const
		{
			return this->current != other.current;
		}

		bool operator==(const Iterator& other) const
		{
			return this->current == other.current;
		}

		T& operator*()
		{
			return *(this->current);
		}

		IIterator<T>* _Next() override
		{
			current++;
			return this;
		}

		IIterator<T>* _Prev() override
		{
			current--;
			return this;
		}

		T& _GetCurrent() override
		{
			return *(this->current);
		}

		bool _isEquals(IIterator<T>* other) override
		{
			return this->current == ((Iterator*) other)->current;
		}
	};

	IIterator<T>* _Begin() const override
	{
		return (IIterator<T>*) new (Iterator)(GetFirstPointer());
	}

	IIterator<T>* _End() const override
	{
		return (IIterator<T>*) new (Iterator)(GetEndPointer());
	}

	Iterator begin()
	{
		return (Iterator) (this->_Begin());
	}

	Iterator end()
	{
		return (Iterator) (this->_End());
	}

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

	T* GetFirstPointer() const override
	{
		return &(GetFirst());
	}

	T* GetEndPointer() const override
	{
		return (&(GetLast()) + 1);
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
