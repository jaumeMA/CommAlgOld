#include "System/memory/cMemoryTracker.h"
#include <execinfo.h>
#include <dlfcn.h>
#include <cxxabi.h>
#include "System/cException.h"

namespace yame
{
namespace memory
{

cMemoryTracker::StackTraceData::StackTraceData(void** i_stackTrace, size_t i_stackTraceSize, const std::vector<std::string>& i_extraInfo)
: stackTrace(i_stackTrace)
, stackTraceSize(i_stackTraceSize)
, extraInfo(i_extraInfo)
{}

cMemoryTracker::cMemoryTracker()
{
    pthread_mutex_init(&m_registerMutex,NULL);
}
cMemoryTracker::~cMemoryTracker()
{
    pthread_mutex_destroy(&m_registerMutex);

    typename std::vector<std::pair<void *, StackTraceData>>::const_iterator itAlloc = m_registeredAllocations.cbegin();

    for(;itAlloc!=m_registeredAllocations.cend();++itAlloc)
    {
        char buf[1024];
        void* allocationAddress = (*itAlloc).first;
        StackTraceData stackTraceData = (*itAlloc).second;
        void** stackTrace = stackTraceData.stackTrace;
        const int numStackFrames = stackTraceData.stackTraceSize;

        printf("Pending alloc in %p done at:\n", allocationAddress);

        for(size_t i=0;i<numStackFrames;i++)
        {
            Dl_info info;
            if (dladdr(stackTrace, &info))
            {
                int status;

                char* demangled = abi::__cxa_demangle(info.dli_sname, NULL, 0, &status);
                snprintf(buf, sizeof(buf), "%-3d %s + %zd",
                        i, status == 0 ? demangled : info.dli_sname,
                        (char *)stackTrace - (char *)info.dli_saddr);

                free(demangled);
            }
            else
            {
                snprintf(buf, sizeof(buf), "%-3d", i);
            }

            printf("%s %s\n", buf, stackTraceData.extraInfo[i].c_str());
        }

        printf("\n");
    }
}
void cMemoryTracker::registerAllocation(void *newAlloc, size_t numUnits) const
{
    pthread_mutex_lock(&m_registerMutex);

    //look for the new Alloc
    size_t indexAlloc = findAllocation(newAlloc);

    ASSERT(indexAlloc==k_invalidPos, "Error: memory allocation already present");

    static const size_t numStackTraces = 10;
    void *trace[numStackTraces];
    size_t size = backtrace(trace, numStackTraces);
    std::vector<std::string> extraInfo;

    char** symbols = backtrace_symbols(trace,size);

    for(size_t i=0;i<size;i++)
    {
        extraInfo.push_back(symbols[i]);
    }

    m_registeredAllocations.push_back(std::pair<void*,StackTraceData>(newAlloc, StackTraceData(trace,size,extraInfo)));

    pthread_mutex_unlock(&m_registerMutex);
}
void cMemoryTracker::deRegisterAllocation(void *alloc) const
{
    pthread_mutex_lock(&m_registerMutex);

    size_t indexAlloc = findAllocation(alloc);

    ASSERT(indexAlloc!=-1, "Error: memory allocation not present");

    m_registeredAllocations.erase(m_registeredAllocations.begin()+indexAlloc);

    pthread_mutex_unlock(&m_registerMutex);
}
size_t cMemoryTracker::findAllocation(void *alloc) const
{
    size_t res = k_invalidPos;

    if(alloc)
    {
        for(size_t i=0;i<m_registeredAllocations.size();i++)
        {
            if(m_registeredAllocations[i].first==alloc)
            {
                res = i;
                break;
            }
        }
    }

    return res;
}

}
}
