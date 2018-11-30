
#include "YTL/container/detail/cContainerOps.h"

#define LESS_THAN(_A,_B) m_compare.eval(_A,_B)

namespace yame
{
namespace container
{
namespace detail
{

template<typename T, typename R>
template<typename ... TT>
cMapNode<T,R>::cMapNode(const R& i_key , TT&& ... i_args)
: m_pLeftChild(NULL)
, m_pRightChild(NULL)
, m_pParent(NULL)
, m_value(i_key, mpl::forward<TT>(i_args)...)
{
}
template<typename T, typename R>
void cMapNode<T,R>::replaceNode(cMapNode<T,R> *other)
{
    if(other)
    {
        //set descendants
        if(m_pLeftChild && m_pLeftChild != other)
        {
            other->m_pLeftChild = m_pLeftChild;
            other->m_pLeftChild->setParent(other);
        }
        if(m_pRightChild && m_pRightChild != other)
        {
            other->m_pRightChild = m_pRightChild;
            other->m_pRightChild->setParent(other);
        }

        //set the parent
        if(m_pParent)
        {
            if(this == m_pParent->m_pLeftChild)
            {
                m_pParent->m_pLeftChild = other;
            }
            else
            {
                m_pParent->m_pRightChild = other;
            }
        }

        other->setParent(m_pParent);

        m_pParent = NULL;
        m_pRightChild = NULL;
        m_pLeftChild = NULL;
    }


    return;
}
template<typename T, typename R>
cMapNode<T,R>* cMapNode<T,R>::getParent()
{
    return m_pParent;
}
template<typename T, typename R>
void cMapNode<T,R>::setParent(cMapNode<T,R> *parent)
{
    if(m_pParent != parent)
    {
        m_pParent = parent;
    }

    return;
}


template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
template<typename _valueType>
void cMapImpl<T,R,N,A,B>::assign(const_lkeyreference key, _valueType&& val)
{
    this->operator[](key) = mpl::forward<_valueType>(val);
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
void cMapImpl<T,R,N,A,B>::createTree(const cMapImpl<T,R,N,A,B>& tree)
{
    node_pointer_type rootNode = tree.getRootNode();

    if(rootNode != iterable_type::m_pHead)
    {
        MapNode* newNode = allocate(detail::receiveNodeFromIterator<MapNode*>(rootNode));

        this->setRootNode(detail::sendNodeToIterator<node_pointer_type>(newNode));
    }
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
void cMapImpl<T,R,N,A,B>::clearTree()
{
    node_pointer_type rootNode = this->getRootNode();

    if(rootNode != iterable_type::m_pHead);
    {
        cMapNode<T,R>* rootMapNode = detail::receiveNodeFromIterator<cMapNode<T,R>*>(rootNode);

        deallocate(rootMapNode);

        this->setRootNode(iterable_type::m_pHead);
    }
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
void cMapImpl<T,R,N,A,B>::eraseNode(node_pointer_type i_node)
{
    if(cMapNode<T,R>* pairNode = detail::receiveNodeFromIterator<cMapNode<T,R>*>(i_node))
    {
        cMapNode<T,R>* otherParent = pairNode->getParent();

        if(pairNode->m_pLeftChild && pairNode->m_pRightChild)
        {
            //complicated case
            cMapNode<T,R> *rightMostNode = findMax(pairNode->m_pLeftChild);

            if(rightMostNode)
            {
                rightMostNode->m_pParent->m_pRightChild = rightMostNode->m_pLeftChild;
            }

            if(pairNode == this->getRootNode())
            {
                this->setRootNode(static_cast<N*>(rightMostNode));
            }

            pairNode->replaceNode(rightMostNode);
       }
        else if(pairNode->m_pLeftChild)
        {//no tan complicat
            if(pairNode == this->getRootNode())
            {
                this->setRootNode(static_cast<N*>(pairNode->m_pLeftChild));
            }

            pairNode->replaceNode(pairNode->m_pLeftChild);
        }
        else if(pairNode->m_pRightChild)
        {
            if(pairNode == this->getRootNode())
            {
                this->setRootNode(static_cast<N*>(pairNode->m_pRightChild));
            }

            pairNode->replaceNode(pairNode->m_pRightChild);
        }
        else
        {
            if(pairNode->m_pParent)
            {
                if(pairNode == pairNode->m_pParent->m_pLeftChild)
                {
                    pairNode->m_pParent->m_pLeftChild = NULL;
                }
                else
                {
                    pairNode->m_pParent->m_pRightChild = NULL;
                }
            }
            else
            {//ens hem quedat sense res (no tenim pares ni fills, erem els unics!)...
                this->setRootNode(iterable_type::m_pHead);
            }
        }

        this->_onNodeErased(static_cast<MapNode*>(pairNode));

        deallocate(pairNode);
    }

    return;
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
typename cMapImpl<T,R,N,A,B>::MapNode* cMapImpl<T,R,N,A,B>::allocate(cMapNode<T,R>* node) const
{
    MapNode* newNode = NULL;
    void *mem = (void *)m_alloc.allocate(1, sizeof(MapNode));

    ASSERT(mem, "voi allocation");

    if(mem)
    {
        newNode = new(mem) MapNode(node->m_value.first, node->m_value.second);
    }

    if(node->m_pLeftChild)
    {
        newNode->m_pLeftChild = allocate(node->m_pLeftChild);
        newNode->m_pLeftChild->setParent(newNode);
    }

    if(node->m_pRightChild)
    {
        newNode->m_pRightChild = allocate(node->m_pRightChild);
        newNode->m_pRightChild->setParent(newNode);
    }

    return newNode;
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
template<typename ... TT>
typename cMapImpl<T,R,N,A,B>::MapNode* cMapImpl<T,R,N,A,B>::allocate(const_lkeyreference key, TT&& ... i_args) const
{
    MapNode*node = NULL;
    void *mem = m_alloc.allocate(1, sizeof(MapNode));

    ASSERT(mem, "void allocation");

    if(mem)
    {
        node = new(mem) MapNode(key, mpl::forward<TT>(i_args)...);
    }

    return node;
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
void cMapImpl<T,R,N,A,B>::deallocate(cMapNode<T,R> *&node) const
{
    if(node && node != iterator_type::m_pHead)
    {
        node->~cMapNode<T,R>();

        if(node->m_pLeftChild)
        {
            deallocate(node->m_pLeftChild);
        }

        if(node->m_pRightChild)
        {
            deallocate(node->m_pRightChild);
        }

        m_alloc.deallocate((void *)node);

        node = NULL;
    }

    return;
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
cMapNode<T,R>* cMapImpl<T,R,N,A,B>::findMin(cMapNode<T,R> *node) const
{
    if(node != iterable_type::m_pHead)
    {
        cMapNode<T,R>* currNode = node;

        while(currNode->m_pLeftChild) currNode = currNode->m_pLeftChild;

        return currNode;
    }
    else
    {
        return NULL;
    }
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
cMapNode<T,R>* cMapImpl<T,R,N,A,B>::findMax(cMapNode<T,R> *node) const
{
    if(node != iterable_type::m_pHead)
    {
        cMapNode<T,R>* currNode = node;

        while(currNode->m_pRightChild) currNode = currNode->m_pRightChild;

        return currNode;
    }
    else
    {
        return NULL;
    }
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
typename cMapImpl<T,R,N,A,B>::node_pointer_type cMapImpl<T,R,N,A,B>::addNode(typename mpl::PairType<const R,T>::type i_value)
{
    node_pointer_type rootNode = this->getRootNode();

    cMapNode<T,R>* rootMapNode = detail::receiveNodeFromIterator<cMapNode<T,R>*>(rootNode);

    cMapNode<T,R>* foundNode = (rootNode != iterable_type::m_pHead) ? getClosestBiggerNode(i_value.first, rootMapNode) : rootMapNode;

    if(foundNode != iterable_type::m_pHead && foundNode->m_value.first == i_value.first)
    {
        foundNode->m_value.second = mpl::forward<typename mpl::PairType<R,T>::second_type>(i_value.second);

        return foundNode;
    }
    else
    {
        MapNode* node = allocate(mpl::forward<typename mpl::PairType<R,T>::first_type>(i_value.first),mpl::forward<typename mpl::PairType<R,T>::second_type>(i_value.second));

        if(rootNode != iterable_type::m_pHead)
        {
            insert(node, foundNode);
        }
        else
        {
            this->setRootNode(node);
        }

        this->_onNodeInserted(node);

        return detail::sendNodeToIterator<node_pointer_type>(node);
    }
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
cMapImpl<T,R,N,A,B>::cMapImpl()
: detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>()
, m_alloc(memory::cMemoryProvider<A<MapNode>>::acquire())
, m_compare([](const R& i_lhs, const R& i_rhs) { return i_lhs < i_rhs; })
{
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
cMapImpl<T,R,N,A,B>::cMapImpl(const compare_func& i_compare)
: detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>()
, m_alloc(memory::cMemoryProvider<A<MapNode>>::acquire())
, m_compare(i_compare)
{
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
cMapImpl<T,R,N,A,B>::cMapImpl(const_lvaluereference value, const_lkeyreference key)
: detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>()
, m_alloc(memory::cMemoryProvider<A<MapNode>>::acquire())
, m_compare([](const R& i_lhs, const R& i_rhs) { return i_lhs < i_rhs; })
{
    addNode(pair_type{key,value});
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
cMapImpl<T,R,N,A,B>::cMapImpl(const_lvaluereference value, const_lkeyreference key, const compare_func& i_compare)
: detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>()
, m_alloc(memory::cMemoryProvider<A<MapNode>>::acquire())
, m_compare(i_compare)
{
    addNode(pair_type{key,value});
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
cMapImpl<T,R,N,A,B>::cMapImpl(const cMapImpl<T,R,N,A,B>& other)
: detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>()
, m_alloc(memory::cMemoryProvider<A<MapNode>>::acquire())
, m_compare(other.m_compare)
{
    createTree(other);
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
cMapImpl<T,R,N,A,B>::cMapImpl(cMapImpl<T,R,N,A,B>&& other)
: detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>()
, m_alloc(memory::cMemoryProvider<A<MapNode>>::acquire())
, m_compare(mpl::move(other.m_compare))
{
    this->setRootNode(other.getRootNode());
    other.setRootNode(NULL);
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
cMapImpl<T,R,N,A,B>::~cMapImpl()
{
    clearTree();
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
cMapImpl<T,R,N,A,B>& cMapImpl<T,R,N,A,B>::operator=(const cMapImpl<T,R,N,A,B>& other)
{
    clearTree();

    createTree(other);

    m_compare = other.m_compare;

    return *this;
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
cMapImpl<T,R,N,A,B>& cMapImpl<T,R,N,A,B>::operator=(cMapImpl<T,R,N,A,B>&& other)
{
    clearTree();

    this->setRootNode(other.getRootNode());

    other.setRootNode(iterable_type::m_pHead);

    m_compare = mpl::move(other.m_compare);

    return *this;
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
typename cMapImpl<T,R,N,A,B>::const_iterator_type cMapImpl<T,R,N,A,B>::search(const_lkeyreference key) const
{
    node_pointer_type rootNode = this->getRootNode();

    if(rootNode != iterable_type::m_pHead)
    {
        return this->constructConstIteratorAtNode(search(key, detail::receiveNodeFromIterator<cMapNode<T,R>*>(rootNode)));
    }

    return this->constructConstIteratorAtNode(iterator_type::m_pHead);
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
typename cMapImpl<T,R,N,A,B>::iterator_type cMapImpl<T,R,N,A,B>::search(const_lkeyreference key)
{
    node_pointer_type rootNode = this->getRootNode();

    if(rootNode != iterable_type::m_pHead)
    {
        return this->constructIteratorAtNode(search(key, detail::receiveNodeFromIterator<cMapNode<T,R>*>(rootNode)));
    }

    return this->constructIteratorAtNode(iterator_type::m_pHead);
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
typename cMapImpl<T,R,N,A,B>::const_lvaluereference cMapImpl<T,R,N,A,B>::operator[](const_lkeyreference key) const
{
    node_pointer_type rootNode = this->getRootNode();

    cMapNode<T,R>* foundNode = search(key, detail::receiveNodeFromIterator<cMapNode<T,R>*>(rootNode));

    return foundNode->m_value.second;
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
typename cMapImpl<T,R,N,A,B>::lvaluereference cMapImpl<T,R,N,A,B>::operator[](const_lkeyreference key)
{
    node_pointer_type rootNode = this->getRootNode();

    cMapNode<T,R>* foundNode = (rootNode != iterable_type::m_pHead) ? getClosestBiggerNode(key, detail::receiveNodeFromIterator<cMapNode<T,R>*>(rootNode)) : NULL;

    if(foundNode && foundNode->m_value.first == key)
    {
        return foundNode->m_value.second;
    }
    else
    {
        MapNode* node = allocate(key,T());

        if(rootNode != iterable_type::m_pHead)
        {
            insert(node, foundNode);
        }
        else
        {
            this->setRootNode(detail::sendNodeToIterator<node_pointer_type>(node));
        }

        this->_onNodeInserted(node);

        return node->m_value.second;
    }
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
template<typename ... TT>
cPair<bool,typename cMapImpl<T,R,N,A,B>::iterator_type> cMapImpl<T,R,N,A,B>::emplace(const_lkeyreference i_key, TT&& ... i_args)
{
    node_pointer_type rootNode = this->getRootNode();

    cMapNode<T,R>* foundNode = (rootNode != iterable_type::m_pHead) ? getClosestBiggerNode(i_key, detail::receiveNodeFromIterator<cMapNode<T,R>*>(rootNode)) : NULL;

    if(foundNode && foundNode->m_value.first == i_key)
    {
        return make_pair<bool,iterator_type>(false, this->constructIteratorAtNode(foundNode));
    }
    else
    {
        MapNode* node = allocate(i_key,mpl::forward<TT>(i_args)...);

        if(rootNode != iterable_type::m_pHead)
        {
            insert(node, foundNode);
        }
        else
        {
            this->setRootNode(detail::sendNodeToIterator<node_pointer_type>(node));
        }

        this->_onNodeInserted(node);

        return make_pair(true, this->constructIteratorAtNode(node));
    }
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
void cMapImpl<T,R,N,A,B>::clear()
{
    eraseSubTree(*this);

    return;
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
typename cMapImpl<T,R,N,A,B>::node_pointer_type cMapImpl<T,R,N,A,B>::getFirstElem() const
{
    cMapNode<T,R>* rootNode = this->template getTypedRootNode<cMapNode<T,R>>();

    if(cMapNode<T,R>* foundNode = findMin(rootNode))
    {
        return foundNode;
    }
    else
    {
        return iterator_type::m_pHead;
    }
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
typename cMapImpl<T,R,N,A,B>::node_pointer_type cMapImpl<T,R,N,A,B>::getLastElem() const
{
    cMapNode<T,R>* rootNode = this->template getTypedRootNode<cMapNode<T,R>>();

    if(cMapNode<T,R>* foundNode = findMax(rootNode))
    {
        return foundNode;
    }
    else
    {
        return iterator_type::m_pHead;
    }
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
typename cMapImpl<T,R,N,A,B>::node_pointer_type cMapImpl<T,R,N,A,B>::getNextElem(node_pointer_type currNode) const
{
    cMapNode<T,R>* currMapNode = static_cast<cMapNode<T,R>*>(currNode);
    cMapNode<T,R>* currNodeParent = currMapNode->getParent();
    cMapNode<T,R>* nextElem = NULL;

    if(currNodeParent)
    {
        if(currNodeParent->m_pLeftChild == currMapNode)
        {
            nextElem = (currMapNode->m_pRightChild) ? findMin(currMapNode->m_pRightChild) : currNodeParent;
        }
        else if(currMapNode->m_pRightChild)
        {
            nextElem = findMin(currMapNode->m_pRightChild);
        }
        else
        {
            //look for the first parent which is the left one
            cMapNode<T,R>* currNodeGrandParent = currNodeParent->getParent();

            while(currNodeGrandParent && currNodeParent == currNodeGrandParent->m_pRightChild)
            {
                currNodeParent = currNodeGrandParent;
                currNodeGrandParent = currNodeParent->getParent();
            }

            nextElem = (currNodeGrandParent) ? currNodeGrandParent : NULL;
        }
    }
    else if(currMapNode->m_pRightChild)
    {
        nextElem = findMin(currMapNode->m_pRightChild);
    }

    return (nextElem) ? detail::sendNodeToIterator<node_pointer_type>(nextElem) : iterable_type::m_pHead;
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
typename cMapImpl<T,R,N,A,B>::node_pointer_type cMapImpl<T,R,N,A,B>::getPrevElem(node_pointer_type currNode) const
{
    cMapNode<T,R>* currMapNode = static_cast<cMapNode<T,R>*>(currNode);
    cMapNode<T,R>* currNodeParent = currMapNode->getParent();
    cMapNode<T,R>* prevElem = NULL;

    if(currNodeParent)
    {
        if(currNodeParent->m_pRightChild == currMapNode)
        {
            prevElem = (currMapNode->m_pLeftChild) ? findMax(currMapNode->m_pLeftChild) : currNodeParent;
        }
        else if(currMapNode->m_pLeftChild)
        {
            prevElem = findMax(currMapNode->m_pLeftChild);
        }
        else
        {
            //look for the first parent which is the left one
            cMapNode<T,R>* currNodeGrandParent = currNodeParent->getParent();

            while(currNodeGrandParent && currNodeParent == currNodeGrandParent->m_pLeftChild)
            {
                currNodeParent = currNodeGrandParent;
                currNodeGrandParent = currNodeParent->getParent();
            }

            prevElem = (currNodeGrandParent) ? currNodeGrandParent : NULL;
        }
    }
    else if(currMapNode->m_pLeftChild)
    {
        return findMax(currMapNode->m_pLeftChild);
    }

    return (prevElem) ? detail::sendNodeToIterator<node_pointer_type>(prevElem) : iterable_type::m_pHead;
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
typename cMapImpl<T,R,N,A,B>::const_reference cMapImpl<T,R,N,A,B>::getValue(node_pointer_type i_currNode) const
{
    const MapNode* currMapNode = detail::receiveNodeFromIterator<const MapNode*>(i_currNode);

    return currMapNode->m_value;
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
typename cMapImpl<T,R,N,A,B>::reference cMapImpl<T,R,N,A,B>::getValue(node_pointer_type i_currNode)
{
    MapNode* currMapNode = detail::receiveNodeFromIterator<MapNode*>(i_currNode);

    return currMapNode->m_value;
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
void cMapImpl<T,R,N,A,B>::_onNodeInserted(MapNode* node)
{
    this->setRootNode(detail::sendNodeToIterator<node_pointer_type>(Balancer::balance(this,node)));
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
void cMapImpl<T,R,N,A,B>::_onNodeErased(MapNode* node)
{
    this->setRootNode(detail::sendNodeToIterator<node_pointer_type>(Balancer::balance(this,node)));
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
cMapNode<T,R>* cMapImpl<T,R,N,A,B>::getClosestBiggerNode(const R& i_key, cMapNode<T,R>* i_rootNode) const
{
    cMapNode<T,R>* currNode = i_rootNode;
    cMapNode<T,R>* candidate = i_rootNode;

    while(currNode)
    {
        if(i_key == currNode->m_value.first)
        {
            candidate = currNode;

            break;
        }
        else if(LESS_THAN(i_key,currNode->m_value.first))
        {
            candidate = currNode;

            currNode = currNode->m_pLeftChild;
        }
        else
        {
            currNode = currNode->m_pRightChild;
        }
    }

    return candidate;
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
cMapNode<T,R>* cMapImpl<T,R,N,A,B>::getClosestSmallerNode(const R& i_key, cMapNode<T,R>* i_rootNode) const
{
    cMapNode<T,R>* currNode = i_rootNode;
    cMapNode<T,R>* candidate = i_rootNode;

    while(currNode)
    {
        if(i_key == currNode->m_value.first)
        {
            candidate = currNode;

            break;
        }
        else if(LESS_THAN(i_key,currNode->m_value.first))
        {
            currNode = currNode->m_pLeftChild;
        }
        else
        {
            candidate = currNode;

            currNode = currNode->m_pRightChild;
        }
    }

    return candidate;
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
cMapNode<T,R>* cMapImpl<T,R,N,A,B>::search(const R& key, cMapNode<T,R>* rootNode) const
{
    cMapNode<T,R>*  currNode = rootNode;
    cMapNode<T,R>* candidate = NULL;

    while(currNode)
    {
        if(key == currNode->m_value.first)
        {
            candidate = currNode;
            break;
        }
        else if(LESS_THAN(key,currNode->m_value.first))
        {
            currNode = currNode->m_pLeftChild;
        }
        else
        {
            currNode = currNode->m_pRightChild;
        }
    }

    return candidate;
}
template<typename T, typename R, typename N, template<typename> class A,template<typename,typename> class B>
void cMapImpl<T,R,N,A,B>::insert(cMapNode<T,R> *other, cMapNode<T,R>* rootNode) const
{
    cMapNode<T,R>* currNode = rootNode;

    while(true)
    {
        if(LESS_THAN(other->m_value.first,currNode->m_value.first))
        {
            if(currNode->m_pLeftChild)
            {
                currNode = currNode->m_pLeftChild;
            }
            else
            {
                other->setParent(currNode);
                currNode->m_pLeftChild = other;

                break;
            }
        }
        else
        {
            if(currNode->m_pRightChild)
            {
                currNode = currNode->m_pRightChild;
            }
            else
            {
                other->setParent(currNode);
                currNode->m_pRightChild = other;

                break;
            }
        }
    }
}

}
}
}
