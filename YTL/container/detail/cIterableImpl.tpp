#include "System/cException.h"

namespace yame
{
namespace container
{
namespace detail
{

template<typename Traits, int ... indexs>
template<typename ... Iterables>
cConstIterableImpl<Traits,mpl::sequence<indexs...>>::cConstIterableImpl(const Iterables& ... i_iterables)
: cIterableImplBackEnd<Traits::numIterables,Traits,Traits::numIterables-1>(ytl::static_lent_cast<iterable_private_interface>(ytl::lend<iterable_private_interface>(const_cast<Iterables&>(i_iterables))) ...)
{
    static_assert(iterable_category_compliant<void, Iterables::category ...>::value, "You have to follow category rules for iterables, look .h file for more details");
    static_assert(iterable_constness_compliant<void, Iterables...>::value, "You cannot have non const iterables with const nested iterables");
    bool circularDepDetected = iterable_circular_dep_compliant<void,Iterables...>::checkCircularDep(*this, i_iterables...);
    ASSERT(circularDepDetected == false, "Circular dependency detected in nested iterables");
}
template<typename Traits, int ... indexs>
typename cConstIterableImpl<Traits,mpl::sequence<indexs...>>::forwarded_node_pointer_type cConstIterableImpl<Traits,mpl::sequence<indexs...>>::createPair(const nested_iterable_interface* i_interface, node_pointer_type i_pointer)
{
    return reinterpret_cast<forwarded_node_pointer_type>((u8*)memcpy((u8*)memcpy(k_pairAllocator.allocate(1, sizeof(pair_node)),&i_interface, sizeof(i_interface)) + sizeof(i_interface),&i_pointer, sizeof(i_pointer)) - sizeof(i_interface));
}
template<typename Traits, int ... indexs>
void cConstIterableImpl<Traits,mpl::sequence<indexs...>>::deletePair(pair_node* i_pointer)
{
    k_pairAllocator.deallocate(i_pointer);
}
template<typename Traits, int ... indexs>
typename cConstIterableImpl<Traits,mpl::sequence<indexs...>>::forwarded_node_pointer_type cConstIterableImpl<Traits,mpl::sequence<indexs...>>::getFirstElem() const
{
    const nested_iterable_interface* firstBackend = static_cast<const nested_iterable_interface*>(this);

    node_pointer_type newType = firstBackend->getPrivateIterable().getFirstElem();

    return (newType != iterator_type::m_pHead) ? createPair(firstBackend,newType) : forwarded_iterator_type::m_pHead;
}
template<typename Traits, int ... indexs>
typename cConstIterableImpl<Traits,mpl::sequence<indexs...>>::node_pointer_type cConstIterableImpl<Traits,mpl::sequence<indexs...>>::getNextElemInCurrentIterable(const nested_iterable_interface* currIterableBackEnd, node_pointer_type currNode) const
{
    const iterable_private_interface& currIterable = currIterableBackEnd->getPrivateIterable();

    node_pointer_type nextNode = currIterable.getNextElem(currNode);

    return nextNode;
}
template<typename Traits, int ... indexs>
ytl::enable_ref_from_this& cConstIterableImpl<Traits,mpl::sequence<indexs...>>::ref_from_this()
{
    return *this;
}
template<typename Traits, int ... indexs>
const ytl::enable_ref_from_this& cConstIterableImpl<Traits,mpl::sequence<indexs...>>::ref_from_this() const
{
    return *this;
}
template<typename Traits, int ... indexs>
typename cConstIterableImpl<Traits,mpl::sequence<indexs...>>::forwarded_node_pointer_type cConstIterableImpl<Traits,mpl::sequence<indexs...>>::getNextElem(forwarded_node_pointer_type currNode) const
{
    pair_node* currPair = receiveNodeFromIterator<pair_node*>(currNode);
    nested_iterable_interface* nextIterableBackEnd = currPair->backendInterface;
    node_pointer_type nextNode =  getNextElemInCurrentIterable(nextIterableBackEnd, currPair->innerNode);

    while(nextIterableBackEnd && nextNode == iterator_type::m_pHead)
    {
        nextIterableBackEnd->getPrivateIterable().releaseNode(nextNode);

        nextIterableBackEnd = nextIterableBackEnd->getNextIterable();

        nextNode = (nextIterableBackEnd) ? nextIterableBackEnd->getPrivateIterable().getFirstElem() : iterator_type::m_pHead;
    }

    if(nextNode != iterator_type::m_pHead)
    {
        currPair->backendInterface = nextIterableBackEnd;
        currPair->innerNode = nextNode;

        return sendNodeToIterator<forwarded_node_pointer_type>(currPair);
    }
    else
    {
        //invalidate current node
        deletePair(currPair);

        return forwarded_iterator_type::m_pHead;
    }
}
template<typename Traits, int ... indexs>
size_t cConstIterableImpl<Traits,mpl::sequence<indexs...>>::getSize() const
{
    size_t size = 0;
    const nested_iterable_interface* currBackend = static_cast<const nested_iterable_interface*>(this);

    while(currBackend)
    {
        const forwarded_iterable_public_interface& currPublicIterable = dynamic_cast<const forwarded_iterable_public_interface&>(currBackend->getPrivateIterable());
        size += currPublicIterable.getSize();
        currBackend = currBackend->getNextIterable();
    }

    return size;
}

template<typename Traits, int ... indexs>
bool cConstIterableImpl<Traits,mpl::sequence<indexs...>>::empty() const
{
    bool isEmpty = true;
    const nested_iterable_interface* currBackend = static_cast<const nested_iterable_interface*>(this);

    while(currBackend)
    {
        if(currBackend->getPrivateIterable().getFirstElem() != iterator_type::m_pHead)
        {
            isEmpty = false;
            break;
        }
        currBackend = currBackend->getNextIterable();
    }

    return isEmpty;
}

template<typename Traits, int ... indexs>
typename cConstIterableImpl<Traits,mpl::sequence<indexs...>>::forwarded_iterator_type cConstIterableImpl<Traits,mpl::sequence<indexs...>>::begin(const ytl::function<bool(const_forwarded_reference)>& filter)
{
    forwarded_iterator_type res(*this, filter);

    forwarded_node_pointer_type newNode = this->getFirstElem();

    if(filter != null_ptr)
    {
        while(newNode != forwarded_iterator_type::m_pHead && filter.eval(this->getValue(newNode)) == false)
        {
            newNode = this->getNextElem(newNode);
        }
    }

    res.setCurrentNode(newNode);

    return res;
}

template<typename Traits, int ... indexs>
typename cConstIterableImpl<Traits,mpl::sequence<indexs...>>::forwarded_iterator_type cConstIterableImpl<Traits,mpl::sequence<indexs...>>::end()
{
    forwarded_iterator_type res(*this);

    res.setCurrentNode(forwarded_iterator_type::m_pHead);

    return res;
}

template<typename Traits, int ... indexs>
typename cConstIterableImpl<Traits,mpl::sequence<indexs...>>::const_forwarded_iterator_type cConstIterableImpl<Traits,mpl::sequence<indexs...>>::cbegin(const ytl::function<bool(const_forwarded_reference)>& filter) const
{
    const_forwarded_iterator_type res(*this, filter);

    forwarded_node_pointer_type newNode = this->getFirstElem();

    if(filter != null_ptr)
    {
        while(newNode != forwarded_iterator_type::m_pHead && filter.eval(this->getValue(newNode)) == false)
        {
            newNode = this->getNextElem(newNode);
        }
    }

    res.setCurrentNode(newNode);

    return res;
}

template<typename Traits, int ... indexs>
typename cConstIterableImpl<Traits,mpl::sequence<indexs...>>::const_forwarded_iterator_type cConstIterableImpl<Traits,mpl::sequence<indexs...>>::cend() const
{
    const_forwarded_iterator_type res(*this);

    res.setCurrentNode(forwarded_iterator_type::m_pHead);

    return res;
}

template<typename Traits, int ... indexs>
typename cConstIterableImpl<Traits,mpl::sequence<indexs...>>::forwarded_node_pointer_type cConstIterableImpl<Traits,mpl::sequence<indexs...>>::acquireNode(forwarded_node_pointer_type i_node) const
{
    if(i_node != forwarded_iterator_type::m_pHead)
    {
        pair_node* currPair = receiveNodeFromIterator<pair_node*>(i_node);

        return createPair(currPair->backendInterface, currPair->backendInterface->getPrivateIterable().acquireNode(currPair->innerNode));
    }
    else
    {
        return i_node;
    }
}

template<typename Traits, int ... indexs>
void cConstIterableImpl<Traits,mpl::sequence<indexs...>>::releaseNode(forwarded_node_pointer_type i_node) const
{
    if(i_node != forwarded_iterator_type::m_pHead)
    {
        pair_node* currPair = receiveNodeFromIterator<pair_node*>(i_node);

        currPair->backendInterface->getPrivateIterable().releaseNode(currPair->innerNode);

        deletePair(currPair);
    }
}


template<typename Traits>
void cOutputIterableImpl<Traits>::setValue(node_pointer_type i_node, const value_type& i_value) const
{
    if(i_node != iterator_type::m_pHead)
    {
        pair_node* currPair = receiveNodeFromIterator<pair_node*>(i_node);

        currPair->backendInterface->getPrivateIterable().setValue(currPair->innerNode, value_type(i_value));
    }
}

template<typename Traits>
typename cBidirectionalIterableImpl<Traits>::reverse_iterator_type cBidirectionalIterableImpl<Traits>::rbegin(const ytl::function<bool(const_reference)>& filter)
{
    reverse_iterator_type res(*this, filter);

    forwarded_node_pointer_type newNode = this->getLastElem();

    if(filter != null_ptr)
    {
        while(newNode != iterator_type::m_pHead && filter.eval(this->getValue(newNode)) == false)
        {
            newNode = this->getPrevElem(newNode);
        }
    }

    res.setCurrentNode(newNode);

    return res;
}
template<typename Traits>
typename cBidirectionalIterableImpl<Traits>::reverse_iterator_type cBidirectionalIterableImpl<Traits>::rend()
{
    reverse_iterator_type res(*this);

    return res;
}
template<typename Traits>
typename cBidirectionalIterableImpl<Traits>::const_reverse_iterator_type cBidirectionalIterableImpl<Traits>::crbegin(const ytl::function<bool(const_reference)>& filter) const
{
    const_reverse_iterator_type res(*this, filter);

    forwarded_node_pointer_type newNode = this->getLastElem();

    if(filter != null_ptr)
    {
        while(newNode != iterator_type::m_pHead && filter.eval(this->getValue(newNode)) == false)
        {
            newNode = this->getPrevElem(newNode);
        }
    }

    res.setCurrentNode(newNode);

    return res;
}
template<typename Traits>
typename cBidirectionalIterableImpl<Traits>::const_reverse_iterator_type cBidirectionalIterableImpl<Traits>::crend() const
{
    const_reverse_iterator_type res(*this);

    return res;
}
template<typename Traits>
typename cBidirectionalIterableImpl<Traits>::node_pointer_type cBidirectionalIterableImpl<Traits>::getLastElem() const
{
    const nested_iterable_interface& lastBackend = this->template getNthIterable<Traits::numIterables-1>();

    return createPair(&lastBackend,lastBackend.getPrivateIterable().getLastElem());
}
template<typename Traits>
typename cBidirectionalIterableImpl<Traits>::inner_node_pointer_type cBidirectionalIterableImpl<Traits>::getPrevElemInCurrentIterable(const nested_iterable_interface* currIterableBackEnd, inner_node_pointer_type currNode) const
{
    const inner_iterable_private_interface& currIterable = currIterableBackEnd->getPrivateIterable();

    inner_node_pointer_type prevNode = currIterable.getPrevElem(currNode);

    return prevNode;
}
template<typename Traits>
typename cBidirectionalIterableImpl<Traits>::node_pointer_type cBidirectionalIterableImpl<Traits>::getPrevElem(node_pointer_type currNode) const
{
    pair_node* currPair = receiveNodeFromIterator<pair_node*>(currNode);
    nested_iterable_interface* prevIterableBackEnd = currPair->backendInterface;
    inner_node_pointer_type prevNode = getPrevElemInCurrentIterable(prevIterableBackEnd, currPair->innerNode);

    while(prevIterableBackEnd && prevNode == inner_iterator_type::m_pHead)
    {
        prevIterableBackEnd->getPrivateIterable().releaseNode(prevNode);

        prevIterableBackEnd = prevIterableBackEnd->getPrevIterable();

        prevNode = (prevIterableBackEnd) ? prevIterableBackEnd->getPrivateIterable().getLastElem() : inner_iterator_type::m_pHead;
    }

    if(prevNode != inner_iterator_type::m_pHead)
    {
        currPair->backendInterface = prevIterableBackEnd;
        currPair->innerNode = prevNode;

        return sendNodeToIterator<node_pointer_type>(currPair);
    }
    else
    {
        //invalidate current node
        deletePair(currPair);

        return iterator_type::m_pHead;
    }
}

template<typename Traits>
size_t cRandomAccessIterableImpl<Traits>::getIndexOfNode(node_pointer_type currNode) const
{
    pair_node* currPair = receiveNodeFromIterator<pair_node*>(currNode);

    const inner_iterable_private_interface& currIterable = currPair->backendInterface->getPrivateIterable();

    return currIterable.getIndexOfNode(currPair->innerNode);
}

template<typename Traits>
typename cRandomAccessIterableImpl<Traits>::node_pointer_type cRandomAccessIterableImpl<Traits>::shiftNodeByIndex(node_pointer_type node, int index) const
{
    pair_node* currPair = receiveNodeFromIterator<pair_node*>(node);

    nested_iterable_interface* currIterableBackEnd = currPair->backendInterface;
    inner_node_pointer_type currNode = currPair->innerNode;
    const inner_iterable_private_interface& currIterable = currIterableBackEnd->getPrivateIterable();

    while(currIterableBackEnd)
    {
        currNode = currIterable.shiftNodeByIndex(currNode,index);

        if(currNode == inner_iterator_type::m_pHead)
        {
            currIterable.releaseNode(currNode);
            currIterableBackEnd = (index > 0) ? currIterableBackEnd->getNextIterable() : currIterableBackEnd->getPrevIterable();
            const inner_iterable_private_interface& nextIterable = currIterableBackEnd->getPrivateIterable();
            index = (index > 0) ? index - nextIterable.getIndexOfNode(currNode) : index + nextIterable.getIndexOfNode(currNode);
            currNode = (index > 0) ? nextIterable.getFirstElem() : nextIterable.getLastElem();
        }
        else
        {
            break;
        }
    }

    if(currNode != inner_iterator_type::m_pHead)
    {
        currPair->backendInterface = currIterableBackEnd;
        currPair->innerNode = currNode;

        return sendNodeToIterator<node_pointer_type>(currPair);
    }
    else
    {
        //invalidate current node
        deletePair(currPair);

        return iterator_type::m_pHead;
    }
}

//cIterableWithoutTransformImpl

template<typename Traits>
typename cConstIterableWithoutTransformImpl<Traits>::const_forwarded_reference cConstIterableWithoutTransformImpl<Traits>::getValue(forwarded_node_pointer_type currNode) const
{
    const pair_node* currPair = receiveNodeFromIterator<const pair_node*>(currNode);

    const nested_iterable_interface* currIterableBackEnd = currPair->backendInterface;

    return currIterableBackEnd->getPrivateIterable().getValue(currPair->innerNode);
}
template<typename Traits>
typename cConstIterableWithoutTransformImpl<Traits>::forwarded_reference cConstIterableWithoutTransformImpl<Traits>::getValue(forwarded_node_pointer_type currNode)
{
    pair_node* currPair = receiveNodeFromIterator<pair_node*>(currNode);

    nested_iterable_interface* currIterableBackEnd = currPair->backendInterface;

    return currIterableBackEnd->getPrivateIterable().getValue(currPair->innerNode);
}

//cIterableWithTransformImpl

template<typename Traits, int ... indexs>
template<typename ... Types>
cConstIterableWithTransformImpl<Traits,mpl::sequence<indexs...>>::cConstIterableWithTransformImpl(Types&& ... i_values)
: cConstIterableImpl<Traits,mpl::sequence<indexs...>>(mpl::nth_val_of<indexs>(i_values...)...)
, m_transform(mpl::nth_val_of<mpl::get_num_ranks<indexs...>::value>(i_values...))
{}
template<typename Traits, int ... indexs>
typename cConstIterableWithTransformImpl<Traits,mpl::sequence<indexs...>>::const_forwarded_reference cConstIterableWithTransformImpl<Traits,mpl::sequence<indexs...>>::getValue(forwarded_node_pointer_type currNode) const
{
    pair_node* currPair = receiveNodeFromIterator<pair_node*>(currNode);

    nested_iterable_interface* nextIterableBackEnd = currPair->backendInterface;

    return m_transform.eval(nextIterableBackEnd->getPrivateIterable().getValue(currPair->innerNode));
}
template<typename Traits, int ... indexs>
typename cConstIterableWithTransformImpl<Traits,mpl::sequence<indexs...>>::forwarded_reference cConstIterableWithTransformImpl<Traits,mpl::sequence<indexs...>>::getValue(forwarded_node_pointer_type currNode)
{
    pair_node* currPair = receiveNodeFromIterator<pair_node*>(currNode);

    nested_iterable_interface* nextIterableBackEnd = currPair->backendInterface;

    return m_transform.eval(nextIterableBackEnd->getPrivateIterable().getValue(currPair->innerNode));
}

}
}
}
