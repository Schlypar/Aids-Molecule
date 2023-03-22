#include <cassert>

#include "moduleTest.h"
#include "ArraySequence.h"

void TestSegmentedList()
{
    TestSegmentedListConstructors();
    TestSegmentedListGet();
    TestSegmentedListInsertAt();
    TestSegmentedListAppend();
    TestSegmentedListPrepend();
    TestSegmentedListMoveAssignment();
}

void TestArray()
{
    TestArrayConstructors();
    TestArrayRealloc();
    TestArraySet();
    TestArrayGet();
    TestArrayAssignmentOperator();
    TestArrayMoveAssignmentOperator();
}
