#include "System/memory/cBuddyAllocator.h"
#include "Math/cMath.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace yame
{
namespace memory
{

cBuddyAllocator::cPool::cBuddyFreeList::cBuddy::cBuddy(size_t address)
: m_pAddress(address)
, m_pNextFreeBuddy(NULL)
{
}
size_t cBuddyAllocator::cPool::cBuddyFreeList::cBuddy::getAddress()
{
    return m_pAddress;
}
void cBuddyAllocator::cPool::cBuddyFreeList::cBuddy::setNextFreeBuddy(cBuddy* pNextFreeBuddy)
{
    if(m_pNextFreeBuddy != pNextFreeBuddy)
    {
        m_pNextFreeBuddy = pNextFreeBuddy;
    }

    return;
}
cBuddyAllocator::cPool::cBuddyFreeList::cBuddy* cBuddyAllocator::cPool::cBuddyFreeList::cBuddy::getNextFreeBuddy() const
{
    return m_pNextFreeBuddy;
}

cBuddyAllocator::cPool::cBuddyFreeList::cBuddyFreeList(u8 i_size)
: m_pFirstFreeBuddy(NULL)
, m_powerOfTwo(i_size)
{}
cBuddyAllocator::cPool::cBuddyFreeList::cBuddyFreeList(cBuddyFreeList&& other)
: m_pFirstFreeBuddy(other.m_pFirstFreeBuddy)
, m_powerOfTwo(other.m_powerOfTwo)
{
    other.m_pFirstFreeBuddy = NULL;
}
cBuddyAllocator::cPool::cBuddyFreeList::~cBuddyFreeList()
{
    cBuddy* currFreeBuddy = m_pFirstFreeBuddy;

    while(currFreeBuddy)
    {
        cBuddy* nextFreeBuddy = currFreeBuddy->getNextFreeBuddy();
        delete currFreeBuddy;
        currFreeBuddy = nextFreeBuddy;
    }
}
cBuddyAllocator::cPool::cBuddyFreeList::cBuddy* cBuddyAllocator::cPool::cBuddyFreeList::getFirstBuddy() const
{
    return m_pFirstFreeBuddy;
}
size_t cBuddyAllocator::cPool::cBuddyFreeList::getSize() const
{
    return 1 << m_powerOfTwo;
}
bool cBuddyAllocator::cPool::cBuddyFreeList::thereAreFreeBuddies() const
{
    return m_pFirstFreeBuddy!=NULL;
}
size_t cBuddyAllocator::cPool::cBuddyFreeList::getNextAvailableBuddy()
{
    ASSERT(thereAreFreeBuddies(), "Trying to obtain chunk in an empty free list!");

    //as alwaysm return first entry
    size_t res = m_pFirstFreeBuddy->getAddress();

    cBuddy* oldFirstBuddy = m_pFirstFreeBuddy;

    m_pFirstFreeBuddy = m_pFirstFreeBuddy->getNextFreeBuddy();

    delete oldFirstBuddy;

    return res;
}
void* cBuddyAllocator::cPool::cBuddyFreeList::getBuddyAt(void* i_address) const
{
    cBuddy* currBuddy = m_pFirstFreeBuddy;

    while(currBuddy && (u8*)currBuddy < (u8*)i_address)
    {
        currBuddy = currBuddy->getNextFreeBuddy();
    }

    return (currBuddy == i_address) ? i_address : NULL;
}
void cBuddyAllocator::cPool::cBuddyFreeList::mergeBuddy(size_t address, typename container::cArray<cBuddyFreeList>::iterator_type itInit, typename container::cArray<cBuddyFreeList>::iterator_type itEnd)
{
    cBuddyFreeList& currFreeList = *itInit;

    bool iAmAligned = (address >> currFreeList.m_powerOfTwo) & 0x1 == 0;

    size_t buddyAddress = (iAmAligned) ? address + (1 << currFreeList.m_powerOfTwo) : address - (1 << currFreeList.m_powerOfTwo);

    cBuddy* currBuddy = currFreeList.getFirstBuddy();
    cBuddy* prevBuddy = NULL;

    while(currBuddy)
    {
        if(currBuddy->getAddress() == buddyAddress)
        {
            currFreeList.removeBuddy(prevBuddy, currBuddy);

            if(++itInit != itEnd)
            {
                mergeBuddy(address, itInit, itEnd);
            }

            return;
        }

        prevBuddy = currBuddy;
        currBuddy = currBuddy->getNextFreeBuddy();
    }

    currFreeList.pushNewBuddy(address);
}
void cBuddyAllocator::cPool::cBuddyFreeList::pushNewBuddy(size_t address)
{
    cBuddy* newBuddy = new cBuddy(address);

    if(m_pFirstFreeBuddy != NULL)
    {
        newBuddy->setNextFreeBuddy(m_pFirstFreeBuddy);
    }

    m_pFirstFreeBuddy = newBuddy;

    return;
}
void cBuddyAllocator::cPool::cBuddyFreeList::removeBuddy(cBuddy* i_prev, cBuddy* i_curr)
{
    if(i_prev)
    {
        i_prev->setNextFreeBuddy(i_curr->getNextFreeBuddy());
    }

    delete i_curr;
}
cBuddyAllocator::cPool::cBuddyFreeList& cBuddyAllocator::cPool::cBuddyFreeList::operator=(cBuddyFreeList&& other)
{
    m_pFirstFreeBuddy = other.m_pFirstFreeBuddy;
    other.m_pFirstFreeBuddy = NULL;

    return *this;
}

cBuddyAllocator::cPool::cPool()
: m_pAddress(NULL)
, m_totalSize(0)
{
}
cBuddyAllocator::cPool::cPool(void* address, u32 totalSize)
: m_pAddress(address)
, m_totalSize(totalSize)
{
    u8 powerOfTwoBitPos = math::powerOf2BitPos(totalSize);

    m_buddyBlocks.reserve(powerOfTwoBitPos);

    for(size_t freeListIndex=0;freeListIndex<powerOfTwoBitPos;++freeListIndex)
    {
        m_buddyBlocks.push_back(freeListIndex + 1);
    }

    m_buddyBlocks.back().pushNewBuddy(0);
}
cBuddyAllocator::cPool::cPool(cPool&& other)
{
    m_buddyBlocks = mpl::move(other.m_buddyBlocks);
    m_pAddress = other.m_pAddress;
    m_totalSize = other.m_totalSize;

    other.m_pAddress = NULL;
    other.m_totalSize = 0;
}
cBuddyAllocator::cPool& cBuddyAllocator::cPool::operator=(cPool&& other)
{
    m_buddyBlocks = mpl::move(other.m_buddyBlocks);
    m_pAddress = other.m_pAddress;
    m_totalSize = other.m_totalSize;

    other.m_pAddress = NULL;
    other.m_totalSize = 0;

    return *this;
}
void* cBuddyAllocator::cPool::allocate(size_t size)
{
    //easy case since we are in the middle
    u8 nextPowerOf2BitPos = math::powerOf2BitPos(size);
    typename container::cArray<cBuddyFreeList>::iterator_type itBuddyList = m_buddyBlocks.begin();

    for(size_t currIndex = nextPowerOf2BitPos;currIndex < m_buddyBlocks.getSize();++currIndex)
    {
        cBuddyFreeList& buddyBlock = itBuddyList[currIndex];

        if(buddyBlock.thereAreFreeBuddies())
        {
            //can we do it even better?
            size_t nextAvailableBuddy = buddyBlock.getNextAvailableBuddy();

            for(size_t freeListIndex = 0;freeListIndex < currIndex; ++freeListIndex)
            {
                m_buddyBlocks[freeListIndex].pushNewBuddy(nextAvailableBuddy + (1 << freeListIndex));
            }

            u8* absResAddress = (u8*)m_pAddress + nextAvailableBuddy;

            m_allocations.emplace(size_t(absResAddress), currIndex);

            return absResAddress;
        }
    }

    return NULL;
}
void cBuddyAllocator::cPool::deallocate(void* address)
{
    ASSERT(address!=NULL, "Bad dealloc");

    typename container::cTrie<u8,container::cByteSet<>>::iterator_type itAllocation = m_allocations.search(size_t(address));

    size_t localAddress = (u8*)address - (u8*)m_pAddress;

    ASSERT(itAllocation!=m_allocations.end(), "Trying to erase unknown chunk");

    cBuddyFreeList::mergeBuddy(localAddress, m_buddyBlocks.begin() + (*itAllocation).second, m_buddyBlocks.end());
}
void* cBuddyAllocator::cPool::getRef() const
{
    return m_pAddress;
}

cBuddyAllocator::cBuddyAllocator(size_t poolSize)
: m_poolSize(poolSize)
{
    init();
}
void cBuddyAllocator::init()
{
    //add the first pool
    m_poolQueue.push(AllocatePool(NULL,m_poolSize), m_poolSize);
}
void* cBuddyAllocator::AllocatePool(void* initialAddress, u32 length) const
{
    void* res = NULL;

    int fd;

    if (length > 0 && (fd = open("/dev/zero", O_RDWR, 0)) != -1)
    {
        res = mmap(initialAddress,length,PROT_READ|PROT_WRITE,MAP_ANON|MAP_PRIVATE,fd,0);
    }

    return res;
}
void* cBuddyAllocator::allocate(size_t size, size_t unitSize) const
{
    void * res = NULL;
    size_t totalSize = size * unitSize;

    ASSERT(totalSize < m_poolSize, "You cannot allocate a block of contigous memory bigger than pool default size");

    //iterate through pools
    typename container::cQueue<cPool>::iterator_type itPool = m_poolQueue.begin();

    for(;itPool!=m_poolQueue.end();itPool++)
    {
        cPool& pool = *itPool;
        void* providedMem = pool.allocate(totalSize);

        if(providedMem != NULL)
        {
            return providedMem;
        }
    }

    //if no left space, think about allocating a new pool
    cPool& newPool = m_poolQueue.push(AllocatePool(NULL,m_poolSize), m_poolSize);

    return newPool.allocate(size);
}
void* cBuddyAllocator::reallocate(void *ptr, size_t numUnits, size_t unitSize) const
{
    //TODO
}
void cBuddyAllocator::deallocate(void* ref)
{
    //look into chunks in use
    typename container::cQueue<cPool>::iterator_type itPool = yame::find_if(m_poolQueue,ytl::function<bool(const cPool&)>([ref,this](const cPool& i_pool){ return size_t(i_pool.getRef()) <= size_t(ref) && size_t(ref) <= (size_t(i_pool.getRef()) + this->m_poolSize); }));

    ASSERT(itPool!=m_poolQueue.end(), "Trying to free unused chunk");

    cPool& refPool = *itPool;

    refPool.deallocate(ref);

    return;
}

}
}
