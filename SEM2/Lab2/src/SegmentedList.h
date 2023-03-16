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
	public:
		Segment<Type>* prev;
		Segment<Type>* next;

		Node<Type>* head;
		Node<Type>* tail;

		Size size;

		const Size minSize = SEGMENT_MIN_SIZE;
		const Size normSize = SEGMENT_NORM_SIZE;
		const Size maxSize = SEGMENT_MAX_SIZE;

		class Iterator
		{
			Node<Type>* current;
		public:
			Iterator()
				: current(nullptr) {}

			Iterator(Segment* segment) : current(segment->head) {}

			Iterator(Node<Type>* first) : current(first) {}

			Iterator operator++ (int) { current = current->next; return *this; }
			Iterator operator-- (int) { current = current->prev; return *this; }
			Iterator operator++ () { current = current->next; return *this; }
			Iterator operator-- () { current = current->prev; return *this; }

			bool operator== (const Iterator& other) { return this->current == other.current; }
			bool operator!= (const Iterator& other) { return this->current != other.current; }

			Node<Type>* operator* () { return this->current; }
		};
		Iterator begin() { return (Iterator)this->head; }
		Iterator end() { return (Iterator)(this->tail->next); }

		Segment<Type>* operator++()
		{
			Segment<Type>* current = this;
			current = current->next;

			return current;
		}

		Segment<Type>* operator+ (int n)
		{
			Segment<T>* current = this;
			while (n != 0)
			{
				current++;
				n--;
			}
			return current;
		}
	};

	Segment<T>* head;
	Segment<T>* tail;
	Size size;

	bool SegmentIsEmpty(Segment<T>* segment) { return (segment) ? segment->size == 0 : true; }

	void AppendToSegment(Segment<T>* segment, const T& data)
	{
		Node<T>* pointer = new Node<T>;

		pointer->data = data;
		pointer->next = nullptr;
		pointer->prev = nullptr;

		if (isEmpty())
		{
			if (segment != this->head)
			{
				pointer->next = segment->prev->tail->next;
				segment->prev->tail->next = pointer;
				pointer->prev = segment->prev->tail;
			}
			if (segment != this->tail)
			{
				segment->next->head->prev = pointer;
				pointer->next = segment->next->head;
			}
			segment->head = pointer;
			segment->tail = pointer;
			segment->size = 1;
		}
		else
		{
			pointer->prev = segment->tail;
			pointer->next = segment->tail->next;
			segment->tail->next = pointer;
			segment->tail = pointer;
			segment->size += 1;
		}
	}

	void PrependToSegment(Segment<T>* segment, const T& data)
	{
		Node<T>* pointer = new Node<T>;

		pointer->data = data;
		pointer->next = nullptr;
		pointer->prev = nullptr;

		if (isEmpty())
		{
			if (segment != this->head)
			{
				segment->prev->tail->next = pointer;
				pointer->prev = segment->prev->tail;
			}
			if (segment != this->tail)
			{
				segment->next->tail->next = pointer;
				pointer->prev = segment->next->tail;
			}
			segment->head = pointer;
			segment->tail = pointer;
			segment->size += 1;
		}
		else
		{
			pointer->next = segment->head;
			pointer->prev = segment->head->prev;
			segment->head->prev = pointer;
			segment->head = pointer;
			segment->size += 1;
		}
	}

	T GetFromSegment(Segment<T>* segment, const Index index)
	{
		if (SegmentIsEmpty(segment)) return GetFirst();

		if (index >= segment->size || index < 0)
		{
			std::cout << "\nindexOutOfRange" << std::endl;
			throw;
		}

		Node<T>* pointer = segment->head;
		int counter = 0;
		while (counter < index)
		{
			counter++;
			pointer = pointer->next;
		}

		return pointer->data;
	}

	void RemoveFromSegment(Segment<T>* segment, Index index)
	{
		if (index > segment->size || index < 0)
		{
			std::cout << "\nindexOutOfRange" << std::endl;
			throw;
		}

		if (segment->size == 1 && segment->prev && segment->next)
		{
			this->tail->prev->next = this->tail->next;
			delete segment->head;
			delete segment;
			return;
		}

		if (index == segment->size - 1)
		{
			if (segment != this->tail)
			{
				segment->tail->prev->next = segment->next->head;
				segment->next->head->prev = segment->tail->prev;
				Node<T>* temp = segment->tail;
				if (segment->size != 1)
				{
					segment->tail = segment->tail->prev;
					delete temp;
					segment->size -= 1;
					return;
				}
				else
				{
					if (segment == this->head)
					{
						this->head = segment->next;
					}
					delete temp;
					delete segment;
					return;
				}
			}

			segment->tail = segment->tail->prev;
			delete segment->tail->next;
			segment->tail->next = nullptr;
			segment->size -= 1;

			return;
		}

		if (index == 0)
		{
			if (segment != this->head)
			{
				segment->head->next->prev = segment->prev->tail;
				segment->prev->tail->next = segment->head->next;
				Node<T>* temp = segment->head;
				if (segment->size != 1)
				{
					segment->head = segment->head->next;
					delete temp;
					segment->size -= 1;
					return;
				}
				else
				{
					if (segment == this->tail)
					{
						this->tail = segment->prev;
					}
					delete temp;
					delete segment;
					return;
				}
			}

			segment->head = segment->head->next;
			delete segment->head->prev;
			segment->head->prev = nullptr;
			segment->size -= 1;
			return;
		}

		Node<T>* node = segment->head;
		for (Size i = 0; i < index; i++) node = node->next;

		node->prev->next = node->next;
		node->next->prev = node->prev;
		delete node;
		segment->size -= 1;
	}

	Segment<T>* maxFrom(Segment<T>* segmentOne, Segment<T>* segmentTwo)
	{
		if (!segmentOne && segmentTwo) return segmentTwo;
		if (segmentOne && !segmentTwo) return segmentOne;
		if (!segmentOne && !segmentTwo) throw;
		return (segmentOne->size > segmentTwo->size) ? segmentOne : segmentTwo;
	}

	Segment<T>* minFrom(Segment<T>* segmentOne, Segment<T>* segmentTwo)
	{
		if (!segmentOne && segmentTwo) return segmentTwo;
		if (segmentOne && !segmentTwo) return segmentOne;
		if (!segmentOne && !segmentTwo) throw;
		return (segmentOne->size < segmentTwo->size) ? segmentOne : segmentTwo;
	}

	void ResizeSegments()
	{
		if (isEmpty()) return;

		Size segmentMeanNormalised = (size / SEGMENT_NORM_SIZE < 2) ? SEGMENT_MIN_SIZE : SEGMENT_NORM_SIZE;

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

public:

	class Iterator
	{
	private:

		Node<T>* current;

	public:

		Iterator()
			: current(nullptr) {}

		Iterator(Node<T>* first)
			: current(first) {}

		Iterator(SegmentedList<T>& list)
			: current(list.head->head) {}

		Iterator operator+ (int n)
		{
			Size counter = 0;
			while (counter != n) current = current->next, counter++;

			return *this;
		}

		Iterator operator- (int n)
		{
			Size counter = 0;
			while (counter != n) current = current->prev, counter++;

			return *this;
		}

		Iterator operator++ (int) { current = current->next; return *this; }
		Iterator operator-- (int) { current = current->prev; return *this; }
		Iterator operator++ () { current = current->next; return *this; }
		Iterator operator-- () { current = current->prev; return *this; }

		bool operator!= (const Iterator& other) { return this->current != other.current; }
		bool operator== (const Iterator& other) { return this->current == other.current; }

		T& operator* () { return this->current->data; }
	};
	Iterator begin() noexcept { return (Iterator)this->head->head; }
	Iterator end() noexcept { return (Iterator)this->tail->tail->next; }

	//default constructor
	SegmentedList()
		: head(nullptr), tail(nullptr), size(0) 
	{
		Logger::Info("Default constructor of SegmentedList<T>");
	}

	//copying constructor
	SegmentedList(const SegmentedList<T>& other)
		: size(0)
	{
		Logger::Info("Copied SegmentedList<T>");
		for (Size i = 0; i < other.size; i++)
		{
			Append(other.Get(i));
		}
	}

	//moving constructor
	SegmentedList(SegmentedList<T>&& other)
		: head(other.head), tail(other.tail), size(other.size)
	{
		Logger::Info("Moved SegmentedList<T>");
		other.head = nullptr;
	}

	//copying from array constructor
	SegmentedList(T* data, Size count)
		: size(0)
	{
		Logger::Info("Copied SegmentedList<T> from array");
		for (Size i = 0; i < count; i++)
		{
			Append(data[i]);
		}
	}

	//copying from List
	SegmentedList(const List<T>& other)
		: size(0)
	{
		Logger::Info("Copied SegmentedList<T> from List<T>");
		for (Size i = 0; i < other.GetLength(); i++)
		{
			Append(other.Get(i));
		}
	}

	//moving from List
	SegmentedList(List<T>&& other)
		: size(0)
	{
		Logger::Info("Moved SegmentedList<T> from List<T>");
		for (Size i = 0; i < other.GetLength(); i++)
		{
			Append(other.Get(i));
		}
	}

	~SegmentedList()
	{
		Logger::Info("Destroyed SegmentedList<T>");
		Clear();
	}

	void Clear()
	{
		if (isEmpty()) return;

		Segment<T>* segment = this->head;
		while (segment)
		{
			Node<T>* pointer = nullptr;
			if (segment->head)
			{
				pointer = segment->head->next;
			}

			while (pointer && pointer != segment->tail->next)
			{
				if (pointer->prev) delete pointer->prev, pointer->prev = nullptr;
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
			else delete segment, segment = nullptr;
		}

		this->head = nullptr;
		size = 0;
	}

	T GetFirst() const
	{
		try
		{
			if (isEmpty())
			{
				throw EXCEPTION_INDEX_OUT_OF_RANGE;
			}
			return this->head->head->data;
		}
		catch (Exception e)
		{
			Logger::Trace("At GetFirst() at SegmentedList.h");
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
			return this->tail->tail->data;
		}
		catch (Exception e)
		{
			Logger::Trace("At GetLast() at SegmentedList.h");
			logException(e);
			return 0;
		}
	}

	T Get(const Index index) const override
	{
		try
		{
			if (isEmpty())
			{
				throw EXCEPTION_INDEX_OUT_OF_RANGE;
			}

			if (index < 0 || index > this->size)
			{
				throw EXCEPTION_INDEX_OUT_OF_RANGE;
			}

			Segment<T>* segment = this->head;
			Index indexFromStart = 0;
			const Index maxIndex = size;

			while (segment)
			{
				Index indexLengthOfSegment = segment->size;

				if ((index >= maxIndex - indexFromStart && index <= indexFromStart + indexLengthOfSegment - 1) || (index < this->head->size))
				{
					Node<T>* current = segment->head;
					Index indexWithinSegment = index - indexFromStart;

					for (Index i = 0; i < indexWithinSegment; i++)
					{
						current = current->next;
					}
					return current->data;
				}
				indexFromStart += indexLengthOfSegment;
				segment = segment->next;
			}
			return GetFirst();
		}
		catch (Exception e)
		{
			Logger::Trace("At Get() at SegmentedList.h");
			logException(e);
			return GetFirst();
		}
	}

	Size GetLength() const override { return this->size; }

	//from an array of conditions
	Tuple<SegmentedList<T>, SegmentedList<T>> Split(const T* condition)
	{
		try
		{
			if (!condition)
			{
				throw EXCEPTION_BAD_POINTER;
			}

			Index index = 0;
			for (auto data : *this)
			{
				for (auto other : condition)
				{
					if (data == other && index != 0 && index != size - 1)
						return Tuple<SegmentedList<T>, SegmentedList<T>>(GetSublist(0, index - 1), GetSublist(index + 1, GetLength() - 1));
				}
				index++;
			}
			return Tuple<SegmentedList<T>, SegmentedList<T>>(*this, SegmentedList<T>());
		}
		catch (Exception e)
		{
			Logger::Trace("At Split(const T*) at SegmentedList.h");
			logException(e);
			return Tuple<SegmentedList<T>, SegmentedList<T>>(*this, SegmentedList<T>());
		}
	}

	//from one single condition
	Tuple<SegmentedList<T>, SegmentedList<T>> Split(const T& condition)
	{
		try
		{
			Index index = 0;
			for (auto data : *this)
			{
				if (data == condition && index != 0 && index != size - 1)
					return Tuple<SegmentedList<T>, SegmentedList<T>>(GetSublist(0, index), GetSublist(index + 1, GetLength()));

				index++;
			}

			return Tuple<SegmentedList<T>, SegmentedList<T>>(*this, SegmentedList<T>());
		}
		catch (Exception e)
		{
			Logger::Trace("At Split(const T&) at SegmentedList.h");
			logException(e);
			return Tuple<SegmentedList<T>, SegmentedList<T>>(*this, SegmentedList<T>());
		}
	}

	Node<T>* GetHead()
	{
		try
		{
			if (isEmpty())
			{
				throw EXCEPTION_INDEX_OUT_OF_RANGE;
			}

			return this->head->head;
		}
		catch (Exception e)
		{
			Logger::Trace("At GetHead() at SegmentedList.h");
			logException(e);
		}
	}

	Node<T>* GetTail()
	{
		try
		{
			if (isEmpty())
			{
				throw EXCEPTION_INDEX_OUT_OF_RANGE;
			}

			return this->tail->tail;
		}
		catch (Exception e)
		{
			Logger::Trace("At GetTail() at SegmentedList.h");
			logException(e);
		}
	}

	//by copying
	void insertAt(const Index index, const T& data)
	{
		try
		{
			if (index < 0 || index > this->size)
			{
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

				if ((index >= maxIndex - indexFromStart && index <= indexFromStart + indexLengthOfSegment - 1) || (index < this->head->size))
				{
					Node<T>* current = segment->head;
					Index indexWithinSegment = index - indexFromStart;

					for (Index i = 0; i < indexWithinSegment; i++)
					{
						current = current->next;
					}

					if (current == segment->head)
					{
						segment->head = node;
					}

					if (current == segment->tail)
					{
						segment->tail = node;
					}

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
		catch (Exception e)
		{
			Logger::Trace("At InsertAt(const Index, const T&) at SegmentedList.h");
			logException(e);
		}
	}

	//by moving
	void insertAt(const Index index, T&& data)
	{
		try
		{
			if (index < 0 || index > this->size)
			{
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

				if ((index >= maxIndex - indexFromStart && index <= indexFromStart + indexLengthOfSegment - 1) || (index < this->head->size))
				{
					Node<T>* current = segment->head;
					Index indexWithinSegment = index - indexFromStart;

					for (Index i = 0; i < indexWithinSegment; i++)
					{
						current = current->next;
					}

					if (current == segment->head)
					{
						segment->head = node;
					}

					if (current == segment->tail)
					{
						segment->tail = node;
					}

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
		catch (Exception e)
		{
			Logger::Trace("At InsertAt(const Index, T&&) at SegmentedList.h");
			logException(e);
		}
	}

	bool isEmpty() const override { return size == 0; }

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
		catch (Exception e)
		{
			Logger::Trace("At Append(const T&) at SegmentedList.h");
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
		catch (Exception e)
		{
			Logger::Trace("At Append(T&&) at SegmentedList.h");
			logException(e);
		}
	}

	//prepend by copying
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
		catch (Exception e)
		{
			Logger::Trace("At Prepend(const T&) at SegmentedList.h");
			logException(e);
		}
	}

	//prepend by moving
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
		catch (Exception e)
		{
			Logger::Trace("At Prepend(T&&) at SegmentedList.h");
			logException(e);
		}
	}

	SegmentedList<T> Concat(SegmentedList<T>& other)
	{
		try
		{
			if (other.isEmpty())
			{
				throw EXCEPTION_INDEX_OUT_OF_RANGE;
			}
			SegmentedList<T> list = SegmentedList(*this);

			for (auto data : other)
				list.Append(data);

			return list;
		}
		catch (Exception e)
		{
			Logger::Trace("At Concat() at SegmentedList.h");
			logException(e);
			return SegmentedList();
		}
	}

	SegmentedList<T> GetSublist(Index start, Index end)
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

			SegmentedList<T> list = SegmentedList();

			for (Size i = start; i < end; i++)
			{
				list.Append(Get(i));
			}

			return list;
		}
		catch (Exception e)
		{
			Logger::Trace("At GetSublist() at SegmentedList.h");
			logException(e);
			return SegmentedList();
		}
	}

	T operator[] (const Index index) { return Get(index); }

	//copying operator =
	SegmentedList<T>& operator= (const SegmentedList<T>& other)
	{
		Logger::Info("Used copying operator = of SegmentedList<T>");
		this->Clear();

		this->head = other.head;
		this->tail = other.tail;
		this->size = other.size;

		return *this;
	}

	//moving operator =
	SegmentedList<T>& operator= (SegmentedList<T>&& other)
	{
		Logger::Info("Used moving operator = of SegmentedList<T>");
		this->Clear();

		this->head = other.head;
		this->tail = other.tail;
		this->size = other.size;

		other.head = nullptr;

		return *this;
	}

	friend std::ostream& operator<< (std::ostream& stream, const SegmentedList<T>& list)
	{
		try
		{
			stream << "[ ";

			for (Index i = 0; i < list.GetLength(); i++)
			{
				stream << list.Get(i) << " ";
			}

			stream << "]";
		}
		catch (Exception e)
		{
			Logger::Trace("At operator overload<< at SegmentedList.h");
			logException(e);
		}

		return stream;
	}
};