

#define IS_DIVISOR(ptr) ptr->isDivider()//(*reinterpret_cast<size_t*>(ptr) & 0x01)
#define MARK_AS_DIVISOR(ptr) ptr->setAsDivider(true);//{ size_t* __foo = reinterpret_cast<size_t*>(ptr); *__foo |= 0x01; }
#define MARK_AS_NOT_DIVISOR(ptr) ptr->setAsDivider(false);//{ size_t* __foo = reinterpret_cast<size_t*>(ptr); *__foo ^= 0x01; }

namespace yame
{
namespace container
{
namespace detail
{

template<typename T>
cStackNode<T>::cStackNode()
{
    memset(&m_value,0,sizeof(T));
    m_pNextNode.store(NULL);
    m_isDivider.store(false);
}
template<typename T>
template<typename ... TT>
cStackNode<T>::cStackNode(TT&& ... i_args)
{
    new (&m_value) T(mpl::forward<TT>(i_args)...);
    m_pNextNode.store(NULL);
    m_isDivider.store(false);
}
template<typename T>
typename cStackNode<T>::lvaluereference cStackNode<T>::getValue()
{
    return *reinterpret_cast<T*>(&m_value);
}
template<typename T>
cStackNode<T>* cStackNode<T>::loadNextNode()
{
    return m_pNextNode.load();
}
template<typename T>
void cStackNode<T>::storeNextNode(cStackNode<T>* i_newNode)
{
    m_pNextNode.store(i_newNode);
}
template<typename T>
bool cStackNode<T>::compareAndExchangeNextNode(cStackNode<T>* i_oldNode, cStackNode<T>* i_newNode)
{
    return m_pNextNode.compare_exchange_weak(i_oldNode,i_newNode);
}
template<typename T>
T cStackNode<T>::extractValue()
{
    return mpl::move(*reinterpret_cast<T*>(&m_value));
}
template<typename T>
void cStackNode<T>::setValue(const T& i_value)
{
    *reinterpret_cast<T*>(&m_value) = i_value;
}
template<typename T>
void cStackNode<T>::setValue(T&& i_value)
{
    *reinterpret_cast<T*>(&m_value) = mpl::move(i_value);
}
template<typename T>
void cStackNode<T>::setAsDivider(bool i_divider)
{
    m_isDivider.store(i_divider);
}
template<typename T>
bool cStackNode<T>::isDivider() const
{
    return m_isDivider.load();
}

template<typename T, typename N, template<typename> class A>
template<typename ... TT>
typename cStackImpl<T,N,A>::StackNode* cStackImpl<T,N,A>::allocate(size_t size, TT&& ... i_args) const
{
    StackNode* node = NULL;
    void *mem = m_alloc.allocate(size, sizeof(N));

    if(mem)
    {
        node = new(mem) StackNode(mpl::forward<TT>(i_args) ...);
    }

    return node;
}
template<typename T, typename N, template<typename> class A>
void cStackImpl<T,N,A>::deallocate(cStackNode<T>* node) const
{
    if(node)
    {
        //virtual mechanism takes care of calling all the related destructors
        node->~cStackNode<T>();
        m_alloc.deallocate((void *)node);
    }

    return;
}
template<typename T, typename N, template<typename> class A>
cStackImpl<T,N,A>::cStackImpl()
: detail::cConstInputIterableBaseImpl<T&>()
, m_alloc(memory::cMemoryProvider<Allocator>::acquire())
{
    StackNode* divider = allocate(1);

    MARK_AS_DIVISOR(divider);

    m_first.storeNextNode(divider);
    m_last.storeNextNode(divider);
}
template<typename T, typename N, template<typename> class A>
cStackImpl<T,N,A>::~cStackImpl()
{
    clear();

    //dealloc divider
    deallocate(m_first.loadNextNode());
}
template<typename T, typename N, template<typename> class A>
template<typename ... TT>
typename cStackImpl<T,N,A>::lvaluereference cStackImpl<T,N,A>::push(TT&& ... i_args)
{
    cStackNode<T>* lastNode = NULL;
    cStackNode<T>* insertedNode = allocate(1,i_args...);

    do
    {
        lastNode = m_last.loadNextNode();
    }
    while(m_last.compareAndExchangeNextNode(lastNode,insertedNode) == false);

    //take into account that is completely impossible to insert a new element without having divider on our right (and so we will get out
    //from this loop eventually)
    do
    {
        if(IS_DIVISOR(lastNode))
        {
            //swap contents (pointers, values and divider marks)
            lastNode->setValue(insertedNode->extractValue());
            lastNode->storeNextNode(insertedNode);
            //oublish new divider to pushers
            MARK_AS_DIVISOR(insertedNode);
            //publish new value to popers
            MARK_AS_NOT_DIVISOR(lastNode);

            break;
        }
    }
    while(true);
}
template<typename T, typename N, template<typename> class A>
ytl::optional<typename cStackImpl<T,N,A>::value_type> cStackImpl<T,N,A>::pop()
{
    if(cStackNode<T>* currNode = extractFirstNode())
    {
        value_type res = currNode->getValue();

        deallocate(currNode);

        return res;
    }

    return none;
}
template<typename T, typename N, template<typename> class A>
typename cStackImpl<T,N,A>::node_pointer_type cStackImpl<T,N,A>::getFirstElem() const
{
    return (this->empty() == false) ? detail::sendNodeToIterator<node_pointer_type>(this->extractFirstNode()) : iterator_type::m_pHead;
}
template<typename T, typename N, template<typename> class A>
typename cStackImpl<T,N,A>::node_pointer_type cStackImpl<T,N,A>::getNextElem(node_pointer_type currNode) const
{
    if(currNode != iterator_type::m_pHead)
    {
        StackNode* currStackNode = detail::receiveNodeFromIterator<StackNode*>(currNode);

        //we just use divider node to get next item
        cStackNode<T>* nextStackNode = extractFirstNode();
        deallocate(currStackNode);

        if(nextStackNode)
        {
            return detail::sendNodeToIterator<T*>(nextStackNode);
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
typename cStackImpl<T,N,A>::const_reference cStackImpl<T,N,A>::getValue(node_pointer_type currNode) const
{
    ASSERT(currNode != iterator_type::m_pHead, "Accessing to value of null node");

    StackNode* currStackNode = detail::receiveNodeFromIterator<StackNode*>(currNode);

    return currStackNode->getValue();
}
template<typename T, typename N, template<typename> class A>
typename cStackImpl<T,N,A>::reference cStackImpl<T,N,A>::getValue(node_pointer_type currNode)
{
    ASSERT(currNode != iterator_type::m_pHead, "Accessing to value of null node");

    StackNode* currStackNode = detail::receiveNodeFromIterator<StackNode*>(currNode);

    return currStackNode->getValue();
}
template<typename T, typename N, template<typename> class A>
bool cStackImpl<T,N,A>::empty() const
{
    return emptyStack();
}
template<typename T, typename N, template<typename> class A>
size_t cStackImpl<T,N,A>::getSize() const
{
    //from the beginning to divisor
    cStackNode<T>* currNode = m_first.loadNextNode();
    size_t stackSize = 0;

    while(IS_DIVISOR(currNode) == false)
    {
        currNode = (IS_DIVISOR(currNode) == false) ? currNode->loadNextNode() : currNode;

        stackSize++;
    }

    return stackSize;
}
template<typename T, typename N, template<typename> class A>
cStackNode<T>* cStackImpl<T,N,A>::extractFirstNode() const
{
    cStackNode<T>* firstNode = NULL;
    cStackNode<T>* nextNode = NULL;

    if(emptyStack() == false)
    {
        do
        {
            firstNode = m_first.loadNextNode();
            nextNode = (IS_DIVISOR(firstNode) == false) ? firstNode->loadNextNode() : firstNode;
        }
        while(m_first.compareAndExchangeNextNode(firstNode,nextNode) == false);
    }

    return (firstNode != nextNode) ? firstNode : NULL;
}
template<typename T, typename N, template<typename> class A>
void cStackImpl<T,N,A>::purge()
{
    cStackNode<T>* currNode = NULL;
    cStackNode<T>* nextNode = NULL;

    while(emptyStack() == false)
    {
        currNode = m_first.loadNextNode();
        nextNode = (currNode != &m_last) ? currNode->loadNextNode() : currNode;

        if(m_first.compareAndExchangeNextNode(currNode,nextNode))
        {
            deallocate(currNode);
        }
    }
}
template<typename T, typename N, template<typename> class A>
void cStackImpl<T,N,A>::clear()
{
    purge();
}
template<typename T, typename N, template<typename> class A>
bool cStackImpl<T,N,A>::emptyStack() const
{
    return IS_DIVISOR(m_first.loadNextNode());
}
template<typename T, typename N, template<typename> class A>
void cStackImpl<T,N,A>::releaseNode(node_pointer_type i_node) const
{
    if(i_node != iterator_type::m_pHead)
    {
        deallocate(detail::receiveNodeFromIterator<StackNode*>(i_node));
    }
}
template<typename T, typename N, template<typename> class A>
typename cStackImpl<T,N,A>::node_pointer_type cStackImpl<T,N,A>::acquireNode(node_pointer_type i_node) const
{
    //stack iterators cannot share nodes
    return iterator_type::m_pHead;
}

}
}
}
