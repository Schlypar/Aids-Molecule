#pragma once

#include <cmath>
#include <iostream>
#include <utility>

class complex
{
private:
	float real;
	float imaginary;

public:
	complex()
	    : real(0)
	    , imaginary(0)
	{
	}

	complex(const float& real)
	    : real(real)
	    , imaginary()
	{
	}

	complex(float&& other)
	    : real(std::move(other))
	    , imaginary()
	{
		other = float();
	}

	complex(const float& real, const float& imaginary)
	    : real(real)
	    , imaginary(imaginary)
	{
	}

	complex(float&& real, float&& imaginary)
	    : real(std::move(real))
	    , imaginary(std::move(imaginary))
	{
		real = float();
		imaginary = float();
	}

	complex(const complex& other)
	    : real(other.real)
	    , imaginary(other.imaginary)
	{
	}

	complex(complex&& other)
	    : real(std::move(other.real))
	    , imaginary(std::move(other.imaginary))
	{
		other.real = float();
		other.imaginary = float();
	}

	complex& operator=(const complex& other);
	complex& operator=(const float& other);

	complex& operator=(complex&& other);
	complex& operator=(float&& other);

	complex& operator+=(const complex& other);
	complex& operator+=(const float& other);

	friend complex operator+(const complex& left, const complex& right);
	friend complex operator+(const float& left, const complex& right);
	friend complex operator+(const complex& left, const float& right);

	complex& operator-=(const complex& other);
	complex& operator-=(const float& other);

	friend complex operator-(const complex& left, const complex& right);
	friend complex operator-(const float& left, const complex& right);
	friend complex operator-(const complex& left, const float& right);

	complex& operator*=(const complex& other);
	complex& operator*=(const float& other);

	friend complex operator*(const complex& left, const complex& right);
	friend complex operator*(const float& left, const complex& right);
	friend complex operator*(const complex& left, const float& right);

	complex& operator/=(const complex& other);
	complex& operator/=(const float& other);

	friend complex operator/(const complex& left, const complex& right);
	friend complex operator/(const float& left, const complex& right);
	friend complex operator/(const complex& left, const float& right);

	friend float sqrt(const complex& other);

	bool operator==(const complex& other) const;
	bool operator==(const float& other) const;

	bool operator!=(const complex& other) const;
	bool operator!=(const float& other) const;

	bool operator>(const complex& other) const;
	bool operator>(const float& other) const;
	bool operator>=(const complex& other) const;
	bool operator>=(const float& other) const;

	bool operator<(const complex& other) const;
	bool operator<(const float& other) const;
	bool operator<=(const complex& other) const;
	bool operator<=(const float& other) const;

	friend std::ostream& operator<<(std::ostream& stream, const complex& complx);
};

complex& complex::operator=(const complex& other)
{
	real = other.real;
	imaginary = other.imaginary;

	return *this;
}

complex& complex::operator=(const float& other)
{
	real = other;
	imaginary = 0;

	return *this;
}

complex& complex::operator=(complex&& other)
{
	real = std::move(other.real);
	imaginary = std::move(other.imaginary);

	return *this;
}

complex& complex::operator=(float&& other)
{
	real = std::move(other);
	imaginary = 0;

	return *this;
}

complex& complex::operator+=(const complex& other)
{
	real += other.real;
	imaginary += other.imaginary;

	return *this;
}

complex& complex::operator+=(const float& other)
{
	real += other;

	return *this;
}

complex operator+(const complex& left, const complex& right)
{
	complex copy = left;

	copy.real = left.real + right.real;
	copy.imaginary = left.imaginary + right.imaginary;

	return copy;
}

complex operator+(const float& left, const complex& right)
{
	complex copy = right;

	copy.real = left + right.real;

	return copy;
}

complex operator+(const complex& left, const float& right)
{
	complex copy = left;

	copy.real = left.real + right;

	return copy;
}

complex& complex::operator-=(const complex& other)
{
	real -= other.real;
	imaginary -= other.imaginary;

	return *this;
}

complex& complex::operator-=(const float& other)
{
	real -= other;

	return *this;
}

complex operator-(const complex& left, const complex& right)
{
	complex copy = left;

	copy.real = left.real - right.real;
	copy.imaginary = left.imaginary - right.imaginary;

	return copy;
}

complex operator-(const float& left, const complex& right)
{
	complex copy = right;

	copy.real = left - right.real;

	return copy;
}

complex operator-(const complex& left, const float& right)
{
	complex copy = left;

	copy.real = left.real - right;

	return copy;
}

complex& complex::operator*=(const complex& other)
{
	real = real * other.real - imaginary * other.imaginary;
	imaginary = real * other.imaginary + imaginary * other.real;

	return *this;
}

complex& complex::operator*=(const float& other)
{
	real = real * other;
	imaginary = imaginary * other;

	return *this;
}

complex operator*(const complex& left, const complex& right)
{
	complex copy = left;

	copy.real = left.real * right.real - left.imaginary * right.imaginary;
	copy.imaginary = left.real * right.imaginary + left.imaginary * right.real;

	return copy;
}

complex operator*(const float& left, const complex& right)
{
	complex copy = right;

	copy.real = left * right.real;
	copy.imaginary = left * right.imaginary;

	return copy;
}

complex operator*(const complex& left, const float& right)
{
	complex copy = left;

	copy.real = left.real * right;
	copy.imaginary = left.imaginary * right;

	return copy;
}

complex& complex::operator/=(const complex& other)
{
	real = (real * other.real + imaginary * other.imaginary)
		/ (other.real * other.real + other.imaginary * other.imaginary);
	imaginary = (imaginary * other.real - real * other.imaginary)
		/ (other.real * other.real + other.imaginary * other.imaginary);

	return *this;
}

complex& complex::operator/=(const float& other)
{
	real = real / other;
	imaginary = imaginary / other;

	return *this;
}

complex operator/(const complex& left, const complex& right)
{
	complex copy = left;

	copy.real = (left.real * right.real + left.imaginary * right.imaginary)
		/ (right.real * right.real + right.imaginary * right.imaginary);
	copy.imaginary = (left.imaginary * right.real - left.real * right.imaginary)
		/ (right.real * right.real + right.imaginary * right.imaginary);

	return copy;
}

complex operator/(const float& left, const complex& right)
{
	complex copy = right;

	copy.real = left / right.real;
	copy.imaginary = left / right.imaginary;

	return copy;
}

complex operator/(const complex& left, const float& right)
{
	complex copy = left;

	copy.real = left.real / right;
	copy.imaginary = left.imaginary / right;

	return copy;
}

float sqrt(const complex& other)
{
	return std::sqrt(other.real * other.real + other.imaginary * other.imaginary);
}

bool complex::operator==(const complex& other) const
{
	return sqrt(*this) == sqrt(other);
}

bool complex::operator==(const float& other) const
{
	return sqrt(*this) == other;
}

bool complex::operator!=(const complex& other) const
{
	return sqrt(*this) != sqrt(other);
}

bool complex::operator!=(const float& other) const
{
	return sqrt(*this) != other;
}

bool complex::operator>(const complex& other) const
{
	return sqrt(*this) > sqrt(other);
}

bool complex::operator>(const float& other) const
{
	return sqrt(*this) > other;
}

bool complex::operator>=(const complex& other) const
{
	return sqrt(*this) >= sqrt(other);
}

bool complex::operator>=(const float& other) const
{
	return sqrt(*this) >= other;
}

bool complex::operator<(const complex& other) const
{
	return sqrt(*this) < sqrt(other);
}

bool complex::operator<(const float& other) const
{
	return sqrt(*this) < other;
}

bool complex::operator<=(const complex& other) const
{
	return sqrt(*this) <= sqrt(other);
}

bool complex::operator<=(const float& other) const
{
	return sqrt(*this) <= other;
}

std::ostream& operator<<(std::ostream& stream, const complex& complx)
{
	if (complx.imaginary == 0)
	{
		stream << complx.real;
		return stream;
	}

	if (complx.real == 0)
	{
		stream << complx.imaginary << "i";
		return stream;
	}

	if (complx.imaginary > 0)
		stream << complx.real << " + " << complx.imaginary << "i";

	if (complx.imaginary < 0)
		stream << complx.real << " - " << -complx.imaginary << "i";

	if (complx.imaginary == 0 && complx.real == 0)
		stream << "0";

	return stream;
}
