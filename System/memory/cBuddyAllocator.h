
/*This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#pragma once

#include "YTL/container/cAVLtree.h"
#include "Utils/engineTypes.h"
#include "YTL/container/cQueue.h"
#include "YTL/container/cArray.h"
#include "YTL/container/cTrie.h"
#include "YTL/container/cBlockSet.h"
#include "YTL/container/cIterator.h"

namespace yame
{
namespace memory
{

class cBuddyAllocator
{
    friend class cMemoryMgr;

public:
    void* allocate(size_t size, size_t unitSize) const;
    void *reallocate(void *ptr, size_t numUnits, size_t unitSize) const;
    void deallocate(void* ref);

private:
    struct cPool
    {
    public:
        cPool();
        cPool(void* address, u32 totalSize);
        cPool(const cPool& other)=delete;
        cPool(cPool&& other);
        inline void* allocate(size_t size);
        inline void deallocate(void* address);
        cPool& operator=(const cPool& other)=delete;
        cPool& operator=(cPool&& other);
        inline void* getRef() const;

    private:
        struct cBuddyFreeList
        {
            //intrusive
            struct cBuddy
            {
                private:
                    size_t       m_pAddress;
                    cBuddy*      m_pNextFreeBuddy;

                public:
                    cBuddy(size_t address);
                    size_t getAddress();
                    void setNextFreeBuddy(cBuddy* pNextFreeBuddy);
                    cBuddy* getNextFreeBuddy() const;
            };
        public:
            cBuddyFreeList() = default;
            cBuddyFreeList(u8 i_size);
            cBuddyFreeList(const cBuddyFreeList& cBuddyFreeList)=delete;
            cBuddyFreeList(cBuddyFreeList&& other);
            ~cBuddyFreeList();
            cBuddy* getFirstBuddy() const;
            size_t getSize() const;
            bool thereAreFreeBuddies() const;
            size_t getNextAvailableBuddy();
            void* getBuddyAt(void* i_adress) const;
            void pushNewBuddy(size_t address);
            void removeBuddy(cBuddy* i_prev, cBuddy* i_curr);
            static void mergeBuddy(size_t address, typename container::cArray<cBuddyFreeList>::iterator_type itInit, typename container::cArray<cBuddyFreeList>::iterator_type itEnd);
            cBuddyFreeList& operator=(const cBuddyFreeList& other)=delete;
            cBuddyFreeList& operator=(cBuddyFreeList&& other);

        private:
            cBuddy*     m_pFirstFreeBuddy = NULL;
            const u8    m_powerOfTwo = 0;
        };

        container::cArray<cBuddyFreeList>       m_buddyBlocks;
        void*                                   m_pAddress;
        size_t                                      m_totalSize;
        container::cTrie<u8,container::cByteSet<>>         m_allocations;
    };

    cBuddyAllocator(size_t poolSize);
    void init();
    void* AllocatePool(void* initialAddress, u32 length) const;

    size_t                                  m_poolSize;
    mutable container::cQueue<cPool>        m_poolQueue;
};

}
}
