
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

#include "Utils/engineTypes.h"
#include <vector>

#ifdef MEMORY_TRACKER
#include "System/memory/cMemoryTracker.h"
#endif

namespace yame
{
namespace memory
{

class IMemoryProvider;

class cMemoryPool
#ifdef MEMORY_TRACKER
: private cMemoryTracker
#endif
{
    friend class cMemoryMgr;

public:
    typedef cMemoryPool allocator;

    void* allocate(size_t numUnits, size_t unitSize) const;
    void deallocate(void *ptr) const;
    void *reallocate(void *ptr, size_t numUnits, size_t unitSize) const;

protected:
    cMemoryPool(size_t maxNumChunks, unsigned char unitSizeBitPos, IMemoryProvider& i_memProvider);
    cMemoryPool(const cMemoryPool&) = delete;
    cMemoryPool(cMemoryPool&&) = delete;
    ~cMemoryPool();

private:
    struct cMemoryBank
    {
        cMemoryBank(void* i_ref, size_t i_maxNumChunks, size_t i_unitSizeBitPos);
        cMemoryBank(const cMemoryBank&) = delete;
        cMemoryBank(cMemoryBank&&);
        ~cMemoryBank();

        size_t          m_nextFreeChunk;
        void*           m_pRef;
        size_t*         m_mem_bufctl;
    };

    size_t                          m_unitSizeBitPos;
    size_t                          m_maxNumChunks;
    IMemoryProvider&                 m_memoryProvider;
    mutable std::vector<cMemoryBank>   m_memoryBankArr;
    mutable std::vector<size_t>       m_memoryBankPointer;
    mutable size_t                    m_currMemoryBank;
};

template<typename T>
class cTypedMemoryPool : public cMemoryPool
{
    friend class cMemoryMgr;

public:
    typedef cMemoryPool allocator;

    using cMemoryPool::allocate;
    using cMemoryPool::deallocate;
    using cMemoryPool::reallocate;

private:
    cTypedMemoryPool(size_t maxNumChunks, IMemoryProvider& i_memProvider);
};

}
}
