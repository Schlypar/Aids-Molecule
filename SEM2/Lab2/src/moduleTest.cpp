#include <cassert>

#include "moduleTest.h"
#include "ArraySequence.h"

void TestSegmentList()
{
    TestSegmentedListGet();
    TestSegmentedListInsertAt();
    TestSegmentedListAppend();
    TestSegmentedListPrepend();
    TestSegmentedListCopyingOperator();
}

