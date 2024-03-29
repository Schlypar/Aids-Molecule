#pragma once

#include <iostream>
#include <variant>

#include "Sequence/Array.h"
#include "Sequence/List.h"

#include "Sequence/Sequence.h"

#include "Logger.h"

/*

template<template <T1, T2>>


*/

template <typename... T>
class Tuple
{
private:
	Array<std::variant<T...>> data;

public:
	Tuple()
	{
		Logger::Info("Default constructor of Tuple");
	}

	Tuple(const T... args)
	{
		data.Realloc(sizeof...(T));
		data.SetSize(sizeof...(T));

		Logger::Info("Initialising constructor of Tuple of size %u", sizeof...(args));

		Index i = 0;
		((data[i++] = std::move(args)), ...);
	}

	template <int N>
	auto& get()
	{
		return std::get<N>(data[N]);
	}

	template <typename... Args>
	friend Tuple<Args...> Zip(Args... args);

	template <typename Head, typename... Rest>
	friend std::ostream& operator<<(std::ostream& stream, Tuple<Head, Rest...>& tuple);
};

template <typename... Args>
Tuple<Args...> Zip(Args... args)
{
	Tuple<Args...> result = Tuple<Args...>(args...);

	return result;
}

template <typename Head, typename... Rest>
std::ostream& operator<<(std::ostream& stream, Tuple<Head, Rest...>& tuple)
{
	stream << "(";

	stream << std::get<Head>(tuple.data[0]);

	Index i = 1;
	((stream << ", " << std::get<Rest>(tuple.data[i++])), ...);

	stream << ")";

	return stream;
}