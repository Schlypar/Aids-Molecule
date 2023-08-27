#include "Sequence/ArraySequence.h"
#include "Sorter/HeapSort.h"

#include "moduleTest.h"
#include "Sequence/SortedSequence.h"

TEST_CASE("SortedSequence constructors")
{
	SECTION("Constructors")
	{
		SortedSequence<int> sorted1 = SortedSequence<int> {
			new ArraySequence<int>(),
			new HeapSorter<int>(),
			[](const int& a, const int& b) -> int
			{ return a - b; }
		};

		REQUIRE(sorted1.GetLength() == 0);

		sorted1.Add(1);

		SortedSequence<int> sorted2 = sorted1;
		REQUIRE(sorted1.GetLength() == sorted2.GetLength());
		REQUIRE(sorted1 == sorted2);

		sorted2.Add(2);
		REQUIRE_FALSE(sorted2 == sorted1);

		SortedSequence<int> sorted3 = std::move(sorted2);

		REQUIRE(sorted2.GetLength() == 0);
		REQUIRE(sorted3.GetLength() == 2);
		REQUIRE(sorted3[0] == 1);
		REQUIRE(sorted3[1] == 2);
	}
	SECTION("Factory")
	{
		SortedSequenceFactory<int> factory;

		REQUIRE_THROWS_AS(
			factory.Build(),
			std::invalid_argument
		);
		REQUIRE_THROWS_AS(
			factory
				.SetSequence(new ArraySequence<int>())
				->Build(),
			std::invalid_argument
		);
		REQUIRE_THROWS_AS(
			factory
				.SetSequence(new ArraySequence<int>())
				->SetSorter(new HeapSorter<int>())
				->Build(),
			std::invalid_argument
		);

		SortedSequence<int> sortedSequence = factory
							     .SetSequence(new ArraySequence<int>())
							     ->SetSorter(new HeapSorter<int>())
							     ->SetComparator([](const int& a, const int& b) -> int
									     { return a - b; })
							     ->Build();
	}
}

TEST_CASE("METHODS")
{
	SECTION("Add")
	{
		SortedSequence<int> sortedSequence = SortedSequenceFactory<int>()
							     .SetSequence(new ArraySequence<int>())
							     ->SetSorter(new HeapSorter<int>())
							     ->SetComparator([](const int& a, const int& b) -> int
									     { return a - b; })
							     ->Build();

		sortedSequence.Add(10);
		sortedSequence.Add(-10);
		sortedSequence.Add(5);
		sortedSequence.Add(7);
		sortedSequence.Add(0);
		sortedSequence.Add(-1);
		sortedSequence.Add(6);
		sortedSequence.Add(3);

		REQUIRE(sortedSequence.GetLength() == 8);

		for (int i = 0; i < sortedSequence.GetLength() - 1; i++)
		{
			REQUIRE(sortedSequence[i] <= sortedSequence[i + 1]);
		}

		REQUIRE(sortedSequence.GetFirst() == -10);
		REQUIRE(sortedSequence.GetLast() == 10);
	}
	SECTION("Remove")
	{
		SECTION("Working correctly")
		{
			SortedSequence<int> sortedSequence = SortedSequenceFactory<int>()
								     .SetSequence(new ArraySequence<int>())
								     ->SetSorter(new HeapSorter<int>())
								     ->SetComparator([](const int& a, const int& b) -> int
										     { return a - b; })
								     ->Build();

			sortedSequence.Add(10);
			sortedSequence.Add(-10);
			sortedSequence.Add(5);
			sortedSequence.Add(7);
			sortedSequence.Add(0);
			sortedSequence.Add(-1);
			sortedSequence.Add(6);
			sortedSequence.Add(3);

			REQUIRE(sortedSequence.GetLength() == 8);

			sortedSequence.Remove(0);

			REQUIRE(sortedSequence.GetLength() == 7);

			REQUIRE(sortedSequence[0] != -10);
			REQUIRE(sortedSequence[0] == -1);

			REQUIRE(sortedSequence[1] == 0);
			sortedSequence.Remove(1);

			REQUIRE(sortedSequence[1] != 0);
			REQUIRE(sortedSequence[1] == 3);
		}
		SECTION("Working incorrectly")
		{
			SortedSequence<int> sortedSequence = SortedSequenceFactory<int>()
								     .SetSequence(new ArraySequence<int>())
								     ->SetSorter(new HeapSorter<int>())
								     ->SetComparator([](const int& a, const int& b) -> int
										     { return a - b; })
								     ->Build();

			REQUIRE_THROWS_AS(sortedSequence.Remove(0), Exception);

			sortedSequence.Add(10);
			sortedSequence.Add(-10);
			sortedSequence.Add(5);
			sortedSequence.Add(7);
			sortedSequence.Add(0);
			sortedSequence.Add(-1);
			sortedSequence.Add(6);
			sortedSequence.Add(3);

			REQUIRE_THROWS_AS(sortedSequence.Remove(1000), Exception);
		}
	}
	SECTION("IndexOf")
	{
		SortedSequence<int> sortedSequence = SortedSequenceFactory<int>()
							     .SetSequence(new ArraySequence<int>())
							     ->SetSorter(new HeapSorter<int>())
							     ->SetComparator([](const int& a, const int& b) -> int
									     { return a - b; })
							     ->Build();

		sortedSequence.Add(10);
		sortedSequence.Add(-10);
		sortedSequence.Add(5);
		sortedSequence.Add(7);
		sortedSequence.Add(0);
		sortedSequence.Add(-1);
		sortedSequence.Add(6);
		sortedSequence.Add(3);

		REQUIRE(sortedSequence.IndexOf(-10) == 0);
		REQUIRE(sortedSequence.IndexOf(10) == 7);
		REQUIRE(sortedSequence.IndexOf(-1) == 1);
		REQUIRE(sortedSequence.IndexOf(0) == 2);
		// rest probably is correct

		REQUIRE(sortedSequence.IndexOf(1000) == -1);
		REQUIRE(sortedSequence.IndexOf(-1000) == -1);
	}
	SECTION("RemoveByValue")
	{
		SECTION("Working correctly")
		{
			SortedSequence<int> sortedSequence = SortedSequenceFactory<int>()
								     .SetSequence(new ArraySequence<int>())
								     ->SetSorter(new HeapSorter<int>())
								     ->SetComparator([](const int& a, const int& b) -> int
										     { return a - b; })
								     ->Build();

			sortedSequence.Add(10);
			sortedSequence.Add(-10);
			sortedSequence.Add(5);
			sortedSequence.Add(7);
			sortedSequence.Add(0);
			sortedSequence.Add(-1);
			sortedSequence.Add(6);
			sortedSequence.Add(3);

			sortedSequence.RemoveByValue(-10);
			REQUIRE(sortedSequence.GetLength() == 7);
			REQUIRE(sortedSequence.IndexOf(-10) == -1);

			sortedSequence.RemoveByValue(0);
			REQUIRE(sortedSequence.GetLength() == 6);
			REQUIRE(sortedSequence.IndexOf(0) == -1);

			sortedSequence.RemoveByValue(10);
			REQUIRE(sortedSequence.GetLength() == 5);
			REQUIRE(sortedSequence.IndexOf(10) == -1);
		}
		SECTION("Working incorrectly")
		{
			SortedSequence<int> sortedSequence = SortedSequenceFactory<int>()
								     .SetSequence(new ArraySequence<int>())
								     ->SetSorter(new HeapSorter<int>())
								     ->SetComparator([](const int& a, const int& b) -> int
										     { return a - b; })
								     ->Build();

			REQUIRE_THROWS_AS(sortedSequence.RemoveByValue(10), Exception);

			sortedSequence.Add(10);
			sortedSequence.Add(-10);
			sortedSequence.Add(5);
			sortedSequence.Add(7);
			sortedSequence.Add(0);
			sortedSequence.Add(-1);
			sortedSequence.Add(6);
			sortedSequence.Add(3);

			REQUIRE_THROWS_AS(sortedSequence.RemoveByValue(11), Exception);

			sortedSequence.RemoveByValue(-10);
			REQUIRE(sortedSequence.GetLength() == 7);
			REQUIRE(sortedSequence.IndexOf(-10) == -1);
			REQUIRE_THROWS_AS(sortedSequence.RemoveByValue(-10), Exception);
		}
	}
}
