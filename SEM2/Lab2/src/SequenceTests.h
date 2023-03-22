#pragma once

#include <cassert>

#include "Sequence.h"
#include "ArraySequence.h"

inline int square (int& x) { return x * x; }
inline float plusOne (float& x) { return x + 1; }
inline bool evenCondition (int& value) { return value % 2 == 0; }
inline int sum(int first, int second) { return first + second; }

inline void TestSequenceMap()
{
    // Create a sequence and populate it with some values
    ArraySequence<int>* sequence = new ArraySequence<int>();
    sequence->Append(1);
    sequence->Append(2);
    sequence->Append(3);

    // Apply the map function with the square function
    ArraySequence<int>* mappedSequence = (ArraySequence<int>*)sequence->Map(square);

    // Check that the length of the mapped sequence is correct
    assert(mappedSequence->GetLength() == 3);

    // Check that the values in the mapped sequence are correct
    assert(mappedSequence->Get(0) == 1);
    assert(mappedSequence->Get(1) == 4);
    assert(mappedSequence->Get(2) == 9);

    // Clean up memory
    delete sequence;
    delete mappedSequence;

    // Test for an empty sequence
    ArraySequence<float>* emptySequence = new ArraySequence<float>();
    bool exceptionThrown = false;
    try 
    {
        emptySequence->Map(plusOne);
    } 
    catch (Exception exception) 
    {
        exceptionThrown = true;
        assert(exception == EXCEPTION_INDEX_OUT_OF_RANGE);
    }
    assert(exceptionThrown);

    // Clean up memory
    delete emptySequence;
}

inline void TestSequenceWhere()
{
    // Create a sequence of integers
    ArraySequence<int>* sequence = new ArraySequence<int>;
    sequence->Append(1);
    sequence->Append(2);
    sequence->Append(3);
    sequence->Append(4);
    sequence->Append(5);

    // Call the Where method with the even condition
    ArraySequence<int>* result = (ArraySequence<int>*)sequence->Where(evenCondition);

    // Check that the result has the correct length and values
    assert(result->GetLength() == 2);
    assert(result->Get(0) == 2);
    assert(result->Get(1) == 4);

    // Clean up the memory used by the result
    delete result;
    delete sequence;

    // Test the case where the sequence is empty
    ArraySequence<int>* emptySequence = new ArraySequence<int>;
    bool exceptionThrown = false;
    try 
    {
        emptySequence->Where(evenCondition);
    } 
    catch (Exception exception) 
    {
        exceptionThrown = true;
        assert(exception == EXCEPTION_INDEX_OUT_OF_RANGE);
    }
    assert(exceptionThrown);
    delete emptySequence;
}

inline void TestSequenceGetSubsequence()
{
    // Create a sequence of integers
     ArraySequence<int>* sequence = new ArraySequence<int>;
    sequence->Append(1);
    sequence->Append(2);
    sequence->Append(3);
    sequence->Append(4);
    sequence->Append(5);

    // Get a subsequence from index 1 to 3
    Sequence<int>* subsequence = sequence->GetSubsequence(1, 3);

    // Check that the subsequence has the correct length and values
    assert(subsequence->GetLength() == 2);
    assert(subsequence->Get(0) == 2);
    assert(subsequence->Get(1) == 3);

    // Clean up the memory used by the subsequence
    delete (ArraySequence<int>*)subsequence;

    // Test the case where the sequence is empty
    ArraySequence<float>* emptySequence = new ArraySequence<float>;
    bool exceptionThrown = false;
    try 
    {
        emptySequence->GetSubsequence(0, 0);
    } 
    catch (Exception exception) 
    {
        exceptionThrown = true;
        assert(exception == EXCEPTION_INDEX_OUT_OF_RANGE);
    }
    assert(exceptionThrown);

    delete emptySequence;

    // Test the case where the start index is greater than the end index
    exceptionThrown = false;
    try 
    {
        sequence->GetSubsequence(3, 1);
    } 
    catch (Exception exception) 
    {
        exceptionThrown = true;
        assert(exception == EXCEPTION_INDEX_OUT_OF_RANGE);
    }
    assert(exceptionThrown);

    // Test the case where the end index is greater than or equal to the length of the sequence
    exceptionThrown = false;
    try 
    {
        sequence->GetSubsequence(3, 5);
    } 
    catch (Exception exception) 
    {
        exceptionThrown = true;
        assert(exception == EXCEPTION_INDEX_OUT_OF_RANGE);
    }
    assert(exceptionThrown);

    exceptionThrown = false;
    try 
    {
        sequence->GetSubsequence(3, 6);
    } 
    catch (Exception exception) 
    {
        exceptionThrown = true;
        assert(exception == EXCEPTION_INDEX_OUT_OF_RANGE);
    }
    assert(exceptionThrown);
    delete sequence;
}

inline void TestSequenceConcat()
{
    // Create the first sequence of integers
    ArraySequence<int>* sequence1 = new ArraySequence<int>;
    sequence1->Append(1);
    sequence1->Append(2);
    sequence1->Append(3);

    // Create the second sequence of integers
    ArraySequence<int>* sequence2 = new ArraySequence<int>;
    sequence2->Append(4);
    sequence2->Append(5);
    sequence2->Append(6);

    // Concatenate the two sequences
    Sequence<int>* result = sequence1->Concat(sequence2);


    // Check that the result has the correct length and values
    assert(result->GetLength() == 6);
    assert(result->Get(0) == 1);
    assert(result->Get(1) == 2);
    assert(result->Get(2) == 3);
    assert(result->Get(3) == 4);
    assert(result->Get(4) == 5);
    assert(result->Get(5) == 6);

    // Clean up the memory used by the result
    delete (ArraySequence<int>*)result;
    delete sequence1;
    delete sequence2;

    // Test the case where the first sequence is empty
    ArraySequence<float>* emptySequence1 = new ArraySequence<float>;
    ArraySequence<float>* sequence3 = new ArraySequence<float>;
    sequence3->Append(1.0f);
    sequence3->Append(2.0f);
    sequence3->Append(3.0f);
    bool exceptionThrown = false;
    try 
    {
        emptySequence1->Concat(sequence3);
    } 
    catch (Exception exception) 
    {
        exceptionThrown = true;
        assert(exception == EXCEPTION_INDEX_OUT_OF_RANGE);
    }
    assert(exceptionThrown);

    delete emptySequence1;
    delete sequence3;

    // Test the case where the second sequence is empty
    ArraySequence<float>* emptySequence2 = new ArraySequence<float>;
    ArraySequence<float>* sequence4 = new ArraySequence<float>;
    sequence4->Append(4.0f);
    sequence4->Append(5.0f);
    sequence4->Append(6.0f);
    exceptionThrown = false;
    try 
    {
        sequence4->Concat(emptySequence2);
    } 
    catch (Exception exception) 
    {
        exceptionThrown = true;
        assert(exception == EXCEPTION_INDEX_OUT_OF_RANGE);
    }
    assert(exceptionThrown);

    delete emptySequence2;
    delete sequence4;
}

inline void TestSequenceSlice()
{
    // Create two sequences of integers
    ArraySequence<int>* sequence1 = new ArraySequence<int>;
    sequence1->Append(1);
    sequence1->Append(2);
    sequence1->Append(3);
    sequence1->Append(4);
    sequence1->Append(5);

    ArraySequence<int>* sequence2 = new ArraySequence<int>;
    sequence2->Append(6);
    sequence2->Append(7);
    sequence2->Append(8);
    sequence2->Append(9);
    sequence2->Append(10);

    // Call the Slice method with valid arguments
    ArraySequence<int>* result = (ArraySequence<int>*)sequence1->Slice(2, 3, sequence2);

    // Check that the result has the correct length and values
    assert(result->GetLength() == sequence1->GetLength());
    assert(result->Get(0) == sequence1->Get(0));
    assert(result->Get(1) == sequence1->Get(1));
    assert(result->Get(2) == 6);

    // Clean up the memory used by the result
    delete result;

    // Test the case where the first sequence is empty
    ArraySequence<int>* emptySequence1 = new ArraySequence<int>;
    bool exceptionThrown = false;
    try 
    {
        emptySequence1->Slice(0, 1, sequence2);
    } 
    catch (Exception exception)
    {
        exceptionThrown = true;
        assert(exception == EXCEPTION_INDEX_OUT_OF_RANGE);
    }
    assert(exceptionThrown);
    delete emptySequence1;

    // Test the case where the index is out of range
    exceptionThrown = false;
    try 
    {
        sequence1->Slice(5, 1, sequence2);
    } 
    catch (Exception exception) 
    {
        exceptionThrown = true;
        assert(exception == EXCEPTION_INDEX_OUT_OF_RANGE);
    }
    assert(exceptionThrown);

    // Test the case where the size is greater than the second sequence
    exceptionThrown = false;
    try 
    {
        sequence1->Slice(2, 4, sequence2);
    } 
    catch (Exception exception) 
    {
        exceptionThrown = true;
        assert(exception == EXCEPTION_INDEX_OUT_OF_RANGE);
    }
    assert(exceptionThrown);

    delete (ArraySequence<int>*)sequence1;
    delete (ArraySequence<int>*)sequence2;
}

inline void TestSequenceReduce() 
{
    // Create a sequence of integers
    ArraySequence<int>* sequence = new ArraySequence<int>;
    sequence->Append(1);
    sequence->Append(2);
    sequence->Append(3);
    sequence->Append(4);
    sequence->Append(5);

    // Test reducing the sequence with the sum function
    int result = sequence->Reduce(sum, 0);
    assert(result == 15);

    // Test reducing an empty sequence
    ArraySequence<int>* emptySequence = new ArraySequence<int>;
    bool exceptionThrown = false;
    try 
    {
        emptySequence->Reduce(sum, 0);
    }
    catch (Exception exception) 
    {
        exceptionThrown = true;
        assert(exception == EXCEPTION_INDEX_OUT_OF_RANGE);
    }
    assert(exceptionThrown);

    delete sequence;
    delete emptySequence;
}