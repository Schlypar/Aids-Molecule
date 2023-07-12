#include "CString.h"

Size String::Count(char suspect)
{
	if (isEmpty())
		return 0;

	Size count = 0;
	for (Index i = 0; i < string.GetLength(); i++)
		if (this->string[i] == suspect)
			count++;

	return count;
}

void String::Replace(char what, char with, Size howMuch)
{
	if (Length() == 0 || howMuch > Length())
	{
		Logger::Trace("At String at Replace(char, char, Size)");
		logException(EXCEPTION_BAD_LOGIC);
		throw EXCEPTION_BAD_LOGIC;
	}

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

bool String::isThere(char suspect) const noexcept
{
	if (this->string[0] == suspect)
		return true;

	return (LFind(suspect) == -1) ? false : true;
}

bool String::isThere(const String& suspect) const noexcept
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

void String::Slice(Index start, const String& toBeInserted)
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

String String::Slice(Index start, const String& toBeInserted) const
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

Index String::LFind(char suspect) const noexcept
{
	if (isEmpty())
		return -1;

	for (Index i = 0; i < Length(); i++)
		if (this->string[i] == suspect)
			return i;

	return -1;
}

Index String::RFind(char suspect) const noexcept
{
	if (isEmpty())
		return -1;

	for (Index i = Length() - 1; i > 0; i--)
		if (this->string[i] == suspect)
			return i;

	return -1;
}

String& String::operator=(const String& other)
{
	this->string = other.string;

	return *this;
}

String& String::operator=(String&& other)
{
	this->string = other.string;
	other.string = Array<char>();

	return *this;
}

String String::GetSubString(Index start, Index end) const
{
	if (start > end || start < 0 || end >= Length())
	{
		Logger::Trace("At String at GetSubString(Index, Index)");
		logException(EXCEPTION_INDEX_OUT_OF_RANGE);
		throw EXCEPTION_INDEX_OUT_OF_RANGE;
	}

	String result = String(size_t(end - start + 1));

	for (Index i = start; i <= end; i++)
		result[i - start] = this->string[i];

	return result;
}

bool String::operator==(const String& other) const noexcept
{
	if (this->Length() != other.Length())
		return false;

	Size length = this->Length();
	for (Index i = 0; i < length; i++)
		if (this->string[i] != other.string[i])
			return false;

	return true;
}

Pair<String, String> String::Split(char pivot) const noexcept
{
	if (!isThere(pivot))
		return Pair<String, String>(String(*this), String());

	Index pivotIndex = LFind(pivot);

	String left = GetSubString(0, pivotIndex);
	String right = GetSubString(pivotIndex + 1, Length() - 1);

	return Pair<String, String>(left, right);
}
