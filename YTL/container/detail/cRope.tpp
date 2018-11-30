
#include "YTL/container/cTreeBalancer.h"
#include "YTL/container/cQueue.h"

namespace yame
{
namespace container
{
namespace detail
{

template<typename StringAllocator>
cRopeLeave<StringAllocator>::cRopeLeave(const cString<StringAllocator>& i_content)
: m_content(i_content)
{
}
template<typename StringAllocator>
void cRopeLeave<StringAllocator>::setContent(const cString<StringAllocator>& i_content)
{
    m_content = i_content;
}
template<typename StringAllocator>
const cString<StringAllocator>& cRopeLeave<StringAllocator>::getContent() const
{
    return m_content;
}
template<typename StringAllocator>
cString<StringAllocator>& cRopeLeave<StringAllocator>::getContent()
{
    return m_content;
}
template<typename StringAllocator>
size_t cRopeLeave<StringAllocator>::getContentSize() const
{
    return m_content.getSize();
}
template<typename StringAllocator>
typename cRopeLeave<StringAllocator>::pointer_type cRopeLeave<StringAllocator>::getFirstElem()
{
    return &m_content.front();
}
template<typename StringAllocator>
typename cRopeLeave<StringAllocator>::pointer_type cRopeLeave<StringAllocator>::getLastElem()
{
    return &m_content.back();
}
template<typename StringAllocator>
typename cRopeLeave<StringAllocator>::pointer_type cRopeLeave<StringAllocator>::getNextElem(pointer_type i_currElem)
{
    return (i_currElem - &m_content.front() + 1 < m_content.getSize()) ? i_currElem+1 : NULL;
}
template<typename StringAllocator>
typename cRopeLeave<StringAllocator>::pointer_type cRopeLeave<StringAllocator>::getPrevElem(pointer_type i_currElem)
{
    return (i_currElem != &m_content.front()) ? i_currElem-1 : NULL;
}
template<typename StringAllocator>
size_t cRopeLeave<StringAllocator>::getIndexOfNode(pointer_type node)
{
    return node - &m_content.front();
}

template<typename StringAllocator>
cRopeNode<StringAllocator>::~cRopeNode()
{
    if(m_pParent)
    {
        if(m_pParent->getLeftChild() == this)
        {
            m_pParent->setLeftChild(NULL);
        }
        else
        {
            m_pParent->setRightChild(NULL);
        }
    }
}
template<typename StringAllocator>
void cRopeNode<StringAllocator>::setParent(cRopeNode<StringAllocator>* i_parent)
{
    m_pParent = i_parent;
}
template<typename StringAllocator>
void cRopeNode<StringAllocator>::setLeftChild(cRopeNode<StringAllocator>* i_leftChild)
{
    if(m_pLeftChild = i_leftChild)
    {
        m_pLeftChild->setParent(this);
    }
}
template<typename StringAllocator>
void cRopeNode<StringAllocator>::setRightChild(cRopeNode<StringAllocator>* i_rightChild)
{
    if(m_pRightChild = i_rightChild)
    {
        m_pRightChild->setParent(this);
    }
}
template<typename StringAllocator>
void cRopeNode<StringAllocator>::setLeaveNode(cRopeLeave<StringAllocator>* i_leave)
{
    m_pLeave = i_leave;

    m_weight = (m_pLeave) ? m_pLeave->getContentSize() : 0;
}
template<typename StringAllocator>
void cRopeNode<StringAllocator>::reParent(cRopeNode<StringAllocator>* i_oldChild, cRopeNode<StringAllocator>* i_newChild)
{
    if(i_oldChild == m_pLeftChild)
    {
        i_oldChild->setParent(NULL);
        i_newChild->setParent(this);
        m_pLeftChild = i_newChild;
    }
    else if(i_oldChild == m_pRightChild)
    {
        i_oldChild->setParent(NULL);
        i_newChild->setParent(this);
        m_pRightChild = i_newChild;
    }
}
template<typename StringAllocator>
cRopeNode<StringAllocator>* cRopeNode<StringAllocator>::getParent() const
{
    return m_pParent;
}
template<typename StringAllocator>
cRopeNode<StringAllocator>* cRopeNode<StringAllocator>::getLeftChild() const
{
    return m_pLeftChild;
}
template<typename StringAllocator>
cRopeNode<StringAllocator>* cRopeNode<StringAllocator>::getRightChild() const
{
    return m_pRightChild;
}
template<typename StringAllocator>
cRopeLeave<StringAllocator>* cRopeNode<StringAllocator>::getLeaveNode() const
{
    return m_pLeave;
}
template<typename StringAllocator>
void cRopeNode<StringAllocator>::setWeight(size_t i_weight)
{
    m_weight = i_weight;
}
template<typename StringAllocator>
size_t cRopeNode<StringAllocator>::getWeight() const
{
    return m_weight;
}
template<typename StringAllocator>
void cRopeNode<StringAllocator>::addWeight(size_t i_val)
{
    m_weight += i_val;
}
template<typename StringAllocator>
void cRopeNode<StringAllocator>::subsWeight(size_t i_val)
{
    ASSERT(m_weight >= i_val, "Droping more weight than it has");
    m_weight -= i_val;
}
template<typename StringAllocator>
void cRopeNode<StringAllocator>::setHeight(s8 i_height)
{
    m_height = i_height;
}
template<typename StringAllocator>
s8 cRopeNode<StringAllocator>::getHeight() const
{
    return m_height;
}
template<typename StringAllocator>
void cRopeNode<StringAllocator>::updateWeight()
{
    if(m_pLeave)
    {
        m_weight = m_pLeave->getContentSize();
    }
    else
    {
        const size_t leftWeight = (m_pLeftChild) ? m_pLeftChild->getWeight() : 0;
        const size_t rightWeight = (m_pRightChild) ? m_pRightChild->getWeight() : 0;

        m_weight = leftWeight + rightWeight;
    }

    if(m_pParent)
    {
        m_pParent->updateWeight();
    }
}
template<typename StringAllocator>
bool cRopeNode<StringAllocator>::hasChildren() const
{
    return m_pLeftChild || m_pRightChild;
}
template<typename StringAllocator>
size_t cRopeNode<StringAllocator>::getNumChildren() const
{
    return static_cast<size_t>(m_pLeftChild != NULL) + static_cast<size_t>(m_pRightChild != NULL);
}

}

template<template<typename> class Allocator, typename StringAllocator>
cRope<Allocator,StringAllocator>::cRope()
: m_alloc(memory::cMemoryProvider<Allocator<RopeNode>>::acquire())
, m_allocLeave(memory::cMemoryProvider<Allocator<RopeLeave>>::acquire())
{
}
template<template<typename> class Allocator, typename StringAllocator>
cRope<Allocator,StringAllocator>::cRope(cRope&& other)
: m_alloc(memory::cMemoryProvider<Allocator<RopeNode>>::acquire())
, m_allocLeave(memory::cMemoryProvider<Allocator<RopeLeave>>::acquire())
{
    this->setRootNode(other.getRootNode());
    other.setRootNode(NULL);
}
template<template<typename> class Allocator, typename StringAllocator>
cRope<Allocator,StringAllocator>::cRope(const char* i_str)
: m_alloc(memory::cMemoryProvider<Allocator<RopeNode>>::acquire())
, m_allocLeave(memory::cMemoryProvider<Allocator<RopeLeave>>::acquire())
{
    insert(this->begin(),i_str);
}
template<template<typename> class Allocator, typename StringAllocator>
cRope<Allocator,StringAllocator>::~cRope()
{
    clear();
}
template<template<typename> class Allocator, typename StringAllocator>
void cRope<Allocator,StringAllocator>::clear()
{
    RopeNode* rootNode = this->template getTypedRootNode<RopeNode>();

    if(rootNode != iterable_type::m_pHead)
    {
        deallocate(rootNode);

        this->setRootNode(iterable_type::m_pHead);
    }
}
template<template<typename> class Allocator, typename StringAllocator>
size_t cRope<Allocator,StringAllocator>::getSize() const
{
    size_t res = 0;

    node_pointer_type currPointer = getFirstElem();
    if(currPointer != iterator_type::m_pHead)
    {
        iterator_pair_pointer currIterator = detail::receiveNodeFromIterator<iterator_pair_pointer>(currPointer);
        RopeNode* currNode = currIterator->currRope;

        while(currNode != iterable_type::m_pHead)
        {
            RopeLeave* currLeave = currNode->getLeaveNode();
            res += (currLeave) ? currLeave->getContentSize() : 0;
        }
    }

    return res;
}
template<template<typename> class Allocator, typename StringAllocator>
bool cRope<Allocator,StringAllocator>::empty() const
{
    RopeNode* rootNode = this->template getTypedRootNode<RopeNode>();

    if(rootNode != iterator_type::m_pHead)
    {
        return rootNode->getWeight() > 0;
    }
    else
    {
        return true;
    }
}
template<template<typename> class Allocator, typename StringAllocator>
typename cRope<Allocator,StringAllocator>::iterator_type cRope<Allocator,StringAllocator>::insert(iterator_type i_it, const cString<StringAllocator>& i_content)
{
    RopeNode* rootNode = this->template getTypedRootNode<RopeNode>();

    if(rootNode == iterable_type::m_pHead)
    {
        rootNode = allocate();
        this->setRootNode(detail::sendNodeToIterator<node_pointer_type>(rootNode));
    }

    size_t initPos = npos;
    RopeNode* currRope = rootNode;

    node_pointer_type currIteratorNode = this->getCurrentNode(i_it);
    if(currIteratorNode == iterator_type::m_pHead)
    {
        currRope = findMax(currRope);
        initPos = (currRope->getLeaveNode()) ? currRope->getLeaveNode()->getContentSize() : 0;
    }
    else
    {
        iterator_pair_pointer itPair = detail::receiveNodeFromIterator<iterator_pair_pointer>(currIteratorNode);
        currRope = itPair->currRope;
        initPos = (currRope->getLeaveNode()) ? currRope->getLeaveNode()->getIndexOfNode(itPair->currLeaveNode) : 0;
    }

    //divide insertion into k_chunkSize portions
    const size_t numChunks = i_content.getSize() / k_chunkSize;
    for(size_t chunkId=0;chunkId<=numChunks;++chunkId)
    {
        const size_t chunkPos = chunkId * k_chunkSize;
        const size_t maxStr = (i_content.getSize() < (chunkId + 1) * k_chunkSize) ? i_content.getSize() - chunkId * k_chunkSize : k_chunkSize;
        currRope = insert(currRope,initPos,i_content.subStr(chunkPos,maxStr));
        initPos = currRope->getLeaveNode()->getContentSize();
    }

    return this->constructIteratorAtNode(detail::sendNodeToIterator<node_pointer_type>(create_iterator(currRope,currRope->getLeaveNode()->getLastElem())));
}
template<template<typename> class Allocator, typename StringAllocator>
typename cRope<Allocator,StringAllocator>::RopeNode* cRope<Allocator,StringAllocator>::insert(RopeNode* i_rootNode, RopeLeave* i_leave, size_t i_pos, const cString<StringAllocator>& i_content)
{
    ASSERT(i_rootNode->hasChildren() == false, "Unconsistent situation");

    RopeNode* currNode = i_rootNode;

    if(i_content.getSize() + i_leave->getContentSize() < k_chunkSize)
    {
        cString<StringAllocator>& thisStr = i_leave->getContent();
        thisStr.insert(thisStr.begin() + i_pos,i_content.cbegin(),i_content.cend());
        i_rootNode->addWeight(i_content.getSize());
    }
    else
    {
        i_rootNode->setLeaveNode(NULL);

        RopeNode* newLeftNode = allocate();
        cString<StringAllocator> leftStr;
        cString<StringAllocator> rightStr;
        cString<StringAllocator> oldLeftStr = i_leave->getContent();

        if(oldLeftStr.getSize() + i_content.getSize() < k_chunkSize)
        {
            leftStr = oldLeftStr.subStr(0,i_pos) + i_content + oldLeftStr.subStr(i_pos + i_content.getSize(),oldLeftStr.getSize() - i_pos);
        }
        else if(i_pos + i_content.getSize() < k_chunkSize)
        {
            cString<StringAllocator> oldLeftStr = i_leave->getContent();
            const size_t leftDiv = k_chunkSize - i_content.getSize() - i_pos;
            leftStr = oldLeftStr.subStr(0,i_pos) + i_content + oldLeftStr.subStr(i_pos,leftDiv);
            rightStr = oldLeftStr.subStr(i_pos + leftDiv);
        }
        else
        {
            cString<StringAllocator> oldLeftStr = i_leave->getContent();
            const size_t contentDiv = k_chunkSize - i_pos;
            leftStr = oldLeftStr.subStr(0,i_pos) + i_content.subStr(0,contentDiv);
            rightStr = i_content.subStr(contentDiv) + oldLeftStr.subStr(i_pos);
        }

        i_leave->setContent(leftStr);
        newLeftNode->setLeaveNode(i_leave);
        i_rootNode->setLeftChild(newLeftNode);
        i_rootNode->setWeight(newLeftNode->getWeight());
        currNode = newLeftNode;

        if(rightStr.empty() == false)
        {
            RopeNode* newRightNode = allocate();
            RopeLeave* newLeave = allocate(rightStr);
            newRightNode->setLeaveNode(newLeave);
            i_rootNode->setRightChild(newRightNode);
            currNode = newRightNode;
        }
    }

    return currNode;
}
template<template<typename> class Allocator, typename StringAllocator>
typename cRope<Allocator,StringAllocator>::RopeNode* cRope<Allocator,StringAllocator>::insert(RopeNode* i_rootNode, size_t i_pos, const cString<StringAllocator>& i_content)
{
    RopeNode* currNode = i_rootNode;
    bool done = false;

    while(!done)
    {
        if(RopeLeave* leaveNode = currNode->getLeaveNode())
        {
            currNode = insert(currNode,leaveNode,i_pos,i_content);

            this->setRootNode(detail::cAVLBalancer<cRope<Allocator,StringAllocator>,detail::cRopeNode<StringAllocator>>::balance(this,i_rootNode));
            done = true;
        }
        else
        {
            if(currNode->getWeight() < i_pos)
            {
                if(RopeNode* rightChild = currNode->getRightChild())
                {
                    i_pos -= currNode->getWeight();
                    currNode = rightChild;
                }
                else
                {
                    rightChild = allocate();
                    RopeLeave* newLeave = allocate(i_content);
                    rightChild->setLeaveNode(newLeave);
                    currNode->setRightChild(rightChild);
                    currNode = rightChild;

                    this->setRootNode(detail::cAVLBalancer<cRope<Allocator,StringAllocator>,detail::cRopeNode<StringAllocator>>::balance(this,i_rootNode));
                    done = true;
                }
            }
            else
            {
                if(RopeNode* leftChild = currNode->getLeftChild())
                {
                    currNode = leftChild;
                }
                else
                {
                    leftChild = allocate();
                    RopeLeave* newLeave = allocate(i_content);
                    leftChild->setLeaveNode(newLeave);
                    currNode->setLeftChild(leftChild);
                    currNode = leftChild;

                    this->setRootNode(detail::cAVLBalancer<cRope<Allocator,StringAllocator>,detail::cRopeNode<StringAllocator>>::balance(this,i_rootNode));
                    done = true;
                }
            }
        }
    }

    if(currNode)
    {
        currNode->updateWeight();
    }

    return currNode;
}
template<template<typename> class Allocator, typename StringAllocator>
const c8& cRope<Allocator,StringAllocator>::getValue(node_pointer_type currNode) const
{
    const iterator_pair_pointer currIterator = detail::receiveNodeFromIterator<iterator_pair_pointer>(currNode);

    return *(currIterator->currLeaveNode);
}
template<template<typename> class Allocator, typename StringAllocator>
c8& cRope<Allocator,StringAllocator>::getValue(node_pointer_type currNode)
{
    iterator_pair_pointer currIterator = detail::receiveNodeFromIterator<iterator_pair_pointer>(currNode);

    return *(currIterator->currLeaveNode);
}
template<template<typename> class Allocator, typename StringAllocator>
typename cRope<Allocator,StringAllocator>::RopeNode* cRope<Allocator,StringAllocator>::search(RopeNode* i_rootNode, size_t i_pos)
{
    RopeNode* currNode = i_rootNode;

    while(currNode)
    {
        if(currNode->getWeight() > i_pos)
        {
            if(RopeLeave* currLeave = currNode->getLeaveNode())
            {
                cString<StringAllocator> leaveStr = currLeave->getContent();

                return (i_pos < leaveStr.getSize()) ? currNode : iterable_type::m_pHead;
            }
            else
            {
                currNode = currNode->getLeftChild();
            }
        }
        else
        {
            currNode = currNode->getRightChild();
        }
    }

    return currNode;
}
template<template<typename> class Allocator, typename StringAllocator>
typename cRope<Allocator,StringAllocator>::iterator_type cRope<Allocator,StringAllocator>::remove(iterator_type i_itInit, iterator_type i_itEnd)
{
    node_pointer_type initNode = this->getCurrentNode(i_itInit);
    node_pointer_type endNode = this->getCurrentNode(i_itEnd);

    if(initNode != iterator_type::m_pHead)
    {
        iterator_pair_pointer initPair = detail::receiveNodeFromIterator<iterator_pair_pointer>(initNode);
        iterator_pair_pointer endPair = (endNode != iterable_type::m_pHead) ? detail::receiveNodeFromIterator<iterator_pair_pointer>(endNode) : NULL;

        return this->constructIteratorAtNode(detail::sendNodeToIterator<node_pointer_type>(remove(initPair,endPair)));
    }
    else
    {
        return this->constructIteratorAtNode(iterator_type::m_pHead);
    }
}
template<template<typename> class Allocator, typename StringAllocator>
typename cRope<Allocator,StringAllocator>::iterator_pair_pointer cRope<Allocator,StringAllocator>::remove(iterator_pair_pointer i_initNode, iterator_pair_pointer i_endNode)
{
    RopeNode* initCurrRope = i_initNode->currRope;
    RopeLeave* initLeave = initCurrRope->getLeaveNode();
    RopeNode* endCurrRope = NULL;
    RopeLeave* endLeave = NULL;
    size_t initPos = initLeave->getIndexOfNode(i_initNode->currLeaveNode);
    size_t endPos = npos;

    if(i_endNode)
    {
        endCurrRope = i_endNode->currRope;
        endLeave = endCurrRope->getLeaveNode();
        endPos = endLeave->getIndexOfNode(i_endNode->currLeaveNode);
    }

    if(initCurrRope == endCurrRope)
    {
        const cString<StringAllocator>& content = initLeave->getContent();
        initLeave->setContent(content.subStr(0,initPos) + content.subStr(endPos));

        return create_iterator(initCurrRope,initCurrRope->getLeaveNode()->getFirstElem() + initPos);
    }
    else
    {
        if(initPos > 0)
        {
            const cString<StringAllocator>& contentInit = initLeave->getContent();
            initLeave->setContent(contentInit.subStr(0,initPos));
            initCurrRope = getNextElem(initCurrRope);
        }

        if(endPos > 0)
        {
            const cString<StringAllocator>& contentEnd = endLeave->getContent();
            endLeave->setContent(contentEnd.subStr(endPos));
        }
    }

    if(initCurrRope == NULL)
    {
        return NULL;
    }
    else
    {
        while(initCurrRope != endCurrRope)
        {
            RopeNode* currNodeParent = initCurrRope->getParent();
            RopeNode* nextNode = getNextElem(initCurrRope);
            deallocate(initCurrRope);
            if(currNodeParent)
            {
                prune(currNodeParent);
            }
            initCurrRope = nextNode;
        }

        return create_iterator(initCurrRope,initCurrRope->getLeaveNode()->getLastElem());
    }
}
template<template<typename> class Allocator, typename StringAllocator>
cString<StringAllocator> cRope<Allocator,StringAllocator>::subStr(const_iterator_type i_itInit, const_iterator_type i_itEnd) const
{
    cString<StringAllocator> res;

    node_pointer_type initNode = this->getCurrentNode(i_itInit);
    if(initNode != iterator_type::m_pHead)
    {
        iterator_pair_pointer itInitPair = detail::receiveNodeFromIterator<iterator_pair_pointer>(initNode);
        RopeNode* initRopeNode = itInitPair->currRope;
        RopeNode* endRopeNode = NULL;

        node_pointer_type endNode = this->getCurrentNode(i_itEnd);
        if(endNode != iterator_type::m_pHead)
        {
            iterator_pair_pointer itEndPair = detail::receiveNodeFromIterator<iterator_pair_pointer>(endNode);
            endRopeNode = itEndPair->currRope;
        }

        while(initRopeNode != endRopeNode)
        {
            const RopeLeave* currLeave = initRopeNode->getLeaveNode();
            res += currLeave->getContent();
            initRopeNode = getNextElem(initRopeNode);
        }
    }

    return res;
}
template<template<typename> class Allocator, typename StringAllocator>
detail::cRopeNode<StringAllocator>* cRope<Allocator,StringAllocator>::allocate() const
{
    RopeNode* node = NULL;
    void *mem = m_alloc.allocate(1, sizeof(RopeNode));

    ASSERT(mem, "void allocation");

    if(mem)
    {
        node = new(mem) RopeNode();
    }

    return node;
}
template<template<typename> class Allocator, typename StringAllocator>
detail::cRopeNode<StringAllocator>* cRope<Allocator,StringAllocator>::allocate(RopeNode* i_node) const
{
    detail::cRopeLeave<StringAllocator>* node = NULL;

    if(i_node)
    {
        void *mem = m_allocLeave.allocate(1, sizeof(RopeLeave));

        ASSERT(mem, "void allocation");

        if(mem)
        {
            node = new(mem) RopeLeave(*i_node);
        }

        if(i_node->m_pLeftChild)
        {
            node->m_pLeftChild = allocate(i_node->m_pRightChild);
        }
        if(i_node->m_pRightChild)
        {
            node->m_pRightChild = allocate(i_node->m_pRightChild);
        }
    }

    return node;
}
template<template<typename> class Allocator, typename StringAllocator>
detail::cRopeLeave<StringAllocator>* cRope<Allocator,StringAllocator>::allocate(const cString<StringAllocator>& i_content) const
{
    detail::cRopeLeave<StringAllocator>* node = NULL;
    void *mem = m_allocLeave.allocate(1, sizeof(RopeLeave));

    ASSERT(mem, "void allocation");

    if(mem)
    {
        node = new(mem) RopeLeave(i_content);
    }

    return node;
}
template<template<typename> class Allocator, typename StringAllocator>
void cRope<Allocator,StringAllocator>::deallocate(RopeNode* i_node) const
{
    if(RopeNode* leftChild = i_node->getLeftChild())
    {
        deallocate(leftChild);
    }

    if(RopeNode* rightChild = i_node->getRightChild())
    {
        deallocate(rightChild);
    }

    if(RopeLeave* leave = i_node->getLeaveNode())
    {
        deallocate(leave);
    }

    i_node->~RopeNode();

    m_alloc.deallocate((void *)i_node);

    i_node = NULL;

    return;
}
template<template<typename> class Allocator, typename StringAllocator>
void cRope<Allocator,StringAllocator>::deallocate(RopeLeave* i_leave) const
{
    i_leave->~RopeLeave();

    m_allocLeave.deallocate((void*)i_leave);

    i_leave = NULL;

    return;
}
template<template<typename> class Allocator, typename StringAllocator>
typename cRope<Allocator,StringAllocator>::RopeNode* cRope<Allocator,StringAllocator>::findMin(RopeNode* node) const
{
    if(node)
    {
        RopeNode* currNode = node;

        while(RopeNode* leftChild = currNode->getLeftChild())
        {
            currNode = leftChild;
        }

        if(currNode->getLeaveNode())
        {
            return currNode;
        }
        else if(RopeNode* rightChild = currNode->getRightChild())
        {
            return findMax(rightChild);
        }
        else
        {
            return currNode;
        }
    }
    else
    {
        return NULL;
    }
}
template<template<typename> class Allocator, typename StringAllocator>
typename cRope<Allocator,StringAllocator>::RopeNode* cRope<Allocator,StringAllocator>::findMax(RopeNode* node) const
{
    if(node)
    {
        RopeNode* currNode = node;

        while(RopeNode* rightChild = currNode->getRightChild())
        {
            currNode = rightChild;
        }

        if(currNode->getLeaveNode())
        {
            return currNode;
        }
        else if(RopeNode* leftChild = currNode->getLeftChild())
        {
            return findMin(leftChild);
        }
        else
        {
            return currNode;
        }
    }
    else
    {
        return NULL;
    }
}
template<template<typename> class Allocator, typename StringAllocator>
typename cRope<Allocator,StringAllocator>::node_pointer_type cRope<Allocator,StringAllocator>::acquireNode(node_pointer_type i_node) const
{
    if(i_node != iterator_type::m_pHead)
    {
        iterator_pair_pointer currIterator = detail::receiveNodeFromIterator<iterator_pair_pointer>(i_node);

        return detail::sendNodeToIterator<node_pointer_type>(create_iterator(currIterator->currRope,currIterator->currLeaveNode));
    }
    else
    {
        return iterator_type::m_pHead;
    }
}
template<template<typename> class Allocator, typename StringAllocator>
void cRope<Allocator,StringAllocator>::releaseNode(node_pointer_type i_node) const
{
    if(i_node != iterator_type::m_pHead)
    {
        delete_iterator(detail::receiveNodeFromIterator<iterator_pair_pointer>(i_node));
    }
}
template<template<typename> class Allocator, typename StringAllocator>
typename cRope<Allocator,StringAllocator>::node_pointer_type cRope<Allocator,StringAllocator>::getFirstElem() const
{
    const RopeNode* rootNode = this->template getTypedRootNode<RopeNode>();

    if(rootNode != iterable_type::m_pHead)
    {
        RopeNode* minNode = findMin(const_cast<RopeNode*>(rootNode));

        if(minNode != iterable_type::m_pHead)
        {
            return detail::sendNodeToIterator<node_pointer_type>(create_iterator(minNode,minNode->getLeaveNode()->getFirstElem()));
        }
        else
        {
            return iterator_type::m_pHead;
        }
    }
    else
    {
        return iterator_type::m_pHead;
    }
}
template<template<typename> class Allocator, typename StringAllocator>
typename cRope<Allocator,StringAllocator>::node_pointer_type cRope<Allocator,StringAllocator>::getLastElem() const
{
    RopeNode* rootNode = this->template getTypedRootNode<RopeNode>();

    if(rootNode != iterable_type::m_pHead)
    {
        RopeNode* maxNode = findMax(rootNode);

        if(maxNode != iterable_type::m_pHead)
        {
            return detail::sendNodeToIterator<node_pointer_type>(create_iterator(maxNode,maxNode->getLeaveNode()->getFirstElem()));
        }
        else
        {
            return iterator_type::m_pHead;
        }
    }
    else
    {
        return iterator_type::m_pHead;
    }
}
template<template<typename> class Allocator, typename StringAllocator>
typename cRope<Allocator,StringAllocator>::node_pointer_type cRope<Allocator,StringAllocator>::getNextElem(node_pointer_type currNode) const
{
    if(currNode != iterator_type::m_pHead)
    {
        iterator_pair_pointer currIterator = detail::receiveNodeFromIterator<iterator_pair_pointer>(currNode);

        RopeLeave* currLeave = currIterator->currRope->getLeaveNode();

        if(typename RopeLeave::pointer_type nextLeaveNode = currLeave->getNextElem(currIterator->currLeaveNode))
        {
            currIterator->currLeaveNode = nextLeaveNode;

            return detail::sendNodeToIterator<pointer_type>(currIterator);
        }
        else
        {
            if(RopeNode* nextRopeNode = getNextElem(currIterator->currRope))
            {
                currIterator->currRope = nextRopeNode;
                RopeLeave* leaveNode = nextRopeNode->getLeaveNode();
                currIterator->currLeaveNode = (leaveNode) ? leaveNode->getFirstElem() : NULL;

                return detail::sendNodeToIterator<pointer_type>(currIterator);
            }
            else
            {
                delete_iterator(currIterator);

                return iterator_type::m_pHead;
            }
        }
    }

    return currNode;
}
template<template<typename> class Allocator, typename StringAllocator>
typename cRope<Allocator,StringAllocator>::RopeNode* cRope<Allocator,StringAllocator>::getNextElem(RopeNode* currNode) const
{
    if(currNode)
    {
        RopeLeave* currLeave = currNode->getLeaveNode();

        do
        {
            RopeNode* rightChild = currNode->getRightChild();

            if(RopeNode* currNodeParent = currNode->getParent())
            {
                RopeNode* leftChild = currNodeParent->getLeftChild();

                if(leftChild == currNode)
                {
                    currNode = (rightChild) ? findMin(rightChild) : currNodeParent;
                }
                else if(rightChild)
                {
                    currNode = findMin(rightChild);
                }
                else
                {
                    //look for the first parent which is the left one
                    RopeNode* currNodeGrandParent = currNodeParent->getParent();

                    while(currNodeGrandParent && currNodeParent != currNodeGrandParent->getLeftChild())
                    {
                        currNodeParent = currNodeGrandParent;
                        currNodeGrandParent = currNodeParent->getParent();
                    }

                    if(currNodeGrandParent)
                    {
                        currNode = currNodeGrandParent;
                    }
                    else
                    {
                        currNode = NULL;
                    }
                }
            }
            else if(rightChild)
            {
                currNode = findMin(rightChild);
            }
            else
            {
                currNode = NULL;
            }
        }
        while(currNode && currNode->getLeaveNode() == NULL);
    }

    return currNode;
}
template<template<typename> class Allocator, typename StringAllocator>
typename cRope<Allocator,StringAllocator>::node_pointer_type cRope<Allocator,StringAllocator>::getPrevElem(node_pointer_type currNode) const
{
    if(currNode != iterator_type::m_pHead)
    {
        iterator_pair_pointer currIterator = detail::receiveNodeFromIterator<iterator_pair_pointer>(currNode);

        RopeLeave* currLeave = currIterator->currRope->getLeaveNode();

        if(typename RopeLeave::pointer_type prevLeaveNode = currLeave->getPrevElem(currIterator->currLeaveNode))
        {
            currIterator->currLeaveNode = prevLeaveNode;

            return detail::sendNodeToIterator<pointer_type>(currIterator);
        }
        else
        {
            if(RopeNode* prevRopeNode = getPrevElem(currIterator->currRope))
            {
                currIterator->currRope = prevRopeNode;
                currLeave = prevRopeNode->getLeaveNode();
                currIterator->currLeaveNode = (currLeave) ? currLeave->getFirstElem() : NULL;

                return detail::sendNodeToIterator<pointer_type>(currIterator);
            }
            else
            {
                delete_iterator(currIterator);

                return iterator_type::m_pHead;
            }
        }
    }

    return currNode;
}
template<template<typename> class Allocator, typename StringAllocator>
typename cRope<Allocator,StringAllocator>::RopeNode* cRope<Allocator,StringAllocator>::getPrevElem(RopeNode* currNode) const
{
    if(currNode)
    {
        do
        {
            RopeNode* currNodeParent = currNode->getParent();
            RopeNode* leftChild = currNode->getLeftChild();

            if(currNodeParent)
            {
                RopeNode* rightChild = currNode->getRightChild();

                if(rightChild == currNode)
                {
                    currNode = (leftChild) ? findMax(leftChild) : currNodeParent;
                }
                else if(leftChild)
                {
                    currNode = findMax(leftChild);
                }
                else
                {
                    //look for the first parent which is the left one
                    RopeNode* currNodeGrandParent = currNodeParent->getParent();

                    while(currNodeGrandParent && currNodeParent != currNodeGrandParent->getRightChild())
                    {
                        currNodeParent = currNodeGrandParent;
                        currNodeGrandParent = currNodeParent->getParent();
                    }

                    if(currNodeGrandParent)
                    {
                        currNode = currNodeGrandParent;
                    }
                    else
                    {
                        currNode = NULL;
                    }
                }
            }
            else if(leftChild)
            {
                currNode = findMax(leftChild);
            }
            else
            {
                currNode = NULL;
            }
        }
        while(currNode && currNode->getLeaveNode() == NULL);
    }

    return currNode;
}
template<template<typename> class Allocator, typename StringAllocator>
typename cRope<Allocator,StringAllocator>::node_pointer_type cRope<Allocator,StringAllocator>::shiftNodeByIndex(node_pointer_type node, int index) const
{
    node_pointer_type shiftedElem = node;
    const bool forward = index > 0;

    for(size_t shiftCounter=0;shiftCounter<index && shiftedElem!=iterator_type::m_pHead;++shiftCounter)
    {
        shiftedElem = (forward) ? getNextElem(shiftedElem) : getPrevElem(shiftedElem);
    }

    return shiftedElem;
}
template<template<typename> class Allocator, typename StringAllocator>
size_t cRope<Allocator,StringAllocator>::getIndexOfNode(node_pointer_type node) const
{
    size_t currDist = 0;

    if(node != iterator_type::m_pHead)
    {
        iterator_pair_pointer currIterator = detail::receiveNodeFromIterator<iterator_pair_pointer>(node);
        RopeNode* currRope = currIterator->currRope;
        RopeLeave* currLeave = currRope->getLeaveNode();
        currDist = currLeave->getIndexOfNode(currIterator->currLeaveNode);

        RopeNode* currParent = currRope->getParent();

        while(currParent)
        {
            currDist += currParent->getWeight();
            currParent = currParent->getParent();
        }
    }

    return currDist;
}
template<template<typename> class Allocator, typename StringAllocator>
void cRope<Allocator,StringAllocator>::eraseNode(node_pointer_type i_valueNode)
{
    node_pointer_type endNode = getNextElem(i_valueNode);

    iterator_pair_pointer initPair = detail::receiveNodeFromIterator<iterator_pair_pointer>(i_valueNode);
    iterator_pair_pointer endPair = (endNode != iterable_type::m_pHead) ? detail::receiveNodeFromIterator<iterator_pair_pointer>(endNode) : NULL;

    remove(initPair,endPair);
}
template<template<typename> class Allocator, typename StringAllocator>
typename cRope<Allocator,StringAllocator>::node_pointer_type cRope<Allocator,StringAllocator>::addNode(add_type i_value)
{
    node_pointer_type rootNode = this->getRootNode();

    if(rootNode != iterable_type::m_pHead)
    {
        iterator_pair_pointer rootPair = detail::receiveNodeFromIterator<iterator_pair_pointer>(rootNode);

        RopeNode* maxNode = findMax(rootPair->currRope);
        RopeLeave* leaveNode = maxNode->getLeaveNode();
        insert(maxNode,leaveNode->getContentSize(),i_value);
    }
}
template<template<typename> class Allocator, typename StringAllocator>
void cRope<Allocator,StringAllocator>::prune(RopeNode* currNode)
{
    RopeNode* nodeToBalance = NULL;

    while(currNode)
    {
        const size_t numChildren = currNode->getNumChildren();
        RopeNode* currNodeParent = currNode->getParent();

        if(numChildren == 0 && currNode->getLeaveNode() == NULL)
        {
            deallocate(currNode);
            nodeToBalance = currNodeParent;
        }
        else if(numChildren == 1 && currNodeParent)
        {
            if(RopeNode* leftChild = currNode->getLeftChild())
            {
                currNodeParent->reParent(currNode,leftChild);
                currNode->setLeftChild(NULL);
                deallocate(currNode);
            }
            else if(RopeNode* rightChild = currNode->getRightChild())
            {
                currNodeParent->reParent(currNode,rightChild);
                currNode->setRightChild(NULL);
                deallocate(currNode);
            }
        }

        currNode = currNodeParent;
    }

    if(nodeToBalance)
    {
        this->setRootNode(detail::cAVLBalancer<cRope<Allocator,StringAllocator>,detail::cRopeNode<StringAllocator>>::balance(this,nodeToBalance));
    }
}
template<template<typename> class Allocator, typename StringAllocator>
typename cRope<Allocator,StringAllocator>::iterator_pair_pointer cRope<Allocator,StringAllocator>::create_iterator(RopeNode* i_currNode, typename RopeLeave::pointer_type i_currElem)
{
    return reinterpret_cast<iterator_pair_pointer>((u8*)memcpy((u8*)memcpy(detail::k_pairAllocator.allocate(1, sizeof(iterator_pair)),&i_currNode, sizeof(i_currNode)) + sizeof(i_currNode),&i_currElem, sizeof(i_currElem)) - sizeof(i_currNode));
}
template<template<typename> class Allocator, typename StringAllocator>
void cRope<Allocator,StringAllocator>::delete_iterator(iterator_pair_pointer i_pair)
{
    detail::k_pairAllocator.deallocate(i_pair);
}

}
}
