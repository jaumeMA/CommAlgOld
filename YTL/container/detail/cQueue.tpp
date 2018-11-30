
namespace yame
{
namespace container
{
namespace detail
{

template<typename T>
template<typename ... TT>
cQueueNode<T>::cQueueNode(TT&& ... i_args)
: m_value(mpl::forward<TT>(i_args)...)
, m_pNextNode(NULL)
{
}
template<typename T>
typename cQueueNode<T>::lvaluereference cQueueNode<T>::getValue()
{
    return m_value;
}
template<typename T>
cQueueNode<T>* cQueueNode<T>::getNextNode()
{
    return m_pNextNode;
}
template<typename T>
void cQueueNode<T>::setNextNode(cQueueNode<T>* node)
{
    m_pNextNode = node;
}

template<typename T, typename N, template<typename> class A>
template<typename ... TT>
typename cQueueImpl<T,N,A>::QueueNode* cQueueImpl<T,N,A>::allocate(size_t size, TT&& ... value)
{
    QueueNode* node = NULL;
    void *mem = m_alloc.allocate(size, sizeof(N));

    if(mem)
    {
        node = new(mem) QueueNode(mpl::forward<TT>(value) ...);
    }

    return node;
}
template<typename T, typename N, template<typename> class A>
void cQueueImpl<T,N,A>::deallocate(QueueNode*& node)
{
    if(node)
    {
        //virtual mechanism takes care of calling all the related destructors
        node->~QueueNode();
        m_alloc.deallocate((void *)node);
    }

    return;
}
template<typename T, typename N, template<typename> class A>
void cQueueImpl<T,N,A>::eraseNode(QueueNode* node)
{
    ASSERT(node, "Erasing null node!");

    T* itNode = reinterpret_cast<T*>(node);

    if(itNode != iterator_type::m_pHead)
    {
        cQueueNode<T>* nextNode = node->getNextNode();

        if(nextNode != NULL)
        {
            this->setRootNode(nextNode);
        }
        else
        {
            this->setRootNode(iterable_type::m_pHead);
        }

        deallocate(node);
    }

    return;
}
template<typename T, typename N, template<typename> class A>
cQueueImpl<T,N,A>::cQueueImpl()
: detail::cForwardIterableBaseImpl<T&>()
, m_alloc(memory::cMemoryProvider<Allocator>::acquire())
{}
template<typename T, typename N, template<typename> class A>
cQueueImpl<T,N,A>::cQueueImpl(const cQueueImpl<T,N,A>& other)
: detail::cForwardIterableBaseImpl<T&>()
, m_alloc(memory::cMemoryProvider<Allocator>::acquire())
{
    const_iterator_type it = other.cbegin();
    for(;it!=other.cend();it++)
    {
        push(*it);
    }
}
template<typename T, typename N, template<typename> class A>
cQueueImpl<T,N,A>::cQueueImpl(cQueueImpl<T,N,A>&& other)
: detail::cForwardIterableBaseImpl<T&>()
, m_alloc(memory::cMemoryProvider<Allocator>::acquire())
{
    this->setRootNode(other.getRootNode());
    other.setRootNode(iterable_type::m_pHead);
}
template<typename T, typename N, template<typename> class A>
cQueueImpl<T,N,A>::~cQueueImpl()
{
    clear();
}
template<typename T, typename N, template<typename> class A>
template<typename ... TT>
typename cQueueImpl<T,N,A>::lvaluereference cQueueImpl<T,N,A>::push(TT&& ... value)
{
    node_pointer_type rootNode = this->getRootNode();

    cQueueNode<T>* nodeInserted = allocate(1,mpl::forward<TT>(value) ...);

    if(rootNode != iterable_type::m_pHead)
    {
        nodeInserted->setNextNode(detail::receiveNodeFromIterator<cQueueNode<T>*>(rootNode));
    }

    this->setRootNode(detail::sendNodeToIterator<node_pointer_type>(nodeInserted));

    return nodeInserted->getValue();
}
template<typename T, typename N, template<typename> class A>
void cQueueImpl<T,N,A>::pop()
{
    ASSERT(this->empty() == false, "Queue void!");

    node_pointer_type node = getFirstElem();

    if(node != iterator_type::m_pHead)
    {
        eraseNode(node);
    }

    return;
}
template<typename T, typename N, template<typename> class A>
typename cQueueImpl<T,N,A>::lvaluereference cQueueImpl<T,N,A>::front()
{
    ASSERT(this->empty() == false, "Queue void!");

    QueueNode* currQueueNode = this->template getTypedRootNode<QueueNode>();

    return currQueueNode->getValue();
}
template<typename T, typename N, template<typename> class A>
void cQueueImpl<T,N,A>::clear()
{
    while(this->empty() == false)
    {
        pop();
    }

    return;
}
template<typename T, typename N, template<typename> class A>
cQueueImpl<T,N,A>& cQueueImpl<T,N,A>::operator=(const cQueueImpl<T,N,A>& other)
{
    clear();

    const_iterator_type it = other.cbegin();

    for(;it!=other.cend();it++)
    {
        push(*it);
    }
}
template<typename T, typename N, template<typename> class A>
typename cQueueImpl<T,N,A>::node_pointer_type cQueueImpl<T,N,A>::getFirstElem() const
{
    return this->getRootNode();
}
template<typename T, typename N, template<typename> class A>
typename cQueueImpl<T,N,A>::node_pointer_type cQueueImpl<T,N,A>::getNextElem(node_pointer_type currNode) const
{
    if(currNode)
    {
        QueueNode* currQueueNode = detail::receiveNodeFromIterator<QueueNode*>(currNode);
        QueueNode* nextQueueNode = currQueueNode->getNextNode();

        if(nextQueueNode)
        {
            return detail::sendNodeToIterator<T*>(nextQueueNode);
        }
        else
        {
            return iterator_type::m_pHead;
        }
    }
    else
    {
        ASSERT(false, "We shouldnt reach this point");

        return iterator_type::m_pHead;
    }
}
template<typename T, typename N, template<typename> class A>
typename cQueueImpl<T,N,A>::const_reference cQueueImpl<T,N,A>::getValue(node_pointer_type currNode) const
{
    ASSERT(currNode != iterator_type::m_pHead, "Accessing to value of null node");

    QueueNode* currQueueNode = detail::receiveNodeFromIterator<QueueNode*>(currNode);

    return currQueueNode->getValue();
}
template<typename T, typename N, template<typename> class A>
typename cQueueImpl<T,N,A>::reference cQueueImpl<T,N,A>::getValue(node_pointer_type currNode)
{
    ASSERT(currNode != iterator_type::m_pHead, "Accessing to value of null node");

    QueueNode* currQueueNode = detail::receiveNodeFromIterator<QueueNode*>(currNode);

    return currQueueNode->getValue();
}
template<typename T, typename N, template<typename> class A>
void cQueueImpl<T,N,A>::eraseNode(node_pointer_type i_node)
{
    if(QueueNode* queueNode = detail::receiveNodeFromIterator<QueueNode*>(i_node))
    {
        cQueueNode<T>* nextNode = queueNode->getNextNode();

        if(nextNode)
        {
            this->setRootNode(nextNode);
        }
        else
        {
            this->setRootNode(iterable_type::m_pHead);
        }

        deallocate(queueNode);
    }
}
template<typename T, typename N, template<typename> class A>
typename cQueueImpl<T,N,A>::node_pointer_type cQueueImpl<T,N,A>::addNode(add_type i_value)
{
    cQueueNode<T>* nodeInserted = allocate(1,mpl::forward<add_type>(i_value));
    node_pointer_type rootNode = this->getRootNode();

    if(rootNode != iterator_type::m_pHead)
    {
        nodeInserted->setNextNode(detail::receiveNodeFromIterator<cQueueNode<T>*>(rootNode));
    }

    this->setRootNode(nodeInserted);

    return detail::sendNodeToIterator<node_pointer_type>(nodeInserted);
}

}
}
}
