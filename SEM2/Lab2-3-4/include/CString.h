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
	    : string()
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
		other.string = Array<char>();
	}

	~String()
	{
	}

	Size Length() const noexcept
	{
		return string.GetLength();
	}

	Size Count(char suspect)
	{
		if (isEmpty())
			return 0;

		Size count = 0;
		for (Index i = 0; i < string.GetLength(); i++)
			if (this->string[i] == suspect)
				count++;

		return count;
	}

	void Replace(char what, char with, Size howMuch)
	{
		if (Count(what) < howMuch)
		{
			Logger::Trace("At String at Replace(char, char, Size)");
			logException(EXCEPTION_BAD_LOGIC);
			throw EXCEPTION_BAD_LOGIC;
		}

		Index i = 0;
		Size count = 0;

		while (count < howMuch)
		{
			if (this->string[i] == what)
			{
				this->string[i] = with;
				count++;
			}

			i++;
		}
	}

	bool isEmpty() const noexcept
	{
		return Length() == 0;
	}

	bool isThere(char suspect) const noexcept
	{
		if (this->string[0] == suspect)
			return true;

		return (LFind(suspect) == 0) ? false : true;
	}

	bool isThere(const String& suspect) const noexcept
	{
		if (isEmpty() && !suspect.isEmpty())
			return false;

		if (!isEmpty() && suspect.isEmpty())
			return false;

		if (isEmpty() && suspect.isEmpty())
			return true;

		if (this->Length() < suspect.Length())
			return false;

		Index suspectStart = LFind(suspect[0]);
		Index suspectEnd = suspectStart + suspect.Length() - 1;
		Size segmentLength = suspectEnd - suspectStart;

		if (segmentLength == 0)
			return isThere(suspect[0]);

		while (1)
		{
			for (Index i = 0; i < segmentLength; i++)
				if (this->string[suspectStart] == suspect[i])
					return true;

			suspectStart++;
			while (this->string[suspectStart] != suspect[0])
			{
				suspectStart++;

				if (suspectStart == this->Length())
					return false;
			}

			Index suspectEnd = suspectStart + suspect.Length() - 1;
			Size segmentLength = suspectEnd - suspectStart;
		}
	}

	void Slice(Index start, const String& toBeInserted)
	{
		if (start >= Length() || start + toBeInserted.Length() >= Length())
		{
			Logger::Trace("At String at Slice(Index, const String&)");
			logException(EXCEPTION_INDEX_OUT_OF_RANGE);
			throw EXCEPTION_INDEX_OUT_OF_RANGE;
		}

		if (toBeInserted.isEmpty())
		{
			for (Index i = start; i < Length() - 1; i++)
				this->string[i] = this->string[i + 1];

			return;
		}

		for (Index i = 0; i < toBeInserted.Length(); i++)
			this->string[start + i] = toBeInserted[i];
	}

	String Slice(Index start, const String& toBeInserted) const
	{
		if (start >= Length() || start + toBeInserted.Length() >= Length())
		{
			Logger::Trace("At String at Slice(Index, const String&)");
			logException(EXCEPTION_INDEX_OUT_OF_RANGE);
			throw EXCEPTION_INDEX_OUT_OF_RANGE;
		}

		String result = *this;

		if (toBeInserted.isEmpty())
		{
			for (Index i = start; i < Length() - 1; i++)
				result.string[i] = result.string[i + 1];

			return result;
		}

		for (Index i = 0; i < toBeInserted.Length(); i++)
			result.string[start + i] = toBeInserted[i];

		return result;
	}

	Index LFind(char suspect) const noexcept
	{
		if (isEmpty())
			return 0;

		for (Index i = 0; i < Length(); i++)
			if (this->string[i] == suspect)
				return i;

		return 0;
	}

	Index RFind(char suspect) const noexcept
	{
		if (isEmpty())
			return 0;

		for (Index i = Length() - 1; i > 0; i--)
			if (this->string[i] == suspect)
				return i;

		return 0;
	}

	String& operator=(const String& other)
	{
		this->string = other.string;

		return *this;
	}

	String& operator=(String&& other)
	{
		this->string = other.string;
		other.string = Array<char>();

		return *this;
	}

	String GetSubString(Index start, Index end) const
	{
		if (start > end || start < 0 || end >= Length())
		{
			Logger::Trace("At String at GetSubString(Index, Index)");
			logException(EXCEPTION_INDEX_OUT_OF_RANGE);
			throw EXCEPTION_INDEX_OUT_OF_RANGE;
		}

		String result = String(size_t(end - start + 2));

		for (Index i = start; i <= end; i++)
			result[i - start] = this->string[i];

		return result;
	}

	Pair<String, String> Split(char pivot) const noexcept
	{
		if (!isThere(pivot))
			return Pair<String, String>(String(), String());

		Index pivotIndex = LFind(pivot);

		String left = GetSubString(0, pivotIndex);
		String right = GetSubString(pivotIndex + 1, Length() - 1);

		return Pair<String, String>(left, right);
	}

	bool operator==(const String& other) const noexcept
	{
		if (this->Length() != other.Length())
			return false;

		Size length = this->Length();
		for (Index i = 0; i < length; i++)
			if (this->string[i] != other.string[i])
				return false;

		return true;
	}

	bool operator!=(const String& other) const noexcept
	{
		return !(*this == other);
	}

	friend String operator+(const String& left, const String& rigth)
	{
		String result = String(size_t(left.Length() + rigth.Length() + 1));

		for (Index i = 0; i < left.Length(); i++)
			result[i] = left[i];

		for (Index i = 0; i < rigth.Length(); i++)
			result.string[left.Length() + i] = rigth.string[i];

		return result;
	}

	friend String operator*(const String& left, const int& right)
	{
		String result = String(size_t(left.Length() * right + 1));

		int counter = right;
		while (counter > 0)
		{
			for (Index i = 0; i < left.Length(); i++)
				result[i + left.Length() * (right - counter)] = left[i];

			counter--;
		}

		return result;
	}

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

	char& operator[](Index index) const noexcept
	{
		return string.Get(index);
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