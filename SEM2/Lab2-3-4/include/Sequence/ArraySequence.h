#pragma once

#include "Array.h"
#include "Logger.h"
#include "Sequence.h"

template <typename T>
class ArraySequence : public Sequence<T>
{
private:
	Array<T> container;

	void Resize()
	{
		container.Realloc(container.GetLength() * CAPACITY_TO_REAL_SIZE);
	}

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

	T& GetFirst() const override
	{
		if (isEmpty())
		{
			Logger::Trace("At Get() at ArraySequence.h");
			logException(EXCEPTION_INDEX_OUT_OF_RANGE);
			throw EXCEPTION_INDEX_OUT_OF_RANGE;
		}

		return container.Get(0);
	}

	T& GetLast() const override
	{
		if (isEmpty())
		{
			Logger::Trace("At Get() at ArraySequence.h");
			logException(EXCEPTION_INDEX_OUT_OF_RANGE);
			throw EXCEPTION_INDEX_OUT_OF_RANGE;
		}

		return container.Get(GetLength() - 1);
	}

	T& Get(const Index index) const override
	{
		if (isEmpty() || index > GetLength())
		{
			Logger::Trace("At Get() at ArraySequence.h");
			logException(EXCEPTION_INDEX_OUT_OF_RANGE);
			throw EXCEPTION_INDEX_OUT_OF_RANGE;
		}

		return container.Get(index);
	}

	Sequence<T>* Append(const T& data) override
	{

		container.Realloc(container.GetLength() + 1);
		container[container.GetLength() - 1] = data;

		return this;
	}

	Sequence<T>* Append(T&& data) override
	{
		container.Realloc(container.GetLength() + 1);
		container[container.GetLength() - 1] = data;

		return this;
	}

	Sequence<T>* Prepend(const T& data) override
	{
		container.Realloc(GetLength() + 1);

		for (Index i = GetLength(); i > 0; i--)
			container[i] = container[i - 1];

		container[0] = data;

		return this;
	}

	Sequence<T>* Prepend(T&& data) override
	{
		container.Realloc(GetLength() + 1);

		for (Index i = GetLength(); i > 0; i--)
			container[i] = container[i - 1];

		container[0] = data;

		return this;
	}

	void InsertAt(const Index index, const T& data) override
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

	void Remove(const Index index) override
	{
		for (Index i = index; i < GetLength() - 1; i++)
			container[i] = container[i + 1];
		container.Realloc(container.GetLength() - 1);
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
		return &(GetFirst());
	}

	T* GetEndPointer() const override
	{
		return (&(GetLast()) + 1);
	}

	Sequence<T>* Create() const override
	{
		return (Sequence<T>*) new ArraySequence<T>();
	}

	Sequence<T>* Copy() const override
	{
		return (Sequence<T>*) new ArraySequence<T>(*this);
	}

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
};
