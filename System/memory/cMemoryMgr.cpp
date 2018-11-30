#include "cMemoryMgr.h"
#include "cMemoryPool.h"
#include "cBuddyAllocator.h"
#include "Math/cMath.h"

namespace yame
{
namespace memory
{

cMemoryMgr k_memoryMgr{};

cMemoryMgr::cMemoryMgr()
: m_numElemsPerPool(1024)
, m_buddyAllocator(1024 * 1024)
{
}
cMemoryMgr::~cMemoryMgr()
{
}
void* cMemoryMgr::allocate(size_t sizeInBytes)
{
    if(sizeInBytes > 0)
    {
        return m_buddyAllocator.allocate(sizeInBytes, 1);
    }
    else
    {
        return NULL;
    }
}
void cMemoryMgr::deallocate(void* ptr)
{
    if(ptr)
    {
        m_buddyAllocator.deallocate(ptr);
    }
}
const cMemoryPool& cMemoryMgr::acquirePool(size_t unitSize)
{
    typename container::cAVLtree<cMemoryPool*,size_t>::iterator_type itPool = m_memoryPoolMap.search(unitSize);

    if(itPool != m_memoryPoolMap.end())
    {
        return *(itPool->second);
    }
    else
    {
        unsigned char unitSizeBitPos = math::powerOf2BitPos(unitSize);
        cMemoryPool* newPool = new cMemoryPool(m_numElemsPerPool >> unitSizeBitPos, unitSizeBitPos, *this);

        m_memoryPoolMap[unitSize] = newPool;

        return *newPool;
    }
}
const cBuddyAllocator& cMemoryMgr::acquirePool()
{
    return m_buddyAllocator;
}


const cMemoryPool& cMemoryProvider<cMemoryPool>::acquire(size_t unitSize)
{
    return k_memoryMgr.acquirePool(unitSize);
}

}
}
