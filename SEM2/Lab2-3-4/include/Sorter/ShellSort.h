#pragma once

#include "ISorter.h"
#include "Sequence/Sequence.h"
#include <functional>

template <typename T>
class ShellSorter : public ISorter<T>
{
public:
	Sequence<T>* Sort(Sequence<T>* sequence, std::function<int(const T&, const T&)> comparator) override
	{
		Sequence<T>* copy = sequence->Copy();

		for (int gap = copy->GetLength() / 2; gap > 0; gap /= 2)
		{
			for (int i = gap; i < copy->GetLength(); i++)
			{
				for (int j = i - gap; j >= 0 && comparator(copy->Get(j), copy->Get(j + gap)) > 0; j -= gap)
				{
					std::swap(copy->Get(j), copy->Get(j + gap));
				}
			}
		}

		return copy;
	}

	ISorter<T>* Copy() const noexcept override
	{
		return new ShellSorter<T>();
	}
};
