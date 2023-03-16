#pragma once

#include "IContainer.h"
#include "Tuple.h"

template <typename T> struct Node
{
public:
	T data;
	struct Node* next;
	struct Node* prev;

	friend std::ostream& operator<< (std::ostream& stream, Node<T>* node)
	{
		stream << node->data << "\n";
		return stream;
	}
};

template <typename T> 
class List : IContainer<T>
{
private:

	Node<T>* head;
	Node<T>* tail;
	Size size;

	void NewHead(Node<T>* node)
	{
		node->prev = nullptr;
		node->next = this->head;
		this->head->prev = node;
		this->head = node;
	}

	void NewTail(Node<T>* node)
	{
		this->tail->next = node;
		node->prev = this->tail;
		this->tail = node;
		node->next = nullptr;
	}

	Node<T>* GetNode(const Index index)
	{
		if (index >= size || index < 0)
		{
			throw EXCEPTION_INDEX_OUT_OF_RANGE;
		}

		Node<T>* pointer = this->head;
		int counter = 0;
		while (counter < index)
		{
			counter++;
			pointer = pointer->next;
		}

		return pointer;
	}


public:

	class Iterator
	{
		Node<T>* current;
	public:
		Iterator()
			: current(nullptr) {}

		Iterator(List<T> list)
			: current(list.head) {}

		Iterator(Node<T>* first)
			: current(first) {}

		Iterator operator+ (int n)
		{
			Size counter = 0;
			while (counter != n)
				current = current->next, counter++;

			return *this;
		}

		Iterator operator- (int n)
		{
			Size counter = 0;
			while (counter != n)
				current = current->prev, counter++;

			return *this;
		}

		Iterator operator++ (int) { current = current->next; return *this; }
		Iterator operator-- (int) { current = current->prev; return *this; }
		Iterator operator++ () { current = current->next; return *this; }
		Iterator operator-- () { current = current->prev; return *this; }

		// const Iterator operator++ (int) { current = current->next; return *this; }
		// const Iterator operator-- (int) { current = current->prev; return *this; }
		// const Iterator operator++ () { current = current->next; return *this; }
		// const Iterator operator-- () { current = current->prev; return *this; }

		bool operator!= (const Iterator& other) const { return this->current != other.current; }
		bool operator== (const Iterator& other) const { return this->current == other.current; }

		T& operator* () { return this->current->data; }
	};
	Iterator begin() noexcept { return (Iterator)this->head; }
	Iterator end() noexcept { return nullptr; }

	//default constructor
	List()
		: head(nullptr), tail(nullptr), size(0) 
	{
		Logger::Info("Default constructor of List<T>");
	}

	//copying constructor
	List(const List<T>& other) noexcept
		: size(0)
	{
		Logger::Info("Copied List<T>");
		for (Size i = 0; i < other.size; i++)
		{
			Append(other.Get(i));
		}
	}

	//moving constructor
	List(List<T>&& other) noexcept
		: head(other.head), tail(other.tail), size(other.size)
	{
		Logger::Info("Moved List<T>");
		other.head = nullptr;
	}

	//copying from array constructor
	List(T* data, Size count) noexcept
		: size(0)
	{
		Logger::Info("Copied List<T> from array");
		for (Index i = 0; i < count; i++)
			Append(data[i]);

		size = count;
	}

	~List()
	{
		Logger::Info("Destoryed List<T>");
		if (isEmpty()) return;

		Node<T>* pointer = this->head;

		while (pointer && pointer != this->tail->next)
		{
			Node<T>* prev = pointer;
			pointer = pointer->next;
			delete prev;
		}
		this->head = nullptr;
	}

	void Clear() noexcept
	{
		if (isEmpty()) return;

		Node<T>* current = this->head;
		while (current != this->tail)
		{
			current = current->next;
			delete current->prev;
		}
		delete this->tail;
		this->head = nullptr;
		this->size = 0;
	}

	T GetFirst() const
	{
		try
		{
			if (isEmpty())
			{
				throw EXCEPTION_INDEX_OUT_OF_RANGE;
			}
			return this->head->data;
		}
		catch (Exception e)
		{
			Logger::Trace("At GetFirst() at List.h");
			logException(e);
			return 0;
		}
	}

	T GetLast() const
	{
		try
		{
			if (isEmpty())
			{
				throw EXCEPTION_INDEX_OUT_OF_RANGE;
			}
			return this->tail->data;
		}
		catch (Exception e)
		{
			Logger::Trace("At GetLast() at List.h");
			logException(e);
			return 0;
		}
	}

	T Get(const Index index) const  override
	{
		try
		{
			if (index >= size || index < 0)
			{
				throw EXCEPTION_INDEX_OUT_OF_RANGE;
			}

			Node<T>* pointer = this->head;
			int counter = 0;
			while (counter < index)
			{
				counter++;
				pointer = pointer->next;
			}
			
			if (!pointer)
			{
				throw EXCEPTION_BAD_POINTER;
			}

			return pointer->data;
		}
		catch (Exception e)
		{
			Logger::Trace("At Get() ai List.h");
			logException(e);
			return 0;
		}
	}

	Node<T>* GetHead() const
	{
		try
		{
			if (isEmpty())
			{
				throw EXCEPTION_INDEX_OUT_OF_RANGE;
			}

			if (!this->head)
			{
				throw EXCEPTION_BAD_POINTER;
			}

			return this->head;
		}
		catch (Exception e)
		{
			Logger::Trace("At GetHead() at List.h");
			logException(e);
		}
	}

	Node<T>* GetTail() const
	{
		try
		{
			if (isEmpty())
			{
				throw EXCEPTION_INDEX_OUT_OF_RANGE;
			}

			if (!this->tail)
			{
				throw EXCEPTION_BAD_POINTER;
			}

			return this->tail;
		}
		catch (Exception e)
		{
			Logger::Trace("At GetTail() aat List.h");
			logException(e);
		}
	}

	Size GetLength() const override { return size; }

	//by copying
	void InsertAt(const Index index, const T& data)
	{
		try
		{
			if (index > size || index < 0)
			{
				throw EXCEPTION_INDEX_OUT_OF_RANGE;
			}

			Node<T>* node = new Node<T>;
			node->data = data;
			node->next = nullptr;
			node->prev = nullptr;

			if (index == size)
			{
				this->NewTail(node);
				size++;
				return;
			}

			if (index == 0)
			{
				this->NewHead(node);
				size++;
				return;
			}

			Node<T>* pointer = this->head;

			int counter = 0;
			while (counter < index)
			{
				counter++;
				pointer = pointer->next;
			}

			pointer->prev->next = node;
			node->prev = pointer->prev;
			node->next = pointer;
			pointer->prev = node;

			size++;
		}
		catch (Exception e)
		{
			Logger::Trace("At InsertAt(const Index, const T&) at List.h");
			logException(e);
		}
	}

	//by moving
	void InsertAt(const Index index, T&& data)
	{
		try
		{
			if (index > size || index < 0)
			{
				throw EXCEPTION_INDEX_OUT_OF_RANGE;
			}

			Node<T>* node = new Node<T>;
			node->data = data;
			node->next = nullptr;
			node->prev = nullptr;

			if (index == size)
			{
				this->NewTail(node);
				size++;
				return;
			}

			if (index == 0)
			{
				this->NewHead(node);
				size++;
				return;
			}

			Node<T>* pointer = this->head;

			int counter = 0;
			while (counter < index)
			{
				counter++;
				pointer = pointer->next;
			}

			pointer->prev->next = node;
			node->prev = pointer->prev;
			node->next = pointer;
			pointer->prev = node;

			size++;
		}
		catch (Exception e)
		{
			Logger::Trace("At InsertAt(const Index, T&&)  at List.h");
			logException(e);
		}
	}

	void Remove(Index index)
	{
		try
		{
			if (index > size || index < 0)
			{
				throw EXCEPTION_INDEX_OUT_OF_RANGE;
			}

			if (index == size - 1)
			{
				this->tail = this->tail->prev;
				delete this->tail->next;
				this->tail->next = nullptr;

				size--;
				return;
			}

			if (index == 0)
			{
				this->head = this->head->next;
				delete this->head->prev;
				this->head->prev = nullptr;

				size--;
				return;
			}

			Node<T>* node = this->head;
			for (Size i = 0; i < index; i++)
				node = node->next;

			node->prev->next = node->next;
			node->next->prev = node->prev;

			delete node;
			size--;
		}
		catch (Exception e)
		{
			Logger::Trace("At Remove() at List.h");
			logException(e);
		}
	}

	//append by copying
	void Append(const T& data)
	{
		try
		{
			if (!this)
			{
				throw EXCEPTION_BAD_POINTER;
			}

			Node<T>* pointer = new Node<T>;

			pointer->data = data;
			pointer->next = nullptr;
			pointer->prev = nullptr;

			if (isEmpty())
			{
				this->head = pointer;
				this->tail = pointer;

				size++;
			}
			else
			{
				pointer->prev = this->tail;
				this->tail->next = pointer;
				this->tail = pointer;

				size++;
			}
		}
		catch (Exception e)
		{
			Logger::Trace("At Append(const T&) at List.h");
			logException(e);
		}
	}

	//append by moving
	void Append(T&& data)
	{
		try
		{
			if (!this)
			{
				throw EXCEPTION_BAD_POINTER;
			}

			Node<T>* pointer = new Node<T>;

			pointer->data = data;
			pointer->next = nullptr;
			pointer->prev = nullptr;

			if (isEmpty())
			{
				this->head = pointer;
				this->tail = pointer;

				size++;
			}
			else
			{
				pointer->prev = this->tail;
				this->tail->next = pointer;
				this->tail = pointer;

				size++;
			}
		}
		catch (Exception e)
		{
			Logger::Trace("At Append(T&&) at List.h");
			logException(e);
		}
	}

	//Prepend by copying
	void Prepend(const T& data)
	{
		try
		{
			if (!this)
			{
				throw EXCEPTION_BAD_POINTER;
			}

			Node<T>* pointer = new Node<T>;

			pointer->data = data;
			pointer->next = nullptr;
			pointer->prev = nullptr;

			if (isEmpty())
			{
				this->head = pointer;
				this->tail = pointer;

				size++;
			}
			else
			{
				this->head->prev = pointer;
				pointer->next = this->head;
				this->head = pointer;

				size++;
			}
		}
		catch (Exception e)
		{
			Logger::Trace("At Prepend(const T&) at List.h");
			logException(e);
		}
	}

	//Prepend by moving
	void Prepend(T&& data)
	{
		try
		{
			if (!this)
			{
				throw EXCEPTION_BAD_POINTER;
			}

			Node<T>* pointer = new Node<T>;

			pointer->data = data;
			pointer->next = nullptr;
			pointer->prev = nullptr;

			if (isEmpty())
			{
				this->head = pointer;
				this->tail = pointer;

				size++;
			}
			else
			{
				this->head->prev = pointer;
				pointer->next = this->head;
				this->head = pointer;

				size++;
			}
		}
		catch (Exception e)
		{
			Logger::Trace("At Prepend(T&&) at List.h");
			logException(e);
		}
	}

	List<T> Concat(List<T>& other)
	{
		try
		{
			if (other.isEmpty())
			{
				throw EXCEPTION_INDEX_OUT_OF_RANGE;
			}
			List<T> list = List(*this);

			for (auto data : other)
				list.Append(data);

			return list;
		}
		catch (Exception e)
		{
			Logger::Trace("At Concat() at List.h");
			logException(e);
			return List();
		}
	}

	List<T> GetSublist(Index start, Index end)
	{
		try
		{
			if (start < 0 || start > this->GetLength())
			{
				throw EXCEPTION_INDEX_OUT_OF_RANGE;
			}

			if (end < 0 || end > this->GetLength())
			{
				throw EXCEPTION_INDEX_OUT_OF_RANGE;
			}

			List<T> list = List();

			for (Size i = start; i < end; i++)
			{
				list.Append(Get(i));
			}

			return list;
		}
		catch (Exception e)
		{
			Logger::Trace("At GetSublist() at List.h");
			logException(e);
			return List();
		}
	}

	const T& operator[] (const Index index) { return Get(index); }

	//copying operator =
	List<T>& operator= (const List<T>& other) 
	{
		Logger::Info("Used copying operator = of List<T>");
		this->Clear();

		for (auto data : other)
			Append(data);

		return *this;
	}

	//moving operator =
	List<T>& operator= (List<T>&& other) 
	{
		Logger::Info("Used moving operator = of List<T>");
		this->Clear();

		this->head = other.head;
		this->tail = other.tail;
		this->size = other.size;

		other.head = nullptr;

		return *this;
	}

	friend std::ostream& operator<< (std::ostream& stream, const List<T>& list)
	{
		try
		{
			if (list.isEmpty())
			{
				throw EXCEPTION_BAD_CONTAINER;
			}

			stream << "[ ";
			for (Index i = 0; i < list.GetLength(); i++)
			{
				stream << list.Get(i) << " ";
			}
			stream << "]";

			return stream;
		}
		catch (Exception e)
		{
			Logger::Trace("At operator overload<< at List.h");
			logException(e);
		}
		return stream;
	}

	bool isEmpty() const override { return size == 0; }
};
