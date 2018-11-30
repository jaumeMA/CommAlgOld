
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

#include "System/memory/cMemoryPool.h"
#include "YTL/mpl/cTemplateHelper.h"
#include "YTL/container/cIterableBase.h"
#include "YTL/container/detail/cIterableImplBackEnd.h"

namespace yame
{
namespace container
{
namespace detail
{

extern const memory::cMemoryPool& k_pairAllocator;

template<typename,typename>
class cConstIterableImpl;

template<typename Traits, int ... indexs>
class cConstIterableImpl<Traits,mpl::sequence<indexs...>> : public cIterableImplBackEnd<Traits::numIterables,Traits,Traits::numIterables-1>, protected Traits::forwarded_iterable_private_interface, public Traits::forwarded_iterable_public_interface, public ytl::enable_ref_from_this
{
    template<typename,typename>
    friend class cConstIterableImpl;

    template<typename,typename...>
    struct iterable_circular_dep_compliant;

    template<typename Foo>
    struct iterable_circular_dep_compliant<Foo>
    {
        static bool checkCircularDep(const typename Traits::forwarded_iterable_private_interface& thisObject)
        {
            return false;
        }
    };

    template<typename Foo, typename Iterable,typename ... Iterables>
    struct iterable_circular_dep_compliant<Foo,Iterable,Iterables...>
    {
        static bool checkCircularDep(const typename Traits::forwarded_iterable_private_interface& thisObject, const Iterable& i_iterable, const Iterables& ... i_iterables)
        {
            return static_cast<const void *>(&thisObject) == static_cast<const void*>(&i_iterable) || iterable_circular_dep_compliant<Foo,Iterables...>::checkCircularDep(thisObject, i_iterables...);
        }
    };

    template<typename,typename...>
    struct iterable_iterator_compliant;

    template<typename Foo>
    struct iterable_iterator_compliant<Foo>
    {
        static const bool value = true;
    };

    template<typename Foo, typename Iterator,typename ... Iterators>
    struct iterable_iterator_compliant<Foo,Iterator,Iterators...>
    {
        static const bool value = mpl::is_base_of<typename Traits::iterator_type, Iterator>::value
                                    && iterable_iterator_compliant<Foo,Iterators...>::value;
    };

    template<typename Foo,ReferenceCategory...>
    struct iterable_category_compliant;

    template<typename Foo>
    struct iterable_category_compliant<Foo>
    {
        static const bool value = true;
    };

    template<typename Foo,ReferenceCategory category,ReferenceCategory ... categories>
    struct iterable_category_compliant<Foo,category,categories...>
    {
        static const bool value = (Traits::forwarded_category == ReferenceCategory::Value ||
                                    (Traits::forwarded_category == ReferenceCategory::ConstReference && category != ReferenceCategory::Value) ||
                                    (Traits::forwarded_category == ReferenceCategory::NonConstReference && category == ReferenceCategory::NonConstReference))
                                    && iterable_category_compliant<Foo,categories...>::value;
    };

    template<typename Foo,typename ... Iterables>
    struct iterable_constness_compliant;

    template<typename Foo>
    struct iterable_constness_compliant<Foo>
    {
        static const bool value = true;
    };

    template<typename Foo,typename Iterable,typename ... Iterables>
    struct iterable_constness_compliant<Foo,Iterable,Iterables...>
    {
        static const bool value = (Traits::iterable_public_interface::is_const == true || Iterable::is_const == false)
                                    && iterable_constness_compliant<Foo,Iterables...>::value;
    };

protected:
	typedef typename Traits::iterable_private_interface iterable_private_interface;
	typedef typename Traits::forwarded_iterable_private_interface forwarded_iterable_private_interface;
	typedef typename Traits::iterable_public_interface iterable_public_interface;
	typedef typename Traits::forwarded_iterable_public_interface forwarded_iterable_public_interface;
    typedef typename Traits::iterator_type iterator_type;
    typedef typename Traits::const_iterator_type const_iterator_type;
    typedef typename Traits::forwarded_iterator_type forwarded_iterator_type;
    typedef typename Traits::const_forwarded_iterator_type const_forwarded_iterator_type;
	typedef typename Traits::value_type value_type;
	typedef typename Traits::pointer_type pointer_type;
	typedef typename Traits::node_pointer_type node_pointer_type;
	typedef typename Traits::forwarded_type forwarded_type;
	typedef typename Traits::forwarded_pointer_type forwarded_pointer_type;
	typedef typename Traits::forwarded_node_pointer_type forwarded_node_pointer_type;
	typedef typename Traits::forwarded_reference forwarded_reference;
	typedef typename Traits::const_forwarded_reference const_forwarded_reference;

public:
    template<typename ... Iterables>
    cConstIterableImpl(const Iterables& ... i_iterables);

    //public interface implementation
    forwarded_iterator_type begin(const ytl::function<bool(const_forwarded_reference)>& filter = null_ptr) override;
    forwarded_iterator_type end() override;
    const_forwarded_iterator_type cbegin(const ytl::function<bool(const_forwarded_reference)>& filter = null_ptr) const override;
    const_forwarded_iterator_type cend() const override;
    size_t getSize() const override;
    bool empty() const override;

protected:
    typedef typename cIterableImplBackEnd<Traits::numIterables,Traits,Traits::numIterables-1>::nested_iterable_interface nested_iterable_interface;

    struct pair_node
    {
        nested_iterable_interface* backendInterface;
        node_pointer_type innerNode;

        pair_node(nested_iterable_interface* _backendInterface, node_pointer_type _innerNode) : backendInterface(_backendInterface), innerNode(_innerNode){}
    };

    //private interface implementation
    inline static forwarded_node_pointer_type createPair(const nested_iterable_interface* i_interface, node_pointer_type i_pointer);
    inline static void deletePair(pair_node* i_pointer);

    forwarded_node_pointer_type getFirstElem() const override;
    forwarded_node_pointer_type getNextElem(forwarded_node_pointer_type currNode) const override;
    forwarded_node_pointer_type acquireNode(forwarded_node_pointer_type i_node) const override;
    void releaseNode(forwarded_node_pointer_type i_node) const override;
    virtual node_pointer_type getNextElemInCurrentIterable(const nested_iterable_interface* currIterableBackEnd, node_pointer_type currNode) const;
    ytl::enable_ref_from_this& ref_from_this() override;
    const ytl::enable_ref_from_this& ref_from_this() const override;
};

template<typename Traits, typename Sequence>
class cConstIterableWithTransformImpl;

template<typename Traits, int ... indexs>
class cConstIterableWithTransformImpl<Traits,mpl::sequence<indexs...>> : public cConstIterableImpl<Traits,mpl::sequence<indexs...>>
{
protected:
	typedef typename Traits::forwarded_iterable_private_interface forwarded_iterable_private_interface;
	typedef typename Traits::iterable_public_interface iterable_public_interface;
	typedef typename Traits::forwarded_iterable_public_interface forwarded_iterable_public_interface;
    typedef typename Traits::iterator_type iterator_type;
    typedef typename Traits::const_iterator_type const_iterator_type;
    typedef typename Traits::forwarded_iterator_type forwarded_iterator_type;
    typedef typename Traits::const_forwarded_iterator_type const_forwarded_iterator_type;
	typedef typename Traits::raw_type raw_type;
	typedef typename Traits::value_type value_type;
	typedef typename Traits::reference reference;
	typedef typename Traits::const_reference const_reference;
	typedef typename Traits::forwarded_raw_type forwarded_raw_type;
	typedef typename Traits::forwarded_type forwarded_type;
	typedef typename Traits::pointer_type pointer_type;
	typedef typename Traits::forwarded_pointer_type forwarded_pointer_type;
	typedef typename Traits::forwarded_node_pointer_type forwarded_node_pointer_type;
	typedef typename Traits::forwarded_reference forwarded_reference;
	typedef typename Traits::const_forwarded_reference const_forwarded_reference;
	typedef typename cConstIterableImpl<Traits,mpl::sequence<indexs...>>::pair_node pair_node;
    typedef typename cConstIterableImpl<Traits,mpl::sequence<indexs...>>::nested_iterable_interface nested_iterable_interface;

    using cConstIterableImpl<Traits,mpl::sequence<indexs...>>::createPair;
    using cConstIterableImpl<Traits,mpl::sequence<indexs...>>::deletePair;

public:
	typedef typename Traits::iterable_private_interface iterable_private_interface;

    template<typename ... Types>
    cConstIterableWithTransformImpl(Types&& ... i_values);

protected:
    const_forwarded_reference getValue(forwarded_node_pointer_type currNode) const override;
    forwarded_reference getValue(forwarded_node_pointer_type currNode) override;

private:
    ytl::function<forwarded_raw_type(reference)> m_transform;
};

template<typename Traits>
class cConstIterableWithoutTransformImpl : public cConstIterableImpl<Traits,typename mpl::create_range_rank<0,Traits::numIterables>::type>
{
protected:
	typedef typename Traits::iterable_private_interface iterable_private_interface;
	typedef typename Traits::forwarded_iterable_private_interface forwarded_iterable_private_interface;
	typedef typename Traits::iterable_public_interface iterable_public_interface;
	typedef typename Traits::forwarded_iterable_public_interface forwarded_iterable_public_interface;
    typedef typename Traits::iterator_type iterator_type;
    typedef typename Traits::const_iterator_type const_iterator_type;
    typedef typename Traits::forwarded_iterator_type forwarded_iterator_type;
    typedef typename Traits::const_forwarded_iterator_type const_forwarded_iterator_type;
	typedef typename Traits::value_type value_type;
	typedef typename Traits::forwarded_type forwarded_type;
	typedef typename Traits::pointer_type pointer_type;
	typedef typename Traits::forwarded_pointer_type forwarded_pointer_type;
	typedef typename Traits::forwarded_node_pointer_type forwarded_node_pointer_type;
	typedef typename Traits::forwarded_reference forwarded_reference;
	typedef typename Traits::const_forwarded_reference const_forwarded_reference;
	typedef typename cConstIterableImpl<Traits,typename mpl::create_range_rank<0,Traits::numIterables>::type>::pair_node pair_node;

    using cConstIterableImpl<Traits,typename mpl::create_range_rank<0,Traits::numIterables>::type>::createPair;
    using cConstIterableImpl<Traits,typename mpl::create_range_rank<0,Traits::numIterables>::type>::deletePair;

public:
    using cConstIterableImpl<Traits,typename mpl::create_range_rank<0,Traits::numIterables>::type>::cConstIterableImpl;

protected:
    typedef typename cConstIterableImpl<Traits,typename mpl::create_range_rank<0,Traits::numIterables>::type>::nested_iterable_interface nested_iterable_interface;

    const_forwarded_reference getValue(forwarded_node_pointer_type currNode) const override;
    forwarded_reference getValue(forwarded_node_pointer_type currNode) override;
};

template<typename Traits>
class cInputIterableImpl : public mpl::static_if<mpl::is_same_type<typename Traits::raw_type,typename Traits::forwarded_raw_type>::value,cConstIterableWithoutTransformImpl<Traits>,cConstIterableWithTransformImpl<Traits, typename mpl::create_range_rank<0,Traits::numIterables>::type>>::type
{
protected:
    typedef typename mpl::static_if<mpl::is_same_type<typename Traits::raw_type,typename Traits::forwarded_raw_type>::value,cConstIterableWithoutTransformImpl<Traits>,cConstIterableWithTransformImpl<Traits, typename mpl::create_range_rank<0,Traits::numIterables>::type>>::type base_type;
    typedef typename Traits::forwarded_iterable_public_interface iterable_public_interface;
	typedef typename base_type::pair_node pair_node;

    using base_type::createPair;
    using base_type::deletePair;

public:
    typedef typename base_type::forwarded_iterator_type iterator_type;
    typedef typename base_type::const_forwarded_iterator_type const_iterator_type;
	typedef typename base_type::forwarded_type value_type;
	typedef typename base_type::forwarded_pointer_type pointer_type;
	typedef typename base_type::forwarded_reference reference;
	typedef typename base_type::const_forwarded_reference const_reference;

    using base_type::base_type;
    using base_type::begin;
    using base_type::end;
    using base_type::cbegin;
    using base_type::cend;
    using base_type::getSize;
    using base_type::empty;
};

template<typename Traits>
class cOutputIterableImpl : public cConstIterableWithoutTransformImpl<Traits>
{
protected:
	typedef typename Traits::value_type inner_value_type;
	typedef typename Traits::pointer_type inner_pointer_type;
	typedef typename Traits::node_pointer_type inner_node_pointer_type;
	typedef typename Traits::reference inner_reference;
	typedef typename Traits::const_reference const_inner_reference;
    typedef typename Traits::forwarded_iterable_public_interface iterable_public_interface;
	typedef typename cConstIterableWithoutTransformImpl<Traits>::pair_node pair_node;

    using cConstIterableWithoutTransformImpl<Traits>::createPair;
    using cConstIterableWithoutTransformImpl<Traits>::deletePair;

public:
    typedef typename cConstIterableWithoutTransformImpl<Traits>::forwarded_iterator_type iterator_type;
    typedef typename cConstIterableWithoutTransformImpl<Traits>::const_forwarded_iterator_type const_iterator_type;
	typedef typename cConstIterableWithoutTransformImpl<Traits>::forwarded_type value_type;
	typedef typename cConstIterableWithoutTransformImpl<Traits>::forwarded_pointer_type pointer_type;
	typedef typename cConstIterableWithoutTransformImpl<Traits>::forwarded_node_pointer_type node_pointer_type;
	typedef typename cConstIterableWithoutTransformImpl<Traits>::forwarded_reference reference;
	typedef typename cConstIterableWithoutTransformImpl<Traits>::const_forwarded_reference const_reference;

    using cConstIterableWithoutTransformImpl<Traits>::cConstIterableWithoutTransformImpl;
    using cConstIterableWithoutTransformImpl<Traits>::begin;
    using cConstIterableWithoutTransformImpl<Traits>::end;
    using cConstIterableWithoutTransformImpl<Traits>::cbegin;
    using cConstIterableWithoutTransformImpl<Traits>::cend;
    using cConstIterableWithoutTransformImpl<Traits>::getSize;
    using cConstIterableWithoutTransformImpl<Traits>::empty;

protected:
    typedef typename cConstIterableWithoutTransformImpl<Traits>::nested_iterable_interface nested_iterable_interface;

    void setValue(node_pointer_type i_valueNode, const value_type& i_value) const override;
};

template<typename Traits>
using cForwardIterableImpl = cInputIterableImpl<Traits>;

template<typename Traits>
class cBidirectionalIterableImpl : public cForwardIterableImpl<Traits>
{
protected:
	typedef typename Traits::value_type inner_value_type;
	typedef typename Traits::pointer_type inner_pointer_type;
	typedef typename Traits::node_pointer_type inner_node_pointer_type;
	typedef typename Traits::reference inner_reference;
	typedef typename Traits::const_reference const_inner_reference;
    typedef typename Traits::iterator_type inner_iterator_type;
    typedef typename Traits::const_iterator_type inner_const_iterator_type;
    typedef typename Traits::iterable_private_interface inner_iterable_private_interface;
    typedef typename Traits::forwarded_iterable_public_interface iterable_public_interface;
	typedef typename Traits::forwarded_pointer_type forwarded_pointer_type;
	typedef typename Traits::forwarded_node_pointer_type forwarded_node_pointer_type;
	typedef typename cForwardIterableImpl<Traits>::pair_node pair_node;
    typedef typename cForwardIterableImpl<Traits>::nested_iterable_interface nested_iterable_interface;

    using cForwardIterableImpl<Traits>::createPair;
    using cForwardIterableImpl<Traits>::deletePair;

public:
    typedef typename cForwardIterableImpl<Traits>::iterator_type iterator_type;
    typedef typename cForwardIterableImpl<Traits>::const_iterator_type const_iterator_type;
	typedef typename cForwardIterableImpl<Traits>::value_type value_type;
	typedef typename cForwardIterableImpl<Traits>::pointer_type pointer_type;
	typedef typename cForwardIterableImpl<Traits>::node_pointer_type node_pointer_type;
	typedef typename cForwardIterableImpl<Traits>::reference reference;
	typedef typename cForwardIterableImpl<Traits>::const_reference const_reference;
    typedef typename Traits::reverse_forwarded_iterator_type reverse_iterator_type;
    typedef typename Traits::const_reverse_forwarded_iterator_type const_reverse_iterator_type;

    using cForwardIterableImpl<Traits>::cForwardIterableImpl;
    using cForwardIterableImpl<Traits>::begin;
    using cForwardIterableImpl<Traits>::end;
    using cForwardIterableImpl<Traits>::cbegin;
    using cForwardIterableImpl<Traits>::cend;
    using cForwardIterableImpl<Traits>::getSize;
    using cForwardIterableImpl<Traits>::empty;

    reverse_iterator_type rbegin(const ytl::function<bool(const_reference)>& filter = null_ptr) override;
    reverse_iterator_type rend() override;
    const_reverse_iterator_type crbegin(const ytl::function<bool(const_reference)>& filter = null_ptr) const override;
    const_reverse_iterator_type crend() const override;

protected:
    node_pointer_type getLastElem() const override;
    node_pointer_type getPrevElem(node_pointer_type currNode) const override;
    virtual inner_node_pointer_type getPrevElemInCurrentIterable(const nested_iterable_interface* currIterableBackEnd, inner_node_pointer_type currNode) const;
};

template<typename Traits>
class cRandomAccessIterableImpl : public cBidirectionalIterableImpl<Traits>
{
protected:
    typedef typename cBidirectionalIterableImpl<Traits>::nested_iterable_interface nested_iterable_interface;
	typedef typename Traits::value_type inner_value_type;
	typedef typename Traits::pointer_type inner_pointer_type;
	typedef typename Traits::node_pointer_type inner_node_pointer_type;
	typedef typename Traits::reference inner_reference;
	typedef typename Traits::const_reference const_inner_reference;
    typedef typename Traits::iterator_type inner_iterator_type;
    typedef typename Traits::const_iterator_type inner_const_iterator_type;
    typedef typename Traits::iterable_private_interface inner_iterable_private_interface;
    typedef typename Traits::forwarded_iterable_public_interface iterable_public_interface;
	typedef typename cBidirectionalIterableImpl<Traits>::pair_node pair_node;

    using cBidirectionalIterableImpl<Traits>::createPair;
    using cBidirectionalIterableImpl<Traits>::deletePair;

public:
    typedef typename cBidirectionalIterableImpl<Traits>::iterator_type iterator_type;
    typedef typename cBidirectionalIterableImpl<Traits>::const_iterator_type const_iterator_type;
	typedef typename cBidirectionalIterableImpl<Traits>::value_type value_type;
	typedef typename cBidirectionalIterableImpl<Traits>::pointer_type pointer_type;
	typedef typename cBidirectionalIterableImpl<Traits>::node_pointer_type node_pointer_type;
	typedef typename cBidirectionalIterableImpl<Traits>::reference reference;
	typedef typename cBidirectionalIterableImpl<Traits>::const_reference const_reference;

    using cBidirectionalIterableImpl<Traits>::cBidirectionalIterableImpl;
    using cBidirectionalIterableImpl<Traits>::begin;
    using cBidirectionalIterableImpl<Traits>::end;
    using cBidirectionalIterableImpl<Traits>::rbegin;
    using cBidirectionalIterableImpl<Traits>::rend;
    using cBidirectionalIterableImpl<Traits>::cbegin;
    using cBidirectionalIterableImpl<Traits>::cend;
    using cBidirectionalIterableImpl<Traits>::crbegin;
    using cBidirectionalIterableImpl<Traits>::crend;
    using cBidirectionalIterableImpl<Traits>::getSize;
    using cBidirectionalIterableImpl<Traits>::empty;

    size_t getIndexOfNode(node_pointer_type node) const override;
    node_pointer_type shiftNodeByIndex(node_pointer_type node, int val) const override;
};

}
}
}

#include "YTL/container/detail/cIterableImpl.tpp"
