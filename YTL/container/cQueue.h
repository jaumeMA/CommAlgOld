
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

#include "YTL/container/cIterableBase.h"
#include "YTL/container/cContainerView.h"
#include "System/memory/cSystemAllocator.h"

namespace yame
{
namespace container
{
namespace detail
{

template<typename T>
class cQueueNode
{
public:
    typedef typename mpl::drop_reference<T>::type& lvaluereference;
    typedef const typename mpl::drop_reference<T>::type& const_lvaluereference;
    typedef typename mpl::drop_reference<T>::type&& rvaluereference;

    template<typename ... TT>
    cQueueNode(TT&& ... i_args);
    lvaluereference getValue();
    cQueueNode<T>* getNextNode();
    void setNextNode(cQueueNode<T>* node);

private:
    T m_value;
    cQueueNode<T>* m_pNextNode;
};

template<typename T, typename N, template<typename> class A>
class cQueueImpl : public detail::cForwardIterableBaseImpl<T&>
{
    static_assert(mpl::is_const<T>::value == false && mpl::is_reference<T>::value == false, "Const/ reference types are not supported");

    typedef typename detail::cForwardIterableBaseImpl<T&>::add_type add_type;
    typedef typename detail::cForwardIterableBaseImpl<T&>::node_pointer_type node_pointer_type;
    typedef detail::cForwardIterableBaseImpl<T&> iterable_type;

public:
    typedef N QueueNode;
    typedef typename A<N>::allocator Allocator;
    typedef typename mpl::drop_reference<T>::type& lvaluereference;
    typedef const typename mpl::drop_reference<T>::type& const_lvaluereference;
    typedef typename mpl::drop_reference<T>::type&& rvaluereference;
    typedef typename detail::cForwardIterableBaseImpl<T&>::reference reference;
    typedef typename detail::cForwardIterableBaseImpl<T&>::const_reference const_reference;
    typedef typename detail::cForwardIterableBaseImpl<T&>::iterator_type iterator_type;
    typedef typename detail::cForwardIterableBaseImpl<T&>::const_iterator_type const_iterator_type;
    typedef typename detail::cForwardIterableBaseImpl<T&>::iterable_private_interface iterable_private_interface;

    using detail::cForwardIterableBaseImpl<T&>::begin;
    using detail::cForwardIterableBaseImpl<T&>::end;
    using detail::cForwardIterableBaseImpl<T&>::cbegin;
    using detail::cForwardIterableBaseImpl<T&>::cend;
    using detail::cForwardIterableBaseImpl<T&>::getSize;
    using detail::cForwardIterableBaseImpl<T&>::empty;
    using detail::cForwardIterableBaseImpl<T&>::add;
    using detail::cForwardIterableBaseImpl<T&>::erase;

    cQueueImpl();
    cQueueImpl(const cQueueImpl<T,N,A>& other);
    cQueueImpl(cQueueImpl<T,N,A>&& other);
    ~cQueueImpl();
    template<typename ... TT>
    lvaluereference push(TT&& ... val);
    void pop();
    lvaluereference front();
    void clear();
    cQueueImpl<T,N,A>& operator=(const cQueueImpl<T,N,A>& other);

private:
    //const iterable implementation
    node_pointer_type getFirstElem() const override;
    node_pointer_type getNextElem(node_pointer_type currNode) const override;
    const_reference getValue(node_pointer_type currNode) const override;
    reference getValue(node_pointer_type currNode) override;

    //non const iterable implementation
    void eraseNode(node_pointer_type i_node) override;
    node_pointer_type addNode(add_type i_value) override;

    template<typename ... TT>
    QueueNode* allocate(size_t size, TT&& ... value);
    void deallocate(QueueNode *&node);
    virtual void eraseNode(QueueNode *other);

    const Allocator&    m_alloc;
};

}

template<typename T, template<typename> class A = memory::cTypedSystemAllocator>
using cQueue = detail::cQueueImpl<T,detail::cQueueNode<T>,A>;


template<typename T>
using queue_view = cForwardView<T&>;
template<typename T>
using const_queue_view = cConstForwardView<T&>;

}
}

#include "detail/cQueue.tpp"
