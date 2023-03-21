#pragma once

#include <iostream>

template <typename... Args> 
void print(Args...args) {}

template <typename T, typename... Args> 
void print(T& data, Args... args)
{
	if (sizeof...(args) == 0)
		std::cout << data;
	else 
		std::cout << data;

	print(args...);
}

template <typename... Args>
struct Tuple;

template <typename T, typename... Args>
struct Tuple<T, Args...> : Tuple<Args...>
{
	Tuple(T data, Args... args)
		: Tuple<Args...>(args...), data(data) {}

	typedef Tuple<Args...> nextType;
	typedef T valueType;

	nextType& next = static_cast<nextType&>(*this);
	T data;

	friend std::ostream& operator<< (std::ostream& stream, Tuple<T, Args...> tuple);
};

template < >
struct Tuple < > {};

template <int index, typename T, typename... Args>
struct getter
{
	typedef typename getter<index - 1, Args...>::returnType returnType;
	static returnType get(Tuple<T, Args...> tuple)
	{
		return getter<index - 1, Args...>::get(tuple);
	}
};

template<typename T, typename... Args>
struct getter<0, T, Args...>
{
	typedef typename Tuple<T, Args...>::valueType returnType;
	static returnType get(Tuple<T, Args...> tuple)
	{
		return tuple.data;
	}
};

template <int index, typename T, typename... Args>
typename getter<index, T, Args...>::returnType
get(Tuple<T, Args...> tuple)
{
	return getter<index, T, Args...>::get(tuple);
}


template <typename T, typename... Args>
void printTuple(Tuple<T, Args...> tuple)
{
	std::cout << (get<0>(tuple)) << " ";
	printTuple(tuple.next);
}

template <typename... Args>
void printTuple(Tuple<Args...> tuple) {}

template<typename T, typename... Args>
std::ostream& operator<< (std::ostream& stream, Tuple<T, Args...> tuple)
{
	stream << "( ";

	printTuple(tuple);

	stream << ")";

	return stream;
}