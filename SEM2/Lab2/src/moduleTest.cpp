#include <cassert>

#include "moduleTest.h"
#include "ArraySequence.h"
#include "ListSequence.h"

void TestSegmentList()
{
    TestSegmentedListGet();
    TestSegmentedListInsertAt();
    TestSegmentedListAppend();
}

void TestSegmentedListGet()
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

void TestSegmentedListInsertAt()
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

void TestSegmentedListAppend()
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