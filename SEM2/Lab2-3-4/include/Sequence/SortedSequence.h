#pragma once

#include "Sequence.h"
#include "Sorter/ISorter.h"
#include <functional>
#include <ostream>
#include <pstl/glue_execution_defs.h>
#include <stdexcept>
#include <variant>

template <typename T>
class SortedSequence
{
private:
	Sequence<T>* sequence;
	ISorter<T>* sorter;
	std::function<int(const T&, const T&)> comparator;

public:
	using Iterator = Sequence<T>::Iterator;

	template <typename U>
	friend class SortedSequenceFactory;

	SortedSequence(Sequence<T>* sequence, ISorter<T>* sorter, std::function<int(const T&, const T&)> comparator)
	    : sequence(sequence)
	    , sorter(sorter)
	    , comparator(comparator)
	{
	}

	SortedSequence(const SortedSequence<T>& other)
	    : sequence(other.sequence->Copy())
	    , sorter(other.sorter->Copy())
	    , comparator(other.comparator)
	{
	}

	SortedSequence(SortedSequence<T>&& other)
	    : sequence(other.sequence)
	    , sorter(other.sorter)
	    , comparator(other.comparator)
	{
		other.sequence = nullptr;
		other.sorter = nullptr;
	}

	~SortedSequence()
	{
		delete sequence;
		delete sorter;
	}

	Iterator begin()
	{
		return this->sequence->begin();
	}

	Iterator end()
	{
		return this->sequence->end();
	}

	SortedSequence<T> operator|(fn::filter<T> filter)
	{
		SortedSequence<T> result;

		for (T& data : *this)
			if (filter._filter(data))
				result.Append(data);

		return result;
	}

	SortedSequence<T> operator|(fn::transformer<T> transformer)
	{
		SortedSequence<T> result;

		for (T& data : *this)
			result.Append(transformer._transformer(data));

		return result;
	}

	/**
	 * @brief calculates how much elements in the sequence.
	 *
	 * @return length of the sequence
	 */
	int GetLength() const;

	/**
	 * @brief O(1) check if sequence is empty.
	 *
	 * @return return true or false whether sequence is empty or not
	 */
	bool isEmpty() const;

	/**
	 * @brief returns element at the positoin index.
	 * pointer is nullptr
	 *
	 * @param index at which position to look for
	 * @return copy of the element at the index
	 */
	T Get(int index) const;

	/**
	 * @brief O(1) operation that returns element at the begining of the sequence.
	 * nullptr
	 *
	 * @return copy of the first element
	 */
	T GetFirst() const;

	/**
	 * @brief O(1) operation that return element at the end of the sequence.
	 *
	 * @return copy of the last element
	 */
	T GetLast() const;

	/**
	 * @brief finds index of element. Returns -1 if element was not found.
	 *
	 * @param element for which element to look for
	 * @return returns index of the given element
	 */
	int IndexOf(const T& element) const;

	/**
	 * @brief constructs new sequence. Both start and end are inclusive. Throws exception if indecies are out of bounds
	 *
	 * @param startIndex at which index to start
	 * @param endIndex ath which index to end
	 * @return returns new SortedSequence
	 */
	SortedSequence<T> GetSubsequence(int startIndex, int endIndex) const;

	/**
	 * @brief adds element to the sequence so the sequence is still sorted.
	 *
	 * @param element what to add to the sequence
	 */
	void Add(const T& element);

	friend std::ostream& operator<<(std::ostream& stream, const SortedSequence<T>& sequence)
	{
		stream << sequence.sequence;

		return stream;
	}

private:
	SortedSequence()
	    : sequence(nullptr)
	    , sorter(nullptr)
	    , comparator(nullptr)
	{
	}

	/**
	 * @brief sets member sequence to the sequence given. Will allocate additional memory with complexity that depends on
	 * ISorter
	 *
	 * @param sequence pointer to the abstract sequence
	 */
	void SetSequence(Sequence<T>* sequence);

	/**
	 * @brief sets member sorter to the sorter given. Function does not make deep copy and free any memory
	 *
	 * @param sorter pointer to the abstract sorter
	 */
	void SetSorter(ISorter<T>* sorter);

	/**
	 * @brief sets member comparator to the comparator given
	 *
	 * @param comparator function by which to compare two elements
	 */
	void SetComparator(std::function<int(const T&, const T&)> comparator);
};

template <typename T>
void SortedSequence<T>::SetSequence(Sequence<T>* sequence)
{
	this->sequence = sequence;
}

template <typename T>
void SortedSequence<T>::SetSorter(ISorter<T>* sorter)
{
	this->sorter = sorter;
}

template <typename T>
void SortedSequence<T>::SetComparator(std::function<int(const T&, const T&)> comparator)
{
	this->comparator = comparator;
}

template <typename T>
int SortedSequence<T>::GetLength() const
{
	return sequence->GetLength();
}

template <typename T>
bool SortedSequence<T>::isEmpty() const
{
	return sequence->isEmpty();
}

template <typename T>
T SortedSequence<T>::Get(int index) const
{
	return sequence->Get(index);
}

template <typename T>
T SortedSequence<T>::GetFirst() const
{
	return sequence->GetFirst();
}

template <typename T>
T SortedSequence<T>::GetLast() const
{
	return sequence->GetLast();
}

template <typename T>
int SortedSequence<T>::IndexOf(const T& element) const
{
	int index = 0;

	for (auto& e : *this->sequence)
		if (e == element)
			return index;
		else
			index++;

	return -1;
}

template <typename T>
SortedSequence<T> SortedSequence<T>::GetSubsequence(int startIndex, int endIndex) const
{
	return SortedSequence<T>()
		.SetSorter(sorter->Copy())
		->SetComparator(comparator)
		->SetSequence(sequence->GetSubsequence(startIndex, endIndex));
}

template <typename T>
void SortedSequence<T>::Add(const T& element)
{
	if (comparator(element, sequence->GetFirst()) <= 0)
	{
		sequence->Prepend(element);
		return;
	}

	if (comparator(element, sequence->GetFirst()) >= 0)
	{
		sequence->Append(element);
		return;
	}

	auto cur = sequence->begin();
	auto end = sequence->end();

	while (element > *cur && cur != end)
		cur++;

	sequence->InsertAt(this->IndexOf(*cur), element);
}

template <typename U>
class SortedSequenceFactory
{
private:
	SortedSequence<U> sequence;

public:
	SortedSequenceFactory()
	    : sequence()
	{
	}

	SortedSequenceFactory<U>* SetSequence(Sequence<U>* sequence)
	{
		this->sequence.SetSequence(sequence);

		return this;
	}

	SortedSequenceFactory<U>* SetSorter(ISorter<U>* sorter)
	{
		this->sequence.SetSorter(sorter);

		return this;
	}

	SortedSequenceFactory<U>* SetComparator(std::function<int(const U&, const U&)> comparator)
	{
		this->sequence.SetComparator(comparator);

		return this;
	}

	SortedSequence<U> Build()
	{
		if (this->sequence.sequence == nullptr)
			throw std::invalid_argument("Sequence is nullptr");

		if (this->sequence.sorter == nullptr)
			throw std::invalid_argument("Sorter is nullptr");

		if (this->sequence.comparator == nullptr)
			throw std::invalid_argument("Comparator is nullptr");

		auto tmp = this->sequence.sequence;
		this->sequence.sequence = this->sequence.sorter->Sort(tmp, this->sequence.comparator);
		delete tmp;

		return this->sequence;
	}
};
