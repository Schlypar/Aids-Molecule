#pragma once

#include <concepts>
#include <functional>
#include <iterator>

template <typename Iterator, typename T>
concept Iter = requires(Iterator iter, Iterator other)
{
	{iter++} -> std::convertible_to<Iterator>;
	{++iter} -> std::convertible_to<Iterator>;

	{iter--} -> std::convertible_to<Iterator>;
	{--iter} -> std::convertible_to<Iterator>;

	{*iter} -> std::convertible_to<T>;

	{iter.equal(other)} -> std::convertible_to<bool>;
	{iter.copy()} -> std::convertible_to<Iterator>;
};	

template <typename Container>
concept hasBeginEnd = requires(Container container)
{
	container.begin();	
	container.end();	
};

template <typename Container>
concept hasIteratorType = requires{
	typename Container::Iterator;
};

template <typename Container>
concept Enumerable = hasBeginEnd<Container> && hasIteratorType<Container>;

template <typename T>
concept Equal = requires(T a, T b) {
	{
		a == b
	} -> std::convertible_to<bool>;
	{
		a != b
	} -> std::convertible_to<bool>;
};

template <typename T>
concept Less = requires(T a, T b) {
	{
		a < b
	} -> std::convertible_to<bool>;
	{
		a <= b
	} -> std::convertible_to<bool>;
};

template <typename T>
concept Greater = requires(T a, T b) {
	{
		a > b
	} -> std::convertible_to<bool>;
	{
		a >= b
	} -> std::convertible_to<bool>;
};

template <typename T>
concept Comparible = Equal<T> && Less<T> && Greater<T>;

template <typename T>
concept Iterable = requires(T t) {
	t.begin();
	t.end();
};
