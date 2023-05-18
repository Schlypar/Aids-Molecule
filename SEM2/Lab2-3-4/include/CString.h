#pragma once

#include <cstring>

#include "Logger.h"
#include "Pair.h"
#include "Sequence/Array.h"
#include "Sequence/IContainer.h"

class String
{
private:
	Array<char> string;
	Size length;

public:
	String()
	    : string()
	    , length(0)
	{
	}

	String(const Array<char>& string)
	    : string(string)
	    , length(string.GetLength())
	{
	}

	String(char* data, Size count)
	    : string(data, count + 1)
	    , length(count)
	{
		this->string[length] = '\0';
	}

	String(const char* string)
	    : string(std::strlen(string) + 1)
	    , length(std::strlen(string))
	{
		Index i = 0;

		while (string[i] != '\0')
		{
			this->string[i] = string[i];
			i++;
		}

		this->string[length] = '\0';
	}

	String(Size count)
	    : string(count)
	    , length(0)
	{
	}

	String(const String& other)
	    : string(other.string)
	    , length(other.length)
	{
	}

	String(String&& other)
	    : string(std::move(other.string))
	    , length(std::move(other.length))
	{
		other.string = Array<char>();
		other.length = 0;
	}

	~String()
	{
	}

	Size Length() const noexcept
	{
		return length;
	}

	Size Count(char suspect)
	{
		if (isEmpty())
			return 0;

		Size count = 0;
		for (Index i = 0; i < length; i++)
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
		return length == 0;
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

		if (this->length < suspect.length)
			return false;

		Index suspectStart = LFind(suspect[0]);
		Index suspectEnd = suspectStart + suspect.length - 1;
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

				if (suspectStart == this->length)
					return false;
			}

			Index suspectEnd = suspectStart + suspect.length - 1;
			Size segmentLength = suspectEnd - suspectStart;
		}
	}

	void Slice(Index start, const String& toBeInserted)
	{
		if (start >= length || start + toBeInserted.length >= length)
		{
			Logger::Trace("At String at Slice(Index, const String&)");
			logException(EXCEPTION_INDEX_OUT_OF_RANGE);
			throw EXCEPTION_INDEX_OUT_OF_RANGE;
		}

		if (toBeInserted.isEmpty())
		{
			for (Index i = start; i < length - 1; i++)
				this->string[i] = this->string[i + 1];

			return;
		}

		for (Index i = 0; i < toBeInserted.length; i++)
			this->string[start + i] = toBeInserted[i];
	}

	String Slice(Index start, const String& toBeInserted) const
	{
		if (start >= length || start + toBeInserted.length >= length)
		{
			Logger::Trace("At String at Slice(Index, const String&)");
			logException(EXCEPTION_INDEX_OUT_OF_RANGE);
			throw EXCEPTION_INDEX_OUT_OF_RANGE;
		}

		String result = *this;

		if (toBeInserted.isEmpty())
		{
			for (Index i = start; i < length - 1; i++)
				result.string[i] = result.string[i + 1];

			return result;
		}

		for (Index i = 0; i < toBeInserted.length; i++)
			result.string[start + i] = toBeInserted[i];

		return result;
	}

	Index LFind(char suspect) const noexcept
	{
		if (isEmpty())
			return 0;

		for (Index i = 0; i < length; i++)
			if (this->string[i] == suspect)
				return i;

		return 0;
	}

	Index RFind(char suspect) const noexcept
	{
		if (isEmpty())
			return 0;

		for (Index i = length - 1; i > 0; i--)
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
		if (start > end || start < 0 || end >= length)
		{
			Logger::Trace("At String at GetSubString(Index, Index)");
			logException(EXCEPTION_INDEX_OUT_OF_RANGE);
			throw EXCEPTION_INDEX_OUT_OF_RANGE;
		}

		String result = String(size_t(end - start + 2));
		result.length = end - start + 1;

		for (Index i = start; i <= end; i++)
			result[i - start] = this->string[i];

		result[result.length] = '\0';

		return result;
	}

	Pair<String, String> Split(char pivot) const noexcept
	{
		if (!isThere(pivot))
			return Pair<String, String>(String(), String());

		Index pivotIndex = LFind(pivot);

		String left = GetSubString(0, pivotIndex);
		String right = GetSubString(pivotIndex + 1, length - 1);

		return Pair<String, String>(left, right);
	}

	friend String operator+(const String& left, const String& rigth)
	{
		String result = String(size_t(left.length + rigth.length + 1));

		for (Index i = 0; i < left.length; i++)
			result[i] = left[i];

		for (Index i = 0; i <= rigth.length; i++)
			result[left.length + i] = rigth[i];

		result.length = left.length + rigth.length;
		result[result.length] = '\0';

		return result;
	}

	friend String operator*(const String& left, const int& right)
	{
		String result = String(size_t(left.length * right + 1));

		int counter = right;
		while (counter > 0)
		{
			for (Index i = 0; i < left.length; i++)
				result[i + left.length * (right - counter)] = left[i];

			counter--;
		}

		result.length = left.length * right;
		result[result.length] = '\0';

		return result;
	}

	char& operator[](const Index index) const noexcept
	{
		return string.Get(index);
	}

	friend std::ostream& operator<<(std::ostream& stream, const String& string)
	{
		stream << "\"";

		for (Index i = 0; i < string.length; i++)
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