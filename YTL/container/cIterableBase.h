
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

#include "YTL/functional/cFunction.h"
#include "YTL/container/cIterator.h"
#include "YTL/types/smartPtr/enable_ref_from_this.h"

namespace yame
{
namespace mpl
{
template<typename T>
struct AddType
{
    typedef typename static_if<is_pointer<T>::value == false && is_copy_constructible<T>::value, typename add_reference<typename add_constness<T>::type>::type, typename add_rreference<T>::type>::type type;
};
}

namespace container
{
namespace detail
{
//public interface

template<typename Iterator>
class cConstIterableBase
{
public:
    typedef Iterator iterator_type;
    typedef typename iterator_type::const_iterator_type const_iterator_type;
    typedef typename iterator_type::raw_type raw_type;
    typedef typename iterator_type::value_type value_type;
    typedef typename iterator_type::pointer_type pointer_type;
    typedef typename iterator_type::reference reference;
    typedef typename iterator_type::const_reference const_reference;
    static const bool is_const = true;

    virtual ~cConstIterableBase() = default;
    virtual iterator_type begin(const ytl::function<bool(const_reference)>& filter = null_ptr) = 0;
    virtual iterator_type end() = 0;
    virtual const_iterator_type cbegin(const ytl::function<bool(const_reference)>& filter = null_ptr) const = 0;
    virtual const_iterator_type cend() const = 0;
    virtual size_t getSize() const = 0;
    virtual bool empty() const = 0;
    virtual ytl::enable_ref_from_this& ref_from_this() = 0;
    virtual const ytl::enable_ref_from_this& ref_from_this() const = 0;
};

template<typename Iterator>
class cIterableBase : virtual public cConstIterableBase<Iterator>
{
public:
    typedef typename cConstIterableBase<Iterator>::iterator_type iterator_type;
    typedef typename cConstIterableBase<Iterator>::const_iterator_type const_iterator_type;
    typedef typename cConstIterableBase<Iterator>::raw_type raw_type;
    typedef typename cConstIterableBase<Iterator>::value_type value_type;
    typedef typename cConstIterableBase<Iterator>::pointer_type pointer_type;
    typedef typename cConstIterableBase<Iterator>::reference reference;
    typedef typename cConstIterableBase<Iterator>::const_reference const_reference;
    typedef typename mpl::AddType<value_type>::type add_type;
    static const bool is_const = false;

    using cConstIterableBase<Iterator>::begin;
    using cConstIterableBase<Iterator>::end;
    using cConstIterableBase<Iterator>::cbegin;
    using cConstIterableBase<Iterator>::cend;
    using cConstIterableBase<Iterator>::getSize;
    using cConstIterableBase<Iterator>::empty;

    virtual iterator_type erase(iterator_type i_it) = 0;
    virtual iterator_type add(add_type i_value) = 0;
};

template<typename Iterator, typename ReverseIterator>
class cConstReversableIterableInterface : virtual public cConstIterableBase<Iterator>
{
public:
    typedef ReverseIterator reverse_iterator_type;
    typedef typename ReverseIterator::const_iterator_type const_reverse_iterator_type;
    typedef typename cConstIterableBase<Iterator>::iterator_type iterator_type;
    typedef typename cConstIterableBase<Iterator>::const_iterator_type const_iterator_type;
    typedef typename cConstIterableBase<Iterator>::raw_type raw_type;
    typedef typename cConstIterableBase<Iterator>::value_type value_type;
    typedef typename cConstIterableBase<Iterator>::pointer_type pointer_type;
    typedef typename cConstIterableBase<Iterator>::reference reference;
    typedef typename cConstIterableBase<Iterator>::const_reference const_reference;
    typedef ytl::enable_ref_from_this::lendable_tag lendable_tag;

    using cConstIterableBase<Iterator>::begin;
    using cConstIterableBase<Iterator>::end;
    using cConstIterableBase<Iterator>::cbegin;
    using cConstIterableBase<Iterator>::cend;
    using cConstIterableBase<Iterator>::getSize;
    using cConstIterableBase<Iterator>::empty;

    virtual reverse_iterator_type rbegin(const ytl::function<bool(const_reference)>& filter = null_ptr) = 0;
    virtual reverse_iterator_type rend() = 0;
    virtual const_reverse_iterator_type crbegin(const ytl::function<bool(const_reference)>& filter = null_ptr) const = 0;
    virtual const_reverse_iterator_type crend() const = 0;
};

template<typename Iterator, typename ReverseIterator>
class cReversableIterableInterface : public cConstReversableIterableInterface<Iterator,ReverseIterator>, virtual public cIterableBase<Iterator>
{
public:
    typedef typename cConstReversableIterableInterface<Iterator,ReverseIterator>::reverse_iterator_type reverse_iterator_type;
    typedef typename cConstReversableIterableInterface<Iterator,ReverseIterator>::const_reverse_iterator_type const_reverse_iterator_type;
    typedef typename cConstReversableIterableInterface<Iterator,ReverseIterator>::iterator_type iterator_type;
    typedef typename cConstReversableIterableInterface<Iterator,ReverseIterator>::const_iterator_type const_iterator_type;
    typedef typename cConstReversableIterableInterface<Iterator,ReverseIterator>::raw_type raw_type;
    typedef typename cConstReversableIterableInterface<Iterator,ReverseIterator>::value_type value_type;
    typedef typename cConstReversableIterableInterface<Iterator,ReverseIterator>::pointer_type pointer_type;
    typedef typename cConstReversableIterableInterface<Iterator,ReverseIterator>::reference reference;
    typedef typename cConstReversableIterableInterface<Iterator,ReverseIterator>::const_reference const_reference;
    typedef typename cIterableBase<Iterator>::add_type add_type;
    typedef typename cConstReversableIterableInterface<Iterator,ReverseIterator>::lendable_tag lendable_tag;

    using cConstReversableIterableInterface<Iterator,ReverseIterator>::begin;
    using cConstReversableIterableInterface<Iterator,ReverseIterator>::end;
    using cConstReversableIterableInterface<Iterator,ReverseIterator>::cbegin;
    using cConstReversableIterableInterface<Iterator,ReverseIterator>::cend;
    using cConstReversableIterableInterface<Iterator,ReverseIterator>::getSize;
    using cConstReversableIterableInterface<Iterator,ReverseIterator>::empty;
    using cIterableBase<Iterator>::add;
    using cIterableBase<Iterator>::erase;

    virtual reverse_iterator_type rbegin(const ytl::function<bool(const_reference)>& filter = null_ptr) = 0;
    virtual reverse_iterator_type rend() = 0;
    virtual const_reverse_iterator_type crbegin(const ytl::function<bool(const_reference)>& filter = null_ptr) const = 0;
    virtual const_reverse_iterator_type crend() const = 0;
};

}

//public interfaces

//const iterables
template<typename T>
using cConstInputIterable = detail::cConstIterableBase<detail::cInputIterator<T>>;
template<typename T>
using cConstOutputIterable = detail::cConstIterableBase<detail::cOutputIterator<T>>;
template<typename T>
using cConstForwardIterable = detail::cConstIterableBase<detail::impl::cAgnosticForwardIterator<T,detail::cForwardIterableBasePrivate<detail::impl::AgnosticIteratorTraits<T,detail::deduceCategoryFromType<T>::category>>,detail::deduceCategoryFromType<T>::category>>;
template<typename T>
using cConstBidirectionalIterable = detail::cConstReversableIterableInterface<detail::impl::cAgnosticBidirectionalIterator<T,detail::cBidirectionalIterableBasePrivate<detail::impl::AgnosticIteratorTraits<T,detail::deduceCategoryFromType<T>::category>>,detail::deduceCategoryFromType<T>::category>,detail::cReverseBidirectionalIterator<T>>;
template<typename T>
using cConstRandomAccessIterable = detail::cConstReversableIterableInterface<detail::impl::cAgnosticRandomAccessIterator<T,detail::cRandomAccessIterableBasePrivate<detail::impl::AgnosticIteratorTraits<T,detail::deduceCategoryFromType<T>::category>>,detail::deduceCategoryFromType<T>::category>,detail::cReverseRandomAccessIterator<T>>;

//non const iterables
template<typename T>
using cInputIterable = detail::cIterableBase<detail::cInputIterator<T>>;
template<typename T>
using cOutputIterable = detail::cIterableBase<detail::cOutputIterator<T>>;
template<typename T>
using cForwardIterable = detail::cIterableBase<detail::impl::cAgnosticForwardIterator<T,detail::cForwardIterableBasePrivate<detail::impl::AgnosticIteratorTraits<T,detail::deduceCategoryFromType<T>::category>>,detail::deduceCategoryFromType<T>::category>>;
template<typename T>
using cBidirectionalIterable = detail::cReversableIterableInterface<detail::impl::cAgnosticBidirectionalIterator<T,detail::cBidirectionalIterableBasePrivate<detail::impl::AgnosticIteratorTraits<T,detail::deduceCategoryFromType<T>::category>>,detail::deduceCategoryFromType<T>::category>,detail::cReverseBidirectionalIterator<T>>;
template<typename T>
using cRandomAccessIterable = detail::cReversableIterableInterface<detail::impl::cAgnosticRandomAccessIterator<T,detail::cRandomAccessIterableBasePrivate<detail::impl::AgnosticIteratorTraits<T,detail::deduceCategoryFromType<T>::category>>,detail::deduceCategoryFromType<T>::category>,detail::cReverseRandomAccessIterator<T>>;


namespace detail
{

template<typename To,typename From>
inline To sendNodeToIterator(From i_from);
template<typename To,typename From>
inline To receiveNodeFromIterator(From i_from);

//private interfaces
template<typename Traits>
class cIterableBasePrivate
{
public:
    typedef typename Traits::raw_type raw_type;
    typedef typename Traits::value_type value_type;
    typedef typename Traits::pointer_type pointer_type;
    typedef typename Traits::reference reference;
    typedef typename Traits::const_reference const_reference;
    typedef typename Traits::node_pointer_type node_pointer_type;
    static const ReferenceCategory category = Traits::category;
    typedef ytl::enable_ref_from_this::lendable_tag lendable_tag;

    virtual ~cIterableBasePrivate() = default;
    virtual node_pointer_type getFirstElem() const = 0;
    virtual node_pointer_type getNextElem(node_pointer_type currNode) const = 0;
    virtual void releaseNode(node_pointer_type i_node) const = 0;
    virtual node_pointer_type acquireNode(node_pointer_type i_node) const = 0;
    virtual const_reference getValue(node_pointer_type currNode) const = 0;
    virtual reference getValue(node_pointer_type currNode) = 0;
    virtual ytl::enable_ref_from_this& ref_from_this() = 0;
    virtual const ytl::enable_ref_from_this& ref_from_this() const = 0;
};

template<typename Traits>
class cInputIterableBasePrivate : public cIterableBasePrivate<Traits>
{
public:
    typedef typename cIterableBasePrivate<Traits>::raw_type raw_type;
    typedef typename cIterableBasePrivate<Traits>::value_type value_type;
    typedef typename cIterableBasePrivate<Traits>::pointer_type pointer_type;
    typedef typename cIterableBasePrivate<Traits>::reference reference;
    typedef typename cIterableBasePrivate<Traits>::const_reference const_reference;
    typedef typename cIterableBasePrivate<Traits>::node_pointer_type node_pointer_type;
    typedef typename cIterableBasePrivate<Traits>::lendable_tag lendable_tag;
    static const ReferenceCategory category = cIterableBasePrivate<Traits>::category;
};

template<typename Traits>
class cOutputIterableBasePrivate : public cIterableBasePrivate<Traits>
{
public:
    typedef typename cIterableBasePrivate<Traits>::raw_type raw_type;
    typedef typename cIterableBasePrivate<Traits>::value_type value_type;
    typedef typename cIterableBasePrivate<Traits>::pointer_type pointer_type;
    typedef typename cIterableBasePrivate<Traits>::reference reference;
    typedef typename cIterableBasePrivate<Traits>::const_reference const_reference;
    typedef typename cIterableBasePrivate<Traits>::node_pointer_type node_pointer_type;
    typedef typename cIterableBasePrivate<Traits>::lendable_tag lendable_tag;

    virtual void setValue(node_pointer_type node, const value_type& value) const = 0;
};

template<typename Traits>
class cForwardIterableBasePrivate : public cInputIterableBasePrivate<Traits>
{
public:
    typedef typename cInputIterableBasePrivate<Traits>::raw_type raw_type;
    typedef typename cInputIterableBasePrivate<Traits>::value_type value_type;
    typedef typename cInputIterableBasePrivate<Traits>::pointer_type pointer_type;
    typedef typename cInputIterableBasePrivate<Traits>::reference reference;
    typedef typename cInputIterableBasePrivate<Traits>::const_reference const_reference;
    typedef typename cInputIterableBasePrivate<Traits>::lendable_tag lendable_tag;
    static const ReferenceCategory category = cInputIterableBasePrivate<Traits>::category;
    typedef typename cIterableBasePrivate<Traits>::node_pointer_type node_pointer_type;
};

template<typename Traits>
class cBidirectionalIterableBasePrivate : public cForwardIterableBasePrivate<Traits>
{
public:
    typedef typename cForwardIterableBasePrivate<Traits>::raw_type raw_type;
    typedef typename cForwardIterableBasePrivate<Traits>::value_type value_type;
    typedef typename cForwardIterableBasePrivate<Traits>::pointer_type pointer_type;
    typedef typename cForwardIterableBasePrivate<Traits>::reference reference;
    typedef typename cForwardIterableBasePrivate<Traits>::const_reference const_reference;
    typedef typename cForwardIterableBasePrivate<Traits>::lendable_tag lendable_tag;
    static const ReferenceCategory category = cForwardIterableBasePrivate<Traits>::category;
    typedef typename cIterableBasePrivate<Traits>::node_pointer_type node_pointer_type;

    virtual node_pointer_type getPrevElem(node_pointer_type currNode) const = 0;
    virtual node_pointer_type getLastElem() const = 0;
};

template<typename Traits>
class cRandomAccessIterableBasePrivate : public cBidirectionalIterableBasePrivate<Traits>
{
public:
    typedef typename cBidirectionalIterableBasePrivate<Traits>::raw_type raw_type;
    typedef typename cBidirectionalIterableBasePrivate<Traits>::value_type value_type;
    typedef typename cBidirectionalIterableBasePrivate<Traits>::pointer_type pointer_type;
    typedef typename cBidirectionalIterableBasePrivate<Traits>::reference reference;
    typedef typename cBidirectionalIterableBasePrivate<Traits>::const_reference const_reference;
    typedef typename cBidirectionalIterableBasePrivate<Traits>::lendable_tag lendable_tag;
    static const ReferenceCategory category = cBidirectionalIterableBasePrivate<Traits>::category;
    typedef typename cIterableBasePrivate<Traits>::node_pointer_type node_pointer_type;

    virtual size_t getIndexOfNode(node_pointer_type node) const = 0;
    virtual node_pointer_type shiftNodeByIndex(node_pointer_type node, int val) const = 0;
};

//const iterable implementation
template<typename PrivateInterface, typename PublicInterface, typename Iterator>
class cConstIterableBaseImpl : protected PrivateInterface, public PublicInterface, public ytl::enable_ref_from_this
{
    template<typename,typename>
    friend class cConstIterableImpl;
    template<size_t,typename,size_t>
    friend class cIterableImplBackEnd;

public:
    typedef PrivateInterface iterable_private_interface;
    typedef PublicInterface iterable_public_interface;
    typedef typename Iterator::raw_type raw_type;
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::pointer_type pointer_type;
    typedef typename Iterator::const_pointer_type const_pointer_type;
    typedef typename Iterator::node_pointer_type node_pointer_type;
    typedef typename Iterator::reference reference;
    typedef typename Iterator::const_reference const_reference;
    typedef typename Iterator::iterator_type iterator_type;
    typedef typename Iterator::const_iterator_type const_iterator_type;
    typedef ytl::enable_ref_from_this::lendable_tag lendable_tag;

    cConstIterableBaseImpl() = default;
    cConstIterableBaseImpl(const cConstIterableBaseImpl&) = delete;

    //public accessors
    iterator_type begin(const ytl::function<bool(const_reference)>& filter = null_ptr) override;
    iterator_type end() override;
    const_iterator_type cbegin(const ytl::function<bool(const_reference)>& filter = null_ptr) const override;
    const_iterator_type cend() const override;
    ytl::enable_ref_from_this& ref_from_this() override;
    const ytl::enable_ref_from_this& ref_from_this() const override;

    size_t getSize() const override;
    bool empty() const override;
    static inline node_pointer_type getHead();

protected:
    //modifiers
    inline void setRootNode(node_pointer_type i_newRoot);
    inline iterator_type constructIteratorAtNode(node_pointer_type i_node, const ytl::function<bool(const_reference)>& filter = null_ptr);
    inline const_iterator_type constructConstIteratorAtNode(node_pointer_type i_node, const ytl::function<bool(const_reference)>& filter = null_ptr) const;
    node_pointer_type acquireNode(node_pointer_type i_node) const override;
    void releaseNode(node_pointer_type i_node) const override;

    //protected accessors
    inline node_pointer_type getRootNode() const;
    inline node_pointer_type getCurrentNode(const iterator_type& i_it);
    inline node_pointer_type getCurrentNode(const const_iterator_type& i_it) const;
    template<typename T>
    inline typename mpl::add_pointer<T>::type getTypedRootNode() const;
    template<typename T>
    inline typename mpl::add_pointer<T>::type getTypedCurrentNode(const iterator_type& i_it);
    template<typename T>
    inline typename mpl::add_pointer<T>::type getTypedCurrentNode(const const_iterator_type& i_it) const;
    operator PrivateInterface&() const;

protected:
    static node_pointer_type const m_pHead;
    node_pointer_type m_pRoot = m_pHead;
};

template<typename NodePointerType, typename AddType>
class cNonConstIterableInterface
{
public:
	virtual void eraseNode(NodePointerType) = 0;
	virtual NodePointerType addNode(AddType) = 0;
};

//non const iterable impl
template<typename PrivateInterface, typename PublicInterface, typename Iterator>
class cIterableBaseImpl : public cNonConstIterableInterface<typename cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::node_pointer_type, typename PublicInterface::add_type>, public cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>
{
    template<template <typename> class Iterable, typename  Traits>
    friend class cIterable;

public:
    typedef typename cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::iterable_private_interface iterable_private_interface;
    typedef typename cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::iterable_public_interface iterable_public_interface;
    typedef typename cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::raw_type raw_type;
    typedef typename cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::value_type value_type;
    typedef typename cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::pointer_type pointer_type;
    typedef typename cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::const_pointer_type const_pointer_type;
    typedef typename cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::node_pointer_type node_pointer_type;
    typedef typename cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::reference reference;
    typedef typename cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::const_reference const_reference;
    typedef typename cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::iterator_type iterator_type;
    typedef typename cConstIterableBaseImpl<PrivateInterface,PublicInterface,Iterator>::const_iterator_type const_iterator_type;
    typedef typename PublicInterface::add_type add_type;

    cIterableBaseImpl() = default;
    cIterableBaseImpl(const cIterableBaseImpl&) = delete;

    iterator_type erase(iterator_type i_it) override;
    iterator_type add(add_type i_value) override;
};

template<typename PrivateInterface, typename PublicInterface, typename Iterator, template<typename,typename,typename> class Base>
class cReversableBaseIterableImpl : public Base<PrivateInterface,PublicInterface,Iterator>
{
public:
    typedef typename PublicInterface::reverse_iterator_type reverse_iterator_type;
    typedef typename PublicInterface::const_reverse_iterator_type const_reverse_iterator_type;
    typedef typename Base<PrivateInterface,PublicInterface,Iterator>::iterable_private_interface iterable_private_interface;
    typedef typename Base<PrivateInterface,PublicInterface,Iterator>::iterable_public_interface iterable_public_interface;
    typedef typename Base<PrivateInterface,PublicInterface,Iterator>::raw_type raw_type;
    typedef typename Base<PrivateInterface,PublicInterface,Iterator>::value_type value_type;
    typedef typename Base<PrivateInterface,PublicInterface,Iterator>::pointer_type pointer_type;
    typedef typename Base<PrivateInterface,PublicInterface,Iterator>::const_pointer_type const_pointer_type;
    typedef typename Base<PrivateInterface,PublicInterface,Iterator>::node_pointer_type node_pointer_type;
    typedef typename Base<PrivateInterface,PublicInterface,Iterator>::reference reference;
    typedef typename Base<PrivateInterface,PublicInterface,Iterator>::const_reference const_reference;
    typedef typename Base<PrivateInterface,PublicInterface,Iterator>::iterator_type iterator_type;
    typedef typename Base<PrivateInterface,PublicInterface,Iterator>::const_iterator_type const_iterator_type;

    reverse_iterator_type rbegin(const ytl::function<bool(const_reference)>& filter = null_ptr) override;
    reverse_iterator_type rend() override;
    const_reverse_iterator_type crbegin(const ytl::function<bool(const_reference)>& filter = null_ptr) const override;
    const_reverse_iterator_type crend() const override;
};

//these are the impl to derive from in case you want to create your own data structure and being able yo iterate from it

//const iterable impl
template<typename ViewType>
using cConstInputIterableBaseImpl = cConstIterableBaseImpl<cInputIterableBasePrivate<impl::AgnosticIteratorTraits<ViewType,ReferenceCategory::ConstReference>>,cConstInputIterable<ViewType>,detail::cInputIterator<ViewType>>;
template<typename ViewType>
using cConstOutputIterableBaseImpl = cConstIterableBaseImpl<cOutputIterableBasePrivate<impl::AgnosticIteratorTraits<ViewType,ReferenceCategory::NonConstReference>>,cConstOutputIterable<ViewType>,detail::cOutputIterator<ViewType>>;
template<typename ViewType>
using cConstForwardIterableBaseImpl = cConstIterableBaseImpl<cForwardIterableBasePrivate<impl::AgnosticIteratorTraits<ViewType,detail::deduceCategoryFromType<ViewType>::category>>,cConstForwardIterable<ViewType>,detail::impl::cAgnosticForwardIterator<ViewType,cForwardIterableBasePrivate<impl::AgnosticIteratorTraits<ViewType,detail::deduceCategoryFromType<ViewType>::category>>,detail::deduceCategoryFromType<ViewType>::category>>;
template<typename ViewType>
using cConstBidirectionalIterableBaseImpl = cReversableBaseIterableImpl<cBidirectionalIterableBasePrivate<impl::AgnosticIteratorTraits<ViewType,detail::deduceCategoryFromType<ViewType>::category>>,cConstBidirectionalIterable<ViewType>,detail::impl::cAgnosticBidirectionalIterator<ViewType,cBidirectionalIterableBasePrivate<impl::AgnosticIteratorTraits<ViewType,detail::deduceCategoryFromType<ViewType>::category>>,detail::deduceCategoryFromType<ViewType>::category>,cConstIterableBaseImpl>;
template<typename ViewType>
using cConstRandomAccessIterableBaseImpl = cReversableBaseIterableImpl<cRandomAccessIterableBasePrivate<impl::AgnosticIteratorTraits<ViewType,detail::deduceCategoryFromType<ViewType>::category>>,cConstRandomAccessIterable<ViewType>,detail::impl::cAgnosticRandomAccessIterator<ViewType,cRandomAccessIterableBasePrivate<impl::AgnosticIteratorTraits<ViewType,detail::deduceCategoryFromType<ViewType>::category>>,detail::deduceCategoryFromType<ViewType>::category>,cConstIterableBaseImpl>;

//non const iterable impl
template<typename ViewType>
using cInputIterableBaseImpl = cIterableBaseImpl<cInputIterableBasePrivate<impl::AgnosticIteratorTraits<ViewType,ReferenceCategory::ConstReference>>,cInputIterable<ViewType>,detail::cInputIterator<ViewType>>;
template<typename ViewType>
using cOutputIterableBaseImpl = cIterableBaseImpl<cOutputIterableBasePrivate<impl::AgnosticIteratorTraits<ViewType,ReferenceCategory::NonConstReference>>,cOutputIterable<ViewType>, detail::cOutputIterator<ViewType>>;
template<typename ViewType>
using cForwardIterableBaseImpl = cIterableBaseImpl<cForwardIterableBasePrivate<impl::AgnosticIteratorTraits<ViewType,detail::deduceCategoryFromType<ViewType>::category>>,cForwardIterable<ViewType>,detail::impl::cAgnosticForwardIterator<ViewType,cForwardIterableBasePrivate<impl::AgnosticIteratorTraits<ViewType,detail::deduceCategoryFromType<ViewType>::category>>,detail::deduceCategoryFromType<ViewType>::category>>;
template<typename ViewType>
using cBidirectionalIterableBaseImpl = cReversableBaseIterableImpl<cBidirectionalIterableBasePrivate<impl::AgnosticIteratorTraits<ViewType,detail::deduceCategoryFromType<ViewType>::category>>,cBidirectionalIterable<ViewType>,detail::impl::cAgnosticBidirectionalIterator<ViewType,cBidirectionalIterableBasePrivate<impl::AgnosticIteratorTraits<ViewType,detail::deduceCategoryFromType<ViewType>::category>>,detail::deduceCategoryFromType<ViewType>::category>,cIterableBaseImpl>;
template<typename ViewType>
using cRandomAccessIterableBaseImpl = cReversableBaseIterableImpl<cRandomAccessIterableBasePrivate<impl::AgnosticIteratorTraits<ViewType,detail::deduceCategoryFromType<ViewType>::category>>,cRandomAccessIterable<ViewType>,detail::impl::cAgnosticRandomAccessIterator<ViewType,cRandomAccessIterableBasePrivate<impl::AgnosticIteratorTraits<ViewType,detail::deduceCategoryFromType<ViewType>::category>>,detail::deduceCategoryFromType<ViewType>::category>,cIterableBaseImpl>;

//private interfaces
template<typename T>
using cInputIterablePrivate = detail::cInputIterableBasePrivate<detail::impl::AgnosticIteratorTraits<T,detail::ReferenceCategory::ConstReference>>;
template<typename T>
using cOutputIterablePrivate = detail::cOutputIterableBasePrivate<detail::impl::AgnosticIteratorTraits<T,detail::ReferenceCategory::NonConstReference>>;
template<typename T>
using cForwardIterablePrivate = detail::cForwardIterableBasePrivate<detail::impl::AgnosticIteratorTraits<T,detail::deduceCategoryFromType<T>::category>>;
template<typename T>
using cBidirectionalIterablePrivate = detail::cBidirectionalIterableBasePrivate<detail::impl::AgnosticIteratorTraits<T,detail::deduceCategoryFromType<T>::category>>;
template<typename T>
using cRandomAccessIterablePrivate = detail::cRandomAccessIterableBasePrivate<detail::impl::AgnosticIteratorTraits<T,detail::deduceCategoryFromType<T>::category>>;

}
}
}

#include "YTL/container/detail/cIterableBase.tpp"
