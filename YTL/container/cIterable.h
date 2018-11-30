
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

#include "YTL/container/cIterator.h"
#include "YTL/container/cPair.h"
#include "YTL/container/detail/cIterableImpl.h"

namespace yame
{
namespace container
{
namespace detail
{

template<template <typename> class Iterable, typename  Traits>
using cConstIterable = Iterable<Traits>;

template<template <typename> class Iterable, typename  Traits>
class cIterable : public Iterable<Traits>
{
    typedef typename cConstIterableWithoutTransformImpl<Traits>::nested_iterable_interface nested_iterable_interface;
    typedef typename Traits::iterator_type inner_iterator_type;
    typedef typename Traits::pointer_type inner_pointer_type;
    typedef typename Traits::node_pointer_type inner_node_pointer_type;
    typedef typename Traits::iterable_public_interface inner_iterable_public_interface;
    typedef typename Traits::iterable_private_interface inner_iterable_private_interface;
    typedef typename inner_iterable_public_interface::add_type add_type;
    typedef cNonConstIterableInterface<inner_node_pointer_type, add_type> inner_iterable;
    typedef typename Iterable<Traits>::pair_node pair_node;

public:
    typedef typename Iterable<Traits>::iterator_type iterator_type;
    typedef typename Iterable<Traits>::const_iterator_type const_iterator_type;
	typedef typename Iterable<Traits>::value_type value_type;
	typedef typename Iterable<Traits>::pointer_type pointer_type;
	typedef typename Iterable<Traits>::node_pointer_type node_pointer_type;
	typedef typename Iterable<Traits>::reference reference;
	typedef typename Iterable<Traits>::const_reference const_reference;

    using Iterable<Traits>::Iterable;
    using Iterable<Traits>::begin;
    using Iterable<Traits>::end;
    using Iterable<Traits>::cbegin;
    using Iterable<Traits>::cend;
    using Iterable<Traits>::getSize;
    using Iterable<Traits>::empty;

    iterator_type erase(iterator_type i_it) override;
    iterator_type add(add_type i_value) override;
};

template<typename T, typename TT, template<class> class IterableBasePrivateInterface, template<class> class IterableBasePublicInterface, int _numIterables>
struct IterableTraits
{
    static const ReferenceCategory category = detail::deduceCategoryFromType<T>::category;
    static const ReferenceCategory forwarded_category = detail::deduceCategoryFromType<TT>::category;
	typedef IterableBasePublicInterface<T> iterable_public_interface;
	typedef IterableBasePrivateInterface<impl::AgnosticIteratorTraits<T,category>> iterable_private_interface;
	typedef IterableBasePublicInterface<TT> forwarded_iterable_public_interface;
	typedef IterableBasePrivateInterface<impl::AgnosticIteratorTraits<TT,forwarded_category>> forwarded_iterable_private_interface;
    typedef typename iterable_public_interface::iterator_type iterator_type;
    typedef typename iterable_public_interface::const_iterator_type const_iterator_type;
    typedef typename forwarded_iterable_public_interface::iterator_type forwarded_iterator_type;
    typedef typename forwarded_iterable_public_interface::const_iterator_type const_forwarded_iterator_type;
	typedef typename iterable_public_interface::iterator_type::raw_type raw_type;
	typedef typename iterable_public_interface::iterator_type::value_type value_type;
	typedef typename iterable_public_interface::iterator_type::pointer_type pointer_type;
	typedef typename iterable_public_interface::iterator_type::node_pointer_type node_pointer_type;
	typedef typename forwarded_iterable_public_interface::iterator_type::raw_type forwarded_raw_type;
	typedef typename forwarded_iterable_public_interface::iterator_type::value_type forwarded_type;
	typedef typename forwarded_iterable_public_interface::iterator_type::pointer_type forwarded_pointer_type;
	typedef typename forwarded_iterable_public_interface::iterator_type::node_pointer_type forwarded_node_pointer_type;
	typedef typename iterable_public_interface::iterator_type::reference reference;
	typedef typename forwarded_iterable_public_interface::iterator_type::reference forwarded_reference;
	typedef typename iterable_public_interface::iterator_type::const_reference const_reference;
	typedef typename forwarded_iterable_public_interface::iterator_type::const_reference const_forwarded_reference;
    static const size_t numIterables = _numIterables;
};

template<typename T, typename TT, template<class> class IterableBasePrivateInterface, template<class> class IterableBasePublicInterface, int _numIterables>
struct ReversableIterableTraits : IterableTraits<T,TT,IterableBasePrivateInterface,IterableBasePublicInterface,_numIterables>
{
    typedef typename IterableBasePublicInterface<TT>::reverse_iterator_type reverse_forwarded_iterator_type;
    typedef typename IterableBasePublicInterface<TT>::const_reverse_iterator_type const_reverse_forwarded_iterator_type;
};

}

//const intra iterable
template<typename T, typename TT = T>
using cConstInputIntraIterable = detail::cConstIterable<detail::cInputIterableImpl,detail::IterableTraits<T,TT,detail::cInputIterableBasePrivate,cConstInputIterable,1>>;
template<typename T>
using cConstOutputIntraIterable = detail::cConstIterable<detail::cOutputIterableImpl,detail::IterableTraits<T,T,detail::cOutputIterableBasePrivate,cConstOutputIterable,1>>;
template<typename T, typename TT = T>
using cConstForwardIntraIterable = detail::cConstIterable<detail::cForwardIterableImpl,detail::IterableTraits<T,TT,detail::cForwardIterableBasePrivate,cConstForwardIterable,1>>;
template<typename T, typename TT = T>
using cConstBidirectionalIntraIterable = detail::cConstIterable<detail::cBidirectionalIterableImpl,detail::ReversableIterableTraits<T,TT,detail::cBidirectionalIterableBasePrivate,cConstBidirectionalIterable,1>>;
template<typename T, typename TT = T>
using cConstRandomAccessIntraIterable = detail::cConstIterable<detail::cRandomAccessIterableImpl,detail::ReversableIterableTraits<T,TT,detail::cRandomAccessIterableBasePrivate,cConstRandomAccessIterable,1>>;

//non const intra iterable
template<typename T>
using cInputIntraIterable = detail::cIterable<detail::cInputIterableImpl,detail::IterableTraits<T,T,detail::cInputIterableBasePrivate,cInputIterable,1>>;
template<typename T>
using cOutputIntraIterable = detail::cIterable<detail::cOutputIterableImpl,detail::IterableTraits<T,T,detail::cOutputIterableBasePrivate,cOutputIterable,1>>;
template<typename T>
using cForwardIntraIterable = detail::cIterable<detail::cForwardIterableImpl,detail::IterableTraits<T,T,detail::cForwardIterableBasePrivate,cForwardIterable,1>>;
template<typename T>
using cBidirectionalIntraIterable = detail::cIterable<detail::cBidirectionalIterableImpl,detail::ReversableIterableTraits<T,T,detail::cBidirectionalIterableBasePrivate,cBidirectionalIterable,1>>;
template<typename T>
using cRandomAccessIntraIterable = detail::cIterable<detail::cRandomAccessIterableImpl,detail::ReversableIterableTraits<T,T,detail::cRandomAccessIterableBasePrivate,cRandomAccessIterable,1>>;

//const inter iterable
template<int numIterables, typename T, typename TT = T>
using cConstInputInterIterable = detail::cConstIterable<detail::cInputIterableImpl,detail::IterableTraits<T,TT,detail::cInputIterableBasePrivate,cConstInputIterable,numIterables>>;
template<int numIterables, typename T>
using cConstOutputInterIterable = detail::cConstIterable<detail::cOutputIterableImpl,detail::IterableTraits<T,T,detail::cOutputIterableBasePrivate,cConstOutputIterable,numIterables>>;
template<int numIterables, typename T, typename TT = T>
using cConstForwardInterIterable = detail::cConstIterable<detail::cForwardIterableImpl,detail::IterableTraits<T,TT,detail::cForwardIterableBasePrivate,cConstForwardIterable,numIterables>>;
template<int numIterables, typename T, typename TT = T>
using cConstBidirectionalInterIterable = detail::cConstIterable<detail::cBidirectionalIterableImpl,detail::ReversableIterableTraits<T,TT,detail::cBidirectionalIterableBasePrivate,cConstBidirectionalIterable,numIterables>>;
template<int numIterables, typename T, typename TT = T>
using cConstRandomAccessInterIterable = detail::cConstIterable<detail::cRandomAccessIterableImpl,detail::ReversableIterableTraits<T,TT,detail::cRandomAccessIterableBasePrivate,cConstRandomAccessIterable,numIterables>>;

//for obvious consistency issues, we do not offer non const inter iterables (iterables in general holds private interface of iterables
// which is agnostic of const/non const, so for the performance sake we avoid dynamic-casting anything here), besides the FACT that
// in inter iterables we manage different kinds of iterables so it does not make any sense to try to add a particular element kind

}
}

#include "YTL/container/detail/cIterable.tpp"
