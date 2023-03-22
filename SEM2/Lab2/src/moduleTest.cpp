#include <cassert>

#include "moduleTest.h"

void TestSegmentedList()
{
    TestSegmentedListConstructors();
    TestSegmentedListGet();
    TestSegmentedListInsertAt();
    TestSegmentedListAppend();
    TestSegmentedListPrepend();
    TestSegmentedListAssignment();
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

void TestSequence()
{
    TestSequenceMap();
    TestSequenceWhere();
    TestSequenceGetSubsequence();
    TestSequenceConcat();
    TestSequenceSlice();
    TestSequenceReduce();
}