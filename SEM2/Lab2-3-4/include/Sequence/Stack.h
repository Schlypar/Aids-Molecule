#pragma once

#include "Sequence/ListSequence.h"

template <typename T>
class Stack
{
private:
	ListSequence<T>* stack;

public:
	Stack()
	    : stack()
	{
	}

	Stack(const T* data, Size count)
	    : stack(data, count)
	{
	}

	Stack(const Sequence<T>& other)
	    : stack(other)
	{
	}

	Stack(Sequence<T>&& other)
	    : stack(other)
	{
	}

	template <typename... Args>
	Stack(Args... args)
	    : stack(args...)
	{
	}

	~Stack()
	{
		delete stack;
	}

	bool isEmpty() const noexcept
	{
		return stack->isEmpty();
	}

	T Pop()
	{
		T res = stack->GetFirst();
		stack->Remove(0);
		return res;
	}

	Stack<T>* Push(const T& value) noexcept
	{
		stack->Append(value);
		return this;
	}

	Size Count() const noexcept
	{
		return stack->GetLenght();
	}

	T Peek(const Index index) const noexcept
	{
		return stack->Get(index);
	}

	friend std::ostream& operator<<(std::ostream& stream, const Stack<T>& stack)
	{
		stream << *(stack->stack);

		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, const Stack<T>* stack)
	{
		stream << *(stack->stack);

		return stream;
	}
};