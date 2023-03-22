#pragma once

#include <cassert>

#include "ListSequence.h"

inline void TestSegmentedListGet()
{
    SegmentedList<int> list;

    // Populate the list
    list.Append(10);
    list.Append(20);
    list.Append(30);

    // Check that Get() returns the correct values
    assert(list.Get(0) == 10);
    assert(list.Get(1) == 20);
    assert(list.Get(2) == 30);

    // Test for out-of-range index
    bool exceptionThrown = false;
    try 
    {
        list.Get(-1);
    } 
    catch (Exception exception) {
        exceptionThrown = true;
        assert(exception == EXCEPTION_INDEX_OUT_OF_RANGE);
    }
    assert(exceptionThrown);

    exceptionThrown = false;
    try 
    {
        list.Get(3);
    } 
    catch (Exception exception) {
        exceptionThrown = true;
        assert(exception == EXCEPTION_INDEX_OUT_OF_RANGE);
    }
    assert(exceptionThrown);

    // Test for empty list
    SegmentedList<float> emptyList;
    exceptionThrown = false;
    try 
    {
        emptyList.Get(0);
    } 
    catch (Exception exception) {
        exceptionThrown = true;
        assert(exception == EXCEPTION_INDEX_OUT_OF_RANGE);
    }
    assert(exceptionThrown);
}

inline void TestSegmentedListInsertAt()
{
    SegmentedList<int> list;

    // Populate the list
    list.Append(10);
    list.Append(20);
    list.Append(30);

    // Insert an element in the middle of the list
    list.insertAt(1, 15);

    // Check that the size of the list has increased
    assert(list.GetLength() == 4);

    // Check that the element was inserted at the correct index
    assert(list.Get(0) == 10);
    assert(list.Get(1) == 15);
    assert(list.Get(2) == 20);
    assert(list.Get(3) == 30);

    // Test for out-of-range index
    bool exceptionThrown = false;
    try 
    {
        list.insertAt(-1, 5);
    } 
    catch (Exception exception) {
        exceptionThrown = true;
        assert(exception == EXCEPTION_INDEX_OUT_OF_RANGE);
    }
    assert(exceptionThrown);

    exceptionThrown = false;
    try 
    {
        list.insertAt(5, 40);
    } 
    catch (Exception exception) {
        exceptionThrown = true;
        assert(exception == EXCEPTION_INDEX_OUT_OF_RANGE);
    }
    assert(exceptionThrown);

    // Test for empty list
    SegmentedList<float> emptyList;
    exceptionThrown = false;
    try 
    {
        emptyList.insertAt(0, 1.0f);
    } 
    catch (Exception exception) {
        exceptionThrown = true;
        assert(exception == EXCEPTION_INDEX_OUT_OF_RANGE);
    }
    assert(exceptionThrown);
}

inline void TestSegmentedListAppend()
{
    // Create an empty list
    SegmentedList<int> list;

    // Append a single element
    list.Append(10);
    assert(list.Get(0) == 10);
    assert(list.GetLength() == 1);

    // Append elements until a new segment is created
    for (int i = 1; i <= SEGMENT_NORM_SIZE - 1; i++)
    {
        list.Append(i * 10);
        assert(list.Get(i) == i * 10);
        assert(list.GetLength() == i + 1);
    }

    // Append one more element to create a new segment
    list.Append(100);
    assert(list.Get(SEGMENT_NORM_SIZE) == 100);
    assert(list.GetLength() == SEGMENT_NORM_SIZE + 1);

    // Append more elements
    for (int i = SEGMENT_NORM_SIZE + 1; i <= SEGMENT_NORM_SIZE * 2; i++)
    {
        list.Append(i * 10);
        assert(list.Get(i) == i * 10);
        assert(list.GetLength() == i + 1);
    }
}

inline void TestSegmentedListPrepend()
{
    // Test that Prepend adds to an empty list
    SegmentedList<int> list1;
    list1.Prepend(5);
    assert(list1[0] == 5);
    assert(list1.GetLength() == 1);

    // Test that Prepend adds to the beginning of a non-empty list
    SegmentedList<int> list2;
    list2.Append(1);
    list2.Append(2);
    list2.Append(3);
    list2.Prepend(0);
    assert(list2[0] == 0);
    assert(list2[1] == 1);
    assert(list2[2] == 2);
    assert(list2[3] == 3);

    // Test that Prepend adds to a new segment if the current one is full
    const int SEGMENT_SIZE = SEGMENT_NORM_SIZE;
    SegmentedList<int> list3;
    for (int i = 0; i < SEGMENT_SIZE; i++)
    {
        list3.Append(i);
    }
    list3.Prepend(SEGMENT_SIZE);
    assert(list3[0] == SEGMENT_SIZE);
    assert(list3[1] == 1 - 1);
    assert(list3[2] == 2 - 1);
    assert(list3[3] == 3 - 1);

    // Test that Prepend adds to a new segment if the current one is full
    SegmentedList<int> list4;
    for (int i = 0; i < SEGMENT_SIZE * 2; i++)
    {
        list4.Prepend(i);
        assert(list4[0] == i);
        assert(list4.GetLength() == i + 1);
    }
}

inline void TestSegmentedListCopyingOperator()
{
    // Create an original list with elements
    SegmentedList<int> originalList;
    for (int i = 0; i < SEGMENT_NORM_SIZE * 3; i++)
    {
        originalList.Append(i);
    }

    // Create a new list and copy the original list to it
    SegmentedList<int> copiedList;
    copiedList = originalList;

    assert(copiedList == originalList);

    // Check if the two lists have the same elements and length
    for (int i = 0; i < originalList.GetLength(); i++)
    {
        assert(copiedList.Get(i) == originalList.Get(i));
    }
    assert(copiedList.GetLength() == originalList.GetLength());

    // Modify the original list and check if the copied list stays the same
    originalList.Clear();
    assert(copiedList.GetLength() == SEGMENT_NORM_SIZE * 3);
    for (int i = 0; i < copiedList.GetLength(); i++)
    {
        assert(copiedList.Get(i) == i);
    }
}