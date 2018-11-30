
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

#include <vector>
#include <string>
#include "Utils/engineTypes.h"
#include <pthread.h>

namespace yame
{
namespace memory
{

class cMemoryTracker
{
public:
    cMemoryTracker();
    ~cMemoryTracker();
    void registerAllocation(void *newAlloc, size_t numUnits) const;
    void deRegisterAllocation(void *alloc) const;

private:
    static const size_t k_invalidPos = -1;

    struct StackTraceData
    {
        StackTraceData(void** i_stackTrace, size_t i_stackTraceSize, const std::vector<std::string>& i_extraInfo);

        void** stackTrace;
        size_t stackTraceSize;
        std::vector<std::string> extraInfo;
    };

    size_t findAllocation(void *alloc) const;

    mutable std::vector<std::pair<void *, StackTraceData>> m_registeredAllocations;
    mutable pthread_mutex_t m_registerMutex;
};

}
}
