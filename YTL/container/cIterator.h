
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

#include "Utils/engineTypes.h"
#include "YTL/mpl/cTemplateHelper.h"
#include "YTL/mpl/cIteratorTemplateHelper.h"
#include "YTL/functional/cFunction.h"
#include "YTL/container/detail/cIteratorTag.h"

namespace yame
{
namespace container
{
namespace detail
{

//iterable forward declarations
template<typename>
class cInputIterableBasePrivate;
template<typename>
class cOutputIterableBasePrivate;
template<typename>
class cForwardIterableBasePrivate;
template<typename>
class cBidirectionalIterableBasePrivate;
template<typename>
class cRandomAccessIterableBasePrivate;
template<typename,typename,typename>
class cIterableBaseImpl;
template<template <typename> class,typename>
class cIterable;

namespace impl
{

template<typename T, ReferenceCategory _category>
struct AgnosticIteratorTraits
{
    typedef T raw_type;
    typedef typename mpl::drop_reference<typename mpl::drop_constness<T>::type>::type value_type;
    typedef typename getPointer<value_type,_category>::type pointer_type;
    typedef typename mpl::add_constness<pointer_type>::type const_pointer_type;
    typedef void* node_pointer_type;
    typedef size_t difference_type;
    typedef typename getReference<value_type,_category>::type reference;
    typedef typename mpl::add_constness<reference>::type const_reference;
    typedef typename mpl::drop_reference<typename mpl::drop_constness<T>::type>::type* reference_pointer;
    static const ReferenceCategory category = _category;
};

template<typename T, template<class> class Iterator, typename Iterable, typename Tag, ReferenceCategory category>
struct IteratorTraits : AgnosticIteratorTraits<T,category>
{
    typedef Tag tag;
    typedef Iterator<IteratorTraits<T,Iterator,Iterable,Tag,category>> iterator_type;
    typedef typename IteratorTraits<T,Iterator,const Iterable,Tag,mpl::static_if<category==ReferenceCategory::Value,getReference<T,ReferenceCategory::Value>,getReference<T,ReferenceCategory::ConstReference>>::type::value>::iterator_type const_iterator_type;
    typedef typename AgnosticIteratorTraits<T,category>::raw_type raw_type;
    typedef typename AgnosticIteratorTraits<T,category>::pointer_type pointer_type;
    typedef typename AgnosticIteratorTraits<T,category>::const_pointer_type const_pointer_type;
    typedef typename AgnosticIteratorTraits<T,category>::node_pointer_type node_pointer_type;
    typedef typename AgnosticIteratorTraits<T,category>::value_type value_type;
    typedef typename AgnosticIteratorTraits<T,category>::difference_type difference_type;
    typedef typename AgnosticIteratorTraits<T,category>::reference reference;
    typedef typename AgnosticIteratorTraits<T,category>::const_reference const_reference;
    typedef typename AgnosticIteratorTraits<T,category>::reference_pointer reference_pointer;
    typedef Iterable iterable_private_interface;
};

template<typename Traits>
class cIteratorImpl;

template<typename T, template<class> class Iterator, typename Iterable, template<typename> class AgnosticTag, typename DirectionTag, ReferenceCategory _category>
class cIteratorImpl<IteratorTraits<T,Iterator,Iterable,AgnosticTag<DirectionTag>,_category>>
{
    typedef IteratorTraits<T,Iterator,Iterable,AgnosticTag<DirectionTag>,_category> Traits;
    template<typename TTraits>
    friend class cIteratorImpl;

public:
    typedef typename Traits::tag tag;
	typedef typename Traits::raw_type raw_type;
	typedef typename Traits::value_type value_type;
	typedef typename Traits::pointer_type pointer_type;
	typedef typename Traits::const_pointer_type const_pointer_type;
	typedef typename Traits::node_pointer_type node_pointer_type;
	typedef typename Traits::difference_type difference_type;
	typedef typename Traits::reference reference;
	typedef typename Traits::const_reference const_reference;
	typedef typename Traits::reference_pointer reference_pointer;
	typedef typename Traits::iterable_private_interface iterable_private_interface;
	typedef typename Traits::iterator_type iterator_type;
	typedef typename Traits::const_iterator_type const_iterator_type;
	static const ReferenceCategory category = Traits::category;

    cIteratorImpl(const cIteratorImpl<Traits>&);
    cIteratorImpl(cIteratorImpl<Traits>&&);
    cIteratorImpl(iterable_private_interface& iter);
    cIteratorImpl(iterable_private_interface& iter, const ytl::function<bool(const_reference)>& filter);
    template<typename DDirectionTag, ReferenceCategory __category>
    explicit cIteratorImpl(const cIteratorImpl<IteratorTraits<T,Iterator,Iterable,AgnosticTag<DDirectionTag>,__category>>&);
    template<typename DDirectionTag, ReferenceCategory __category>
    explicit cIteratorImpl(cIteratorImpl<IteratorTraits<T,Iterator,Iterable,AgnosticTag<DDirectionTag>,__category>>&&);
    ~cIteratorImpl();

    inline iterator_type& operator=(const cIteratorImpl<Traits>& other);
    inline iterator_type operator++(int);

    static node_pointer_type const m_pHead;

protected:
    void setCurrentNode(node_pointer_type currNode);
    node_pointer_type getCurrentNode() const;

	iterable_private_interface* m_iterable;
    node_pointer_type m_pCurrentNode = m_pHead;
    ytl::function<bool(const_reference)> m_filter;
};

template<typename Traits>
class cInputIteratorImpl : public cIteratorImpl<Traits>
{
    template<typename,typename,typename>
    friend class cConstIterableBaseImpl;
    template<typename,typename,typename>
    friend class cIterableBaseImpl;
    template<template <typename> class,typename>
    friend class cIterable;
    template<typename,typename>
    friend class cConstIterableImpl;
    template<typename>
    friend class cBidirectionalIterableImpl;
    template<typename,typename,typename,template<typename,typename,typename>class>
    friend class cReversableBaseIterableImpl;

public:
    typedef typename cIteratorImpl<Traits>::tag tag;
	typedef typename cIteratorImpl<Traits>::raw_type raw_type;
	typedef typename cIteratorImpl<Traits>::value_type value_type;
	typedef typename cIteratorImpl<Traits>::pointer_type pointer_type;
	typedef typename cIteratorImpl<Traits>::const_pointer_type const_pointer_type;
	typedef typename cIteratorImpl<Traits>::difference_type difference_type;
	typedef typename cIteratorImpl<Traits>::reference reference;
	typedef typename cIteratorImpl<Traits>::const_reference const_reference;
	typedef typename cIteratorImpl<Traits>::reference_pointer reference_pointer;
	typedef typename cIteratorImpl<Traits>::iterator_type iterator_type;
	typedef typename cIteratorImpl<Traits>::const_iterator_type const_iterator_type;
	static const ReferenceCategory category = cIteratorImpl<Traits>::category;

	using cIteratorImpl<Traits>::operator++;
	using cIteratorImpl<Traits>::operator=;

    inline iterator_type& operator++();
    inline reference operator*();
    inline bool operator==(const cInputIteratorImpl<Traits>& other) const;
    inline bool operator!=(const cInputIteratorImpl<Traits>& other) const;
    inline pointer_type operator->();

protected:
	using cIteratorImpl<Traits>::cIteratorImpl;
};

template<typename Traits>
class cOutputIteratorImpl : public cIteratorImpl<Traits>
{
    template<typename,typename,typename>
    friend class cConstIterableBaseImpl;
    template<typename,typename,typename>
    friend class cIterableBaseImpl;
    template<template <typename> class,typename>
    friend class cIterable;
    template<typename,typename>
    friend class cConstIterableImpl;
    template<typename>
    friend class cBidirectionalIterableImpl;
    template<typename,typename,typename,template<typename,typename,typename>class>
    friend class cReversableBaseIterableImpl;

public:
    typedef typename cIteratorImpl<Traits>::tag tag;
	typedef typename cIteratorImpl<Traits>::raw_type raw_type;
	typedef typename cIteratorImpl<Traits>::value_type value_type;
	typedef typename cIteratorImpl<Traits>::pointer_type pointer_type;
	typedef typename cIteratorImpl<Traits>::const_pointer_type const_pointer_type;
	typedef typename cIteratorImpl<Traits>::difference_type difference_type;
	typedef typename cIteratorImpl<Traits>::reference reference;
	typedef typename cIteratorImpl<Traits>::const_reference const_reference;
	typedef typename cIteratorImpl<Traits>::reference_pointer reference_pointer;
	typedef typename cIteratorImpl<Traits>::iterator_type iterator_type;
	typedef typename cIteratorImpl<Traits>::const_iterator_type const_iterator_type;
	static const ReferenceCategory category = cIteratorImpl<Traits>::category;

	using cIteratorImpl<Traits>::operator++;
	using cIteratorImpl<Traits>::operator=;
	using cIteratorImpl<Traits>::operator==;
	using cIteratorImpl<Traits>::operator!=;

    inline iterator_type& operator++();
    inline iterator_type& operator*();
	inline iterator_type& operator=(const value_type&);

protected:
	using cIteratorImpl<Traits>::cIteratorImpl;
};

template<typename Traits>
class cForwardIteratorImpl : public cInputIteratorImpl<Traits>
{
    template<typename,typename,typename>
    friend class cConstIterableBaseImpl;
    template<typename,typename,typename>
    friend class cIterableBaseImpl;
    template<template <typename> class,typename>
    friend class cIterable;
    template<typename,typename>
    friend class cConstIterableImpl;
    template<typename>
    friend class cBidirectionalIterableImpl;
    template<typename,typename,typename,template<typename,typename,typename>class>
    friend class cReversableBaseIterableImpl;

public:
    typedef typename cInputIteratorImpl<Traits>::tag tag;
	typedef typename cInputIteratorImpl<Traits>::raw_type raw_type;
	typedef typename cInputIteratorImpl<Traits>::value_type value_type;
	typedef typename cInputIteratorImpl<Traits>::pointer_type pointer_type;
	typedef typename cInputIteratorImpl<Traits>::const_pointer_type const_pointer_type;
	typedef typename cInputIteratorImpl<Traits>::difference_type difference_type;
	typedef typename cInputIteratorImpl<Traits>::reference reference;
	typedef typename cInputIteratorImpl<Traits>::const_reference const_reference;
	typedef typename cInputIteratorImpl<Traits>::reference_pointer reference_pointer;
	typedef typename cInputIteratorImpl<Traits>::iterator_type iterator_type;
	typedef typename cInputIteratorImpl<Traits>::const_iterator_type const_iterator_type;
	static const ReferenceCategory category = cInputIteratorImpl<Traits>::category;

	using cInputIteratorImpl<Traits>::operator++;
	using cInputIteratorImpl<Traits>::operator=;
	using cInputIteratorImpl<Traits>::operator==;
	using cInputIteratorImpl<Traits>::operator!=;
    using  cInputIteratorImpl<Traits>::operator->;

protected:
	using cInputIteratorImpl<Traits>::cInputIteratorImpl;
};

template<typename Traits>
class cBidirectionalIteratorImpl : public cForwardIteratorImpl<Traits>
{
    template<typename,typename,typename>
    friend class cConstIterableBaseImpl;
    template<typename,typename,typename>
    friend class cIterableBaseImpl;
    template<template <typename> class,typename>
    friend class cIterable;
    template<typename,typename>
    friend class cConstIterableImpl;
    template<typename>
    friend class cBidirectionalIterableImpl;
    template<typename,typename,typename,template<typename,typename,typename>class>
    friend class cReversableBaseIterableImpl;

public:
    typedef typename cForwardIteratorImpl<Traits>::tag tag;
	typedef typename cForwardIteratorImpl<Traits>::raw_type raw_type;
	typedef typename cForwardIteratorImpl<Traits>::value_type value_type;
	typedef typename cForwardIteratorImpl<Traits>::pointer_type pointer_type;
	typedef typename cForwardIteratorImpl<Traits>::const_pointer_type const_pointer_type;
	typedef typename cForwardIteratorImpl<Traits>::difference_type difference_type;
	typedef typename cForwardIteratorImpl<Traits>::reference reference;
	typedef typename cForwardIteratorImpl<Traits>::const_reference const_reference;
	typedef typename cForwardIteratorImpl<Traits>::reference_pointer reference_pointer;
	typedef typename cForwardIteratorImpl<Traits>::iterator_type iterator_type;
	typedef typename cForwardIteratorImpl<Traits>::const_iterator_type const_iterator_type;
	static const ReferenceCategory category = cForwardIteratorImpl<Traits>::category;

	using cForwardIteratorImpl<Traits>::operator=;
	using cForwardIteratorImpl<Traits>::operator*;
	using cForwardIteratorImpl<Traits>::operator==;
	using cForwardIteratorImpl<Traits>::operator!=;
    using cForwardIteratorImpl<Traits>::operator->;

    //apply shadowing to increment operators for checking reverse condition
    inline iterator_type operator++(int);
    inline iterator_type& operator++();
    inline iterator_type& operator--();
    inline iterator_type operator--(int);

protected:
	using cForwardIteratorImpl<Traits>::cForwardIteratorImpl;
};

template<typename Traits>
class cRandomAccessIteratorImpl : public cBidirectionalIteratorImpl<Traits>
{
    template<typename,typename,typename>
    friend class cConstIterableBaseImpl;
    template<typename,typename,typename>
    friend class cIterableBaseImpl;
    template<template <typename> class,typename>
    friend class cIterable;
    template<typename,typename>
    friend class cConstIterableImpl;
    template<typename>
    friend class cBidirectionalIterableImpl;
    template<typename,typename,typename,template<typename,typename,typename>class>
    friend class cReversableBaseIterableImpl;

public:
    typedef typename cBidirectionalIteratorImpl<Traits>::tag tag;
	typedef typename cBidirectionalIteratorImpl<Traits>::raw_type raw_type;
	typedef typename cBidirectionalIteratorImpl<Traits>::value_type value_type;
	typedef typename cBidirectionalIteratorImpl<Traits>::pointer_type pointer_type;
	typedef typename cBidirectionalIteratorImpl<Traits>::const_pointer_type const_pointer_type;
	typedef typename cBidirectionalIteratorImpl<Traits>::difference_type difference_type;
	typedef typename cBidirectionalIteratorImpl<Traits>::reference reference;
	typedef typename cBidirectionalIteratorImpl<Traits>::const_reference const_reference;
	typedef typename cBidirectionalIteratorImpl<Traits>::reference_pointer reference_pointer;
	typedef typename cBidirectionalIteratorImpl<Traits>::iterator_type iterator_type;
	typedef typename cBidirectionalIteratorImpl<Traits>::const_iterator_type const_iterator_type;
	static const ReferenceCategory category = cBidirectionalIteratorImpl<Traits>::category;

	using cBidirectionalIteratorImpl<Traits>::operator++;
	using cBidirectionalIteratorImpl<Traits>::operator=;
	using cBidirectionalIteratorImpl<Traits>::operator--;
	using cBidirectionalIteratorImpl<Traits>::operator*;
	using cBidirectionalIteratorImpl<Traits>::operator==;
	using cBidirectionalIteratorImpl<Traits>::operator!=;
    using cBidirectionalIteratorImpl<Traits>::operator->;

    inline difference_type operator-(const cRandomAccessIteratorImpl<Traits>& other) const;
    inline iterator_type operator-(difference_type i_index) const;
    inline iterator_type& operator-=(difference_type i_index) const;
    inline iterator_type operator+(difference_type i_index) const;
    inline iterator_type& operator+=(difference_type i_index) const;
	inline reference operator[](size_t i_index);
	inline bool operator<(const cRandomAccessIteratorImpl<Traits>& other) const;
	inline bool operator>(const cRandomAccessIteratorImpl<Traits>& other) const;
	inline bool operator<=(const cRandomAccessIteratorImpl<Traits>& other) const;
	inline bool operator>=(const cRandomAccessIteratorImpl<Traits>& other) const;

protected:
	using cBidirectionalIteratorImpl<Traits>::cBidirectionalIteratorImpl;
};

template<typename T,typename IterableInterface,ReferenceCategory>
using cAgnosticInputIterator = impl::cInputIteratorImpl<impl::IteratorTraits<T,impl::cInputIteratorImpl,IterableInterface,InputIteratorTag,ReferenceCategory::ConstReference>>;

template<typename T,typename IterableInterface,ReferenceCategory>
using cAgnosticOutputIterator = impl::cOutputIteratorImpl<impl::IteratorTraits<T,impl::cOutputIteratorImpl,IterableInterface,OutputIteratorTag,ReferenceCategory::NonConstReference>>;

template<typename T, typename IterableInterface,ReferenceCategory category>
using cAgnosticForwardIterator = impl::cForwardIteratorImpl<impl::IteratorTraits<T,impl::cForwardIteratorImpl,IterableInterface,ForwardIteratorTag,category>>;

template<typename T, typename IterableInterface,ReferenceCategory category>
using cAgnosticBidirectionalIterator = impl::cBidirectionalIteratorImpl<impl::IteratorTraits<T,impl::cBidirectionalIteratorImpl,IterableInterface,BidirectionalIteratorTag,category>>;

template<typename T, typename IterableInterface,ReferenceCategory category>
using cAgnosticRandomAccessIterator = impl::cRandomAccessIteratorImpl<impl::IteratorTraits<T,impl::cRandomAccessIteratorImpl,IterableInterface,RandomAccessIteratorTag,category>>;

template<typename T, typename IterableInterface,ReferenceCategory category>
using cReverseAgnosticBidirectionalIterator = impl::cBidirectionalIteratorImpl<impl::IteratorTraits<T,impl::cBidirectionalIteratorImpl,IterableInterface,ReverseBidirectionalIteratorTag,category>>;

template<typename T, typename IterableInterface,ReferenceCategory category>
using cReverseAgnosticRandomAccessIterator = impl::cRandomAccessIteratorImpl<impl::IteratorTraits<T,impl::cRandomAccessIteratorImpl,IterableInterface,ReverseRandomAccessIteratorTag,category>>;

}

template<typename T>
using cInputIterator = impl::cAgnosticInputIterator<T,cInputIterableBasePrivate<impl::AgnosticIteratorTraits<T,ReferenceCategory::ConstReference>>,ReferenceCategory::ConstReference>;
template<typename T>
using cOutputIterator = impl::cAgnosticOutputIterator<T,cOutputIterableBasePrivate<impl::AgnosticIteratorTraits<T,ReferenceCategory::NonConstReference>>,ReferenceCategory::NonConstReference>;
template<typename T>
using cForwardIterator = impl::cAgnosticForwardIterator<T,cForwardIterableBasePrivate<impl::AgnosticIteratorTraits<T,deduceCategoryFromType<T>::category>>,deduceCategoryFromType<T>::category>;
template<typename T>
using cBidirectionalIterator = impl::cAgnosticBidirectionalIterator<T,cBidirectionalIterableBasePrivate<impl::AgnosticIteratorTraits<T,deduceCategoryFromType<T>::category>>,deduceCategoryFromType<T>::category>;
template<typename T>
using cRandomAccessIterator = impl::cAgnosticRandomAccessIterator<T,cRandomAccessIterableBasePrivate<impl::AgnosticIteratorTraits<T,deduceCategoryFromType<T>::category>>,deduceCategoryFromType<T>::category>;

template<typename T>
using cReverseBidirectionalIterator = impl::cReverseAgnosticBidirectionalIterator<T,cBidirectionalIterableBasePrivate<impl::AgnosticIteratorTraits<T,deduceCategoryFromType<T>::category>>,deduceCategoryFromType<T>::category>;
template<typename T>
using cReverseRandomAccessIterator = impl::cReverseAgnosticRandomAccessIterator<T,cRandomAccessIterableBasePrivate<impl::AgnosticIteratorTraits<T,deduceCategoryFromType<T>::category>>,deduceCategoryFromType<T>::category>;

}
}
}

#include "detail/cIterator.tpp"
