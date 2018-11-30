
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

#include "cPair.h"
#include "cIterableBase.h"
#include "YTL/container/cContainerView.h"
#include "System/memory/cSystemAllocator.h"
#include <stdarg.h>
#include "System/cException.h"
#include <new>

namespace yame
{
namespace container
{
namespace detail
{

template<typename T, typename R>
class cMapNode
{
public:
    typedef T value_type;
    typedef R key_type;

    cMapNode<T,R>() = default;
    template<typename ... TT>
    cMapNode<T,R>(const R& i_key , TT&& ... i_args);
    virtual ~cMapNode<T,R>() = default;
    inline void replaceNode(cMapNode<T,R> *other);
    inline cMapNode<T,R> *getParent();
    inline void setParent(cMapNode<T,R> *parent);

    cPair<const R,T> m_value;
    cMapNode<T,R>  *m_pParent;
    cMapNode<T,R>  *m_pLeftChild;
    cMapNode<T,R>  *m_pRightChild;
};

template<typename Map,typename Node>
class EmptyBalancer
{
public:
typedef EmptyBalancer Balancer;

inline static Node* balance(Map* i_map,Node* i_node) { return i_map->template getTypedRootNode<Node>(); };
};

template<typename T, typename R, typename N, template<typename> class A, template<typename,typename> class B>
class cMapImpl : public detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>
{
    static_assert(mpl::is_const<T>::value == false && mpl::is_reference<T>::value == false, "Const/ reference types are not supported");

    typedef typename mpl::PairType<const R,T>::type pair_type;
    typedef detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&> iterable_type;
    typedef typename detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>::node_pointer_type node_pointer_type;
    friend typename B<cMapImpl<T,R,N,A,B>,N>::Balancer;

public:
    typedef B<cMapImpl<T,R,N,A,B>,N> Balancer;
    typedef N MapNode;
    typedef typename A<MapNode>::allocator Allocator;
    typedef typename mpl::add_constness<T>::type const_T;
    typedef typename mpl::add_constness<R>::type const_R;
    typedef typename mpl::drop_reference<typename mpl::drop_constness<T>::type>::type value_type;
    typedef typename mpl::drop_reference<T>::type& lvaluereference;
    typedef const typename mpl::drop_reference<T>::type& const_lvaluereference;
    typedef typename mpl::drop_reference<R>::type& lkeyreference;
    typedef const typename mpl::drop_reference<R>::type& const_lkeyreference;
    typedef typename mpl::drop_reference<T>::type&& rvaluereference;
    typedef typename mpl::drop_reference<R>::type&& rkeyreference;
    typedef const typename mpl::drop_reference<R>::type key_type;
    typedef typename detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>::reference reference;
    typedef typename detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>::const_reference const_reference;
    typedef typename detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>::iterator_type iterator_type;
    typedef typename detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>::const_iterator_type const_iterator_type;
    typedef typename detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>::reverse_iterator_type reverse_iterator_type;
    typedef typename detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>::const_reverse_iterator_type const_reverse_iterator_type;
    typedef typename detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>::iterable_private_interface iterable_private_interface;
    typedef typename detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>::lendable_tag lendable_tag;
    typedef ytl::function<bool(const_lkeyreference,const_lkeyreference)> compare_func;

    using detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>::begin;
    using detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>::end;
    using detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>::cbegin;
    using detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>::cend;
    using detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>::crbegin;
    using detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>::crend;
    using detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>::getSize;
    using detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>::empty;
    using detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>::add;
    using detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>::erase;

    cMapImpl<T,R,N,A,B>();
    cMapImpl<T,R,N,A,B>(const compare_func& i_compare);
    cMapImpl<T,R,N,A,B>(const_lvaluereference value, const_lkeyreference key);
    cMapImpl<T,R,N,A,B>(const_lvaluereference value, const_lkeyreference key, const compare_func& i_compare);
    cMapImpl<T,R,N,A,B>(const cMapImpl<T,R,N,A,B>& other);
    cMapImpl<T,R,N,A,B>(cMapImpl<T,R,N,A,B>&& other);
    virtual ~cMapImpl<T,R,N,A,B>();
    cMapImpl<T,R,N,A,B>& operator=(const cMapImpl<T,R,N,A,B>& other);
    cMapImpl<T,R,N,A,B>& operator=(cMapImpl<T,R,N,A,B>&& other);
    const_iterator_type search(const_lkeyreference key) const;
    iterator_type search(const_lkeyreference key);
    const_lvaluereference operator[](const_lkeyreference key) const;
    lvaluereference operator[](const_lkeyreference key);
    template<typename ... TT>
    cPair<bool,iterator_type> emplace(const_lkeyreference i_key, TT&& ... i_args);
    void clear();

private:

    template<typename _valueType>
    inline void assign(const_lkeyreference key, _valueType&& val);

    inline void _onNodeInserted(MapNode* node);
    inline void _onNodeErased(MapNode* node);
    inline cMapNode<T,R>* search(const R& key, cMapNode<T,R>* rootNode) const;
    inline void insert(cMapNode<T,R> *other, cMapNode<T,R>* rootNode) const;
    inline cMapNode<T,R>* getClosestSmallerNode(const R& i_key, cMapNode<T,R>* i_rootNode) const;
    inline cMapNode<T,R>* getClosestBiggerNode(const R& i_key, cMapNode<T,R>* i_rootNode) const;

protected:
    //const iterable implementation
    node_pointer_type getFirstElem() const override;
    node_pointer_type getLastElem() const override;
    node_pointer_type getNextElem(node_pointer_type currNode) const override;
    node_pointer_type getPrevElem(node_pointer_type currNode) const override;
    const_reference getValue(node_pointer_type i_currNode) const override;
    reference getValue(node_pointer_type i_currNode) override;

    //non const iterable implementation
    void eraseNode(node_pointer_type i_pair) override;
    node_pointer_type addNode(typename mpl::PairType<const R,T>::type i_value) override;

    inline void createTree(const cMapImpl<T,R,N,A,B>& tree);
    inline void clearTree();
    inline MapNode* allocate(cMapNode<T,R>* node) const;
    template<typename ... TT>
    inline MapNode* allocate(const_lkeyreference key, TT&& ... i_args) const;
    inline void deallocate(cMapNode<T,R> *&node) const;
    inline cMapNode<T,R>* findMin(cMapNode<T,R> *node) const;
    inline cMapNode<T,R>* findMax(cMapNode<T,R> *node) const;

    const Allocator&   m_alloc;
    compare_func m_compare;
};
}

template<typename T, typename R, template<typename> class A = memory::cTypedSystemAllocator, template<typename,typename> class B = detail::EmptyBalancer>
using cMap = detail::cMapImpl<T,R,detail::cMapNode<T,R>,A,B>;

template<typename T, typename R>
using map_view = cBidirectionalView<cPair<const R,T>&>;
template<typename T, typename R>
using const_map_view = cConstBidirectionalView<cPair<const R,T>&>;

}
}

#include "detail/cMap.tpp"
