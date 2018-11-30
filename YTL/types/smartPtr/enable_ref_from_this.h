
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

#include "YTL/types/smartPtr/reference_counter.h"
#include "System/cException.h"

namespace yame
{
namespace ytl
{

class enable_ref_from_this
{
public:
    typedef void lendable_tag;

    enable_ref_from_this();
    enable_ref_from_this(const enable_ref_from_this& other);
    enable_ref_from_this(enable_ref_from_this&& other)=delete;
    ~enable_ref_from_this();
    enable_ref_from_this& operator=(const enable_ref_from_this& other);
    enable_ref_from_this& operator=(const enable_ref_from_this&& other)=delete;
    enable_ref_from_this& ref_from_this();
    const enable_ref_from_this& ref_from_this() const;
    unique_reference_counter* getRefCounter();
    const unique_reference_counter* getRefCounter() const;

private:

    unique_reference_counter   m_counter;
};

}
}

#include "YTL/types/smartPtr/detail/enable_ref_from_this.tpp"
