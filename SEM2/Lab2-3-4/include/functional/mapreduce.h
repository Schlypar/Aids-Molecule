#pragma once

#include <functional>

namespace fn
{

template <typename T>
struct filter
{
	std::function<bool(T&)> _filter;

	filter(std::function<bool(T&)> func)
	    : _filter(func)
	{
	}
};

template <typename T>
struct transformer
{
	std::function<T(T&)> _transformer;

	transformer(std::function<T(T&)> func)
	    : _transformer(func)
	{
	}
};

}
