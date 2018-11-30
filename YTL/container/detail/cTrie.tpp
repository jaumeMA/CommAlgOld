
#define IS_TRIE_LEAVE(node) node & 0x1
#define GET_TRIE_LEAVE(node) node & ~0x1
#define SET_TRIE_LEAVE(node) node = node | 0x1;

namespace yame
{
namespace container
{
namespace impl
{

template<typename T, typename R, typename N, template<typename> class A>
const cTrieNode<T,R>* cTrieImpl<T,R,N,A>::m_pTrieHead = reinterpret_cast<const cTrieNode<T,R>*>(cTrieImpl<T,R,N,A>::iterable_type::m_pHead);

template<typename T, typename R>
template<typename ... TT>
cTrieLeave<T,R>::cTrieLeave(TT&& ... i_args)
: m_value(mpl::forward<TT>(i_args)...)
{
}

template<typename T, typename R>
cTrieNode<T,R>::cTrieNode(const particle& i_key)
: m_pParent(NULL)
, m_pLeftChild(NULL)
, m_pMiddleChild(NULL)
, m_pRightChild(NULL)
, m_pLeave(NULL)
, m_key(i_key)
{
}
template<typename T, typename R>
cTrieNode<T,R>* cTrieNode<T,R>::getParent()
{
    return m_pParent;
}
template<typename T, typename R>
void cTrieNode<T,R>::setParent(cTrieNode<T,R>* parent)
{
    if(m_pParent != parent)
    {
        m_pParent = parent;
    }
}
template<typename T, typename R>
bool cTrieNode<T,R>::hasChildren() const
{
    return m_pLeftChild != NULL || m_pMiddleChild != NULL || m_pRightChild != NULL;
}
template<typename T, typename R>
bool cTrieNode<T,R>::canMerge(cTrieNode<T,R> *from, cTrieNode<T,R> *to)
{
    if(to && from)
    {
        return (to->m_pLeftChild == NULL || from->m_pLeftChild == NULL || to->m_pLeftChild == from) && (to->m_pMiddleChild == NULL || from->m_pMiddleChild == NULL || to->m_pMiddleChild == from) && (to->m_pRightChild == NULL || from->m_pRightChild == NULL || to->m_pRightChild == from);
    }
    else if(from)
    {
        return from->m_pLeftChild == NULL && from->m_pMiddleChild == NULL && from->m_pRightChild == NULL;
    }
    else
    {
        true;
    }
}
template<typename T, typename R>
bool cTrieNode<T,R>::merge(cTrieNode<T,R> *from, cTrieNode<T,R> *to)
{
    ASSERT(canMerge(from, to), "You cannot merge this two nodes");

    if(to)
    {
        if(from->m_pLeftChild || to->m_pLeftChild == from)
        {
            to->m_pLeftChild = from->m_pLeftChild;

            if(from->m_pLeftChild)
            {
                from->m_pLeftChild->setParent(to);
                from->m_pLeftChild = NULL;
            }
        }

        if(from->m_pMiddleChild || to->m_pMiddleChild == from)
        {
            to->m_pMiddleChild = from->m_pMiddleChild;

            if(from->m_pMiddleChild)
            {
                from->m_pMiddleChild->setParent(to);
                from->m_pMiddleChild = NULL;
            }
        }

        if(from->m_pRightChild || to->m_pRightChild == from)
        {
            to->m_pRightChild = from->m_pRightChild;

            if(from->m_pRightChild)
            {
                from->m_pRightChild->setParent(to);
                from->m_pRightChild = NULL;
            }
        }
    }
    else if(from)
    {
        from->m_pLeftChild = NULL;
        from->m_pMiddleChild = NULL;
        from->m_pRightChild = NULL;
    }
}

template<typename T, typename R, typename N, template<typename> class A>
cTrieImpl<T,R,N,A>::cTrieImpl()
: detail::cBidirectionalIterableBaseImpl<cPair<const R,T&>>()
, m_alloc(memory::cMemoryProvider<Allocator>::acquire())
, m_leaveAlloc(memory::cMemoryProvider<LeaveAllocator>::acquire())
{
}
template<typename T, typename R, typename N, template<typename> class A>
cTrieImpl<T,R,N,A>::cTrieImpl(const cTrieImpl& i_other)
: detail::cBidirectionalIterableBaseImpl<cPair<const R,T&>>()
, m_alloc(memory::cMemoryProvider<Allocator>::acquire())
, m_leaveAlloc(memory::cMemoryProvider<LeaveAllocator>::acquire())
{
    createTree(i_other);
}
template<typename T, typename R, typename N, template<typename> class A>
cTrieImpl<T,R,N,A>::cTrieImpl(cTrieImpl&& i_other)
: detail::cBidirectionalIterableBaseImpl<cPair<const R,T&>>()
, m_alloc(memory::cMemoryProvider<Allocator>::acquire())
, m_leaveAlloc(memory::cMemoryProvider<LeaveAllocator>::acquire())
{
    this->setRootNode(i_other.getRootNode());

    i_other.getRootNode(NULL);
}
template<typename T, typename R, typename N, template<typename> class A>
cTrieImpl<T,R,N,A>::~cTrieImpl()
{
    clearTree();
}
template<typename T, typename R, typename N, template<typename> class A>
template<typename P, typename ... TT>
cTrieNode<T,R>* cTrieImpl<T,R,N,A>::_insert(P&& key, TT&& ... args)
{
    node_pointer_type rootNode = this->getRootNode();
    cTrieNode<T,R>* rootTrieNode = detail::receiveNodeFromIterator<cTrieNode<T,R>*>(rootNode);

    ASSERT(*key != R::invalid_value, "Passing empty chain");

    if(rootNode == iterable_type::m_pHead)
    {
        rootTrieNode = allocate(*key);

        this->setRootNode(detail::sendNodeToIterator<node_pointer_type>(rootTrieNode));
    }

    cTrieNode<T,R>* currNode = rootTrieNode;
    cTrieNode<T,R>** childNode = NULL;
    cTrieNode<T,R>* prevNode = NULL;
    cTrieNode<T,R>* parentNode = NULL;

    while(*key != R::invalid_value)
    {
        if(currNode == NULL)
        {
            currNode = allocate(*key);
            currNode->setParent(parentNode);
            prevNode = currNode;
            if(childNode)
            {
                *childNode = currNode;
            }
        }

        parentNode = currNode;

        if(currNode->m_key == *key)
        {
            childNode = &currNode->m_pMiddleChild;
            currNode = currNode->m_pMiddleChild;

            key++;
        }
        else if(currNode->m_key > *key)
        {
            childNode = &currNode->m_pLeftChild;
            currNode = currNode->m_pLeftChild;
        }
        else
        {
            childNode = &currNode->m_pRightChild;
            currNode = currNode->m_pRightChild;
        }
    }

    ASSERT(prevNode && prevNode->m_pLeave == NULL, "Inserting value in already initialized leave");

    prevNode->m_pLeave = allocateLeave(mpl::forward<TT>(args)...);

    return prevNode;
}
template<typename T, typename R, typename N, template<typename> class A>
void cTrieImpl<T,R,N,A>::_erase(cTrieNode<T,R>* currNode)
{
    ASSERT(currNode, "Erasing null node");

    //dealloc middle children as long as no other child is under current node
    bool keepRemoving = currNode;

    while(keepRemoving)
    {
        cTrieNode<T,R>* nodeToDealloc = currNode;

        currNode = currNode->getParent();

        keepRemoving = currNode != NULL && (currNode->m_pMiddleChild == NULL || currNode->m_pMiddleChild == nodeToDealloc)
                                        && (currNode->m_pLeftChild == NULL || currNode->m_pLeftChild== nodeToDealloc)
                                        && (currNode->m_pRightChild == NULL || currNode->m_pRightChild == nodeToDealloc)
                                        && currNode->m_pLeave == NULL;

        if(cTrieNode<T,R>::canMerge(nodeToDealloc, currNode))
        {
            cTrieNode<T,R>::merge(nodeToDealloc, currNode);

            deallocate(nodeToDealloc);
        }
        else if(nodeToDealloc->m_pLeave)
        {
            //the only thing we can do is to dealloc the leave
            m_leaveAlloc.deallocate(nodeToDealloc->m_pLeave);

            nodeToDealloc->m_pLeave = NULL;

            return;
        }
    }

    if(currNode == NULL)
    {
        this->setRootNode(iterable_type::m_pHead);
    }

    return;
}
template<typename T, typename R, typename N, template<typename> class A>
template<typename P>
cTrieNode<T,R>* cTrieImpl<T,R,N,A>::_search(P&& key) const
{
    node_pointer_type rootNode = this->getRootNode();

    if(rootNode != iterable_type::m_pHead)
    {
        cTrieNode<T,R>* currNode = this->template getTypedRootNode<cTrieNode<T,R>>();

        while(currNode)
        {
            if(*key == currNode->m_key)
            {
                key++;

                if(*key != R::invalid_value)
                {
                    currNode = currNode->m_pMiddleChild;
                }
                else if(currNode->m_pLeave)
                {
                    return currNode;
                }
                else
                {
                    return NULL;
                }
            }
            else if(*key < currNode->m_key)
            {
                currNode = currNode->m_pLeftChild;
            }
            else if(*key > currNode->m_key)
            {
                currNode = currNode->m_pRightChild;
            }
        }
    }

    return NULL;
}
template<typename T, typename R, typename N, template<typename> class A>
typename cTrieImpl<T,R,N,A>::const_lvaluereference cTrieImpl<T,R,N,A>::operator[](const_lkeyreference i_key) const
{
    cTrieNode<T,R>* foundNode = _search(&i_key[0]);

    return foundNode->m_pLeave->m_value;
}
template<typename T, typename R, typename N, template<typename> class A>
typename cTrieImpl<T,R,N,A>::lvaluereference cTrieImpl<T,R,N,A>::operator[](const_lkeyreference i_key)
{
    cTrieNode<T,R>* foundNode = _search(&i_key[0]);

    return (foundNode != this->getHead()) ? foundNode->m_pLeave->m_value : _insert(&i_key[0],T())->m_pLeave->m_value;
}
template<typename T, typename R, typename N, template<typename> class A>
cTrieImpl<T,R,N,A>& cTrieImpl<T,R,N,A>::operator=(const cTrieImpl<T,R,N,A>& other)
{
    clearTree();

    createTree(other);

    return *this;
}
template<typename T, typename R, typename N, template<typename> class A>
cTrieImpl<T,R,N,A>& cTrieImpl<T,R,N,A>::operator=(cTrieImpl<T,R,N,A>&& other)
{
    clearTree();

    this->setRootNode(other.getRootNode());

    other.setRootNode(iterable_type::m_pHead);

    return *this;
}
template<typename T, typename R, typename N, template<typename> class A>
typename cTrieImpl<T,R,N,A>::const_iterator_type cTrieImpl<T,R,N,A>::search(const_lkeyreference i_key) const
{
    cTrieNode<T,R>* foundNode = _search(&i_key[0]);

    if(foundNode)
    {
        return this->constructConstIteratorAtNode(detail::sendNodeToIterator<cPair<const R,T&>*>(foundNode));
    }
    else
    {
        return this->constructConstIteratorAtNode(iterable_type::m_pHead);
    }
}
template<typename T, typename R, typename N, template<typename> class A>
typename cTrieImpl<T,R,N,A>::iterator_type cTrieImpl<T,R,N,A>::search(const_lkeyreference i_key)
{
    cTrieNode<T,R>* foundNode = _search(&i_key[0]);

    if(foundNode)
    {
        return this->constructIteratorAtNode(detail::sendNodeToIterator<cPair<const R,T&>*>(foundNode));
   }
    else
    {
        return this->constructIteratorAtNode(iterable_type::m_pHead);
    }
}
template<typename T, typename R, typename N, template<typename> class A>
template<typename ... TT>
cPair<bool,typename cTrieImpl<T,R,N,A>::iterator_type> cTrieImpl<T,R,N,A>::emplace(const_lkeyreference i_key, TT&& ... i_args)
{
    cTrieNode<T,R>* foundNode = _search(&i_key[0]);

    if(foundNode)
    {
        return make_pair<bool,iterator_type>(false, this->constructIteratorAtNode(detail::sendNodeToIterator<cPair<const R,T&>*>(foundNode)));
    }
    else
    {
        return make_pair(true, this->constructIteratorAtNode(detail::sendNodeToIterator<cPair<const R,T&>*>(_insert(&i_key[0], mpl::forward<TT>(i_args)...)->m_pLeave->m_value)));
    }
}
template<typename T, typename R, typename N, template<typename> class A>
cTrieNode<T,R>* cTrieImpl<T,R,N,A>::getNextElem(cTrieNode<T,R>* i_currNode) const
{
    ASSERT(i_currNode != iterable_type::m_pHead, "Checking invalid node");

    cTrieNode<T,R>* currTrieNode = i_currNode;

    do
    {
        cTrieNode<T,R>* currNodeParent = currTrieNode->getParent();

        if(currNodeParent)
        {
            if(currNodeParent->m_pLeftChild == currTrieNode)
            {
                currTrieNode = (currTrieNode->m_pRightChild) ? findMin(currTrieNode->m_pRightChild) : (currNodeParent->m_pMiddleChild) ? findMin(currNodeParent->m_pMiddleChild) : currNodeParent;
            }
            else if(currNodeParent->m_pMiddleChild == currTrieNode)
            {
                currTrieNode = (currTrieNode->m_pRightChild) ? findMin(currTrieNode->m_pRightChild) : currNodeParent;
            }
            else if(currTrieNode->m_pRightChild)
            {
                currTrieNode = findMin(currTrieNode->m_pRightChild);
            }
            else
            {
                //look for the first parent which is the left one
                cTrieNode<T,R>* currNodeGrandParent = currNodeParent->getParent();
                bool comeFromMiddle = false;

                while(currNodeGrandParent && currNodeParent != currNodeGrandParent->m_pLeftChild)
                {
                    comeFromMiddle = currNodeParent == currNodeGrandParent->m_pMiddleChild;
                    currNodeParent = currNodeGrandParent;
                    currNodeGrandParent = currNodeParent->getParent();
                }

                if(currNodeGrandParent)
                {
                    if(currNodeGrandParent->m_pMiddleChild)
                    {
                        currTrieNode = findMin(currNodeGrandParent->m_pMiddleChild);
                    }
                    else
                    {
                        currTrieNode = currNodeGrandParent;
                    }
                }
                else if(comeFromMiddle)
                {
                    currTrieNode = currNodeParent;
                }
                else
                {
                    currTrieNode = NULL;
                }
            }
        }
        else if(currTrieNode->m_pRightChild)
        {
            currTrieNode = findMin(currTrieNode->m_pRightChild);
        }
        else
        {
            currTrieNode = NULL;
        }
    }
    while(currTrieNode && currTrieNode->m_pLeave == NULL);

    return currTrieNode;
}
template<typename T, typename R, typename N, template<typename> class A>
cTrieNode<T,R>* cTrieImpl<T,R,N,A>::getPrevElem(cTrieNode<T,R>* i_currNode) const
{
    ASSERT(i_currNode != iterable_type::m_pHead, "Checking invalid node");

    cTrieNode<T,R>* currTrieNode = i_currNode;

    do
    {
        cTrieNode<T,R>* currNodeParent = currTrieNode->getParent();

        if(currNodeParent)
        {
            if(currNodeParent->m_pRightChild == currTrieNode)
            {
                currTrieNode = (currTrieNode->m_pLeftChild) ? findMax(currTrieNode->m_pLeftChild) : (currNodeParent->m_pMiddleChild) ? findMax(currNodeParent->m_pMiddleChild) : currNodeParent;
            }
            else if(currNodeParent->m_pMiddleChild == currTrieNode)
            {
                currTrieNode = (currTrieNode->m_pLeftChild) ? findMax(currTrieNode->m_pLeftChild) : currNodeParent;
            }
            else if(currTrieNode->m_pLeftChild)
            {
                currTrieNode = findMax(currTrieNode->m_pLeftChild);
            }
            else
            {
                //look for the first parent which is the left one
                cTrieNode<T,R>* currNodeGrandParent = currNodeParent->getParent();
                bool comeFromMiddle = false;

                while(currNodeGrandParent && currNodeParent != currNodeGrandParent->m_pRightChild)
                {
                    comeFromMiddle = currNodeParent == currNodeGrandParent->m_pMiddleChild;
                    currNodeParent = currNodeGrandParent;
                    currNodeGrandParent = currNodeParent->getParent();
                }

                if(currNodeGrandParent)
                {
                    if(currNodeGrandParent->m_pMiddleChild)
                    {
                        currTrieNode = findMax(currNodeGrandParent->m_pMiddleChild);
                    }
                    else
                    {
                        currTrieNode = currNodeGrandParent;
                    }
                }
                else if(comeFromMiddle)
                {
                    currTrieNode = currNodeParent;
                }
                else
                {
                    currTrieNode = NULL;
                }
            }
        }
        else if(currTrieNode->m_pLeftChild)
        {
            currTrieNode = findMax(currTrieNode->m_pLeftChild);
        }
        else
        {
            currTrieNode = NULL;
        }
    }
    while(currTrieNode && currTrieNode->m_pLeave == NULL);

    return currTrieNode;
}
template<typename T, typename R, typename N, template<typename> class A>
typename cTrieImpl<T,R,N,A>::node_pointer_type cTrieImpl<T,R,N,A>::getFirstElem() const
{
    node_pointer_type rootNode = this->getRootNode();

    if(rootNode != iterable_type::m_pHead)
    {
        if(cTrieNode<T,R>* foundNode = findMin(detail::receiveNodeFromIterator<cTrieNode<T,R>*>(rootNode)))
        {
            while(foundNode && foundNode->m_pLeave == NULL)
            {
                foundNode = getNextElem(foundNode);
            }

            return detail::sendNodeToIterator<node_pointer_type>(foundNode);
        }
    }

    return iterable_type::m_pHead;
}
template<typename T, typename R, typename N, template<typename> class A>
typename cTrieImpl<T,R,N,A>::node_pointer_type cTrieImpl<T,R,N,A>::getLastElem() const
{
    node_pointer_type rootNode = this->getRootNode();

    if(rootNode != iterable_type::m_pHead)
    {
        if(cTrieNode<T,R>* foundNode = findMax(detail::receiveNodeFromIterator<cTrieNode<T,R>*>(rootNode)))
        {
            while(foundNode && foundNode->m_pLeave == NULL)
            {
                foundNode = getPrevElem(foundNode);
            }

            return detail::sendNodeToIterator<node_pointer_type>(foundNode);
        }
    }

    return iterable_type::m_pHead;
}
template<typename T, typename R, typename N, template<typename> class A>
typename cTrieImpl<T,R,N,A>::node_pointer_type cTrieImpl<T,R,N,A>::getNextElem(node_pointer_type i_currNode) const
{
    cTrieNode<T,R>* currTrieNode = detail::receiveNodeFromIterator<cTrieNode<T,R>*>(i_currNode);

    cTrieNode<T,R>* nextNode = getNextElem(currTrieNode);

    return (nextNode) ? detail::sendNodeToIterator<cPair<const R,T&>*>(nextNode) : iterator_type::m_pHead;
}
template<typename T, typename R, typename N, template<typename> class A>
typename cTrieImpl<T,R,N,A>::node_pointer_type cTrieImpl<T,R,N,A>::getPrevElem(node_pointer_type currNode) const
{
    cTrieNode<T,R>* currTrieNode = detail::receiveNodeFromIterator<cTrieNode<T,R>*>(currNode);

    cTrieNode<T,R>* prevNode = getPrevElem(currTrieNode);

    return (prevNode) ? detail::sendNodeToIterator<cPair<const R,T&>*>(prevNode) : iterator_type::m_pHead;
}
template<typename T, typename R, typename N, template<typename> class A>
void cTrieImpl<T,R,N,A>::eraseNode(node_pointer_type i_pair)
{
    cTrieNode<T,R>* currNode = detail::receiveNodeFromIterator<cTrieNode<T,R>*>(i_pair);

    _erase(currNode);
}
template<typename T, typename R, typename N, template<typename> class A>
typename cTrieImpl<T,R,N,A>::node_pointer_type cTrieImpl<T,R,N,A>::addNode(typename mpl::PairType<const R,T&>::type i_value)
{
    cTrieNode<T,R>* newNode = _insert(&i_value.first[0], i_value.second);

    return detail::sendNodeToIterator<node_pointer_type>(newNode);
}
template<typename T, typename R, typename N, template<typename> class A>
const cPair<const R,T&> cTrieImpl<T,R,N,A>::getValue(node_pointer_type currNode) const
{
    cTrieNode<T,R>* currTrieNode = detail::receiveNodeFromIterator<cTrieNode<T,R>*>(currNode);
    cTrieNode<T,R>* prevNode = NULL;

    ASSERT(currTrieNode->m_pLeave != NULL, "Accessing null leave");

    T& currTrieValue = currTrieNode->m_pLeave->m_value;

    R currTrieKey;

    while(currTrieNode)
    {
        currTrieNode->m_key >> currTrieKey;

        do
        {
            prevNode = currTrieNode;
            currTrieNode = currTrieNode->m_pParent;
        }
        while(currTrieNode && currTrieNode->m_pMiddleChild != prevNode);
    }

    return make_pair<const R,T&>(currTrieKey, currTrieValue);
}
template<typename T, typename R, typename N, template<typename> class A>
cPair<const R,T&> cTrieImpl<T,R,N,A>::getValue(node_pointer_type currNode)
{
    cTrieNode<T,R>* currTrieNode = detail::receiveNodeFromIterator<cTrieNode<T,R>*>(currNode);
    cTrieNode<T,R>* prevNode = NULL;

    ASSERT(currTrieNode->m_pLeave != NULL, "Accessing null leave");

    T& currTrieValue = currTrieNode->m_pLeave->m_value;

    R currTrieKey;

    while(currTrieNode)
    {
        currTrieNode->m_key >> currTrieKey;

        do
        {
            prevNode = currTrieNode;
            currTrieNode = currTrieNode->m_pParent;
        }
        while(currTrieNode && currTrieNode->m_pMiddleChild != prevNode);
    }

    return make_pair<const R,T&>(currTrieKey, currTrieValue);
}
template<typename T, typename R, typename N, template<typename> class A>
void cTrieImpl<T,R,N,A>::createTree(const cTrieImpl<T,R,N,A>& tree)
{
    node_pointer_type rootNode = tree.getRootNode();

    if(rootNode != iterable_type::m_pHead)
    {
        MapNode* newNode = allocate(detail::receiveNodeFromIterator<cTrieNode<T,R>*>(rootNode));

        this->setRootNode(detail::sendNodeToIterator<node_pointer_type>(newNode));
    }
}
template<typename T, typename R, typename N, template<typename> class A>
void cTrieImpl<T,R,N,A>::clearTree()
{
    node_pointer_type rootNode = this->getRootNode();

    if(rootNode != iterable_type::m_pHead)
    {
        cTrieNode<T,R>* rootTrieNode = detail::receiveNodeFromIterator<cTrieNode<T,R>*>(rootNode);

        deallocate(rootTrieNode);

        this->setRootNode(iterable_type::m_pHead);
    }
}
template<typename T, typename R, typename N, template<typename> class A>
typename cTrieImpl<T,R,N,A>::MapNode* cTrieImpl<T,R,N,A>::allocate(cTrieNode<T,R>* node)
{
    MapNode* newNode = allocate(node->m_key);

    if(node->m_pLeave)
    {
        newNode->m_pLeave = allocateLeave(node->m_pLeave->m_value);
    }
    if(node->m_pLeftChild)
    {
        newNode->m_pLeftChild = allocate(node->m_pLeftChild);
        newNode->m_pLeftChild->setParent(newNode);
    }
    if(node->m_pMiddleChild)
    {
        newNode->m_pMiddleChild = allocate(node->m_pMiddleChild);
        newNode->m_pMiddleChild->setParent(newNode);
    }
    if(node->m_pRightChild)
    {
        newNode->m_pRightChild = allocate(node->m_pRightChild);
        newNode->m_pRightChild->setParent(newNode);
    }

    return newNode;
}
template<typename T, typename R, typename N, template<typename> class A>
typename cTrieImpl<T,R,N,A>::MapNode* cTrieImpl<T,R,N,A>::allocate(const particle& i_key)
{
    MapNode*node = NULL;
    void *mem = m_alloc.allocate(1, sizeof(MapNode));

    if(mem)
    {
        node = new(mem) MapNode(i_key);
    }

    return node;
}
template<typename T, typename R, typename N, template<typename> class A>
template<typename ... TT>
cTrieLeave<T,R>* cTrieImpl<T,R,N,A>::allocateLeave(TT&& ... i_args)
{
    cTrieLeave<T,R>* node = NULL;
    void *mem = m_leaveAlloc.allocate(1, sizeof(cTrieLeave<T,R>));

    if(mem)
    {
        node = new(mem) cTrieLeave<T,R>(mpl::forward<TT>(i_args)...);
    }

    return node;
}
template<typename T, typename R, typename N, template<typename> class A>
void cTrieImpl<T,R,N,A>::deallocate(cTrieNode<T,R>*& node)
{
    if(node)
    {
        if(node->m_pLeave)
        {
            m_leaveAlloc.deallocate(node->m_pLeave);
            node->m_pLeave = NULL;
        }
        if(node->m_pLeftChild)
        {
            deallocate(node->m_pLeftChild);
        }
        if(node->m_pMiddleChild)
        {
            deallocate(node->m_pMiddleChild);
        }
        if(node->m_pRightChild)
        {
            deallocate(node->m_pRightChild);
        }

        m_alloc.deallocate(node);

        node = NULL;
    }
}
template<typename T, typename R, typename N, template<typename> class A>
cTrieNode<T,R>* cTrieImpl<T,R,N,A>::findMin(cTrieNode<T,R>* node) const
{
    if(node)
    {
        cTrieNode<T,R>* currNode = node;

        while(currNode->m_pLeftChild || currNode->m_pMiddleChild)
        {
            currNode = (currNode->m_pLeftChild) ? currNode->m_pLeftChild : currNode->m_pMiddleChild;
        }

        return currNode;
    }
    else
    {
        return NULL;
    }
}
template<typename T, typename R, typename N, template<typename> class A>
cTrieNode<T,R>* cTrieImpl<T,R,N,A>::findMax(cTrieNode<T,R>* node) const
{
    if(node)
    {
        cTrieNode<T,R>* currNode = node;

        while(currNode->m_pRightChild || currNode->m_pMiddleChild)
        {
            currNode = (currNode->m_pRightChild) ? currNode->m_pRightChild : currNode->m_pMiddleChild;
        }

        return currNode;
    }
    else
    {
        return NULL;
    }
}

}
}
}
