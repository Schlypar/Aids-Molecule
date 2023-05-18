#pragma once

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
};