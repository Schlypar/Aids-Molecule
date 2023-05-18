#pragma once

#include <concepts>
#include <functional>
#include <iterator>

template <typename T>
concept PrefixIncrementable = requires(T t) {
				      {
					      ++t
					      } -> std::same_as<T>;
			      };

template <typename T>
concept PostfixIncrementable = requires(T t) {
				       {
					       t++
					       } -> std::same_as<T&>;
			       };

template <typename T>
concept Incerementable = PrefixIncrementable<T> && PostfixIncrementable<T>;

template <typename T>
concept PrefixDecrementable = requires(T t) {
				      {
					      --t
					      } -> std::same_as<T>;
			      };

template <typename T>
concept PostfixDecrementable = requires(T t) {
				       {
					       t--
					       } -> std::same_as<T>;
			       };

template <typename T>
concept Decrementable = PrefixDecrementable<T> && PostfixDecrementable<T>;

template <typename T>
concept IterEq = requires(T& a, T& b) {
			 {
				 *a == *b
				 } -> std::convertible_to<bool>;

			 {
				 *a != *b
				 } -> std::convertible_to<bool>;
		 };

template <typename T>
concept CanReference = requires() { typename std::type_identity_t<T&>; };

template <typename T>
concept Dereferenceable = requires(T t) {
				  {
					  *t
					  } -> CanReference;
			  };

template <class T>
concept HasBeginEnd = requires(T t) {
			      t.begin();
			      t.end();
		      };

template <class T>
concept HasDereferenceOverload = requires(T& t) { t.operator*(); };

template <typename T>
concept Iter = HasDereferenceOverload<T> && IterEq<T>;

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
