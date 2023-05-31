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

	friend complex operator+(const complex& left, const float& right)
	{
		complex copy = complex(left);

		copy.real += right;

		return copy;
	}

	friend complex operator+(const float& other, const complex& complex)
	{
		return complex + other;
	}

	// friend complex operator+ (const float& other, complex&& complex)
	// {
	//     return complex + other;
	// }

	complex operator+(const complex& other)
	{
		complex copy = complex(*this);

		copy.real += other.real;
		copy.imaginary += other.imaginary;

		return copy;
	}

	complex& operator+=(const float& other)
	{
		real += other;

		return *this;
	}

	complex& operator+=(const complex& other)
	{
		real += other.real;
		imaginary += other.imaginary;

		return *this;
	}

	complex& operator+=(complex&& other)
	{
		real += other.real;
		imaginary += other.imaginary;

		return *this;
	}

	complex& operator-(const float& other)
	{
		real -= other;

		return *this;
	}

	complex operator-(const complex& other)
	{
		complex copy = complex(*this);

		copy.real -= other.real;
		copy.imaginary -= other.imaginary;

		return copy;
	}

	friend complex operator-(const float& other, complex& complex)
	{
		return complex - other;
	}

	// friend complex operator- (const float& other, complex&& complex)
	// {
	//     return complex - other;
	// }

	// complex& operator- (complex& other)
	// {
	//     real -= other.real;
	//     imaginary -= other.imaginary;

	//     return *this;
	// }

	complex& operator-=(const float& other)
	{
		real -= other;

		return *this;
	}

	complex& operator-=(const complex& other)
	{
		real -= other.real;
		imaginary -= other.imaginary;

		return *this;
	}

	complex& operator-=(complex&& other)
	{
		real -= other.real;
		imaginary -= other.imaginary;

		return *this;
	}

	complex operator*(const float& other)
	{
		complex copy = complex(*this);

		copy.real = real * other;
		copy.imaginary = imaginary * other;

		return copy;
	}

	complex operator*(const complex& other)
	{
		complex copy = complex(*this);

		copy.real = real * other.real - imaginary * other.imaginary;
		copy.imaginary = real * other.imaginary + other.real * imaginary;

		return copy;
	}

	complex operator*(const complex& other) const
	{
		complex copy = complex(*this);

		copy.real = real * other.real - imaginary * other.imaginary;
		copy.imaginary = real * other.imaginary + other.real * imaginary;

		return copy;
	}

	friend complex operator*(const float& other, complex& complex)
	{
		return complex * other;
	}

	complex operator/(const float& other) const
	{
		complex copy = complex(*this);

		copy.real = copy.real / other;
		copy.imaginary = copy.imaginary / other;

		return copy;
	}

	complex operator/(const complex& other) const
	{
		complex copy = complex(*this);

		float denominator = (other.real * other.real + other.imaginary * other.imaginary);

		copy.real = (real * other.real + imaginary * other.imaginary)
			/ (other.real * other.real + other.imaginary * other.imaginary);
		copy.imaginary = (imaginary * other.real - real * other.imaginary)
			/ (other.real * other.real + other.imaginary * other.imaginary);

		return copy;
	}

	friend complex operator/(const float& first, const complex& second)
	{
		complex firstValue = complex(first);

		return firstValue / second;
	}

	complex& operator=(const float& other)
	{
		real = other;
		imaginary = 0;

		return *this;
	}

	complex& operator=(const complex& other)
	{
		real = other.real;
		imaginary = other.imaginary;

		return *this;
	}

	complex& operator=(float&& other)
	{
		real = std::move(other);
		imaginary = 0;

		other = float();

		return *this;
	}

	complex& operator=(complex&& other)
	{
		real = other.real;
		imaginary = other.imaginary;

		other = 0;

		return *this;
	}

	bool operator==(const float& other)
	{
		return (std::sqrt(real * real + imaginary * imaginary) == other) ? true : false;
	}

	friend bool operator==(const float& other, complex& complex)
	{
		return complex == other;
	}

	friend bool operator==(const float& other, complex&& complex)
	{
		return complex == other;
	}

	bool operator==(const complex& other)
	{
		return (std::sqrt(real * real + imaginary * imaginary)
			       == std::sqrt(other.real * other.real + other.imaginary * other.imaginary))
			? true
			: false;
	}

	bool operator!=(const float& other)
	{
		return !(*this == other);
	}

	friend bool operator!=(const float& other, complex& complex)
	{
		return complex != other;
	}

	friend bool operator!=(const float& other, complex&& complex)
	{
		return complex != other;
	}

	bool operator!=(const complex& other)
	{
		return !(*this == other);
	}

	bool operator<(const float& other)
	{
		return (std::sqrt(real * real + imaginary * imaginary) < other) ? true : false;
	}

	friend bool operator<(const float& other, complex& complex)
	{
		return complex > other;
	}

	friend bool operator<(const float& other, complex&& complex)
	{
		return complex > other;
	}

	bool operator<(const complex& other)
	{
		return (std::sqrt(real * real + imaginary * imaginary)
			       < std::sqrt(other.real * other.real + other.imaginary * other.imaginary))
			? true
			: false;
	}

	bool operator>(const float& other)
	{
		return !(*this < other);
	}

	friend bool operator>(const float& other, complex& complex)
	{
		return complex < other;
	}

	friend bool operator>(const float& other, complex&& complex)
	{
		return complex < other;
	}

	bool operator>(const complex& other)
	{
		return !(*this < other);
	}

	bool operator<=(const float& other)
	{
		if (*this == other || *this < other)
			return true;
		else
			return false;
	}

	friend bool operator<=(const float& other, complex& complex)
	{
		return complex >= other;
	}

	friend bool operator<=(const float& other, complex&& complex)
	{
		return complex >= other;
	}

	bool operator<=(const complex& other)
	{
		if (*this == other || *this < other)
			return true;
		else
			return false;
	}

	bool operator>=(const float& other)
	{
		if (*this == other || *this > other)
			return true;
		else
			return false;
	}

	friend bool operator>=(const float& other, complex& complex)
	{
		return complex <= other;
	}

	friend bool operator>=(const float& other, complex&& complex)
	{
		return complex <= other;
	}

	bool operator>=(const complex& other)
	{
		if (*this == other || *this > other)
			return true;
		else
			return false;
	}

	friend std::ostream& operator<<(std::ostream& stream, const complex& complex)
	{
		if (complex.imaginary == 0)
			stream << complex.real;
		else if (complex.imaginary >= 0)
			stream << complex.real << " + " << complex.imaginary << 'i';
		else
			stream << complex.real << " - " << -complex.imaginary << 'i';

		return stream;
	}
};
