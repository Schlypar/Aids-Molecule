#pragma once

#include <utility>

#include "Sequence.h"
#include "ListSequence.h"
#include "ArraySequence.h"


template <typename T>
class Allocator
{
public:

    template <typename... Args>
    static Array<T>* AllocateArray(Args... args)
    {
        return new Array<T>(args...);
    }

    template <typename... Args>
    static List<T>* AllocateList(Args... args)
    {
        return new List<T>(args...);
    }

    template <typename... Args>
    static SegmentedList<T>* AllocateSegmentedList(Args... args)
    {
        return new SegmentedList<T>(args...);
    }

    template <typename... Args>
    static Sequence<T>* AllocateArraySequence(Args... args)
    {
        return (Sequence<T>*) new ArraySequence<T>(args...);
    }

    template <typename... Args>
    static Sequence<T>* AllocateListSequence(Args... args)
    {
        return (Sequence<T>*) new ListSequence<T>(args...);
    }
};