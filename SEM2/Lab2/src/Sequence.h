#pragma once

#include "ADT.h"

template <typename T>
class Sequence
{
    T GetFirst() const = 0;
    T GetLast() const = 0;
    T Get(const Index index) const = 0;

    Sequence<T>* GetSubsequence(const Index start, const Index end) = 0;
    Sequence<T>* Concat(Sequence<T>* sequence) = 0;

    Size GetLength() const = 0;

    void Append(const T& data) = 0;
    void Append(T&& data) = 0;

    void Prepend(const T& data) = 0;
    void Prepend(T&& data) = 0;

    void InsertAt(const Index index) = 0;
};