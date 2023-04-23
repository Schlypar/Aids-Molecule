
#include "ADT.h"
#include "Matrix.h"
#include <memory>

LogPriority Logger::priority = TracePriority;

int main()
{
    Logger::setPriority(ErrorPriority);

    return 0;        
}