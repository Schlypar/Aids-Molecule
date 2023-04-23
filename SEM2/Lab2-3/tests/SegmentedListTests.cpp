#include "Logger.h"
#include "moduleTest.h"

#include "SegmentedList.h"

TEST_CASE("SegmentedList constructors", "[SegmentedList]")
{
    // Default constructor
    SegmentedList<int> list1;
    REQUIRE(list1.GetLength() == 0);

    // Copy constructor
    SegmentedList<int> list2 = {1, 2, 3};
    SegmentedList<int> list3(list2);
    for (Index i = 0; i < 3; i++)
        REQUIRE(list2[i] == list3[i]);

    // Move constructor
    SegmentedList<int> list4(std::move(list2));
    REQUIRE(list2.GetLength() == 0);
    for (Index i = 0; i < 3; i++)
        REQUIRE(list4[i] == list3[i]);

    // Copy from array constructor
    int arr[] = {4, 5, 6};
    SegmentedList<int> list5(arr, 3);
    REQUIRE(list5.GetLength() == 3);
    REQUIRE(list5.Get(0) == 4);
    REQUIRE(list5.Get(1) == 5);
    REQUIRE(list5.Get(2) == 6);

    // Copy from List constructor
    List<int> l1({7, 8, 9});
    SegmentedList<int> list6(l1);
    REQUIRE(list6.GetLength() == 3);
    REQUIRE(list6.Get(0) == 7);
    REQUIRE(list6.Get(1) == 8);
    REQUIRE(list6.Get(2) == 9);

    // Move from List constructor
    List<int> l2({10, 11, 12});
    SegmentedList<int> list7(std::move(l2));
    REQUIRE(l2.GetLength() == 0);
    REQUIRE(list7.GetLength() == 3);
    REQUIRE(list7.Get(0) == 10);
    REQUIRE(list7.Get(1) == 11);
    REQUIRE(list7.Get(2) == 12);

    // Parameter pack constructor
    SegmentedList<int> list8(13, 14, 15);
    REQUIRE(list8.GetLength() == 3);
    REQUIRE(list8.Get(0) == 13);
    REQUIRE(list8.Get(1) == 14);
    REQUIRE(list8.Get(2) == 15);
}

TEST_CASE("Clear empties SegmentedList", "[SegmentedList]")
{
    SegmentedList<int> list = {1, 2, 3};
    REQUIRE(list.GetLength() == 3);

    list.Clear();

    REQUIRE(list.GetLength() == 0);
    REQUIRE_THROWS(list.GetFirst());
    REQUIRE_THROWS(list.GetLast());
}

TEST_CASE("GetFirst returns first element in non-empty SegmentedList", "[SegmentedList]")
{
    SegmentedList<int> list = {1, 2, 3};
    REQUIRE(list.GetFirst() == 1);

    list.Append(4);

    REQUIRE(list.GetFirst() == 1);
}

TEST_CASE("GetLast returns last element in non-empty SegmentedList", "[SegmentedList]")
{
    SegmentedList<int> list = {1, 2, 3};
    REQUIRE(list.GetLast() == 3);

    list.Append(4);

    REQUIRE(list.GetLast() == 4);
}

TEST_CASE("Get element at index", "[SegmentedList]")
{
    // Arrange
    SegmentedList<int> list;
    list.Append(1);
    list.Append(2);
    list.Append(3);
    list.Append(4);
    list.Append(5);

    // Act
    int first = list.Get(0);
    int middle = list.Get(2);
    int last = list.Get(4);

    // Assert
    REQUIRE(first == 1);
    REQUIRE(middle == 3);
    REQUIRE(last == 5);

    // Check out of range exception
    REQUIRE_THROWS_AS(list.Get(-1), Exception);
    REQUIRE_THROWS_AS(list.Get(5), Exception);
}

TEST_CASE("InsertAt adds element at correct index") {
    SegmentedList<int> list;
    list.Append(1);
    list.Append(2);
    list.Append(3);
    
    list.insertAt(1, 10);
    
    REQUIRE(list.GetLength() == 4);
    REQUIRE(list.Get(0) == 1);
    REQUIRE(list.Get(1) == 10);
    REQUIRE(list.Get(2) == 2);
    REQUIRE(list.Get(3) == 3);
}

TEST_CASE("InsertAt throws exception if index out of range") {
    SegmentedList<int> list;
    list.Append(1);
    list.Append(2);
    list.Append(3);
    
    REQUIRE_THROWS_AS(list.insertAt(-1, 10), Exception);
    REQUIRE_THROWS_AS(list.insertAt(4, 10), Exception);
}

// Test case for the assignment operator overload that performs a deep copy
TEST_CASE("SegmentedList operator= deep copy", "[SegmentedList]")
{
    // Create the original list
    SegmentedList<int> original;
    original.Append(1);
    original.Append(2);
    original.Append(3);

    // Create a new list and perform a deep copy
    SegmentedList<int> copy;
    copy = original;

    // Check that the copy was successful
    REQUIRE(copy.GetLength() == 3);
    REQUIRE(copy.Get(0) == 1);
    REQUIRE(copy.Get(1) == 2);
    REQUIRE(copy.Get(2) == 3);

    // Modify the original list and check that the copy is unchanged
    original.Append(0);
    REQUIRE(original.GetLength() == 4);
    REQUIRE(copy.GetLength() == 3);
    REQUIRE(copy.Get(0) == 1);
    REQUIRE(copy.Get(1) == 2);
    REQUIRE(copy.Get(2) == 3);
}

// Test case for the assignment operator overload that performs a move
TEST_CASE("SegmentedList operator= move", "[SegmentedList]")
{
    // Create the original list
    SegmentedList<int> original;
    original.Append(1);
    original.Append(2);
    original.Append(3);

    // Create a new list and perform a move
    SegmentedList<int> moved;
    moved = std::move(original);

    // Check that the move was successful
    REQUIRE(moved.GetLength() == 3);
    REQUIRE(moved.Get(0) == 1);
    REQUIRE(moved.Get(1) == 2);
    REQUIRE(moved.Get(2) == 3);

    // Check that the original list is now empty
    REQUIRE(original.GetLength() == 0);
}