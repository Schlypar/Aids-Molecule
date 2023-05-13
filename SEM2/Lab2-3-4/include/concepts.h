#pragma once

#include <concepts>
#include <iterator>

template <typename T>
concept Incerementable = requires(T t)
{
	{
		t++
		} -> std::same_as<T&>;

	{
		++t
		} -> std::same_as<T>;
};

template <typename T>
concept Decrementable = requires(T t)
{
	{
		t--
		} -> std::same_as<T&>;

	{
		--t
		} -> std::same_as<T>;
};

template <typename T>
concept IterEq = requires(T& a, T& b)
{
	{
		*a == *b
		} -> std::convertible_to<bool>;

	{
		*a != *b
		} -> std::convertible_to<bool>;
};

template <typename T>
concept CanReference = requires()
{
	typename std::type_identity_t<T&>;
};

template <typename T>
concept Dereferenceable = requires(T t)
{
	{
		*t
		} -> CanReference;
};

template <class T>
concept HasBeginEnd = requires(T t)
{
	t.begin();
	t.end();
};

template <class T>
concept HasDereferenceOverload = requires(T& t)
{
	t.operator*();
};

template <typename T>
concept Iterator = HasDereferenceOverload<T> && IterEq<T>;