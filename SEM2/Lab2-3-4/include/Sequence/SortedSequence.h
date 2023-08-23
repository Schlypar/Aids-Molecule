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
	SortedSequence()
	    : sequence(nullptr)
	    , sorter(nullptr)
	    , comparator(nullptr)
	{
	}

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

	/**
	 * @brief sets member sequence to the sequence given. Will allocate additional memory with complexity that depends on
	 * ISorter. Will throw error if sorter or comparator is nullptr
	 *
	 * @param sequence pointer to the abstract sequence
	 */
	SortedSequence<T>* SetSequence(Sequence<T>* sequence);

	/**
	 * @brief sets member sorter to the sorter given. Function does not make deep copy and free any memory
	 *
	 * @param sorter pointer to the abstract sorter
	 */
	SortedSequence<T>* SetSorter(ISorter<T>* sorter);

	/**
	 * @brief sets member comparator to the comparator given
	 *
	 * @param comparator function by which to compare two elements
	 */
	SortedSequence<T>* SetComparator(std::function<int(const T&, const T&)> comparator);

	/**
	 * @brief calculates how much elements in the sequence. Throws exception if sequence pointer is nullptr
	 *
	 * @return length of the sequence
	 */
	int GetLength() const;

	/**
	 * @brief O(1) check if sequence is empty. Throws exception if sequence pointer is nullptr
	 *
	 * @return return true or false whether sequence is empty or not
	 */
	bool isEmpty() const;

	/**
	 * @brief returns element at the positoin index. Throws exception if index out of bounds. Throws exception if sequence
	 * pointer is nullptr
	 *
	 * @param index at which position to look for
	 * @return copy of the element at the index
	 */
	T Get(int index) const;

	/**
	 * @brief O(1) operation that returns element at the begining of the sequence. Throws exception if sequence pointer is
	 * nullptr
	 *
	 * @return copy of the first element
	 */
	T GetFirst() const;

	/**
	 * @brief O(1) operation that return element at the end of the sequence. Throws exception if sequence pointer is nullptr
	 *
	 * @return copy of the last element
	 */
	T GetLast() const;

	/**
	 * @brief finds index of element. Returns -1 if element was not found. Throws exception if sequence pointer is nullptr
	 *
	 * @param element for which element to look for
	 * @return returns index of the given element
	 */
	int IndexOf(const T& element) const;

	/**
	 * @brief constructs new sequence. Both start and end are inclusive. Throws exception if indecies are out of bounds or if
	 * sequence pointer is nullptr
	 *
	 * @param startIndex at which index to start
	 * @param endIndex ath which index to end
	 * @return returns new SortedSequence
	 */
	SortedSequence<T> GetSubsequence(int startIndex, int endIndex) const;

	/**
	 * @brief adds element to the sequence so the sequence is still sorted. Throws exception if sequence or comparator is
	 * pointer is nullptr
	 *
	 * @param element what to add to the sequence
	 */
	void Add(const T& element);

	friend std::ostream& operator<<(std::ostream& stream, const SortedSequence<T>& sequence)
	{
		stream << sequence.sequence;

		return stream;
	}
};

template <typename T>
SortedSequence<T>* SortedSequence<T>::SetSequence(Sequence<T>* sequence)
{
	if (sorter == nullptr || comparator == nullptr)
		throw std::invalid_argument("Sorter member or Comparator member was nullptr");

	Sequence<T>* sorted = sorter->Sort(sequence, comparator);
	delete sequence;

	this->sequence = sorted;

	return this;
}

template <typename T>
SortedSequence<T>* SortedSequence<T>::SetSorter(ISorter<T>* sorter)
{
	this->sorter = sorter;

	return this;
}

template <typename T>
SortedSequence<T>* SortedSequence<T>::SetComparator(std::function<int(const T&, const T&)> comparator)
{
	this->comparator = comparator;

	return this;
}

template <typename T>
int SortedSequence<T>::GetLength() const
{
	if (sequence == nullptr)
		throw std::invalid_argument("Sequence was nulltpr");

	return sequence->GetLength();
}

template <typename T>
bool SortedSequence<T>::isEmpty() const
{
	if (this->sequence == nullptr)
		throw std::invalid_argument("sequence was nullptr");

	return sequence->isEmpty();
}

template <typename T>
T SortedSequence<T>::Get(int index) const
{
	if (this->sequence == nullptr)
		throw std::invalid_argument("sequence was nullptr");

	return sequence->Get(index);
}

template <typename T>
T SortedSequence<T>::GetFirst() const
{
	if (this->sequence == nullptr)
		throw std::invalid_argument("sequence was nullptr");

	return sequence->GetFirst();
}

template <typename T>
T SortedSequence<T>::GetLast() const
{
	if (this->sequence == nullptr)
		throw std::invalid_argument("sequence was nullptr");

	return sequence->GetLast();
}

template <typename T>
int SortedSequence<T>::IndexOf(const T& element) const
{
	if (this->sequence == nullptr)
		throw std::invalid_argument("sequence was nullptr");

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
	if (this->sequence == nullptr)
		throw std::invalid_argument("sequence was nullptr");

	return SortedSequence<T>()
		.SetSorter(sorter->Copy())
		->SetComparator(comparator)
		->SetSequence(sequence->GetSubsequence(startIndex, endIndex));
}

template <typename T>
void SortedSequence<T>::Add(const T& element)
{
	if (this->sequence == nullptr || this->comparator == nullptr)
		throw std::invalid_argument("sequence or comparator was nullptr");

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
