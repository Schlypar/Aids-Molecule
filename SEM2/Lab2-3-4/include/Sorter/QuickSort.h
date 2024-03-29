#pragma once

#include "ISorter.h"
#include "Pointer.h"
#include "Sequence/Sequence.h"

template <typename T>
class QuickSorter : public ISorter<T>
{
public:
	Sequence<T>* Sort(Sequence<T>* sequence, std::function<int(const T&, const T&)> comparator) override
	{
		if (sequence->isEmpty())
		{
			return sequence->Create();
		}

		T middle = sequence->Get(sequence->GetLength() / 2);

		SharedPtr<Sequence<T>> lesser = sequence->Where([middle, comparator](T& e) -> bool
								{ return (comparator(e, middle) < 0) ? true : false; });
		SharedPtr<Sequence<T>> equal = sequence->Where([middle, comparator](T& e) -> bool
							       { return (comparator(e, middle) == 0) ? true : false; });
		SharedPtr<Sequence<T>> greater = sequence->Where([middle, comparator](T& e) -> bool
								 { return (comparator(e, middle) > 0) ? true : false; });

		return (UniquePtr<Sequence<T>>(QuickSort(lesser, comparator)
						       ->Concat(equal.Get())))
			->Concat(QuickSort(greater, comparator).Get());
	}

	ISorter<T>* Copy() const noexcept override
	{
		return new QuickSorter<T>();
	}

private:
	SharedPtr<Sequence<T>> QuickSort(SharedPtr<Sequence<T>> sequence, std::function<int(const T&, const T&)> comparator)
	{
		if (sequence->isEmpty())
		{
			return SharedPtr<Sequence<T>>(sequence->Create());
		}

		T middle = sequence->Get(sequence->GetLength() / 2);

		SharedPtr<Sequence<T>> lesser = sequence->Where([middle, comparator](T& e) -> bool
								{ return (comparator(e, middle) < 0) ? true : false; });
		SharedPtr<Sequence<T>> equal = sequence->Where([middle, comparator](T& e) -> bool
							       { return (comparator(e, middle) == 0) ? true : false; });
		SharedPtr<Sequence<T>> greater = sequence->Where([middle, comparator](T& e) -> bool
								 { return (comparator(e, middle) > 0) ? true : false; });

		return SharedPtr<Sequence<T>>((UniquePtr<Sequence<T>>(QuickSort(lesser, comparator)
									      ->Concat(equal.Get())))
						      ->Concat(QuickSort(greater, comparator).Get()));
	}
};
