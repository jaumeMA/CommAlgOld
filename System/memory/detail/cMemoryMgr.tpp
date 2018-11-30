
#include "System/memory/cMemoryPool.h"

namespace yame
{
namespace memory
{
extern cMemoryMgr k_memoryMgr;

template<typename T>
const cMemoryPool& cMemoryProvider<cTypedMemoryPool<T>>::acquire()
{
    return k_memoryMgr.acquirePool(sizeof(T));
}

}
}
