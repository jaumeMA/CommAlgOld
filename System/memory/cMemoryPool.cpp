#include "System/memory/cMemoryPool.h"
#include "System/memory/IMemoryProvider.h"
#include "System/cException.h"
#include <cstdlib>

#define ADD_MEMORY_BANK \
if(m_memoryBankPointer.empty() == false && m_memoryBankPointer[m_currMemoryBank] != m_currMemoryBank) \
{ \
    m_currMemoryBank = m_memoryBankPointer[m_currMemoryBank]; \
} \
else \
{ \
    m_memoryBankArr.emplace_back(m_memoryProvider.allocate(m_maxNumChunks << m_unitSizeBitPos),m_maxNumChunks,m_unitSizeBitPos); \
    \
    m_currMemoryBank = m_memoryBankPointer.size();\
    \
    m_memoryBankPointer.push_back(m_memoryBankPointer.size());\
}

namespace yame
{
namespace memory
{

cMemoryPool::cMemoryBank::cMemoryBank(void* i_ref, size_t i_maxNumChunks, size_t i_unitSizeBitPos)
: m_pRef(i_ref)
, m_nextFreeChunk(0)
{
    const size_t capacity = i_maxNumChunks << i_unitSizeBitPos;

    m_mem_bufctl = static_cast<size_t *>(malloc(i_maxNumChunks * sizeof(size_t)));

    for(size_t i=0;i<i_maxNumChunks-1;i++)
    {
        m_mem_bufctl[i] = i+1;
    }

    m_mem_bufctl[i_maxNumChunks-1] = i_maxNumChunks-1;
}
cMemoryPool::cMemoryBank::cMemoryBank(cMemoryBank&& other)
: m_nextFreeChunk(other.m_nextFreeChunk)
, m_pRef(other.m_pRef)
, m_mem_bufctl(other.m_mem_bufctl)
{
    other.m_nextFreeChunk = 0;
    other.m_pRef = NULL;
    other.m_mem_bufctl = NULL;
}
cMemoryPool::cMemoryBank::~cMemoryBank()
{
    delete m_mem_bufctl;
}

cMemoryPool::cMemoryPool(size_t maxNumChunks, unsigned char unitSizeBitPos, IMemoryProvider& i_memProvider)
: m_unitSizeBitPos(unitSizeBitPos)
, m_maxNumChunks(maxNumChunks)
, m_currMemoryBank(0)
, m_memoryProvider(i_memProvider)
{
    ASSERT(m_unitSizeBitPos < 32, "wtf");

    ADD_MEMORY_BANK
};
cMemoryPool::~cMemoryPool()
{
    typename std::vector<cMemoryBank>::iterator itBank = m_memoryBankArr.begin();
    for(;itBank!=m_memoryBankArr.end();++itBank)
    {
        m_memoryProvider.deallocate((*itBank).m_pRef);
    }
}
void* cMemoryPool::allocate(size_t numUnits, size_t unitSize) const
{
    ASSERT(unitSize <= (1 << m_unitSizeBitPos), "MemoryPool only work with a unit size given at construction-time");
    ASSERT(numUnits == 1, "Memory pools can only provide 1 piece of memory");

    cMemoryBank& currMemoryBank = m_memoryBankArr[m_currMemoryBank];

    u8 *res = (u8 *)currMemoryBank.m_pRef + (currMemoryBank.m_nextFreeChunk << m_unitSizeBitPos);

    if(currMemoryBank.m_mem_bufctl[currMemoryBank.m_nextFreeChunk] == currMemoryBank.m_nextFreeChunk)
    {
        m_memoryBankPointer[m_currMemoryBank] = m_currMemoryBank;

        ADD_MEMORY_BANK
    }
    else
    {
        currMemoryBank.m_nextFreeChunk = currMemoryBank.m_mem_bufctl[currMemoryBank.m_nextFreeChunk];
    }

#ifdef MEMORY_TRACKER
        registerAllocation(res, numUnits);
#endif

    return (void *)res;
}
void cMemoryPool::deallocate(void *ptr) const
{
    //look for the pool that hols this ptr
    typename std::vector<cMemoryBank>::iterator itBank = m_memoryBankArr.begin();
    for(;itBank!=m_memoryBankArr.end();++itBank)
    {
        if(size_t((*itBank).m_pRef) <= size_t(ptr) && size_t(ptr) < (size_t((*itBank).m_pRef) + (m_maxNumChunks << m_unitSizeBitPos)))
        {
            cMemoryBank& currMemoryBank = *itBank;
            size_t objMemoryBankIndex = std::distance(m_memoryBankArr.begin(), itBank);

            u32 objPos = ((u8 *)ptr - (u8 *)currMemoryBank.m_pRef) >> m_unitSizeBitPos;

            ASSERT(objPos < m_maxNumChunks, "Unconsistent situation");

            currMemoryBank.m_mem_bufctl[objPos] = currMemoryBank.m_nextFreeChunk;

            currMemoryBank.m_nextFreeChunk = objPos;

            //look for the tail in the memory banks list (tail means to point to itself)
            size_t currBank = m_currMemoryBank;
            bool memoryBankInPath = false;

            while(m_memoryBankPointer[currBank] != currBank)
            {
                if(currBank != objMemoryBankIndex)
                {
                    currBank = m_memoryBankPointer[currBank];
                }
                else
                {
                    memoryBankInPath = true;
                    break;
                }
            }

            if(!memoryBankInPath)
            {
                m_memoryBankPointer[currBank] = objMemoryBankIndex;
            }

#ifdef MEMORY_TRACKER
            deRegisterAllocation(ptr);
#endif
        }
    }
}
void* cMemoryPool::reallocate(void *ptr, size_t numUnits, size_t unitSize) const
{
    ASSERT(false, "Memory pools do not support rellocations");

    return NULL;
}

}
}
