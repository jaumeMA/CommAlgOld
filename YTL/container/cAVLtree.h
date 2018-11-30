
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

#include "cMap.h"
#include "YTL/container/cTreeBalancer.h"

namespace yame
{
namespace container
{
namespace detail
{

//since the map class will need this allocator for allocating its objects we need a built in map (take the chance for creating a red black tree)
template<typename T, typename R>
class cAVLnode : public cMapNode<T,R>
{
public:
    typedef T value_type;
    typedef R key_type;

    cAVLnode<T,R>();
    template<typename ... TT>
    cAVLnode<T,R>(const R& i_key, TT&& ... i_args);
    cAVLnode<T,R>(const cAVLnode<T,R>& other);
    cAVLnode<T,R>& operator=(const cAVLnode<T,R>& other);

    void setParent(cAVLnode<T,R>* i_parent);
    cAVLnode<T,R>* getParent();
    void setLeftChild(cAVLnode<T,R>* i_left);
    cAVLnode<T,R>* getLeftChild();
    void setRightChild(cAVLnode<T,R>* i_right);
    cAVLnode<T,R>* getRightChild();
    void setHeight(s8 i_height);
    s8 getHeight() const;

    s8  m_height;
};

}

template<typename T, typename R, template<typename> class A = memory::cTypedSystemAllocator, template<typename,typename> class B = detail::cAVLBalancer>
using cAVLtree = detail::cMapImpl<T,R,detail::cAVLnode<T,R>,A,B>;

}
}

#include "detail/cAVLtree.tpp"
