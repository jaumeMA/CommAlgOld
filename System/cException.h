
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

#include "errno.h"
#include <exception>
#include "execinfo.h"
#include <assert.h>
#include "Utils/engineTypes.h"

#ifdef _DEBUG
    #define ASSERT(cond,msg) assert(cond && msg)
    #define FAIL(msg) assert(false && msg)
#else
    #define ASSERT(cond,msg)
    #define FAIL(msg)
#endif

#define STATIC_ASSERT(cond,msg) static_assert(cond,msg)
#define NUM_STACK_ENTRIES 10

enum E_ERROR_CODE
{
    EEC_NONE,
    EEC_DIVZERO,
    EEC_NULLPOINTER,
    EEC_FILENOEXISTS,
    EEC_COLLISION,
    EEC_NO_MEM_AVAIL,
    EEC_INVALID_INPUT,
    EEC_COUNT
};

static const c8 *errStr[EEC_COUNT]
=
{"None error",
"Division by zero",
"Null pointer",
"File does not exist",
"Error in collision detection",
"The block requested does not fit the current pool"
"The input is not allowed for this operation"
};

namespace yame
{
namespace info
{
class cException : public std::exception
{
private:
    E_ERROR_CODE    m_error;
public:
    cException(E_ERROR_CODE code)
    {
        m_error = code;
    }
    E_ERROR_CODE getErrorcCode() const throw()
    {
        return m_error;
    }
    const c8 *getErrorStr() const throw()
    {
        return errStr[m_error];
    }
    c8 **getStackTrace(int fileDesc)
    {
        void *array[NUM_STACK_ENTRIES];
        size_t size;
        c8 **str;

        size = backtrace(array, NUM_STACK_ENTRIES);

        str = backtrace_symbols(array,size);

        return str;
    }
};
}
}
