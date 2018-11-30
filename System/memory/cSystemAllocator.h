
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

#include <cstddef>

#ifdef MEMORY_TRACKER
#include "System/memory/cMemoryTracker.h"
#endif

namespace yame
{
namespace memory
{

template<typename>
struct cMemoryProvider;

class cSystemAllocator
#ifdef MEMORY_TRACKER
: private cMemoryTracker
#endif
{
    friend struct cMemoryProvider<cSystemAllocator>;

public:
    typedef cSystemAllocator allocator;
    typedef void type;
    typedef void* pointer;
    typedef const void* const_pointer;
    typedef ptrdiff_t difference_type;

    void* allocate(size_t numUnits, size_t unitSize) const;
    void deallocate(void *ptr) const;
    void* reallocate(void *ptr, size_t numUnits, size_t unitSize) const;

protected:
    cSystemAllocator() = default;
    cSystemAllocator(const cSystemAllocator& other) = delete;
    cSystemAllocator(cSystemAllocator&& other) = delete;
};

template<typename T>
class cTypedSystemAllocator : public cSystemAllocator
{
    friend struct cMemoryProvider<cTypedSystemAllocator<T>>;

public:
    typedef cSystemAllocator allocator;
    typedef T type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T *const_pointer;
    typedef const T& const_reference;
    typedef ptrdiff_t difference_type;

    using cSystemAllocator::allocate;
    using cSystemAllocator::deallocate;
    using cSystemAllocator::reallocate;

protected:
    cTypedSystemAllocator() = default;
    cTypedSystemAllocator(const cTypedSystemAllocator<T>& other) = delete;
    cTypedSystemAllocator(cTypedSystemAllocator<T>&& other) = delete;
};

template<typename T>
struct cMemoryProvider<cTypedSystemAllocator<T>>
{
    static const cTypedSystemAllocator<T>& acquire()
    {
        //use clauses {} so we avoid parsing problems with gcc
        static cTypedSystemAllocator<T> cSystemAllocatorSingleton{};

        return cSystemAllocatorSingleton;
    }
};

template<>
struct cMemoryProvider<cSystemAllocator>
{
    static const cSystemAllocator& acquire()
    {
        //use clauses {} so we avoid parsing problems with gcc
        static cSystemAllocator cSystemAllocatorSingleton{};

        return cSystemAllocatorSingleton;
    }
};

}
}
