#pragma once

#include "IContainer.h"
#include "IIterator.h"
#include "Sequence.h"

#include "Tuple.h"
#include "List.h"
#include "SegmentedList.h"
#include "Array.h"
#include "ArraySequence.h"
#include "Logger.h"

template <typename T>
using Allocator = Sequence<T>*(*)();
