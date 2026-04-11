#include "Sequence/ArraySequence.h"
#include "Sequence/ListSequence.h"
#include "Sequence/SeqIter.h"
#include "Sequence/Sequence.h"
#include <ADT.h>
#include <cassert>
#include <string>

LogPriority Logger::priority = FatalPriority;

int main()
{
	// 1. Identity collect — ArraySequence<int> -> ArraySequence<int>
	Sequence<int>* arr = new ArraySequence<int> { 1, 2, 3 };
	auto*          c1  = arr->Iter()->Collect();
	assert(c1->GetLength() == 3);
	assert(c1->Get(0) == 1);
	assert(c1->Get(2) == 3);
	delete c1;

	// 2. MapTo with type change — result is ArraySequence<double>
	auto* c2 = arr->Iter()
	               ->MapTo<double>([](const int& x) { return x * 2.0; })
	               ->Collect();
	assert(c2->GetLength() == 3);
	assert(c2->Get(0) == 2.0);
	assert(c2->Get(2) == 6.0);
	delete c2;

	// 3. Chained MapTo
	auto* c3 = arr->Iter()
	               ->MapTo<double>([](const int& x) { return x * 2.0; })
	               ->MapTo<std::string>([](const double& x) { return std::to_string(x); })
	               ->Collect();
	assert(c3->GetLength() == 3);
	assert(!c3->Get(0).empty());
	delete c3;

	// 4. ListSequence preserves List kind
	Sequence<int>* lst = new ListSequence<int> { 10, 20, 30 };
	auto*          c4  = lst->Iter()
	                ->MapTo<int>([](const int& x) { return x + 1; })
	                ->Collect();
	assert(c4->GetLength() == 3);
	assert(c4->Get(0) == 11);
	auto* inner = c4->Iter();
	assert(inner->hasNext());
	delete inner;
	delete c4;

	delete arr;
	delete lst;

	std::cout << "All SeqIter assertions passed.\n";
	return 0;
}
