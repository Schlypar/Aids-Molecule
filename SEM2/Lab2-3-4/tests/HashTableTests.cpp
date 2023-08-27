#include "Dict/STLHasher.h"
#include "moduleTest.h"

#include "Dict/HashTable.h"

TEST_CASE("HashTable constructors")
{
	SECTION("Default constructor")
	{
		HashTable<std::string, int> table;

		REQUIRE(table.GetCount() == 0);
		REQUIRE(table.GetCapacity() == DEFAULT_SIZE);
	}
	SECTION("Constructor that takes hasher")
	{
		HashTable<std::string, int> table(new STLHasher<std::string>());

		REQUIRE(table.GetCount() == 0);
		REQUIRE(table.GetCapacity() == DEFAULT_SIZE);
	}
	SECTION("Constructor that takes unlimited number of pairs")
	{
		HashTable<std::string, int> table = {
			{"abc", 1},
			{"bca", 2},
			{"cab", 3}
		};

		REQUIRE(table.GetCount() == 3);
		REQUIRE(table.GetCapacity() >= 3);

		REQUIRE(table["abc"] == 1);
		REQUIRE(table["bca"] == 2);
		REQUIRE(table["cab"] == 3);
	}
	SECTION("Constructor that takes hasher and unlimited number of pairs")
	{
		HashTable<std::string, int> table = {
			new STLHasher<std::string>(),
			{{ "abc", 1 }, { "bca", 2 }, { "cab", 3 }}
		};

		REQUIRE(table.GetCount() == 3);
		REQUIRE(table.GetCapacity() >= 3);

		REQUIRE(table["abc"] == 1);
		REQUIRE(table["bca"] == 2);
		REQUIRE(table["cab"] == 3);
	}
	SECTION("Copying constructor")
	{
		HashTable<std::string, int> table = {
			new STLHasher<std::string>(),
			{{ "abc", 1 }, { "bca", 2 }, { "cab", 3 }}
		};

		REQUIRE(table.GetCount() == 3);
		REQUIRE(table.GetCapacity() >= 3);

		HashTable<std::string, int> copy = table;

		REQUIRE(table.GetCount() == 3);
		REQUIRE(table.GetCapacity() >= 3);

		REQUIRE(copy.GetCount() == 3);
		REQUIRE(copy.GetCapacity() >= 3);

		REQUIRE(table["abc"] == 1);
		REQUIRE(table["bca"] == 2);
		REQUIRE(table["cab"] == 3);

		REQUIRE(copy["abc"] == 1);
		REQUIRE(copy["bca"] == 2);
		REQUIRE(copy["cab"] == 3);
	}
	SECTION("Moving constructor")
	{
		HashTable<std::string, int> table = {
			new STLHasher<std::string>(),
			{{ "abc", 1 }, { "bca", 2 }, { "cab", 3 }}
		};

		REQUIRE(table.GetCount() == 3);
		REQUIRE(table.GetCapacity() >= 3);

		HashTable<std::string, int> copy = std::move(table);

		REQUIRE(table.GetCount() == 0);
		REQUIRE(table.GetCapacity() == 0);

		REQUIRE(copy.GetCount() == 3);
		REQUIRE(copy.GetCapacity() >= 3);

		REQUIRE(copy["abc"] == 1);
		REQUIRE(copy["bca"] == 2);
		REQUIRE(copy["cab"] == 3);
	}
}

TEST_CASE("HashTable methods")
{
	SECTION("Add")
	{
		HashTable<std::string, int> table = {
			new STLHasher<std::string>(),
			{{ "abc", 1 }, { "bca", 2 }, { "cab", 3 }}
		};

		REQUIRE(table.GetCount() == 3);
		REQUIRE(table.GetCapacity() >= 3);

		table.Add({ "add10", 10 });
		REQUIRE(table.GetCount() == 4);
		REQUIRE(table.GetCapacity() >= table.GetCount());
		REQUIRE(table["add10"] == 10);

		table.Add({ "add20", 20 });
		REQUIRE(table.GetCount() == 5);
		REQUIRE(table.GetCapacity() >= table.GetCount());
		REQUIRE(table["add20"] == 20);

		table.Add({ "add30", 30 });
		REQUIRE(table.GetCount() == 6);
		REQUIRE(table.GetCapacity() >= table.GetCount());
		REQUIRE(table["add30"] == 30);

		table.Add({ "add40", 40 });
		REQUIRE(table.GetCount() == 7);
		REQUIRE(table.GetCapacity() >= table.GetCount());
		REQUIRE(table["add40"] == 40);

		REQUIRE(table["abc"] == 1);
		REQUIRE(table["bca"] == 2);
		REQUIRE(table["cab"] == 3);
	}
	SECTION("Remove")
	{
		SECTION("Working correctly")
		{
			HashTable<std::string, int> table = {
				new STLHasher<std::string>(),
				{{ "abc", 1 }, { "bca", 2 }, { "cab", 3 }}
			};

			table.Add({ "add10", 10 });
			table.Add({ "add20", 20 });
			table.Add({ "add30", 30 });
			table.Add({ "add40", 40 });

			REQUIRE(table.GetCount() == 7);
			REQUIRE(table.GetCapacity() >= table.GetCount());

			table.Remove("add10");
			REQUIRE(table.GetCount() == 6);
			REQUIRE(table.GetCapacity() >= table.GetCount());

			table.Remove("add20");
			REQUIRE(table.GetCount() == 5);
			REQUIRE(table.GetCapacity() >= table.GetCount());

			REQUIRE(table["abc"] == 1);
			REQUIRE(table["bca"] == 2);
			REQUIRE(table["cab"] == 3);
			REQUIRE(table["add30"] == 30);
			REQUIRE(table["add40"] == 40);
		}
		SECTION("Working incorrectly")
		{
			HashTable<std::string, int> table = {
				new STLHasher<std::string>(),
				{{ "abc", 1 }, { "bca", 2 }, { "cab", 3 }}
			};

			REQUIRE_THROWS_AS(table.Remove(""), std::out_of_range);
			REQUIRE_THROWS_AS(table.Remove("awdasd"), std::out_of_range);
		}
	}
}
