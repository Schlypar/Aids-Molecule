#include "Tree/BinaryTree.h"
#include <ADT.h>


LogPriority Logger::priority = TracePriority;


int main()
{
    // Logger::setPriority(ErrorPriority);

    SharedPtr<Tree<int, int>> tree = (Tree<int, int>*) new BinaryTree<int, int>(1, [](int value) -> int { return value * value / 2 - 1 - value; } );
    tree->Add(2)->Add(5);

    tree->Add(-1);

    tree->Add(10)->Add(-10)->Add(15)->Add(-15);

    std::cout << tree << "\n";

    return 0;        
}