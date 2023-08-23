#pragma once

#include "Pointer.h"
#include "Sequence/Sequence.h"
#include <functional>

template <typename T>
class ISorter
{
public:
	virtual ~ISorter()
	{
	}

	virtual Sequence<T>* Sort(Sequence<T>* sequence, std::function<int(const T&, const T&)> comparator) = 0;

    virtual ISorter<T>* Copy() const noexcept = 0;
};
