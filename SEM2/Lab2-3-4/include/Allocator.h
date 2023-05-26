#pragma once

#include "Pointer.h"

#include "Sequence/ArraySequence.h"
#include "Sequence/ListSequence.h"
#include "Sequence/Sequence.h"
#include "Tuple.h"

template <typename T>
class Allocator
{
public:
	template <typename... Args>
	static SharedPtr<Array<T>> AllocateArray(Args... args)
	{
		return new Array<T>(args...);
	}

	template <typename... Args>
	static SharedPtr<List<T>> AllocateList(Args... args)
	{
		return new SegmentedList<T>(args...);
	}

	template <typename... Args>
	static SharedPtr<SegmentedList<T>> AllocateSegmentedList(Args... args)
	{
		return new SegmentedList<T>(args...);
	}

	template <typename... Args>
	static SharedPtr<Sequence<T>> AllocateArraySequence(Args... args)
	{
		return (Sequence<T>*) new ArraySequence<T>(args...);
	}

	template <typename... Args>
	static SharedPtr<Sequence<T>> AllocateListSequence(Args... args)
	{
		return (Sequence<T>*) new ListSequence<T>(args...);
	}
};