#include "Sequence/ArraySequence.h"
#include "Sequence/ListSequence.h"
#include "Sequence/SeqIter.h"
#include "Sequence/Sequence.h"
#include <ADT.h>
#include <cassert>
#include <string>

LogPriority Logger::priority = WarnPriority;

int main() {
    Sequence<int>* arr = new ArraySequence<int>{1, 2, 3, 4, 5};
    std::cout << "Initial array is:\n";
    for (auto x : *arr) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    std::cout << "Map initital (* 2.333):\n";
    auto* c2 = arr->Iter()
                   ->MapTo<double>([](const int& x) { return x * 2.333; })
                   ->Collect();
    for (auto x : *c2) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    delete c2;

    std::cout << "Map initital (* 2.0) -> to_string():\n";
    auto* c3 = arr->Iter()
                   ->MapTo<double>([](const int& x) { return x * 2.0; })
                   ->MapTo<std::string>([](const double& x) { return std::to_string(x); })
                   ->Collect();
    for (auto x : *c3) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    std::cout << "Map initital (* 1.0):\n";
    Sequence<int>* lst = new ListSequence<int>{10, 20, 30};
    auto* c4 = lst->Iter()
                   ->MapTo<int>([](const int& x) { return x + 1; })
                   ->Collect();
    for (auto x : *c4) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    delete c4;

    delete arr;
    delete lst;

    return 0;
}
