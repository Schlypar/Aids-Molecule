
#include "ADT.h"
#include "ArraySequence.h"
#include "Pointer.h"

LogPriority Logger::priority = TracePriority;

void fn(SharedPtr<ArraySequence<int>> ptr)
{
    print(*ptr, '\n');
}

int main()
{
    Logger::setPriority(ErrorPriority);

    SharedPtr<ArraySequence<int>> ptr(new ArraySequence(5,6,7,8,8,9,10));

    fn(ptr);
    
    print(*ptr, '\n');

    return 0;        
}