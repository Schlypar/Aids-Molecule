#include "Logger.h"
#include "moduleTest.h"

#include "Tree/OrderedSet.h"

template <typename T1>
using SetPtr = SharedPtr<OrderedSet<T1>>;

TEST_CASE("Default constructor of OrderedSet")
{
	SetPtr<int> set = new OrderedSet<int>();

	REQUIRE(Cardinalis(set) == 0);
}

TEST_CASE("Starting value of OrderedSet")
{
	int startingValue = 1;
	SetPtr<int> set = new OrderedSet<int>(1);

	REQUIRE(Cardinalis(set) == 1);
	REQUIRE(set->Includes(1));
}

TEST_CASE("Copying constructor from another")
{
	int startingValue = 1;
	SetPtr<int> set = new OrderedSet<int>(startingValue);
	set->Include(2)->Include(3)->Include(7);

	SetPtr<int> copy = new OrderedSet<int>(*set);

	REQUIRE(copy->Includes(startingValue) == true);
	REQUIRE(copy->Includes(2) == true);
	REQUIRE(copy->Includes(3) == true);
	REQUIRE(copy->Includes(7) == true);
}

TEST_CASE("Copying constructor from pointer to another")
{
	int startingValue = 1;
	SetPtr<int> set = new OrderedSet<int>(startingValue);
	set->Include(2)->Include(3)->Include(7);

	SetPtr<int> copy = new OrderedSet<int>(set.Get());

	REQUIRE(copy->Includes(startingValue) == true);
	REQUIRE(copy->Includes(2) == true);
	REQUIRE(copy->Includes(3) == true);
	REQUIRE(copy->Includes(7) == true);
}

TEST_CASE("Erase method correctly deletes value from a set")
{
	int startingValue = 1;
	SetPtr<int> set = new OrderedSet<int>(startingValue);
	set->Include(2)->Include(3)->Include(7);

	REQUIRE(set->Includes(startingValue) == true);
	REQUIRE(set->Includes(2) == true);
	REQUIRE(set->Includes(3) == true);
	REQUIRE(set->Includes(7) == true);

	set->Erase(3);
	REQUIRE(set->Includes(3) == false);

	set->Erase(startingValue);
	REQUIRE(set->Includes(startingValue) == false);

	set->Erase(7);
	REQUIRE(set->Includes(7) == false);

	REQUIRE(set->Includes(2) == true);
	REQUIRE(Cardinalis(set) == 1);
}

TEST_CASE("Union of sets")
{
	SECTION("Union of two full sets")
	{
		int startingValue = 1;
		SetPtr<int> set = new OrderedSet<int>(startingValue);
		set->Include(2)->Include(3)->Include(4);

		int startingValueOther = 10;
		SetPtr<int> otherSet = new OrderedSet<int>(startingValueOther);
		otherSet->Include(20)->Include(30)->Include(40);

		SetPtr<int> un = set->Union(otherSet.Get());

		REQUIRE(Cardinalis(un) == Cardinalis(set) + Cardinalis(otherSet));

		REQUIRE(un->Includes(1) == true);
		REQUIRE(un->Includes(2) == true);
		REQUIRE(un->Includes(3) == true);
		REQUIRE(un->Includes(4) == true);

		REQUIRE(un->Includes(10) == true);
		REQUIRE(un->Includes(20) == true);
		REQUIRE(un->Includes(30) == true);
		REQUIRE(un->Includes(40) == true);
	}
	SECTION("Union with full set and empty set")
	{
		int startingValue = 1;
		SetPtr<int> set = new OrderedSet<int>(startingValue);
		set->Include(2)->Include(3)->Include(4);

		SetPtr<int> otherSet = new OrderedSet<int>();
		REQUIRE(Cardinalis(otherSet) == 0);

		SetPtr<int> un = set->Union(otherSet.Get());

		REQUIRE(Cardinalis(un) == Cardinalis(set));

		REQUIRE(un->Includes(1) == true);
		REQUIRE(un->Includes(2) == true);
		REQUIRE(un->Includes(3) == true);
		REQUIRE(un->Includes(4) == true);

		SetPtr<int> unAlt = otherSet->Union(set.Get());

		REQUIRE(Cardinalis(unAlt) == Cardinalis(set));

		REQUIRE(unAlt->Includes(1) == true);
		REQUIRE(unAlt->Includes(2) == true);
		REQUIRE(unAlt->Includes(3) == true);
		REQUIRE(unAlt->Includes(4) == true);
	}
	SECTION("Union of two empty sets")
	{
		SetPtr<int> set = new OrderedSet<int>();
		REQUIRE(Cardinalis(set) == 0);

		SetPtr<int> otherSet = new OrderedSet<int>();
		REQUIRE(Cardinalis(otherSet) == 0);

		SetPtr<int> un = set->Union(otherSet.Get());
		REQUIRE(Cardinalis(un) == 0);
	}
}

TEST_CASE("Intersection of sets")
{
	SECTION("Intersection of two full sets")
	{
		SECTION("Intersection is an empty set")
		{
			int startingValue = 1;
			SetPtr<int> set = new OrderedSet<int>(startingValue);
			set->Include(2)->Include(3)->Include(7);

			int startingValueOther = 10;
			SetPtr<int> otherSet = new OrderedSet<int>(startingValueOther);
			otherSet->Include(20)->Include(30)->Include(40);

			SetPtr<int> intersection = set->Intersection(otherSet.Get());
			REQUIRE(Cardinalis(intersection) == 0);
		}
		SECTION("Intersection is a non-empty set")
		{
			int startingValue = 1;
			SetPtr<int> set = new OrderedSet<int>(startingValue);
			set->Include(2)->Include(3)->Include(4);

			int startingValueOther = 10;
			SetPtr<int> otherSet = new OrderedSet<int>(startingValueOther);
			otherSet->Include(20)->Include(3)->Include(4);

			SetPtr<int> intersection = set->Intersection(otherSet.Get());
			REQUIRE(Cardinalis(intersection) == 2);

			REQUIRE(intersection->Includes(3) == true);
			REQUIRE(intersection->Includes(4) == true);
		}
	}
	SECTION("Intersection of full set and an empty set")
	{
		int startingValue = 1;
		SetPtr<int> set = new OrderedSet<int>(startingValue);
		set->Include(2)->Include(3)->Include(7);

		SetPtr<int> otherSet = new OrderedSet<int>();

		SetPtr<int> intersection = set->Intersection(otherSet.Get());
		REQUIRE(Cardinalis(intersection) == 0);
	}
	SECTION("Intersection of two empty sets")
	{
		SetPtr<int> set = new OrderedSet<int>();

		SetPtr<int> otherSet = new OrderedSet<int>();

		SetPtr<int> intersection = set->Intersection(otherSet.Get());
		REQUIRE(Cardinalis(intersection) == 0);
	}
}

TEST_CASE("Difference of two sets")
{
	SECTION("Difference between two full sets")
	{
		SECTION("Difference does nothing")
		{
			int startingValue = 1;
			SetPtr<int> set = new OrderedSet<int>(startingValue);
			set->Include(2)->Include(3)->Include(7);

			int startingValueOther = 10;
			SetPtr<int> otherSet = new OrderedSet<int>(startingValueOther);
			otherSet->Include(20)->Include(30)->Include(40);

			SetPtr<int> intersection = set->Difference(otherSet.Get());
			REQUIRE(Cardinalis(intersection) == Cardinalis(set));

			REQUIRE(intersection->Includes(startingValue) == true);
			REQUIRE(intersection->Includes(2) == true);
			REQUIRE(intersection->Includes(3) == true);
			REQUIRE(intersection->Includes(7) == true);
		}
		SECTION("Difference makes different set")
		{
			int startingValue = 1;
			SetPtr<int> set = new OrderedSet<int>(startingValue);
			set->Include(2)->Include(3)->Include(4);

			int startingValueOther = 10;
			SetPtr<int> otherSet = new OrderedSet<int>(startingValueOther);
			otherSet->Include(20)->Include(3)->Include(4);

			SetPtr<int> intersection = set->Difference(otherSet.Get());
			REQUIRE(Cardinalis(intersection) == 2);

			REQUIRE(intersection->Includes(startingValue) == true);
			REQUIRE(intersection->Includes(2) == true);

			REQUIRE(intersection->Includes(3) == false);
			REQUIRE(intersection->Includes(7) == false);
		}
	}
	SECTION("Difference between a full set and an empty one")
	{
		int startingValue = 1;
		SetPtr<int> set = new OrderedSet<int>(startingValue);
		set->Include(2)->Include(3)->Include(4);

		SetPtr<int> otherSet = new OrderedSet<int>();

		SetPtr<int> intersection = set->Difference(otherSet.Get());
		REQUIRE(Cardinalis(intersection) == Cardinalis(set));

		REQUIRE(intersection->Includes(startingValue) == true);
		REQUIRE(intersection->Includes(2) == true);
		REQUIRE(intersection->Includes(3) == true);
		REQUIRE(intersection->Includes(4) == true);
	}

	SECTION("Difference between two empty sets")
	{
		SetPtr<int> set = new OrderedSet<int>();

		SetPtr<int> otherSet = new OrderedSet<int>();

		SetPtr<int> intersection = set->Difference(otherSet.Get());
		REQUIRE(Cardinalis(intersection) == 0);
	}
}

TEST_CASE("Subset method correctly determines truth")
{
	int startingValue = 1;
	SetPtr<int> set = new OrderedSet<int>(startingValue);
	set->Include(2)->Include(3)->Include(7);

	REQUIRE(set->IncludesSubset(set.Get()) == true);

	SetPtr<int> emptySet = new OrderedSet<int>();

	REQUIRE(set->IncludesSubset(emptySet.Get()) == true);
	REQUIRE(emptySet->IncludesSubset(set.Get()) == false);

	SetPtr<int> subset = new OrderedSet<int>();
	subset->Include(3)->Include(startingValue);

	REQUIRE(set->IncludesSubset(subset.Get()) == true);
}