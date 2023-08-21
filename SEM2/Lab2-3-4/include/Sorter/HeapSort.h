#pragma once

#include "ISorter.h"
#include "Pointer.h"
#include "Sequence/Sequence.h"
#include <array>
#include <functional>

template <typename T>
class HeapSorter : public ISorter<T>
{
public:
	Sequence<T>* Sort(Sequence<T>* sequence, std::function<int(const T&, const T&)> comparator) override
	{
		Sequence<T>* copy = sequence->Copy();

		for (int i = copy->GetLength() / 2 - 1; i >= 0; i--)
			Heapify(copy, copy->GetLength(), i, comparator);

		for (int i = copy->GetLength() - 1; i > 0; i--)
		{
			std::swap(copy->Get(0), copy->Get(i));
			Heapify(copy, i, 0, comparator);
		}

		return copy;
	}

private:
	void Heapify(Sequence<T>* sequence, int size, int i, std::function<int(const T&, const T&)> comparator)
	{
		int largest = i;

		int left = 2 * i + 1;
		int right = 2 * i + 2;

		if (left < size && comparator(sequence->Get(left), sequence->Get(largest)) > 0)
			largest = left;

		if (right < size && comparator(sequence->Get(right), sequence->Get(largest)) > 0)
			largest = right;

		if (largest != i)
		{
			std::swap(sequence->Get(i), sequence->Get(largest));
			Heapify(sequence, size, largest, comparator);
		}
	}
};
