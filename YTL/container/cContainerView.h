#pragma once

#include "YTL/container/detail/cContainerViewImpl.h"
#include "YTL/container/cIterableBase.h"

namespace yame
{
namespace container
{
namespace detail
{

template<typename Iterable>
struct ViewTraits
{
    typedef Iterable iterable_type;
    typedef typename Iterable::iterator_type iterator_type;
    typedef typename Iterable::const_iterator_type const_iterator_type;
    typedef typename Iterable::reverse_iterator_type reverse_iterator_type;
    typedef typename Iterable::const_reverse_iterator_type const_reverse_iterator_type;
    typedef typename Iterable::const_reference const_reference;
    typedef typename Iterable::reference reference;
    typedef typename Iterable::pointer_type pointer_type;
    typedef typename mpl::add_constness<pointer_type>::type const_pointer_type;
};

}

template<typename T>
using cInputView = detail::cContainerViewImpl<cConstInputIterable<T>,detail::ViewTraits<cConstInputIterable<T>>>;
template<typename T>
using cOutputView = detail::cContainerViewImpl<cConstOutputIterable<T>,detail::ViewTraits<cConstOutputIterable<T>>>;
template<typename T>
using cForwardView = detail::cContainerViewImpl<cConstForwardIterable<T>,detail::ViewTraits<cConstForwardIterable<T>>>;
template<typename T>
using cBidirectionalView = detail::cContainerViewImpl<cConstBidirectionalIterable<T>,detail::ViewTraits<cConstBidirectionalIterable<T>>>;
template<typename T>
using cRandomAccessView = detail::cContainerViewImpl<cConstRandomAccessIterable<T>,detail::ViewTraits<cConstRandomAccessIterable<T>>>;

template<typename T>
using cConstInputView = detail::cContainerViewImpl<const cConstInputIterable<T>,detail::ViewTraits<const cConstInputIterable<T>>>;
template<typename T>
using cConstOutputView = detail::cContainerViewImpl<const cConstOutputIterable<T>,detail::ViewTraits<const cConstOutputIterable<T>>>;
template<typename T>
using cConstForwardView = detail::cContainerViewImpl<const cConstForwardIterable<T>,detail::ViewTraits<const cConstForwardIterable<T>>>;
template<typename T>
using cConstBidirectionalView = detail::cContainerViewImpl<const cConstBidirectionalIterable<T>,detail::ViewTraits<const cConstBidirectionalIterable<T>>>;
template<typename T>
using cConstRandomAccessView = detail::cContainerViewImpl<const cConstRandomAccessIterable<T>,detail::ViewTraits<const cConstRandomAccessIterable<T>>>;

}
}
