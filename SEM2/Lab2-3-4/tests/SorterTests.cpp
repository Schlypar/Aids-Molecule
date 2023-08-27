#include "Sequence/ArraySequence.h"

#include "Sorter/HeapSort.h"
#include "Sorter/QuickSort.h"
#include "Sorter/ShellSort.h"

#include "moduleTest.h"

TEST_CASE("Heap Sort")
{
	ISorter<int>* sorter = new HeapSorter<int>();

	const auto comparator = [](const int& a, const int& b) -> int
	{ return a - b; };

	const auto anticomparator = [](const int& a, const int& b) -> int
	{ return b - a; };

	Sequence<int>* seq = new ArraySequence<int>();
	REQUIRE(seq->GetLength() == 0);

	Sequence<int>* sortedseq = sorter->Sort(seq, comparator);
	REQUIRE(sortedseq->GetLength() == 0);
	delete sortedseq;

	seq->Append(1);
	sortedseq = sorter->Sort(seq, comparator);
	REQUIRE(sortedseq->GetLength() == 1);
	delete sortedseq;

	seq->Append(-1);
	sortedseq = sorter->Sort(seq, comparator);
	REQUIRE(sortedseq->GetLength() == 2);
	REQUIRE(sortedseq->Get(0) == -1);
	REQUIRE(sortedseq->Get(1) == 1);
	delete sortedseq;

	Sequence<int>* seq2 = new ArraySequence<int>();
	seq2->Append(1)->Append(2);
	sortedseq = sorter->Sort(seq2, comparator);
	REQUIRE(sortedseq->GetLength() == 2);
	REQUIRE(sortedseq->Get(0) == 1);
	REQUIRE(sortedseq->Get(1) == 2);
	delete sortedseq;

	seq2->Append(3)->Append(4)->Append(5)->Append(6)->Append(7)->Append(8);
	sortedseq = sorter->Sort(seq2, comparator);
	REQUIRE(sortedseq->GetLength() == 8);
	REQUIRE(sortedseq->Get(0) == 1);
	REQUIRE(sortedseq->Get(1) == 2);
	REQUIRE(sortedseq->Get(2) == 3);
	REQUIRE(sortedseq->Get(3) == 4);
	REQUIRE(sortedseq->Get(4) == 5);
	REQUIRE(sortedseq->Get(5) == 6);
	REQUIRE(sortedseq->Get(6) == 7);
	REQUIRE(sortedseq->Get(7) == 8);
	delete seq2;

	Sequence<int>* antisorted = sorter->Sort(sortedseq, anticomparator);
	REQUIRE(antisorted->GetLength() == 8);
	REQUIRE(antisorted->Get(0) == 8);
	REQUIRE(antisorted->Get(1) == 7);
	REQUIRE(antisorted->Get(2) == 6);
	REQUIRE(antisorted->Get(3) == 5);
	REQUIRE(antisorted->Get(4) == 4);
	REQUIRE(antisorted->Get(5) == 3);
	REQUIRE(antisorted->Get(6) == 2);
	REQUIRE(antisorted->Get(7) == 1);
	delete sortedseq;
	delete antisorted;
	delete seq;

	Sequence<int>* random = new ArraySequence<int>(5, 6, -1, 1, 1, 1, 0, 10, -15, 25, 20);

	sortedseq = sorter->Sort(random, comparator);
	REQUIRE(sortedseq->GetLength() == 11);
	REQUIRE(sortedseq->Get(0) == -15);
	REQUIRE(sortedseq->Get(1) == -1);
	REQUIRE(sortedseq->Get(2) == 0);
	REQUIRE(sortedseq->Get(3) == 1);
	REQUIRE(sortedseq->Get(4) == 1);
	REQUIRE(sortedseq->Get(5) == 1);
	REQUIRE(sortedseq->Get(6) == 5);
	REQUIRE(sortedseq->Get(7) == 6);
	REQUIRE(sortedseq->Get(8) == 10);
	REQUIRE(sortedseq->Get(9) == 20);
	REQUIRE(sortedseq->Get(10) == 25);
	delete sortedseq;
	delete random;
	delete sorter;
}

TEST_CASE("Quick Sort")
{
	ISorter<int>* sorter = new QuickSorter<int>();

	const auto comparator = [](const int& a, const int& b) -> int
	{ return a - b; };

	const auto anticomparator = [](const int& a, const int& b) -> int
	{ return b - a; };

	Sequence<int>* seq = new ArraySequence<int>();
	REQUIRE(seq->GetLength() == 0);

	Sequence<int>* sortedseq = sorter->Sort(seq, comparator);
	REQUIRE(sortedseq->GetLength() == 0);
	delete sortedseq;

	seq->Append(1);
	sortedseq = sorter->Sort(seq, comparator);
	REQUIRE(sortedseq->GetLength() == 1);
	delete sortedseq;

	seq->Append(-1);
	sortedseq = sorter->Sort(seq, comparator);
	REQUIRE(sortedseq->GetLength() == 2);
	REQUIRE(sortedseq->Get(0) == -1);
	REQUIRE(sortedseq->Get(1) == 1);
	delete sortedseq;

	Sequence<int>* seq2 = new ArraySequence<int>();
	seq2->Append(1)->Append(2);
	sortedseq = sorter->Sort(seq2, comparator);
	REQUIRE(sortedseq->GetLength() == 2);
	REQUIRE(sortedseq->Get(0) == 1);
	REQUIRE(sortedseq->Get(1) == 2);
	delete sortedseq;

	seq2->Append(3)->Append(4)->Append(5)->Append(6)->Append(7)->Append(8);
	sortedseq = sorter->Sort(seq2, comparator);
	REQUIRE(sortedseq->GetLength() == 8);
	REQUIRE(sortedseq->Get(0) == 1);
	REQUIRE(sortedseq->Get(1) == 2);
	REQUIRE(sortedseq->Get(2) == 3);
	REQUIRE(sortedseq->Get(3) == 4);
	REQUIRE(sortedseq->Get(4) == 5);
	REQUIRE(sortedseq->Get(5) == 6);
	REQUIRE(sortedseq->Get(6) == 7);
	REQUIRE(sortedseq->Get(7) == 8);
	delete seq2;

	Sequence<int>* antisorted = sorter->Sort(sortedseq, anticomparator);
	REQUIRE(antisorted->GetLength() == 8);
	REQUIRE(antisorted->Get(0) == 8);
	REQUIRE(antisorted->Get(1) == 7);
	REQUIRE(antisorted->Get(2) == 6);
	REQUIRE(antisorted->Get(3) == 5);
	REQUIRE(antisorted->Get(4) == 4);
	REQUIRE(antisorted->Get(5) == 3);
	REQUIRE(antisorted->Get(6) == 2);
	REQUIRE(antisorted->Get(7) == 1);
	delete sortedseq;
	delete antisorted;
	delete seq;

	Sequence<int>* random = new ArraySequence<int>(5, 6, -1, 1, 1, 1, 0, 10, -15, 25, 20);

	sortedseq = sorter->Sort(random, comparator);
	REQUIRE(sortedseq->GetLength() == 11);
	REQUIRE(sortedseq->Get(0) == -15);
	REQUIRE(sortedseq->Get(1) == -1);
	REQUIRE(sortedseq->Get(2) == 0);
	REQUIRE(sortedseq->Get(3) == 1);
	REQUIRE(sortedseq->Get(4) == 1);
	REQUIRE(sortedseq->Get(5) == 1);
	REQUIRE(sortedseq->Get(6) == 5);
	REQUIRE(sortedseq->Get(7) == 6);
	REQUIRE(sortedseq->Get(8) == 10);
	REQUIRE(sortedseq->Get(9) == 20);
	REQUIRE(sortedseq->Get(10) == 25);
	delete sortedseq;
	delete random;
	delete sorter;
}

TEST_CASE("Shell Sort")
{
	ISorter<int>* sorter = new ShellSorter<int>();

	const auto comparator = [](const int& a, const int& b) -> int
	{ return a - b; };

	const auto anticomparator = [](const int& a, const int& b) -> int
	{ return b - a; };

	Sequence<int>* seq = new ArraySequence<int>();
	REQUIRE(seq->GetLength() == 0);

	Sequence<int>* sortedseq = sorter->Sort(seq, comparator);
	REQUIRE(sortedseq->GetLength() == 0);
	delete sortedseq;

	seq->Append(1);
	sortedseq = sorter->Sort(seq, comparator);
	REQUIRE(sortedseq->GetLength() == 1);
	delete sortedseq;

	seq->Append(-1);
	sortedseq = sorter->Sort(seq, comparator);
	REQUIRE(sortedseq->GetLength() == 2);
	REQUIRE(sortedseq->Get(0) == -1);
	REQUIRE(sortedseq->Get(1) == 1);
	delete sortedseq;

	Sequence<int>* seq2 = new ArraySequence<int>();
	seq2->Append(1)->Append(2);
	sortedseq = sorter->Sort(seq2, comparator);
	REQUIRE(sortedseq->GetLength() == 2);
	REQUIRE(sortedseq->Get(0) == 1);
	REQUIRE(sortedseq->Get(1) == 2);
	delete sortedseq;

	seq2->Append(3)->Append(4)->Append(5)->Append(6)->Append(7)->Append(8);
	sortedseq = sorter->Sort(seq2, comparator);
	REQUIRE(sortedseq->GetLength() == 8);
	REQUIRE(sortedseq->Get(0) == 1);
	REQUIRE(sortedseq->Get(1) == 2);
	REQUIRE(sortedseq->Get(2) == 3);
	REQUIRE(sortedseq->Get(3) == 4);
	REQUIRE(sortedseq->Get(4) == 5);
	REQUIRE(sortedseq->Get(5) == 6);
	REQUIRE(sortedseq->Get(6) == 7);
	REQUIRE(sortedseq->Get(7) == 8);
	delete seq2;

	Sequence<int>* antisorted = sorter->Sort(sortedseq, anticomparator);
	REQUIRE(antisorted->GetLength() == 8);
	REQUIRE(antisorted->Get(0) == 8);
	REQUIRE(antisorted->Get(1) == 7);
	REQUIRE(antisorted->Get(2) == 6);
	REQUIRE(antisorted->Get(3) == 5);
	REQUIRE(antisorted->Get(4) == 4);
	REQUIRE(antisorted->Get(5) == 3);
	REQUIRE(antisorted->Get(6) == 2);
	REQUIRE(antisorted->Get(7) == 1);
	delete sortedseq;
	delete antisorted;
	delete seq;

	Sequence<int>* random = new ArraySequence<int>(5, 6, -1, 1, 1, 1, 0, 10, -15, 25, 20);

	sortedseq = sorter->Sort(random, comparator);
	REQUIRE(sortedseq->GetLength() == 11);
	REQUIRE(sortedseq->Get(0) == -15);
	REQUIRE(sortedseq->Get(1) == -1);
	REQUIRE(sortedseq->Get(2) == 0);
	REQUIRE(sortedseq->Get(3) == 1);
	REQUIRE(sortedseq->Get(4) == 1);
	REQUIRE(sortedseq->Get(5) == 1);
	REQUIRE(sortedseq->Get(6) == 5);
	REQUIRE(sortedseq->Get(7) == 6);
	REQUIRE(sortedseq->Get(8) == 10);
	REQUIRE(sortedseq->Get(9) == 20);
	REQUIRE(sortedseq->Get(10) == 25);
	delete sortedseq;
	delete random;
	delete sorter;
}
