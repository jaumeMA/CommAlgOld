
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
#include "cContainerView.h"
#include "System/memory/cSystemAllocator.h"

namespace yame
{
namespace container
{
namespace impl
{

template<typename T, typename R>
class cTrieLeave
{
public:
    template<typename ... TT>
    cTrieLeave(TT&& ... i_args);
    virtual ~cTrieLeave() = default;

    T m_value;
};

template<typename T, typename R>
class cTrieNode
{
public:
    typedef typename R::particle particle;

    cTrieNode(const particle& i_key);
    virtual ~cTrieNode() = default;
    inline cTrieNode<T,R> *getParent();
    inline void setParent(cTrieNode<T,R> *parent);
    inline bool hasChildren() const;
    inline static bool canMerge(cTrieNode<T,R> *from, cTrieNode<T,R> *to);
    inline static bool merge(cTrieNode<T,R> *from, cTrieNode<T,R> *to);

    cTrieNode<T,R>* m_pLeftChild = NULL;
    cTrieNode<T,R>* m_pMiddleChild = NULL;
    cTrieNode<T,R>* m_pRightChild = NULL;
    cTrieNode<T,R>* m_pParent = NULL;
    cTrieLeave<T,R>* m_pLeave = NULL;
    const particle m_key;
};

template<typename T, typename R, typename N, template<typename> class A>
class cTrieImpl : public detail::cBidirectionalIterableBaseImpl<cPair<const R,T&>>
{
    static_assert(mpl::is_const<T>::value == false && mpl::is_reference<T>::value == false, "Const/ reference types are not supported");
    typedef typename detail::cBidirectionalIterableBaseImpl<cPair<const R,T&>>::node_pointer_type node_pointer_type;
    typedef detail::cBidirectionalIterableBaseImpl<cPair<const R,T&>> iterable_type;

public:
    typedef typename R::particle particle;
    typedef N MapNode;
    typedef A<MapNode> Allocator;
    typedef A<cTrieLeave<T,R>> LeaveAllocator;
    typedef typename mpl::drop_reference<T>::type& lvaluereference;
    typedef const typename mpl::drop_reference<T>::type& const_lvaluereference;
    typedef typename mpl::drop_reference<T>::type&& rvaluereference;
    typedef const typename mpl::drop_reference<R>::type& const_lkeyreference;
    typedef typename detail::cBidirectionalIterableBaseImpl<cPair<const R,T&>>::iterator_type iterator_type;
    typedef typename detail::cBidirectionalIterableBaseImpl<cPair<const R,T&>>::const_iterator_type const_iterator_type;
    typedef typename detail::cBidirectionalIterableBaseImpl<cPair<const R,T&>>::reverse_iterator_type reverse_iterator_type;
    typedef typename detail::cBidirectionalIterableBaseImpl<cPair<const R,T&>>::const_reverse_iterator_type const_reverse_iterator_type;
    typedef typename detail::cBidirectionalIterableBaseImpl<cPair<const R,T&>>::iterable_private_interface iterable_private_interface;

    cTrieImpl();
    cTrieImpl(const cTrieImpl& i_other);
    cTrieImpl(cTrieImpl&& i_other);
    ~cTrieImpl();

    const_lvaluereference operator[](const_lkeyreference i_key) const;
    lvaluereference operator[](const_lkeyreference i_key);
    cTrieImpl<T,R,N,A>& operator=(const cTrieImpl<T,R,N,A>& other);
    cTrieImpl<T,R,N,A>& operator=(cTrieImpl<T,R,N,A>&& other);
    iterator_type search(const_lkeyreference key);
    const_iterator_type search(const_lkeyreference key) const;
    template<typename ... TT>
    cPair<bool,iterator_type> emplace(const_lkeyreference i_key, TT&& ... i_args);

private:
    //const iterable implementation
    node_pointer_type getFirstElem() const override;
    node_pointer_type getLastElem() const override;
    node_pointer_type getNextElem(node_pointer_type currNode) const override;
    node_pointer_type getPrevElem(node_pointer_type currNode) const override;
    const cPair<const R,T&> getValue(node_pointer_type currNode) const override;
    cPair<const R,T&> getValue(node_pointer_type currNode) override;

    inline cTrieNode<T,R>* getNextElem(cTrieNode<T,R>* currNode) const;
    inline cTrieNode<T,R>* getPrevElem(cTrieNode<T,R>* currNode) const;

    //non const iterable implementation
    void eraseNode(node_pointer_type i_pair) override;
    node_pointer_type addNode(typename mpl::PairType<const R,T&>::type i_value) override;

    template<typename P, typename ... TT>
    inline cTrieNode<T,R>* _insert(P&& key, TT&& ... args);
    inline void _erase(cTrieNode<T,R>* currNode);
    template<typename P>
    inline cTrieNode<T,R>* _search(P&& key) const;
    inline void createTree(const cTrieImpl<T,R,N,A>& tree);
    inline void clearTree();
    inline MapNode* allocate(cTrieNode<T,R>* node);
    inline MapNode*allocate(const particle& i_key);
    template<typename ... TT>
    inline cTrieLeave<T,R>* allocateLeave(TT&& ... i_args);
    inline void deallocate(cTrieNode<T,R>*& node);
    inline cTrieNode<T,R>* findMin(cTrieNode<T,R> *node) const;
    inline cTrieNode<T,R>* findMax(cTrieNode<T,R> *node) const;

    const Allocator&   m_alloc;
    const LeaveAllocator&   m_leaveAlloc;
    static const cTrieNode<T,R>* m_pTrieHead;
};

}

template<typename T, typename R, template<typename> class A = memory::cTypedSystemAllocator>
using cTrie = impl::cTrieImpl<T,R,impl::cTrieNode<T,R>,A>;

template<typename T, typename R>
using trie_view = cBidirectionalView<cPair<const R,T&>>;
template<typename T, typename R>
using const_trie_view = cConstBidirectionalView<cPair<const R,T&>>;

}
}

#include "YTL/container/detail/cTrie.tpp"
