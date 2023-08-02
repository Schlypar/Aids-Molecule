#pragma once

#include <iostream>
#include <memory>

#include "IContainer.h"
#include "List.h"

#define SEGMENT_MIN_SIZE 6
#define SEGMENT_NORM_SIZE 12
#define SEGMENT_MAX_SIZE 18

template <typename T>
class SegmentedList : IContainer<T>
{
private:
	template <typename Type>
	struct Segment
	{
		Segment<Type>* prev;
		Segment<Type>* next;

		Node<Type>* head;
		Node<Type>* tail;

		Size size;

		const Size minSize = SEGMENT_MIN_SIZE;
		const Size normSize = SEGMENT_NORM_SIZE;
		const Size maxSize = SEGMENT_MAX_SIZE;
	};

	Segment<T>* head;
	Segment<T>* tail;
	Size size;

public:
	class Iterator;

	Iterator begin() noexcept
	{
		return (Iterator) this->head->head;
	}

	Iterator end() noexcept
	{
		return (Iterator) this->tail->tail->next;
	}

	SegmentedList()
	    : head(nullptr)
	    , tail(nullptr)
	    , size(0)
	{
		Logger::Info("Default constructor of SegmentedList<T>");
	}

	SegmentedList(const SegmentedList<T>& other)
	    : size(0)
	{
		Logger::Info("Copied SegmentedList<T>");
		for (Size i = 0; i < other.size; i++)
			Append(other.Get(i));
	}

	SegmentedList(SegmentedList<T>&& other)
	    : head(other.head)
	    , tail(other.tail)
	    , size(other.size)
	{
		Logger::Info("Moved SegmentedList<T>");
		other.head = nullptr;
		other.size = 0;
	}

	SegmentedList(const T* data, Size count)
	    : size(0)
	{
		Logger::Info("Copied SegmentedList<T> from array");
		for (Size i = 0; i < count; i++)
			Append(data[i]);
	}

	SegmentedList(const List<T>& other)
	    : size(0)
	{
		Logger::Info("Copied SegmentedList<T> from List<T>");
		for (Size i = 0; i < other.GetLength(); i++)
			Append(other.Get(i));
	}

	SegmentedList(List<T>&& other)
	    : size(0)
	{
		Logger::Info("Moved SegmentedList<T> from List<T>");
		for (Size i = 0; i < other.GetLength(); i++)
			Append(std::move(other.Get(i)));

		other.Clear();
	}

	template <typename... Args>
	SegmentedList(T head, Args... args)
	    : size(0)
	{
		Logger::Info("Parameter Pack constructor of List<T> of size %u", sizeof...(args) + 1);

		Append(head);

		((Append(args)), ...);
	}

	~SegmentedList()
	{
		Logger::Info("Destroyed SegmentedList<T>");
		Clear();
	}

	// Degrades list to initial state (empty list)
	void Clear();

	/*
	 * Return an instance of what is at the head
	 * */
	T& GetFirst() const;

	/*
	 * Return an instance of what is at the tail
	 * */
	T& GetLast() const;

	/*
	 * Returns an instance of what is ath the index
	 * */
	T& Get(const Index index) const override;

	Size GetLength() const override
	{
		return this->size;
	}

	// Returns poitner to the head node
	Node<T>* GetHead() const
	{
		return (this->head) ? this->head->head : nullptr;
	}

	// Returns poitner to the tail node
	Node<T>* GetTail() const
	{
		return (this->tail) ? this->tail->tail : nullptr;
	}

	/*
	 * Inserts exactly at the index inputed.
	 * */
	void insertAt(Index index, const T& data);

	bool isEmpty() const override
	{
		return size == 0;
	}

	void Append(const T& data);

	void Prepend(const T& data);

	/*
	 * Returns an instance of what is ath the index
	 * */
	T& operator[](const Index index)
	{
		return Get(index);
	}

	/*
	 * Destroys old list and makes deep copy of other
	 * */
	SegmentedList<T>& operator=(const SegmentedList<T>& other);

	/*
	 * Destroys old list and steals pointer from another
	 * */
	SegmentedList<T>& operator=(SegmentedList<T>&& other);

	/*
	 * Two lists are equal if they are the same size and
	 * have the same elements in the same order
	 * */
	bool operator==(const SegmentedList<T>& other);

	void SetSize(Size newSize)
	{
		this->size = newSize;
	}

	friend std::ostream& operator<<(std::ostream& stream, const SegmentedList<T>& list)
	{
		stream << "[ ";

		for (Index i = 0; i < list.GetLength(); i++)
			stream << list.Get(i) << " ";

		stream << "]";

		return stream;
	}

private:
	bool SegmentIsEmpty(Segment<T>* segment)
	{
		return (segment) ? segment->size == 0 : true;
	}

	/*
	 * Returns segment that has greater size.
	 * If sizes are equal then returns left (segmentOne)
	 * */
	Segment<T>* maxFrom(Segment<T>* segmentOne, Segment<T>* segmentTwo);

	/*
	 * Returns segment that has smaller size.
	 * If sizes are equal then returns left (segmentOne)
	 * */
	Segment<T>* minFrom(Segment<T>* segmentOne, Segment<T>* segmentTwo);

	/*
	 * If one segment becomes too small (determined by the parameter in the segment itself)
	 * then we take elements from the bigger neighbour and transef them to the current.
	 *
	 * If one segment becomes too big (determined by the parameter in the segment itself)
	 * then we give elements from it to smallest neighbour and transfer them to the smaller one.
	 * */
	void ResizeSegments();
};

template <typename T>
void SegmentedList<T>::Clear()
{
	if (isEmpty())
		return;

	Segment<T>* segment = this->head;
	while (segment)
	{
		Node<T>* pointer = nullptr;
		if (segment->head)
			pointer = segment->head->next;

		while (pointer && pointer != segment->tail->next)
		{
			if (pointer->prev)
				delete pointer->prev, pointer->prev = nullptr;
			pointer = pointer->next;
		}

		if (segment->tail)
		{
			delete (segment->tail);
			segment->tail = nullptr;
		}

		if (segment->next)
		{
			segment = segment->next;
			segment->prev->size = 0;
			delete segment->prev;
			segment->prev = nullptr;
		}
		else
			delete segment, segment = nullptr;
	}

	this->head = nullptr;
	size = 0;
}

template <typename T>
T& SegmentedList<T>::GetFirst() const
{
	if (isEmpty())
	{
		Logger::Trace("At GetFirst() at SegmentedList.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw EXCEPTION_INDEX_OUT_OF_RANGE;
	}
	return this->head->head->data;
}

template <typename T>
T& SegmentedList<T>::GetLast() const
{
	if (isEmpty())
	{
		Logger::Trace("At GetLast() at SegmentedList.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw EXCEPTION_INDEX_OUT_OF_RANGE;
	}
	return this->tail->tail->data;
}

template <typename T>
T& SegmentedList<T>::Get(const Index index) const
{
	if (isEmpty())
	{
		Logger::Trace("At Get() at SegmentedList.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw EXCEPTION_INDEX_OUT_OF_RANGE;
	}

	if (index < 0 || index >= this->size)
	{
		Logger::Trace("At Get() at SegmentedList.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw EXCEPTION_INDEX_OUT_OF_RANGE;
	}

	Segment<T>* segment = this->head;
	Index indexFromStart = 0;
	const Index maxIndex = size;

	while (segment)
	{
		Index indexLengthOfSegment = segment->size;

		if ((index >= indexFromStart && index <= indexFromStart + indexLengthOfSegment - 1) || (index < this->head->size))
		{
			Node<T>* current = segment->head;
			Index indexWithinSegment = index - indexFromStart;

			for (Index i = 0; i < indexWithinSegment; i++)
				current = current->next;

			return current->data;
		}
		indexFromStart += indexLengthOfSegment;
		segment = segment->next;
	}
	return GetFirst();
}

template <typename T>
void SegmentedList<T>::insertAt(Index index, const T& data)
{
	if (index <= 0 || index > this->size)
	{
		Logger::Trace("At InsertAt(const Index, const T&) at SegmentedList.h");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw EXCEPTION_INDEX_OUT_OF_RANGE;
	}

	Node<T>* node = new Node<T>;
	node->data = data;
	node->next = nullptr;
	node->prev = nullptr;

	Segment<T>* segment = this->head;
	Index indexFromStart = 0;
	const Index maxIndex = size;

	while (segment)
	{
		Index indexLengthOfSegment = segment->size;

		if ((index >= maxIndex - indexFromStart && index <= indexFromStart + indexLengthOfSegment - 1)
			|| (index < this->head->size))
		{
			Node<T>* current = segment->head;
			Index indexWithinSegment = index - indexFromStart;

			for (Index i = 0; i < indexWithinSegment; i++)
				current = current->next;

			if (current == segment->head)
				segment->head = node;

			if (current == segment->tail)
				segment->tail = node;

			current->prev->next = node;
			node->prev = current->prev;

			node->next = current;
			current->prev = node;

			segment->size += 1;
			size++;

			ResizeSegments();
			return;
		}
		indexFromStart += indexLengthOfSegment;
		segment = segment->next;
	}
}

template <typename T>
void SegmentedList<T>::Append(const T& data)
{
	Node<T>* pointer = new Node<T>;

	pointer->data = data;
	pointer->next = nullptr;
	pointer->prev = nullptr;

	if (isEmpty())
	{
		Segment<T>* segment = new Segment<T>;

		segment->prev = nullptr;
		segment->next = nullptr;

		segment->head = pointer;
		segment->tail = pointer;

		this->head = segment;
		this->tail = segment;

		segment->size = 1;
		size++;
	}
	else if (this->tail->size < SEGMENT_NORM_SIZE)
	{
		Node<T>* segmentTail = this->tail->tail;

		segmentTail->next = pointer;
		pointer->prev = segmentTail;

		this->tail->tail = pointer;

		this->tail->size += 1;
		size++;
	}
	else
	{
		Segment<T>* segment = new Segment<T>;
		this->tail->next = segment;

		segment->prev = this->tail;
		segment->next = nullptr;

		segment->head = pointer;
		segment->tail = pointer;

		this->tail->tail->next = pointer;
		pointer->prev = this->tail->tail;

		this->tail = segment;

		segment->size = 1;
		size++;
	}

	ResizeSegments();
}

template <typename T>
void SegmentedList<T>::Prepend(const T& data)
{
	Node<T>* pointer = new Node<T>;

	pointer->data = data;
	pointer->next = nullptr;
	pointer->prev = nullptr;

	if (isEmpty())
	{
		Segment<T>* segment = new Segment<T>;

		segment->prev = nullptr;
		segment->next = nullptr;

		segment->head = pointer;
		segment->tail = pointer;

		this->head = segment;
		this->tail = segment;

		segment->size = 1;
		size++;
	}
	else if (this->head->size < SEGMENT_NORM_SIZE)
	{
		Node<T>* segmentHead = this->head->head;

		segmentHead->prev = pointer;
		pointer->next = segmentHead;

		this->head->head = pointer;

		this->head->size += 1;
		size++;
	}
	else
	{
		Segment<T>* segment = new Segment<T>;
		this->head->prev = segment;

		segment->prev = nullptr;
		segment->next = this->head;

		segment->head = pointer;
		segment->tail = pointer;

		this->head->head->prev = pointer;
		pointer->next = this->head->head;

		this->head = segment;

		segment->size = 1;
		size++;
	}

	ResizeSegments();
}

template <typename T>
SegmentedList<T>& SegmentedList<T>::operator=(const SegmentedList<T>& other)
{
	Logger::Info("Used copying operator = of SegmentedList<T>");
	this->Clear();

	for (Index i = 0; i < other.GetLength(); i++)
		Append(other.Get(i));

	return *this;
}

template <typename T>

SegmentedList<T>& SegmentedList<T>::operator=(SegmentedList<T>&& other)
{
	Logger::Info("Used moving operator = of SegmentedList<T>");
	this->Clear();

	this->head = other.head;
	this->tail = other.tail;
	this->size = other.size;

	other.head = nullptr;
	other.tail = nullptr;
	other.size = 0;

	return *this;
}

template <typename T>
bool SegmentedList<T>::operator==(const SegmentedList<T>& other)
{
	if (isEmpty() && other.isEmpty())
		return true;

	if (GetLength() != other.GetLength())
		return false;

	for (Index i = 0; i < GetLength(); i++)
		if (Get(i) != other.Get(i))
			return false;

	return true;
}

template <typename T>
typename SegmentedList<T>::template Segment<T>* SegmentedList<T>::maxFrom(SegmentedList<T>::Segment<T>* segmentOne,
	SegmentedList<T>::Segment<T>* segmentTwo)
{
	if (!segmentOne && segmentTwo)
		return segmentTwo;
	if (segmentOne && !segmentTwo)
		return segmentOne;
	if (!segmentOne && !segmentTwo)
		throw;
	return (segmentOne->size > segmentTwo->size) ? segmentOne : segmentTwo;
}

template <typename T>
typename SegmentedList<T>::template Segment<T>* SegmentedList<T>::minFrom(SegmentedList<T>::Segment<T>* segmentOne,
	SegmentedList<T>::Segment<T>* segmentTwo)
{
	if (!segmentOne && segmentTwo)
		return segmentTwo;
	if (segmentOne && !segmentTwo)
		return segmentOne;
	if (!segmentOne && !segmentTwo)
		throw;
	return (segmentOne->size < segmentTwo->size) ? segmentOne : segmentTwo;
}

template <typename T>
void SegmentedList<T>::ResizeSegments()
{
	if (isEmpty())
		return;

	Size segmentMeanNormalised = (size / SEGMENT_NORM_SIZE < 2) ? SEGMENT_MIN_SIZE : SEGMENT_NORM_SIZE - SEGMENT_MIN_SIZE;

	Segment<T>* current = this->head;
	while (current)
	{
		if (current->size < SEGMENT_MIN_SIZE)
		{
			Size diffrence = segmentMeanNormalised - current->size;
			if (current == this->head && current->next)
			{
				for (Index i = 0; i < diffrence && !SegmentIsEmpty(current->next); i++)
				{
					current->tail = current->next->head;
					current->next->head = current->tail->next;
					current->size += 1;
					current->next->size -= 1;
				}
				ResizeSegments();
			}
			else if (current == this->tail && current->prev)
			{
				for (Index i = 0; i < diffrence && !SegmentIsEmpty(current->prev); i++)
				{
					current->head = current->prev->tail;
					current->prev->tail = current->head->prev;
					current->size += 1;
					current->prev->size -= 1;
				}
				ResizeSegments();
			}
			else if (current->next || current->prev)
			{
				for (Index i = 0; i < diffrence && !SegmentIsEmpty(maxFrom(current->prev, current->next)); i++)
				{
					Segment<T>* maximum = maxFrom(current->prev, current->next);
					if (maximum == current->prev)
					{
						current->head = maximum->tail;
						maximum->tail = current->head->prev;
						current->size += 1;
						maximum->size -= 1;
					}
					else
					{
						current->tail = maximum->head;
						maximum->head = current->tail->next;
						current->size += 1;
						maximum->size -= 1;
					}
				}
				ResizeSegments();
			}
		}
		else if (current->size > SEGMENT_MAX_SIZE && current)
		{
			Size diffrence = current->size - segmentMeanNormalised;
			if (current == this->head && current->next)
			{
				for (Index i = 0; i < diffrence && !SegmentIsEmpty(current->next); i++)
				{
					current->next->head = current->tail;
					current->tail = current->tail->prev;
					current->size -= 1;
					current->next->size += 1;
				}
				ResizeSegments();
			}
			else if (current == this->tail && current->prev)
			{
				for (Index i = 0; i < diffrence && !SegmentIsEmpty(current->prev); i++)
				{
					current->prev->tail = current->head;
					current->head = current->head->next;
					current->size -= 1;
					current->prev->size += 1;
				}
				ResizeSegments();
			}
			else if (current->prev || current->next)
			{
				for (Index i = 0; i < diffrence && !SegmentIsEmpty(maxFrom(current->prev, current->next)); i++)
				{
					Segment<T>* minimum = minFrom(current->prev, current->next);
					if (minimum == current->prev)
					{
						minimum->tail = current->head;
						current->head = current->head->next;
						current->size -= 1;
						minimum->size += 1;
					}
					else
					{
						minimum->head = current->tail;
						current->tail = current->tail->prev;
						current->size -= 1;
						minimum->size += 1;
					}
				}
				ResizeSegments();
			}
		}
		current = current->next;
	}
}

template <typename T>
class SegmentedList<T>::Iterator
{
private:
	Node<T>* current;

public:
	Iterator()
	    : current(nullptr)
	{
	}

	Iterator(Node<T>* first)
	    : current(first)
	{
	}

	Iterator(SegmentedList<T>& list)
	    : current(list.head->head)
	{
	}

	Iterator operator+(int n)
	{
		Size counter = 0;
		while (counter != n)
			current = current->next, counter++;

		return *this;
	}

	Iterator operator-(int n)
	{
		Size counter = 0;
		while (counter != n)
			current = current->prev, counter++;

		return *this;
	}

	Iterator operator++(int)
	{
		current = current->next;
		return *this;
	}

	Iterator operator--(int)
	{
		current = current->prev;
		return *this;
	}

	Iterator operator++()
	{
		current = current->next;
		return *this;
	}

	Iterator operator--()
	{
		current = current->prev;
		return *this;
	}

	bool operator!=(const Iterator& other)
	{
		return this->current != other.current;
	}

	bool operator==(const Iterator& other)
	{
		return this->current == other.current;
	}

	T& operator*()
	{
		return this->current->data;
	}
};
