#pragma once

#include <cstring>

#include "Logger.h"
#include "Pair.h"
#include "Sequence/Array.h"

class String
{
private:
	Array<char> string;

public:
	String()
	    : string(Size(0))
	{
	}

	String(const Array<char>& string)
	    : string(string)
	{
	}

	String(char* data, Size count)
	    : string(data, count)
	{
	}

	String(const char* string)
	    : string(std::strlen(string))
	{
		Index i = 0;

		while (string[i] != '\0')
		{
			this->string[i] = string[i];
			i++;
		}
	}

	String(Size count)
	    : string(count)
	{
	}

	String(const String& other)
	    : string(other.string)
	{
	}

	String(String&& other)
	    : string(std::move(other.string))
	{
		other.string = Array<char>(Size(0));
	}

	~String()
	{
	}

	Size Length() const noexcept
	{
		return string.GetLength();
	}

	// Counts how many of suspect is in the string
	Size Count(char suspect);

	// Replaces first 'howMuch' chars of 'what' with 'with'
	void Replace(char what, char with, Size howMuch);

	bool isEmpty() const noexcept
	{
		return Length() == 0;
	}

	char& operator[](Index index) const noexcept
	{
		return string.Get(index);
	}

	// Checks if suspect char is in string
	bool isThere(char suspect) const noexcept;

	// Checks if suspect string is a substring of a string
	bool isThere(const String& suspect) const noexcept;

	// Slices entire string into the curren from starting from index (in the current)
	void Slice(Index start, const String& toBeInserted);

	// Slices entire string into the curren from starting from index (in the current). This method is is for immutable String
	String Slice(Index start, const String& toBeInserted) const;

	Index LFind(char suspect) const noexcept;

	Index RFind(char suspect) const noexcept;

	String& operator=(const String& other);

	String& operator=(String&& other);

	// Creates substring from start to inclusive end
	String GetSubString(Index start, Index end) const;

	// Splits on the first pivot char
	Pair<String, String> Split(char pivot) const noexcept;

	bool operator==(const String& other) const noexcept;

	bool operator!=(const String& other) const noexcept
	{
		return !(*this == other);
	}

	// Concatenation of two strings
	friend String operator+(const String& left, const String& rigth)
	{
		String result = String(size_t(left.Length() + rigth.Length()));

		for (Index i = 0; i < left.Length(); i++)
			result[i] = left[i];

		for (Index i = 0; i < rigth.Length(); i++)
			result.string[left.Length() + i] = rigth.string[i];

		return result;
	}

	// Repeats same string for inputed number of times
	friend String operator*(const String& left, const int& right)
	{
		String result = String(size_t(left.Length() * right));

		int counter = right;
		while (counter > 0)
		{
			for (Index i = 0; i < left.Length(); i++)
				result[i + left.Length() * (right - counter)] = left[i];

			counter--;
		}

		return result;
	}

	// Repeats same string for inputed number of times
	friend String operator*(int left, const String& right)
	{
		String result = String(size_t(right.Length() * left + 1));

		int counter = left;
		while (counter > 0)
		{
			for (Index i = 0; i < right.Length(); i++)
				result[i + right.Length() * (left - counter)] = right[i];

			counter--;
		}

		return result;
	}

	friend std::ostream& operator<<(std::ostream& stream, const String& string)
	{
		stream << "\"";

		for (Index i = 0; i < string.Length(); i++)
			if (string.string[i] != '\0')
				stream << string[i];
			else
			{
				stream << "\"";
				return stream;
			}

		stream << "\"";

		return stream;
	}
};
