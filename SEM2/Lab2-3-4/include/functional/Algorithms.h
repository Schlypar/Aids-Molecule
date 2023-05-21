#pragma once

#include <functional>

#include "Sequence/List.h"

template <typename T>
using Iterator = typename List<T>::Iterator;

template <typename T>
static List<T> CreateList()
{
	return List<T>();
}

static List<int> range(List<int>&& list, int start, int end)
{
	return (start <= end) ? list.Append(start), range(std::move(list), start + 1, end) : list;
}

template <typename T, typename Function>
static void forEach(Iterator<T> begin, Iterator<T> end, Function func)
{
	if (begin != end)
	{
		func(*begin);
		forEach(begin++, end, func);
	}
}

template <typename T, typename Function>
static List<T> filter(Iterator<T> begin, Iterator<T> end, Function where)
{
	List<T> result;
	forEach(begin, end, [&result, &where](Iterator<T> begin, Iterator<T> end) -> void {
		auto current = begin;
		while (current != end)
			if (where(*current))
				result.Append(*current);
	});

	return result;
}
