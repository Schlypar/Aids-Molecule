
#include "ADT.h"
#include "ArraySequence.h"
#include "Pointer.h"

LogPriority Logger::priority = TracePriority;

void fn(WeakPtr<ArraySequence<int>> ptr)
{
    print(*ptr, '\n');
}

int main()
{
    Logger::setPriority(ErrorPriority);

    WeakPtr<ArraySequence<int>> ptr(new ArraySequence(5,6,7,8,8,9,10));

    // errors as expected
    fn(ptr);
    
    print(*ptr, '\n');

    return 0;        
}