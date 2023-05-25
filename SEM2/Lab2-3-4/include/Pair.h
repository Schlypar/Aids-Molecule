#pragma once

#include <iostream>
#include <utility>

template <typename L, typename R>
class Pair
{
private:
	L left;
	R right;

public:
	Pair()
	    : left(L())
	    , right(R())
	{
	}

	Pair(const L& left, const R& right)
	    : left(left)
	    , right(right)
	{
	}

	Pair(const Pair<L, R>& other)
	    : left(other.left)
	    , right(other.right)
	{
	}

	Pair(Pair<L, R>&& other)
	    : left(std::move(other.left))
	    , right(std::move(other.right))
	{
	}

	~Pair()
	{
	}

	L GetLeft() const noexcept
	{
		return left;
	}

	R GetRight() const noexcept
	{
		return right;
	}

	Pair& operator=(const Pair<L, R>& other)
	{
		left = other.left;
		right = other.right;

		return *this;
	}

	Pair& operator=(Pair<L, R>&& other)
	{
		left = std::move(other.left);
		right = std::move(other.right);

		return *this;
	}

	bool operator==(const Pair<L, R>& other) const noexcept
	{
		return (left == other.left) && (right == other.right);
	}

	friend std::ostream& operator<<(std::ostream& stream, const Pair<L, R>& pair)
	{
		stream << "(" << pair.left << ", " << pair.right << ")";

		return stream;
	}
};