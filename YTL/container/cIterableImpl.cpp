#include "YTL/container/detail/cIterableImpl.h"
#include "System/memory/cMemoryMgr.h"
#include "System/memory/cMemoryPool.h"

namespace yame
{
namespace container
{
namespace detail
{

const size_t k_maxNumPairs = 1024;

const memory::cMemoryPool& k_pairAllocator = memory::cMemoryProvider<memory::cMemoryPool>::acquire(sizeof(void*) * 2);

}
}
}
