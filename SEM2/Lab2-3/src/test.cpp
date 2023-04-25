
#include "ADT.h"
#include "Pointer.h"

LogPriority Logger::priority = TracePriority;

int main()
{
    Logger::setPriority(ErrorPriority);

    UniquePtr<int> ptr(new int(5));

    print(*ptr, '\n');
    
    return 0;        
}