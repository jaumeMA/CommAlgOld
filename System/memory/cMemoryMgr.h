
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

#include <pthread.h>
#include "YTL/container/cAVLtree.h"
#include "System/memory/IMemoryProvider.h"
#include "System/memory/cBuddyAllocator.h"

namespace yame
{
namespace memory
{

class cMemoryPool;

class cMemoryMgr : public IMemoryProvider
{
    template<typename T>
    friend class cMemoryProvider;

public:
    cMemoryMgr();
    cMemoryMgr(const cMemoryMgr&) = delete;
    ~cMemoryMgr();
    void deInit();
    void Init();

private:
    const cMemoryPool& acquirePool(size_t unitSize);
    const cBuddyAllocator& acquirePool();

    //IMemoryProvider api impl
    void* allocate(size_t sizeInBytes) override;
    void deallocate(void* ptr) override;

    size_t m_numElemsPerPool;
    container::cAVLtree<cMemoryPool*,size_t> m_memoryPoolMap;
    cBuddyAllocator m_buddyAllocator;
};

template<typename T>
struct cMemoryProvider<cTypedMemoryPool<T>>
{
    static const cMemoryPool& acquire();
};

template<>
struct cMemoryProvider<cMemoryPool>
{
    static const cMemoryPool& acquire(size_t unitSize);
};

}
}

#include "System/memory/detail/cMemoryMgr.tpp"
