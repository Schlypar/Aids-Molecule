#pragma once

#include "Logger.h"
#include "SegmentedList.h"
#include "Sequence.h"

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

		T& operator*()
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

	IIterator<T>* _Begin() override
	{
		return (IIterator<T>*) new (Iterator)(container.GetHead());
	}

	IIterator<T>* _End() override
	{
		return (IIterator<T>*) new (Iterator)();
	}

	Iterator begin()
	{
		return (Iterator) (this->_Begin());
	}

	Iterator end()
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

	ListSequence(const Sequence<T>& other)
	    : container(other)
	{
	}

	ListSequence(Sequence<T>&& other)
	    : container(other)
	{
	}

	template <typename... Args>
	ListSequence(Args... args)
	    : container(args...)
	{
	}

	virtual ~ListSequence()
	{
		Logger::Info("Destroyed ListSequence<T>");
	}

	T& GetFirst() const override
	{
		if (isEmpty())
			{
				Logger::Trace("At Get() at ArraySequence.h");
				logException(EXCEPTION_INDEX_OUT_OF_RANGE);
				exit(EXIT_FAILURE);
			}

		return container.GetFirst();
	}

	T& GetLast() const override
	{
		if (isEmpty())
			{
				Logger::Trace("At Get() at ArraySequence.h");
				logException(EXCEPTION_INDEX_OUT_OF_RANGE);
				exit(EXIT_FAILURE);
			}

		return container.GetLast();
	}

	T& Get(const Index index) const override
	{
		if (isEmpty())
			{
				Logger::Trace("At Get() at ArraySequence.h");
				logException(EXCEPTION_INDEX_OUT_OF_RANGE);
				exit(EXIT_FAILURE);
			}

		return container.Get(index);
	}

	Sequence<T>* Append(T&& data) override
	{
		container.Append(data);

		return this;
	}

	Sequence<T>* Append(const T& data) override
	{
		container.Append(data);

		return this;
	}

	Sequence<T>* Prepend(T&& data) override
	{
		container.Prepend(data);

		return this;
	}

	Sequence<T>* Prepend(const T& data) override
	{
		container.Prepend(data);

		return this;
	}

	void InsertAt(const Index index, const T& data) override
	{
		container.insertAt(index, data);
	}

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

	T* GetFirstPointer() override
	{
		return &container.GetFirst();
	}

	T* GetEndPointer() override
	{
		return nullptr;
	}

	Sequence<T>* Create() override
	{
		return (Sequence<T>*) new ListSequence<T>();
	}

	Sequence<T>* Copy() override
	{
		return (Sequence<T>*) new ListSequence<T>(*this);
	}

	T& operator[](const Index index)
	{
		return container[index];
	}

	friend std::ostream& operator<<(std::ostream& stream, ListSequence<T>& list)
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
