#pragma once

#include <cassert>

#include "SegmentedList.h"

inline void TestSegmentedListConstructors()
{
    // Test default constructor
    SegmentedList<int> list;
    assert(list.GetLength() == 0);

    // Test copying constructor
    SegmentedList<int> list1;
    list1.Append(10);
    list1.Append(20);
    SegmentedList<int> list2(list1);
    assert(list2.GetLength() == 2);
    assert(list2.Get(0) == 10);
    assert(list2.Get(1) == 20);

    // Test moving constructor
    SegmentedList<int> list3(std::move(list1));
    assert(list3.GetLength() == 2);
    assert(list3.Get(0) == 10);
    assert(list3.Get(1) == 20);
    assert(list1.GetLength() == 0);

    // Test copying from array constructor
    int arr[5] = { 1, 2, 3, 4, 5 };
    SegmentedList<int> list4(arr, 5);
    assert(list4.GetLength() == 5);
    assert(list4.Get(0) == 1);
    assert(list4.Get(1) == 2);
    assert(list4.Get(2) == 3);
    assert(list4.Get(3) == 4);
    assert(list4.Get(4) == 5);

    // Test copying from List constructor
    List<int> l;
    l.Append(1);
    l.Append(2);
    l.Append(3);
    SegmentedList<int> list5(l);
    assert(list5.GetLength() == 3);
    assert(list5.Get(0) == 1);
    assert(list5.Get(1) == 2);
    assert(list5.Get(2) == 3);

    // Test moving from List constructor
    List<int> l1;
    l1.Append(10);
    l1.Append(20);
    l1.Append(30);
    SegmentedList<int> list6(std::move(l1));
    assert(list6.GetLength() == 3);
    assert(list6.Get(0) == 10);
    assert(list6.Get(1) == 20);
    assert(list6.Get(2) == 30);
    assert(l1.GetLength() == 0);
}

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

inline void TestSegmentedListAssignment()
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

inline void TestSegmentedListMoveAssignment()
{
    SegmentedList<int> list1;
    list1.Append(1);
    list1.Append(2);
    list1.Append(3);

    SegmentedList<int> list2;
    list2.Append(4);
    list2.Append(5);
    list2.Append(6);

    list1 = std::move(list2);

    assert(list1.GetLength() == 3);
    assert(list1.Get(0) == 4);
    assert(list1.Get(1) == 5);
    assert(list1.Get(2) == 6);

    // Check that the move left the source object in a valid but unspecified state
    assert(list2.GetLength() == 0);
    assert(list2.GetHead() == nullptr);
    assert(list2.GetTail() == nullptr);
}