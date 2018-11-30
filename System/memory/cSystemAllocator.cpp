#include "System/memory/cSystemAllocator.h"
#include <cstdlib>

namespace yame
{
namespace memory
{

void* cSystemAllocator::allocate(size_t numUnits, size_t unitSize) const
{
    void *res = NULL;

    if(numUnits >= 1)
    {
        res = malloc(numUnits * unitSize);

#ifdef MEMORY_TRACKER
        registerAllocation(res, numUnits);
#endif
    }

    return res;
}
void cSystemAllocator::deallocate(void *ptr) const
{
    if(ptr)
    {
#ifdef MEMORY_TRACKER
        deRegisterAllocation(ptr);
#endif
        free(ptr);
    }

    return;
}
void* cSystemAllocator::reallocate(void *ptr, size_t numUnits, size_t unitSize) const
{
    void *res = NULL;

    if(numUnits >= 1)
    {
        //num allocations remains the same
        res = realloc(ptr,numUnits * unitSize);

#ifdef MEMORY_TRACKER
        if(res != ptr)
        {
            deRegisterAllocation(ptr);
            registerAllocation(res, numUnits);
        }
#endif
    }

    return res;
}

}
}
