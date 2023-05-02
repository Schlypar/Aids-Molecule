#include "Tree/BinaryTree.h"
#include <ADT.h>


LogPriority Logger::priority = TracePriority;


int main()
{
    // Logger::setPriority(ErrorPriority);

    SharedPtr<BinaryTree<int, int>> tree = new BinaryTree<int, int>(1);

    tree->Add(2);

    tree->Add(-1);

    tree->Add(10);

    std::cout << *tree << "\n";

    return 0;        
}