#include "moduleTest.h"

#include "Allocator.h"
#include "Sequence/ArraySequence.h"
#include <catch2/catch.hpp>

// inline int square (int& x) { return x * x; }
// inline float plusOne (float& x) { return x + 1; }
// inline bool evenCondition (int& value) { return value % 2 == 0; }
// inline int sum(int first, int second) { return first + second; }


TEST_CASE("Sequence::Map applies function to every element") {
    auto* seq = (Sequence<int>*) new ArraySequence<int>(1, 2, 3, 4, 5);

    // apply a function to every element of the sequence
    auto* mappedSeq = seq->Map([](int& x) { return x * x; });

    // check that the resulting sequence has the expected length and contents
    REQUIRE(mappedSeq->GetLength() == 5);
    REQUIRE(mappedSeq->Get(0) == 1);
    REQUIRE(mappedSeq->Get(1) == 4);
    REQUIRE(mappedSeq->Get(2) == 9);
    REQUIRE(mappedSeq->Get(3) == 16);
    REQUIRE(mappedSeq->Get(4) == 25);

    // free the memory allocated on the heap
    delete seq;
    delete mappedSeq;
}

TEST_CASE("Where method of Sequence", "[Where]") {
    auto* seq = (Sequence<int>*) new ArraySequence<int>({ 1, 2, 3, 4, 5 });

    // Define lambda function for condition
    auto condition = [](int& x) -> bool { return x % 2 == 0; };

    // Apply Where method
    auto* filtered = seq->Where(condition);

    // Check the result
    REQUIRE(filtered->GetLength() == 2);
    REQUIRE(filtered->Get(0) == 2);
    REQUIRE(filtered->Get(1) == 4);

    // Free memory
    delete filtered;
    delete seq;
}

TEST_CASE("Sequence - GetSubsequence", "[Sequence]")
{
    const int arr[] = { 1, 2, 3, 4, 5 };
    const int size = sizeof(arr) / sizeof(int);
    auto seq = (Sequence<int>*) new ArraySequence<int>(arr, size);

    SECTION("Returns correct subsequence")
    {
        auto subseq = seq->GetSubsequence(1, 3);
        REQUIRE(subseq->GetLength() == 3);
        REQUIRE(subseq->Get(0) == 2);
        REQUIRE(subseq->Get(1) == 3);
        delete subseq;
    }

    SECTION("Returns correct subsequence when start is 0")
    {
        auto subseq = seq->GetSubsequence(0, 2);
        REQUIRE(subseq->GetLength() == 3);
        REQUIRE(subseq->Get(0) == 1);
        REQUIRE(subseq->Get(1) == 2);
        REQUIRE(subseq->Get(2) == 3);
        delete subseq;
    }

    SECTION("Returns empty subsequence when start is greater than end")
    {
        auto subseq = seq->GetSubsequence(3, 1);
        REQUIRE(subseq->GetLength() == 0);
        delete subseq;
    }

    delete seq;
}

TEST_CASE("Sequence Concat method") {
    // create two Sequences
    auto seq1 = (Sequence<int>*) new ArraySequence<int>({1, 2, 3});
    auto seq2 = (Sequence<int>*) new ArraySequence<int>({4, 5, 6});

    // concatenate them
    auto seq3 = seq1->Concat(seq2);

    // check that the length of the new sequence is the sum of the lengths of the original sequences
    REQUIRE(seq3->GetLength() == seq1->GetLength() + seq2->GetLength());

    // check that the new sequence contains all the elements from the original sequences in the correct order
    for (int i = 0; i < seq1->GetLength(); i++) {
        REQUIRE(seq3->Get(i) == seq1->Get(i));
    }
    for (int i = 0; i < seq2->GetLength(); i++) {
        REQUIRE(seq3->Get(i + seq1->GetLength()) == seq2->Get(i));
    }

    // clean up
    delete seq1;
    delete seq2;
    delete seq3;
}

TEST_CASE("Slice method test", "[Sequence]") {
    ArraySequence<int> arr({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    // Slice the array from index 2 and size 5
    Sequence<int>* slice = arr.Slice(2, 5, NULL);

    SECTION("Verify that the slice is correct") {
        REQUIRE(slice->GetLength() == 5);
        REQUIRE(slice->Get(0) == 1);
        REQUIRE(slice->Get(1) == 2);
        REQUIRE(slice->Get(2) == 8);
        REQUIRE(slice->Get(3) == 9);
        REQUIRE(slice->Get(4) == 10);
    }

    // Slice the array from index 5 with size larger than remaining elements
    Sequence<int>* slice2 = arr.Slice(5, 10, NULL);

    // Verify that the slice is correct
    SECTION("Verify that the slice is correct with size larger than remaining elements") {
        REQUIRE(slice2->GetLength() == 5);
        REQUIRE(slice2->Get(0) == 1);
        REQUIRE(slice2->Get(1) == 2);
        REQUIRE(slice2->Get(2) == 3);
        REQUIRE(slice2->Get(3) == 4);
        REQUIRE(slice2->Get(4) == 5);
    }

    // Concatenate the two slices
    Sequence<int>* concat = slice->Concat(slice2);

    // Verify that the concatenated array is correct
    REQUIRE(concat->GetLength() == 10);

    // Free the memory
    delete slice;
    delete slice2;
    delete concat;
}

TEST_CASE("Reduce") {
    auto* seq = (Sequence<int>*) new ArraySequence<int>({ 1, 2, 3, 4, 5 });

    SECTION("sum of elements") {
        int sum = seq->Reduce([](int a, int b) { return a + b; }, 0);
        REQUIRE(sum == 15);
    }

    SECTION("product of elements") {
        int product = seq->Reduce([](int a, int b) { return a * b; }, 1);
        REQUIRE(product == 120);
    }

    delete seq;
}