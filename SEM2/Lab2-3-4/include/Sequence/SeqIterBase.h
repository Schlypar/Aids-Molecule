#pragma once

#include <functional>

template <typename T>
class Sequence;

template <typename T, typename U>
class MapIterator;

enum class SequenceKind { Array, List };

template <typename T>
class SeqIter
{
protected:
	SequenceKind kind_;

public:
	explicit SeqIter(SequenceKind kind)
	    : kind_(kind)
	{
	}

	virtual ~SeqIter()
	{
	}

	virtual bool hasNext() const = 0;
	virtual T    next()          = 0;

	SequenceKind kind() const
	{
		return kind_;
	}

	virtual Sequence<T>* Collect() = 0;

	// Defined out-of-line in SeqIter.h after MapIterator is fully defined
	template <typename U>
	MapIterator<T, U>* MapTo(std::function<U(const T&)> func);
};
