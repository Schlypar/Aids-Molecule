
#include "ADT.h"
#include "Allocator.h"
#include "Sequence/ArraySequence.h"
#include "Pointer.h"
#include "Tree/BinaryTree.h"
#include "Linal/Matrix.h"

LogPriority Logger::priority = TracePriority;


int main()
{
    // Logger::setPriority(ErrorPriority);

    SharedPtr<BinaryTree<int>> tree = new BinaryTree<int>(1);

    tree->Insert(2);

    tree->Insert(-1);

    tree->Insert(10);

    std::cout << *tree << "\n";

    return 0;        
}