
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

#include "YTL/container/cIterable.h"

namespace yame
{
namespace container
{

template<typename R, typename T, typename TT>
class cMultiMapIterable : public detail::cBidirectionalIterableImpl<detail::ReversableIterableTraits<cPair<const R,T>&,TT,detail::cBidirectionalIterableBasePrivate,cConstBidirectionalIterable,1>>
{
protected:
    typedef typename detail::ReversableIterableTraits<cPair<const R,T>&,TT,detail::cBidirectionalIterableBasePrivate,cConstBidirectionalIterable,1>::forwarded_type forwarded_value_type;
    typedef typename detail::ReversableIterableTraits<cPair<const R,T>&,TT,detail::cBidirectionalIterableBasePrivate,cConstBidirectionalIterable,1>::value_type inner_value_type;
    typedef typename detail::ReversableIterableTraits<cPair<const R,T>&,TT,detail::cBidirectionalIterableBasePrivate,cConstBidirectionalIterable,1>::iterator_type inner_iterator_type;
    typedef typename detail::ReversableIterableTraits<cPair<const R,T>&,TT,detail::cBidirectionalIterableBasePrivate,cConstBidirectionalIterable,1>::node_pointer_type node_pointer_type;
    typedef typename inner_value_type::second_type multimap_node;
    typedef typename detail::cBidirectionalIterableImpl<detail::ReversableIterableTraits<cPair<const R,T>&,TT,detail::cBidirectionalIterableBasePrivate,cConstBidirectionalIterable,1>>::nested_iterable_interface nested_iterable_interface;

public:
    typedef typename detail::cBidirectionalIterableImpl<detail::ReversableIterableTraits<cPair<const R,T>&,TT,detail::cBidirectionalIterableBasePrivate,cConstBidirectionalIterable,1>>::iterator_type iterator_type;
    typedef typename detail::cBidirectionalIterableImpl<detail::ReversableIterableTraits<cPair<const R,T>&,TT,detail::cBidirectionalIterableBasePrivate,cConstBidirectionalIterable,1>>::const_iterator_type const_iterator_type;
	typedef typename detail::cBidirectionalIterableImpl<detail::ReversableIterableTraits<cPair<const R,T>&,TT,detail::cBidirectionalIterableBasePrivate,cConstBidirectionalIterable,1>>::value_type value_type;
	typedef typename detail::cBidirectionalIterableImpl<detail::ReversableIterableTraits<cPair<const R,T>&,TT,detail::cBidirectionalIterableBasePrivate,cConstBidirectionalIterable,1>>::reference reference;

    using detail::cBidirectionalIterableImpl<detail::ReversableIterableTraits<cPair<const R,T>&,TT,detail::cBidirectionalIterableBasePrivate,cConstBidirectionalIterable,1>>::cBidirectionalIterableImpl;
    using detail::cBidirectionalIterableImpl<detail::ReversableIterableTraits<cPair<const R,T>&,TT,detail::cBidirectionalIterableBasePrivate,cConstBidirectionalIterable,1>>::begin;
    using detail::cBidirectionalIterableImpl<detail::ReversableIterableTraits<cPair<const R,T>&,TT,detail::cBidirectionalIterableBasePrivate,cConstBidirectionalIterable,1>>::end;
    using detail::cBidirectionalIterableImpl<detail::ReversableIterableTraits<cPair<const R,T>&,TT,detail::cBidirectionalIterableBasePrivate,cConstBidirectionalIterable,1>>::cbegin;
    using detail::cBidirectionalIterableImpl<detail::ReversableIterableTraits<cPair<const R,T>&,TT,detail::cBidirectionalIterableBasePrivate,cConstBidirectionalIterable,1>>::cend;
    using detail::cBidirectionalIterableImpl<detail::ReversableIterableTraits<cPair<const R,T>&,TT,detail::cBidirectionalIterableBasePrivate,cConstBidirectionalIterable,1>>::getSize;
    using detail::cBidirectionalIterableImpl<detail::ReversableIterableTraits<cPair<const R,T>&,TT,detail::cBidirectionalIterableBasePrivate,cConstBidirectionalIterable,1>>::empty;

    bool empty() const override;
    size_t getSize() const override;

protected:
    node_pointer_type getNextElemInCurrentIterable(const nested_iterable_interface* currIterableBackEnd, node_pointer_type currNode) const override;
    node_pointer_type getPrevElemInCurrentIterable(const nested_iterable_interface* currIterableBackEnd, node_pointer_type currNode) const override;
};

template<typename T>
using multimap_view = cBidirectionalView<T>;

template<typename T>
using const_multimap_view = cConstBidirectionalView<T>;

}
}

#include "YTL/container/detail/cMultiMapIterable.tpp"
