#pragma once

#include "IContainer.h"
#include "Iterator.h"

template <typename T>
struct Node
{
public:
	T data;
	struct Node* next;
	struct Node* prev;

	friend std::ostream& operator<<(std::ostream& stream, Node<T>* node)
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
			throw EXCEPTION_INDEX_OUT_OF_RANGE;

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
	class Iterator;

	Iterator begin() noexcept
	{
		return (Iterator) this->head;
	}

	Iterator end() noexcept
	{
		return nullptr;
	}

	List()
	    : head(nullptr)
	    , tail(nullptr)
	    , size(0)
	{
		Logger::Info("Default constructor of List<T>");
	}

	List(const List<T>& other) noexcept
	    : size(0)
	{
		Logger::Info("Copied List<T>");
		for (Size i = 0; i < other.size; i++)
			Append(other.Get(i));
	}

	List(List<T>&& other) noexcept
	    : head(other.head)
	    , tail(other.tail)
	    , size(other.size)
	{
		Logger::Info("Moved List<T>");
		other.head = nullptr;
	}

	List(const T* data, Size count) noexcept
	    : size(0)
	{
		Logger::Info("Copied List<T> from array");
		for (Index i = 0; i < count; i++)
			Append(data[i]);

		size = count;
	}

	template <typename... Args>
	List(T head, Args... args)
	    : size(0)
	{
		Logger::Info("Parameter Pack constructor of List<T> of size %u", sizeof...(args) + 1);

		Append(head);

		((Append(args)), ...);
	}

	~List()
	{
		Logger::Info("Destoryed List<T>");
		if (isEmpty())
			return;

		Node<T>* pointer = this->head;

		while (pointer && pointer != this->tail->next)
		{
			Node<T>* prev = pointer;
			pointer = pointer->next;
			delete prev;
		}
		this->head = nullptr;
	}

	// Makes empty list
	void Clear() noexcept;

	/*
	 * Gets an instance of what is at the head.
	 * Throws int (Exception) if list is empty.
	 * */
	T& GetFirst() const;

	/*
	 * Gets an instance of what is at the tail.
	 * Throws int (Exception) if list is empty.
	 * */
	T& GetLast() const;

	/*
	 * Gets an instance of what is at the index.
	 * Throws int (Exception) if list is empty or index is greater or lower than size of list
	 * */
	T& Get(Index index) const override;

	/*
	 * Return pointer to node at the head of list. Throws int
	 * (Exception) if list is empty or there are no head definded.
	 * */
	Node<T>* GetHead() const;

	/*
	 * Return pointer to node at the tail of list. Throws int
	 * (Exception) if list is empty or there are no tail definded.
	 * */
	Node<T>* GetTail() const;

	Size GetLength() const override
	{
		return size;
	}

	/*
	 * Inserts exactly at the index inputed.
	 * So if we have 1, 2, 3 and insert to index 1 value 5 then we have
	 * 1, 5, 2, 3.
	 *
	 * Throws int (Exception) if index is negative (or zero) or if index is
	 * greater then size of list.
	 * */
	void InsertAt(Index index, const T& data);

	/*
	 * Removes data exactly at the index inputed.
	 * So if we have 1, 2, 3 and remove index 1 then we have
	 * 1, 3.
	 *
	 * Throws int (Exception) if index is negative (or zero) or if index is
	 * greater then size of list
	 * */
	void Remove(Index index);

	/*
	 * Adds data at the begining of list.
	 *
	 * Return pointer to itself.
	 * */
	List<T>* Append(const T& data) noexcept;

	/*
	 * Adds data at the end of list.
	 *
	 * Return pointer to itself.
	 * */
	List<T>* Prepend(const T& data);

	/*
	 * Gets an instance of what is at the index.
	 * Throws int (Exception) if list is empty or index is greater or lower than size of list
	 * */
	T& operator[](Index index) const
	{
		return Get(index);
	}

	/*
	 * Destroys old list and makes deep copy of other
	 * */
	List<T>& operator=(List<T>& other);

	/*
	 * Destroys old list and steals pointers from other
	 * */
	List<T>& operator=(List<T>&& other);

	bool isEmpty() const override
	{
		return size == 0;
	}

	void SetSize(Size newSize)
	{
		size = newSize;
	}

	friend std::ostream& operator<<(std::ostream& stream, const List<T>& list)
	{
		if (list.isEmpty())
		{
			Logger::Trace("At operator overload<< at List.h");
			logException(EXCEPTION_INDEX_OUT_OF_RANGE);
			throw EXCEPTION_BAD_CONTAINER;
		}

		stream << "[ ";
		for (Index i = 0; i < list.GetLength(); i++)
			stream << list.Get(i) << " ";
		stream << "]";

		return stream;
	}
};

template <typename T>
void List<T>::Clear() noexcept
{
	if (isEmpty())
		return;

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

template <typename T>
T& List<T>::GetFirst() const
{
	if (isEmpty())
	{
		Logger::Trace("At GetFirst() at List.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw EXCEPTION_INDEX_OUT_OF_RANGE;
	}
	return this->head->data;
}

template <typename T>
T& List<T>::GetLast() const
{
	if (isEmpty())
	{
		Logger::Trace("At GetLast() at List.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw EXCEPTION_INDEX_OUT_OF_RANGE;
	}
	return this->tail->data;
}

template <typename T>
T& List<T>::Get(const Index index) const
{
	if (index >= size || index < 0)
	{
		Logger::Trace("At Get() ai List.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
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
		Logger::Trace("At Get() ai List.h");
		logException(EXCEPTION_BAD_POINTER);
		throw EXCEPTION_BAD_POINTER;
	}

	return pointer->data;
}

template <typename T>
Node<T>* List<T>::GetHead() const
{
	if (isEmpty())
	{
		Logger::Trace("At GetHead() at List.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw EXCEPTION_INDEX_OUT_OF_RANGE;
	}

	if (!this->head)
	{
		Logger::Trace("At GetHead() at List.h");
		logException(EXCEPTION_BAD_POINTER);
		throw EXCEPTION_BAD_POINTER;
	}

	return this->head;
}

template <typename T>
Node<T>* List<T>::GetTail() const
{
	if (isEmpty())
	{
		Logger::Trace("At GetHead() at List.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw EXCEPTION_INDEX_OUT_OF_RANGE;
	}

	if (!this->tail)
	{
		Logger::Trace("At GetHead() at List.h");
		logException(EXCEPTION_BAD_POINTER);
		throw EXCEPTION_BAD_POINTER;
	}

	return this->tail;
}

template <typename T>
void List<T>::InsertAt(Index index, const T& data)
{
	if (index > size || index < 0)
	{
		Logger::Trace("At InsertAt(const Index, const T&) at List.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
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

template <typename T>
void List<T>::Remove(Index index)
{
	if (index > size || index < 0)
	{
		Logger::Trace("At Remove() at List.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
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

template <typename T>
List<T>* List<T>::Append(const T& data) noexcept
{
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

	return this;
}

template <typename T>
List<T>* List<T>::Prepend(const T& data)
{
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

	return this;
}

template <typename T>
List<T>& List<T>::operator=(List<T>& other)
{
	Logger::Info("Used copying operator = of List<T>");
	this->Clear();

	for (T data : other)
		Append(data);

	return *this;
}

template <typename T>
List<T>& List<T>::operator=(List<T>&& other)
{
	Logger::Info("Used moving operator = of List<T>");
	this->Clear();

	this->head = other.head;
	this->tail = other.tail;
	this->size = other.size;

	other.head = nullptr;

	return *this;
}

template <typename T>
class List<T>::Iterator : public AbstractIterator<T>
{
	Node<T>* current;

public:
	Iterator()
	    : current(nullptr)
	{
	}

	Iterator(List<T> list)
	    : current(list.head)
	{
	}

	Iterator(Node<T>* first)
	    : current(first)
	{
	}

	AbstractIterator<T>& operator++() override
	{
		current = current->next;
		return *this;
	}

	Iterator operator++(int)
	{
		Iterator temp = *this;
		(*this)++;
		return temp;
	}

	AbstractIterator<T>& operator--() override
	{
		current = current->prev;
		return *this;
	}

	Iterator operator--(int)
	{
		Iterator temp = *this;
		(*this)--;
		return temp;
	}

	bool operator!=(const Iterator& other) const
	{
		return this->current != other.current;
	}

	bool operator==(const Iterator& other) const
	{
		return this->current == other.current;
	}

	bool equal(const AbstractIterator<T>& other) const override
	{
		return &(this->current->data) == &(*other);
	}

	T& operator*() const override
	{
		return this->current->data;
	}
};
