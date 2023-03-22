#pragma once

#include <cassert>

#include "Array.h"

inline void TestArrayConstructors()
{
    // Test default constructor
    Array<int> arr1;
    assert(arr1.GetLength() == 0);
    assert(arr1.GetCapacity() == 2);

    // Test constructor that takes a pointer and count
    int arr2_data[] = { 1, 2, 3 };
    Array<int> arr2(arr2_data, 3);
    assert(arr2.GetLength() == 3);
    assert(arr2.GetCapacity() == 3);
    assert(arr2.Get(0) == 1);
    assert(arr2.Get(1) == 2);
    assert(arr2.Get(2) == 3);

    // Test constructor that takes a size
    Array<int> arr3(4);
    assert(arr3.GetLength() == 4);
    assert(arr3.GetCapacity() == 4);

    // Test copying constructor
    Array<int> arr4(arr2);
    assert(arr4.GetLength() == 3);
    assert(arr4.GetCapacity() == arr2.GetCapacity());
    assert(arr4.Get(0) == 1);
    assert(arr4.Get(1) == 2);
    assert(arr4.Get(2) == 3);

    // Test moving constructor
    Array<int> arr5(std::move(arr4));
    assert(arr5.GetLength() == 3);
    assert(arr5.GetCapacity() == arr2.GetCapacity());
    assert(arr5.Get(0) == 1);
    assert(arr5.Get(1) == 2);
    assert(arr5.Get(2) == 3);
    assert(arr4.GetLength() == 0);  // arr4 should be empty after moving
}

inline void TestArrayRealloc()
{
    // Create an array with initial size and capacity of 3
    Array<int> arr(3);

    // Populate the array with values
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;

    // Call Realloc with a new capacity of 5
    arr.Realloc(5);

    // Check that the array has been resized correctly
    assert(arr.GetLength() == 3);
    assert(arr.GetCapacity() == 5);

    // Check that the values in the array are still correct
    assert(arr[0] == 1);
    assert(arr[1] == 2);
    assert(arr[2] == 3);

    // Call Realloc with a new capacity of -1
    bool exceptionThrown = false;
    try
    {
        arr.Realloc(-1);
    }
    catch (Exception exception)
    {
        exceptionThrown = true;
        assert(exception == EXCEPTION_INDEX_OUT_OF_RANGE);
    }
    assert(exceptionThrown);

    // Check that the array has not been resized
    assert(arr.GetLength() == 3);
    assert(arr.GetCapacity() == 5);

    // Check that the values in the array are still correct
    assert(arr[0] == 1);
    assert(arr[1] == 2);
    assert(arr[2] == 3);
}

inline void TestArraySet()
{
    // Create an array with initial size and capacity of 3
    Array<int> arr(3);

    // Populate the array with values
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;

    // Call Set with a valid index and data value
    arr.Set(1, 5);

    // Check that the value in the array has been set correctly
    assert(arr[1] == 5);

    // Call Set with an invalid index
    bool exceptionThrown = false;
    try
    {
        arr.Set(-1, 5);
    }
    catch (Exception exception)
    {
        exceptionThrown = true;
        assert(exception == EXCEPTION_INDEX_OUT_OF_RANGE);
    }
    assert(exceptionThrown);

    // Check that the value in the array has not been changed
    assert(arr[1] == 5);

    // Call Set with an index greater than or equal to the size of the array
    exceptionThrown = false;
    try
    {
        arr.Set(3, 5);
    }
    catch (Exception exception)
    {
        exceptionThrown = true;
        assert(exception == EXCEPTION_INDEX_OUT_OF_RANGE);
    }
    assert(exceptionThrown);

    // Check that the value in the array has not been changed
    assert(arr[1] == 5);
}

inline void TestArrayGet()
{
    // Create an array with initial size and capacity of 3
    Array<int> arr(3);

    // Populate the array with values
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;

    // Call Get with a valid index
    int value = arr.Get(1);

    // Check that the value returned by Get is the expected value
    assert(value == 2);

    // Call Get with an invalid index
    bool exceptionThrown = false;
    try
    {
        int value = arr.Get(-1);
    }
    catch (Exception exception)
    {
        exceptionThrown = true;
        assert(exception == EXCEPTION_INDEX_OUT_OF_RANGE);
    }
    assert(exceptionThrown);

    // Call Get with an index greater than or equal to the size of the array
    exceptionThrown = false;
    try
    {
        int value = arr.Get(3);
    }
    catch (Exception exception)
    {
        exceptionThrown = true;
        assert(exception == EXCEPTION_INDEX_OUT_OF_RANGE);
    }
    assert(exceptionThrown);
}

inline void TestArrayAssignmentOperator()
{
    // Create the original array and populate it with values
    Array<int> originalArr(3);
    originalArr[0] = 1;
    originalArr[1] = 2;
    originalArr[2] = 3;

    // Create a new array and assign the original array to it
    Array<int> newArr(2);
    newArr = originalArr;

    // Check that the new array has the same values as the original array
    assert(newArr[0] == originalArr[0]);
    assert(newArr[1] == originalArr[1]);
    assert(newArr[2] == originalArr[2]);

    // Modify the original array and check that the new array is not affected
    originalArr[1] = 5;
    assert(newArr[1] == 2);

    // Create an empty array and assign the original array to it
    Array<int> emptyArr(0);
    emptyArr = originalArr;

    // Check that the empty array has the same values as the original array
    assert(emptyArr[0] == originalArr[0]);
    assert(emptyArr[1] == originalArr[1]);
    assert(emptyArr[2] == originalArr[2]);
}

inline void TestArrayMoveAssignmentOperator()
{
    // Create an array and populate it with values
    Array<int> originalArr(3);
    originalArr[0] = 1;
    originalArr[1] = 2;
    originalArr[2] = 3;

    // Move the original array to a new array using the move assignment operator
    Array<int> newArr(2);
    newArr = std::move(originalArr);

    // Check that the new array has the same values as the original array
    assert(newArr[0] == 1);
    assert(newArr[1] == 2);
    assert(newArr[2] == 3);

    // Check that the original array has been emptied
    assert(originalArr.isEmpty());

    // Move an empty array to another array using the move assignment operator
    Array<int> emptyArr(0);
    newArr = std::move(emptyArr);

    // Check that the new array is also empty
    assert(newArr.isEmpty());
}