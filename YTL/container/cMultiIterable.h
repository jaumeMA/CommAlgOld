#pragma once

#include "cIterator.h"
#include "YTL/optional/optional.h"

namespace core
{
namespace container
{
namespace detail
{

template<typename Traits>
class cMultiIterableImpl : public Traits::forwarded_iterable_type
{
public:
	typedef typename Traits::iterable_type iterable_type;
	typedef typename Traits::forwarded_iterable_type forwarded_iterable_type;
    typedef typename Traits::iterator_type iterator_type;
    typedef typename Traits::const_iterator_type const_iterator_type;
    typedef typename Traits::forwarded_iterator_type forwarded_iterator_type;
    typedef typename Traits::const_forwarded_iterator_type const_forwarded_iterator_type;
	typedef typename Traits::value_type value_type;
	typedef typename Traits::forwarded_type forwarded_type;
	typedef typename Traits::reference reference;
	typedef typename Traits::const_reference const_reference;
	typedef typename Traits::forwarded_reference forwarded_reference;
	typedef typename Traits::const_forwarded_reference const_forwarded_reference;

    cMultiIterableImpl(const iterable_type& i_privateIterable, const ytl::function<forwarded_reference, reference>& i_transform);

    forwarded_iterator_type begin(const ytl::function<bool, forwarded_reference>& filter = null_ptr) override;
    forwarded_iterator_type end() override;
    const_forwarded_iterator_type cbegin(const ytl::function<bool, const_forwarded_reference>& filter = null_ptr) const override;
    const_forwarded_iterator_type cend() const override;
    size_t getSize() const override;
    bool empty() const override;

protected:
    forwarded_type* getFirstElem() const override;
    forwarded_type* getNextElem(forwarded_type* currNode) const override;
    forwarded_reference getValue(forwarded_type* currNode) const override;

	cArray<iterable_type&> m_iterableArray;
	cQueue<cPair<iterable_type&,value_type*>> m_createdPointers; // think about having this as an intrusive list
    ytl::function<forwarded_reference, reference> m_transform;
};

template<typename Traits>
using cInputIterableImpl = cMultiIterableImpl<Traits>;

template<typename Traits>
using cOutputIterableImpl = cMultiIterableImpl<Traits>;

template<typename Traits>
using cForwardIterableImpl = cInputIterableImpl<Traits>;

template<typename Traits>
class cBidirectionalIterableImpl : public cForwardIterableImpl<Traits>
{
public:
	typedef typename cForwardIterableImpl<Traits>::iterable_type iterable_type;
	typedef typename cForwardIterableImpl<Traits>::forwarded_iterable_type forwarded_iterable_type;
    typedef typename cForwardIterableImpl<Traits>::iterator_type iterator_type;
    typedef typename cForwardIterableImpl<Traits>::const_iterator_type const_iterator_type;
    typedef typename cForwardIterableImpl<Traits>::forwarded_iterator_type forwarded_iterator_type;
    typedef typename cForwardIterableImpl<Traits>::const_forwarded_iterator_type const_forwarded_iterator_type;
	typedef typename cForwardIterableImpl<Traits>::value_type value_type;
	typedef typename cForwardIterableImpl<Traits>::forwarded_type forwarded_type;
	typedef typename cForwardIterableImpl<Traits>::reference reference;
	typedef typename cForwardIterableImpl<Traits>::forwarded_reference forwarded_reference;
	typedef typename cForwardIterableImpl<Traits>::const_reference const_reference;
	typedef typename cForwardIterableImpl<Traits>::const_forwarded_reference const_forwarded_reference;

    using cForwardIterableImpl<Traits>::cForwardIterableImpl;
    using cForwardIterableImpl<Traits>::begin;
    using cForwardIterableImpl<Traits>::end;
    using cForwardIterableImpl<Traits>::cbegin;
    using cForwardIterableImpl<Traits>::cend;
    using cForwardIterableImpl<Traits>::getSize;
    using cForwardIterableImpl<Traits>::empty;

protected:
    forwarded_type* getLastElem() const override;
    forwarded_type* getPrevElem(forwarded_type* currNode) const override;
};

template<typename Traits>
class cRandomAccessIterableImpl : public cBidirectionalIterableImpl<Traits>
{
public:
	typedef typename cBidirectionalIterableImpl<Traits>::iterable_type iterable_type;
	typedef typename cBidirectionalIterableImpl<Traits>::forwarded_iterable_type forwarded_iterable_type;
    typedef typename cBidirectionalIterableImpl<Traits>::iterator_type iterator_type;
    typedef typename cBidirectionalIterableImpl<Traits>::const_iterator_type const_iterator_type;
    typedef typename cBidirectionalIterableImpl<Traits>::forwarded_iterator_type forwarded_iterator_type;
    typedef typename cBidirectionalIterableImpl<Traits>::const_forwarded_iterator_type const_forwarded_iterator_type;
	typedef typename cBidirectionalIterableImpl<Traits>::value_type value_type;
	typedef typename cBidirectionalIterableImpl<Traits>::forwarded_type forwarded_type;
	typedef typename cBidirectionalIterableImpl<Traits>::reference reference;
	typedef typename cBidirectionalIterableImpl<Traits>::forwarded_reference forwarded_reference;
	typedef typename cBidirectionalIterableImpl<Traits>::const_reference const_reference;
	typedef typename cBidirectionalIterableImpl<Traits>::const_forwarded_reference const_forwarded_reference;

    using cBidirectionalIterableImpl<Traits>::cBidirectionalIterableImpl;
    using cBidirectionalIterableImpl<Traits>::begin;
    using cBidirectionalIterableImpl<Traits>::end;
    using cBidirectionalIterableImpl<Traits>::cbegin;
    using cBidirectionalIterableImpl<Traits>::cend;
    using cBidirectionalIterableImpl<Traits>::getSize;
    using cBidirectionalIterableImpl<Traits>::empty;

    size_t getIndexOfValue(forwarded_type* node) const override;
    forwarded_type* getValueAtIndex(size_t val) const override;
};

template<typename T, typename TT, template<class,ReferenceCategory> class IterableBasePrivateInterface, ReferenceCategory category>
struct MultiIterableTraits
{
	typedef IterableBasePrivateInterface<T,ReferenceCategory::NonConstReference> iterable_type;
	typedef IterableBasePrivateInterface<TT,category> forwarded_iterable_type;
    typedef typename iterable_type::iterator_type iterator_type;
    typedef typename iterable_type::const_iterator_type const_iterator_type;
    typedef typename forwarded_iterable_type::iterator_type forwarded_iterator_type;
    typedef typename forwarded_iterable_type::const_iterator_type const_forwarded_iterator_type;
	typedef typename iterator_type::value_type value_type;
	typedef typename forwarded_iterator_type::value_type forwarded_type;
	typedef typename iterator_type::reference reference;
	typedef typename forwarded_iterator_type::reference forwarded_reference;
	typedef typename iterator_type::const_reference const_reference;
	typedef typename forwarded_iterator_type::const_reference const_forwarded_reference;
};

}

//template<typename T, typename TT>
//using cInputIterable = detail::cInputIterableImpl<detail::IterableTraits<T,TT,detail::cInputIterableBasePrivate,false>>;
template<typename T, typename TT, detail::ReferenceCategory category = detail::ReferenceCategory::NonConstReference>
using cForwardIterable = detail::cForwardIterableImpl<detail::MultiIterableTraits<T,TT,detail::cForwardIterableBasePrivate,category>>;
template<typename T, typename TT, detail::ReferenceCategory category = detail::ReferenceCategory::NonConstReference>
using cBidirectionalIterable = detail::cBidirectionalIterableImpl<detail::MultiIterableTraits<T,TT,detail::cBidirectionalIterableBasePrivate,category>>;
template<typename T, typename TT, detail::ReferenceCategory category = detail::ReferenceCategory::NonConstReference>
using cRandomAccessIterable = detail::cRandomAccessIterableImpl<detail::MultiIterableTraits<T,TT,detail::cRandomAccessIterableBasePrivate,category>>;

template<typename T, typename TT>
using cConstForwardIterable = cForwardIterable<T,TT,detail::ReferenceCategory::ConstReference>;
template<typename T, typename TT>
using cConstBidirectionalIterable = cBidirectionalIterable<T,TT,detail::ReferenceCategory::ConstReference>;
template<typename T, typename TT>
using cConstRandomAccessIterable = cRandomAccessIterable<T,TT,detail::ReferenceCategory::ConstReference>;

template<typename T, typename TT>
using cCopyForwardIterable = cForwardIterable<T,TT,detail::ReferenceCategory::Value>;
template<typename T, typename TT>
using cCopyBidirectionalIterable = cBidirectionalIterable<T,TT,detail::ReferenceCategory::Value>;
template<typename T, typename TT>
using cCopyRandomAccessIterable = cRandomAccessIterable<T,TT,detail::ReferenceCategory::Value>;

}
}

#include "detail/cIterable.tpp"
