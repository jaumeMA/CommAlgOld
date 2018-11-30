
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

#include "cMemoryMgr.h"
#include "cAVLtree.h"
#include "cQueue.h"
#include "cString.h"

//since the allocators are done basically for speeding the containers here some especializations
namespace yame
{
namespace container
{
template<typename T, typename R>
class cCustomMap : public cMap<T,R,memory::customAlloc>{};

template<typename T, typename R>
class cCustomAVL : public cAVLtree<T,R,memory::customAlloc>{};

template<typename T>
class cCustomQueue : public cQueue<T,memory::customAlloc>{};

template<typename T>
class cCustomArray : public cArray<T,memory::customAlloc<T>>{};

template<typename T>
class cCustomString : public cString<memory::customAlloc<T>>{};
}
}
