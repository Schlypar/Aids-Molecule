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
    Array<T>* AllocateArray(Args... args)
    {
        return new Array<T>(args...);
    }

    template <typename... Args>
    List<T>* AllocateList(Args... args)
    {
        return new List<T>(args...);
    }

    template <typename... Args>
    Sequence<T>* AllocateArraySequence(Args... args)
    {
        return (Sequence<T>*) new ArraySequence<T>(args...);
    }

    template <typename... Args>
    ListSequence<T>* AllocateListSequence(Args... args)
    {
        return (Sequence<T>*) new ListSequence<T>(args...);
    }
};