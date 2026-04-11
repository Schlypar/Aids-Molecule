#pragma once

#include "ArraySequence.h"
#include "ListSequence.h"

#include <functional>

template <typename T, typename U>
class MapIterator : public SeqIter<U>
{
	SeqIter<T>*                source_;
	std::function<U(const T&)> mapper_;

public:
	MapIterator(SeqIter<T>* source, std::function<U(const T&)> mapper)
	    : SeqIter<U>(source->kind())
	    , source_(source)
	    , mapper_(mapper)
	{
	}

	~MapIterator() override
	{
		delete source_;
	}

	bool hasNext() const override
	{
		return source_->hasNext();
	}

	U next() override
	{
		return mapper_(source_->next());
	}

	Sequence<U>* Collect() override
	{
		Sequence<U>* result = (this->kind_ == SequenceKind::Array)
		                          ? (Sequence<U>*) new ArraySequence<U>()
		                          : (Sequence<U>*) new ListSequence<U>();
		while (hasNext())
			result->Append(next());
		return result;
	}
};

// Out-of-line definition of SeqIter<T>::MapTo<U>()
// Must appear after MapIterator<T,U> is fully defined
template <typename T>
template <typename U>
MapIterator<T, U>* SeqIter<T>::MapTo(std::function<U(const T&)> func)
{
	return new MapIterator<T, U>(this, func);
}
