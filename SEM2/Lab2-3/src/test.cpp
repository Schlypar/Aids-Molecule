
#include "ADT.h"
#include "Allocator.h"
#include "ArraySequence.h"
#include "Pointer.h"
#include "Tree.h"

#include <memory>

LogPriority Logger::priority = TracePriority;


int main()
{
    // Logger::setPriority(ErrorPriority);

    SharedPtr<BinaryTree<int>> tree = new BinaryTree<int>(1);

    tree->Insert(2);

    tree->Insert(-1);

    tree->Insert(10);

    std::cout << tree->Depth(tree->GetRoot(), 1) << "\n";

    return 0;        
}