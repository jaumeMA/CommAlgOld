#pragma once

#include "System/memory/cSystemAllocator.h"
#include "YTL/container/cIterableBase.h"
#include "YTL/container/cContainerView.h"
#include "YTL/types/optional/optional.h"
#include <atomic>

namespace yame
{
namespace container
{
namespace detail
{

template<typename T>
class cStackNode
{
public:
    typedef typename mpl::drop_reference<T>::type& lvaluereference;
    typedef const typename mpl::drop_reference<T>::type& const_lvaluereference;
    typedef typename mpl::drop_reference<T>::type&& rvaluereference;

    cStackNode();
    template<typename ... TT>
    cStackNode(TT&& ... i_args);
    inline lvaluereference getValue();
    inline T extractValue();
    inline void setValue(const T& i_value);
    inline void setValue(T&& i_value);
    inline cStackNode<T>* loadNextNode();
    inline void storeNextNode(cStackNode<T>* i_newNode);
    inline bool compareAndExchangeNextNode(cStackNode<T>* i_oldNode, cStackNode<T>* i_newNode);
    inline void setAsDivider(bool i_divider);
    inline bool isDivider() const;

private:
    typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type m_value;
    std::atomic<cStackNode<T>*> m_pNextNode;
    std::atomic<bool>           m_isDivider;
};

template<typename T, typename N, template<typename> class A>
class cStackImpl  : public detail::cConstInputIterableBaseImpl<T&>
{
    typedef typename detail::cConstInputIterableBaseImpl<T&>::node_pointer_type node_pointer_type;
    typedef detail::cConstInputIterableBaseImpl<T&> iterable_type;

public:
    typedef N StackNode;
    typedef typename A<N>::allocator Allocator;
    typedef T value_type;
    typedef typename mpl::drop_reference<T>::type& lvaluereference;
    typedef const typename mpl::drop_reference<T>::type& const_lvaluereference;
    typedef typename mpl::drop_reference<T>::type&& rvaluereference;
    typedef typename detail::cConstInputIterableBaseImpl<T&>::iterator_type iterator_type;
    typedef typename detail::cConstInputIterableBaseImpl<T&>::const_iterator_type const_iterator_type;
    typedef typename detail::cConstInputIterableBaseImpl<T&>::iterable_private_interface iterable_private_interface;
    typedef typename detail::cConstInputIterableBaseImpl<T&>::reference reference;
    typedef typename detail::cConstInputIterableBaseImpl<T&>::const_reference const_reference;

    using detail::cConstInputIterableBaseImpl<T&>::begin;
    using detail::cConstInputIterableBaseImpl<T&>::end;
    using detail::cConstInputIterableBaseImpl<T&>::cbegin;
    using detail::cConstInputIterableBaseImpl<T&>::cend;
    using detail::cConstInputIterableBaseImpl<T&>::getSize;
    using detail::cConstInputIterableBaseImpl<T&>::empty;

    cStackImpl();
    cStackImpl(const cStackImpl<T,N,A>&) = delete;
    ~cStackImpl();
    template<typename ... TT>
    lvaluereference push(TT&& ... i_args);
    ytl::optional<value_type> pop();
    bool empty() const override;
    size_t getSize() const override;

private:
    //const iterable implementation
    node_pointer_type getFirstElem() const override;
    node_pointer_type getNextElem(node_pointer_type currNode) const override;
    const_reference getValue(node_pointer_type currNode) const override;
    reference getValue(node_pointer_type currNode) override;
    void releaseNode(node_pointer_type i_node) const override;
    node_pointer_type acquireNode(node_pointer_type i_node) const override;

    template<typename ... TT>
    inline StackNode* allocate(size_t size, TT&& ... i_args) const;
    void deallocate(cStackNode<T>* node) const;
    inline void purge();
    inline cStackNode<T>* extractFirstNode() const;
    inline void clear();
    inline bool emptyStack() const;

    mutable cStackNode<T> m_first;
    mutable cStackNode<T> m_last;
    const Allocator&    m_alloc;
};

}

template<typename T, template<typename> class A = memory::cTypedSystemAllocator>
using cStack = detail::cStackImpl<T,detail::cStackNode<T>,A>;

template<typename T>
using stack_view = cInputView<T&>;
template<typename T>
using const_stack_view = cConstInputView<T&>;

}
}

#include "detail/cStack.tpp"
