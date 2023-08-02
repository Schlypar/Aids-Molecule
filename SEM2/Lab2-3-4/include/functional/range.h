#pragma once

#include <Sequence/ArraySequence.h>
#include <concepts.h>
#include <functional>
#include <ranges>

using Range = ArraySequence<int>;

namespace fn
{

Range range(int _range)
{
	ArraySequence<int> range;

	for (size_t i = 0; i < _range; i++)
		range.Append(i);

	return range;
}

Range range(int min, int max)
{
	ArraySequence<int> range;

	for (size_t i = min; i < max; i++)
		range.Append(i);

	return range;
}

template <Iterable iterable>
Range range(iterable container)
{
	ArraySequence<int> range;

	for (auto data : container)
		range.Append(data);

	return range;
}

template <Iterable iterable, typename T>
Range range(iterable container)
{
	ArraySequence<int> range;

	size_t i = 0;
	for (auto data : container)
		range.Append(i++);

	return range;
}

template <Iterable iterable>
int foldl(iterable container, std::function<int(int, int)> func, int acc)
{
	auto cur = container.begin();
	auto end = container.end();

	while (cur != end)
	{
		acc = func(*cur, acc);
		++cur;
	}

	return acc;
}

template <Iterable iterable>
int foldr(iterable container, std::function<int(int, int)> func, int acc)
{
	auto cur = container.end();
	auto end = container.begin();
	--cur;
	--end;

	while (cur != end)
	{
		acc = func(*cur, acc);
		--cur;
	}

	return acc;
}

}
