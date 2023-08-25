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

	SortedSequence<T>& operator=(const SortedSequence<T>& other)
	{
		delete this->sequence;
		delete this->sorter;

		this->sequence = other.sequence->Copy();
		this->sorter = other.sorter->Copy();
		this->comparator = other.comparator;

		return *this;
	}

	SortedSequence<T>& operator=(SortedSequence<T>&& other)
	{
		if (this->sequence != nullptr)
		{
			delete this->sequence;
		}

		if (this->sorter != nullptr)
		{
			delete this->sorter;
		}

		this->sequence = other.sequence;
		this->sorter = other.sorter;
		this->comparator = other.comparator;

		other.sequence = nullptr;
		other.sorter = nullptr;

		return *this;
	}

	Iterator begin()
	{
		return this->sequence->begin();
	}

	Iterator end()
	{
		return this->sequence->end();
	}

	SortedSequence<T> operator|(fn::filter<T> filter);

	SortedSequence<T> operator|(fn::transformer<T> transformer);

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
	 *
	 * @param index at which position to look for
	 * @return reference of the element at the index
	 */
	T& Get(int index) const;

	T& operator[](int index) const
	{
		return Get(index);
	}

	/**
	 * @brief O(1) operation that returns element at the begining of the sequence.
	 *
	 * @return reference of the first element
	 */
	T& GetFirst() const;

	/**
	 * @brief O(1) operation that return element at the end of the sequence.
	 *
	 * @return reference of the last element
	 */
	T& GetLast() const;

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

	/**
     * @brief removes element at index from sequence
     *
     * @param index at what index to remove element from
     */
	void Remove(int index);

	/**
     * @brief removes element if its there. Throws exception if element is not in sequence
     *
     * @param element what to delete
     */
	void RemoveByValue(const T& element);

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
SortedSequence<T> SortedSequence<T>::operator|(fn::filter<T> filter)
{
	SortedSequence<T> result;
	result.sequence = this->sequence->Create();
	result.comparator = this->comparator;
	result.sorter = this->sorter->Copy();

	auto cur = begin();
	auto next = begin()++;

	for (T& data : *this)
	{
		if (filter._filter(data))
		{
			result.sequence->Append(data);
		}
	}

	return result;
}

template <typename T>
SortedSequence<T> SortedSequence<T>::operator|(fn::transformer<T> transformer)
{
	SortedSequence<T> result;
	result.sequence = this->sequence->Create();
	result.comparator = this->comparator;
	result.sorter = this->sorter->Copy();

	for (T& data : *this)
	{
		result.sequence->Append(transformer._transformer(data));
	}

	return result;
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
T& SortedSequence<T>::Get(int index) const
{
	return sequence->Get(index);
}

template <typename T>
T& SortedSequence<T>::GetFirst() const
{
	return sequence->GetFirst();
}

template <typename T>
T& SortedSequence<T>::GetLast() const
{
	return sequence->GetLast();
}

template <typename T>
int SortedSequence<T>::IndexOf(const T& element) const
{
	int index = 0;

	for (auto& e : *this->sequence)
	{
		if (e == element)
		{
			return index;
		}
		else
		{
			index++;
		}
	}

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
	if (isEmpty())
	{
		this->sequence->Append(element);
		return;
	}

	// std::cout << element << " VS " << sequence->GetFirst() << " AND GET LAST IS " << sequence->GetLast() << '\n';
	// std::cout << comparator(element, element) << "\n";
	// std::cout << comparator(element, sequence->GetLast()) << "\n";

	if (comparator(element, sequence->GetFirst()) <= 0)
	{
		sequence->Prepend(element);
		return;
	}

	if (comparator(element, sequence->GetLast()) >= 0)
	{
		sequence->Append(element);
		return;
	}

	auto cur = sequence->begin();
	auto end = sequence->end();

	while (comparator(element, *cur) > 0 && cur != end)
	{
		cur++;
	}

	sequence->InsertAt(this->IndexOf(*cur), element);
}

template <typename T>
void SortedSequence<T>::Remove(int index)
{
	this->sequence->Remove(index);
}

template <typename T>
void SortedSequence<T>::RemoveByValue(const T& value)
{
	this->sequence->Remove(IndexOf(value));
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
		{
			throw std::invalid_argument("Sequence is nullptr");
		}

		if (this->sequence.sorter == nullptr)
		{
			throw std::invalid_argument("Sorter is nullptr");
		}

		if (this->sequence.comparator == nullptr)
		{
			throw std::invalid_argument("Comparator is nullptr");
		}

		auto tmp = this->sequence.sequence;
		this->sequence.sequence = this->sequence.sorter->Sort(tmp, this->sequence.comparator);
		delete tmp;

		return this->sequence;
	}
};
