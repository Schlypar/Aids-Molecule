#include "Logger.h"
#include "Sequence/Array.h"
#include "moduleTest.h"

#include "CString.h"
#include <catch2/catch.hpp>

TEST_CASE("Default constructor of String class")
{
	String s;
	REQUIRE(s.Length() == 0);
}

TEST_CASE("Constructor of String class that uses Array")
{
	Array<char> arr = { 'a', 'b', 'c' };
	String s(arr);
	REQUIRE(s.Length() == 3);
	REQUIRE(s[0] == 'a');
	REQUIRE(s[1] == 'b');
	REQUIRE(s[2] == 'c');
}

TEST_CASE("Constructor of String class that uses char* and count")
{
	char s[] = "abc";
	String s2(s, 3);
	REQUIRE(s2.Length() == 3);
	REQUIRE(s2[0] == 'a');
	REQUIRE(s2[1] == 'b');
	REQUIRE(s2[2] == 'c');
}

TEST_CASE("Constructor of String class that uses Size")
{
	String s(Size(3));
	REQUIRE(s.Length() == 3);
}

TEST_CASE("Constructor of String class that uses const char*")
{
	String s = "abc";
	REQUIRE(s.Length() == 3);
	REQUIRE(s[0] == 'a');
	REQUIRE(s[1] == 'b');
	REQUIRE(s[2] == 'c');
}

TEST_CASE("Copy constructor of String class")
{
	String s("abc");
	REQUIRE(s.Length() == 3);
	REQUIRE(s[0] == 'a');
	REQUIRE(s[1] == 'b');
	REQUIRE(s[2] == 'c');

	String s2(s);
	REQUIRE(s2.Length() == 3);
	REQUIRE(s2[0] == 'a');
	REQUIRE(s2[1] == 'b');
	REQUIRE(s2[2] == 'c');
}

TEST_CASE("Move constructor of String class")
{
	String s("abc");
	REQUIRE(s.Length() == 3);
	REQUIRE(s[0] == 'a');
	REQUIRE(s[1] == 'b');
	REQUIRE(s[2] == 'c');

	String s2(std::move(s));

	REQUIRE(s.Length() == 0);

	REQUIRE(s2.Length() == 3);
	REQUIRE(s2[0] == 'a');
	REQUIRE(s2[1] == 'b');
	REQUIRE(s2[2] == 'c');
}

TEST_CASE("Count method of String class correctly coubnt chars in a string")
{
	String s("cabccc");
	REQUIRE(s.Count('a') == 1);
	REQUIRE(s.Count('b') == 1);
	REQUIRE(s.Count('c') == 4);

	REQUIRE(s.Count('d') == 0);
	REQUIRE(s.Count('e') == 0);
}

TEST_CASE("Replace method of String class correctly replaces chars in a string")
{
	String s("cabccc");

	s.Replace('c', 'd', s.Count('c'));
	REQUIRE(s[0] == 'd');
	REQUIRE(s[1] == 'a');
	REQUIRE(s[2] == 'b');
	REQUIRE(s[3] == 'd');

	REQUIRE(s.Count('d') == 4);
}

TEST_CASE("isThere method correctly determines if a string contains a char")
{
	SECTION("isThere that takes a char")
	{
		String s("cabccc");
		REQUIRE(s.isThere('a'));
		REQUIRE(s.isThere('b'));
		REQUIRE(s.isThere('c'));
		REQUIRE(s.isThere('d') == false);
	}
	SECTION("isThere that takes a string")
	{
		String s("cabccc");
		REQUIRE(s.isThere("ab"));
		REQUIRE(s.isThere("bc"));
		REQUIRE(s.isThere("cc"));
		REQUIRE(s.isThere("dd") == false);
	}
}

TEST_CASE("Slice method of String class correctly slices a string")
{
	const String s = "cabccc";
	String s2 = s.Slice(1, "bc");

	REQUIRE(s2[1] == 'b');
	REQUIRE(s2[2] == 'c');

	REQUIRE(s2.Length() == 6);
}

TEST_CASE("LFind method of String class correctly finds a char in a string")
{
	SECTION("LFind that finds a char")
	{
		String s("cabccc");
		REQUIRE(s.LFind('a') == 1);
		REQUIRE(s.LFind('b') == 2);
		REQUIRE(s.LFind('c') == 0);
	}
	SECTION("LFind that doesn't find a char")
	{
		String s("cabccc");
		REQUIRE(s.LFind('d') == -1);
		REQUIRE(s.LFind('f') == -1);
	}
}

TEST_CASE("RFind method of String class correctly finds a char in a string")
{
	SECTION("RFind that finds a char")
	{
		String s("cabccc");
		REQUIRE(s.RFind('a') == 1);
		REQUIRE(s.RFind('b') == 2);
		REQUIRE(s.RFind('c') == 5);
	}
	SECTION("RFind that doesn't find a char")
	{
		String s("cabccc");
		REQUIRE(s.RFind('d') == -1);
		REQUIRE(s.RFind('f') == -1);
	}
}

TEST_CASE("GetSubstring method of String class correctly returns a substring")
{
	SECTION("GetSubstring that returns a substring")
	{
		String s("cabccc");
		String s2 = s.GetSubString(1, 3);

		REQUIRE(s2[0] == 'a');
		REQUIRE(s2[1] == 'b');
		REQUIRE(s2[2] == 'c');
		REQUIRE(s2.Length() == 3);
	}
	SECTION("GetSubstring that returns a null substring")
	{
		String s("cabccc");
		String s2 = s.GetSubString(0, 0);
		REQUIRE(s2.Length() == 1);
		REQUIRE(s2[0] == 'c');
	}
}

TEST_CASE("Split method of String class correctly splits a string")
{
	SECTION("Split that splits a string")
	{
		String s("cabccc");
		Pair<String, String> p = s.Split('b');

		String left = p.GetLeft();
		String right = p.GetRight();

		REQUIRE(left[0] == 'c');
		REQUIRE(left[1] == 'a');
		REQUIRE(left[2] == 'b');

		REQUIRE(right[0] == 'c');
		REQUIRE(right[1] == 'c');
		REQUIRE(right[2] == 'c');
	}
	SECTION("Split that doesn't split a string")
	{
		String s("cabccc");
		Pair<String, String> p = s.Split('f');

		String left = p.GetLeft();
		String right = p.GetRight();

		REQUIRE(left == s);
		REQUIRE(right.Length() == 0);
	}
}

TEST_CASE("Operator+ of String class correctly concatenates two strings")
{
	SECTION("Operator+ that concatenates two non-empty strings")
	{
		String s1("abc");
		String s2("def");
		String s3 = s1 + s2;

		REQUIRE(s3.Length() == 6);

		REQUIRE(s3[0] == 'a');
		REQUIRE(s3[1] == 'b');
		REQUIRE(s3[2] == 'c');

		REQUIRE(s3[3] == 'd');
		REQUIRE(s3[4] == 'e');
		REQUIRE(s3[5] == 'f');

		String s4 = String("abcd") + String("efgh") + String("ijkl");
		REQUIRE(s4.Length() == 12);

		REQUIRE(s4[0] == 'a');
		REQUIRE(s4[1] == 'b');
		REQUIRE(s4[2] == 'c');
		REQUIRE(s4[3] == 'd');

		REQUIRE(s4[4] == 'e');
		REQUIRE(s4[5] == 'f');
		REQUIRE(s4[6] == 'g');
		REQUIRE(s4[7] == 'h');

		REQUIRE(s4[8] == 'i');
		REQUIRE(s4[9] == 'j');
		REQUIRE(s4[10] == 'k');
		REQUIRE(s4[11] == 'l');
	}
	SECTION("Operator+ that concatenates two empty strings")
	{
		String s1("");
		String s2("");
		String s3 = s1 + s2;

		REQUIRE(s3.Length() == 0);
	}
	SECTION("Operator+ that concatenates a full string and an empty string")
	{
		String s1("abc");
		String s2("");
		String s3 = s1 + s2;

		REQUIRE(s3 == s1);
	}
}

TEST_CASE("Operator* of String class correctly multiplies a string by a number")
{
	SECTION("Operator* that multiplies a non-empty string by a number")
	{
		String s1("abc");
		String s2 = s1 * 2;

		REQUIRE(s2.Length() == 6);

		REQUIRE(s2[0] == 'a');
		REQUIRE(s2[1] == 'b');
		REQUIRE(s2[2] == 'c');

		REQUIRE(s2[3] == 'a');
		REQUIRE(s2[4] == 'b');
		REQUIRE(s2[5] == 'c');

		String s3 = s1 * 0;
		REQUIRE(s3.Length() == 0);
	}
	SECTION("Operator* that multiplies an empty string by a number")
	{
		String s1("");
		String s2 = s1 * 2;

		REQUIRE(s2.Length() == 0);
	}
}
