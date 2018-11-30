#pragma once

#include "YTL/container/cString.h"
#include "System/memory/cMemoryPool.h"

namespace yame
{
namespace container
{
namespace detail
{

extern const memory::cMemoryPool& k_pairAllocator;

template<typename Allocator>
class cRopeLeave
{
public:
    typedef typename cString<Allocator>::iterator_type iterator_type;
    typedef typename cString<Allocator>::const_iterator_type const_iterator_type;
    typedef typename cString<Allocator>::pointer_type pointer_type;

    cRopeLeave(const cString<Allocator>& i_content);
    const cString<Allocator>& getContent() const;
    cString<Allocator>& getContent();
    void setContent(const cString<Allocator>& i_content);
    size_t getContentSize() const;

    pointer_type getFirstElem();
    pointer_type getLastElem();
    pointer_type getNextElem(pointer_type i_currElem);
    pointer_type getPrevElem(pointer_type i_currElem);
    size_t getIndexOfNode(pointer_type node);

private:
    cString<Allocator> m_content;
};

template<typename StringAllocator>
class cRopeNode
{
public:
    cRopeNode() = default;
    ~cRopeNode();
    inline void setParent(cRopeNode<StringAllocator>* i_parent);
    inline void setLeftChild(cRopeNode<StringAllocator>* i_leftChild);
    inline void setRightChild(cRopeNode<StringAllocator>* i_rightChild);
    inline void setLeaveNode(cRopeLeave<StringAllocator>* i_leave);
    inline void reParent(cRopeNode<StringAllocator>* i_oldChild, cRopeNode<StringAllocator>* i_newChild);
    inline cRopeNode<StringAllocator>* getParent() const;
    inline cRopeNode<StringAllocator>* getLeftChild() const;
    inline cRopeNode<StringAllocator>* getRightChild() const;
    inline cRopeLeave<StringAllocator>* getLeaveNode() const;
    inline void setWeight(size_t i_weight);
    inline size_t getWeight() const;
    inline void setHeight(s8 i_height);
    inline s8 getHeight() const;
    inline void addWeight(size_t i_val);
    inline void subsWeight(size_t i_val);
    inline void updateWeight();
    inline bool hasChildren() const;
    inline size_t getNumChildren() const;

private:
    cRopeNode<StringAllocator>* m_pParent = NULL;
    cRopeNode<StringAllocator>* m_pLeftChild = NULL;
    cRopeNode<StringAllocator>* m_pRightChild = NULL;
    cRopeLeave<StringAllocator>* m_pLeave = NULL;
    size_t m_weight = 0;
    s8  m_height = 0;
};

}

template<template<typename> class Allocator = memory::cTypedSystemAllocator, typename StringAllocator = memory::cTypedSystemAllocator<c8>>
class cRope : public detail::cRandomAccessIterableBaseImpl<c8&>
{
    typedef detail::cRopeNode<StringAllocator> RopeNode;
    typedef detail::cRopeLeave<StringAllocator> RopeLeave;
    typedef detail::cRandomAccessIterableBaseImpl<c8&> iterable_type;
    static const size_t k_chunkSize = NUM_STACK_CHARS * sizeof(c8);

    struct iterator_pair
    {
        iterator_pair(RopeNode* i_currRope, typename RopeLeave::pointer_type i_currLeaveNode)
        : currRope(i_currRope)
        , currLeaveNode(i_currLeaveNode)
        {}

        RopeNode* currRope;
        typename RopeLeave::pointer_type currLeaveNode;
    };

public:
    typedef typename mpl::add_pointer<iterator_pair>::type iterator_pair_pointer;
    typedef typename detail::cRandomAccessIterableBaseImpl<c8&>::iterator_type iterator_type;
    typedef typename detail::cRandomAccessIterableBaseImpl<c8&>::const_iterator_type const_iterator_type;
    typedef typename detail::cRandomAccessIterableBaseImpl<c8&>::reverse_iterator_type reverse_iterator_type;
    typedef typename detail::cRandomAccessIterableBaseImpl<c8&>::const_reverse_iterator_type const_reverse_iterator_type;
    typedef typename detail::cRandomAccessIterableBaseImpl<c8&>::pointer_type pointer_type;
    typedef typename detail::cRandomAccessIterableBaseImpl<c8&>::node_pointer_type node_pointer_type;
    typedef typename detail::cRandomAccessIterableBaseImpl<c8&>::iterable_public_interface iterable_public_interface;
    typedef typename detail::cRandomAccessIterableBaseImpl<c8&>::add_type add_type;
    static const size_t npos=-1;

    cRope();
    cRope(const cRope&) = delete; //we do not allow copies since its quite cost performance
    cRope(cRope&& other);
    cRope(const char* i_str);
    ~cRope();
    iterator_type insert(iterator_type i_it, const cString<StringAllocator>& i_content);
    iterator_type remove(iterator_type i_itInit, iterator_type i_itEnd);
    cString<StringAllocator> subStr(const_iterator_type i_itInit, const_iterator_type i_itEnd) const;
    void clear();
    size_t getSize() const;
    bool empty() const;

private:
    inline RopeNode* insert(RopeNode* i_rootNode, size_t i_pos, const cString<StringAllocator>& i_content);
    inline RopeNode* insert(RopeNode* i_rootNode, RopeLeave* i_leave, size_t i_pos, const cString<StringAllocator>& i_content);
    inline RopeNode* search(RopeNode* i_rootNode, size_t i_pos);
    inline iterator_pair_pointer remove(iterator_pair_pointer i_initNode, iterator_pair_pointer i_endNode);
    inline detail::cRopeNode<StringAllocator>* allocate() const;
    inline detail::cRopeNode<StringAllocator>* allocate(RopeNode* i_node) const;
    inline detail::cRopeLeave<StringAllocator>* allocate(const cString<StringAllocator>& i_content) const;
    inline void deallocate(RopeNode* i_node) const;
    inline void deallocate(RopeLeave* i_leave) const;
    inline RopeNode* findMin(RopeNode* node) const;
    inline RopeNode* findMax(RopeNode* node) const;
    inline static iterator_pair_pointer create_iterator(RopeNode* i_currNode, typename RopeLeave::pointer_type i_currElem);
    inline static void delete_iterator(iterator_pair_pointer i_pair);
    inline RopeNode* getNextElem(RopeNode* currNode) const;
    inline RopeNode* getPrevElem(RopeNode* currNode) const;
    inline void prune(RopeNode* currNode);

    //const iterable implementation
    node_pointer_type acquireNode(node_pointer_type i_node) const override;
    void releaseNode(node_pointer_type i_node) const override;
    node_pointer_type getFirstElem() const override;
    node_pointer_type getLastElem() const override;
    node_pointer_type getNextElem(node_pointer_type currNode) const override;
    node_pointer_type getPrevElem(node_pointer_type currNode) const override;
    node_pointer_type shiftNodeByIndex(node_pointer_type node, int index) const override;
    size_t getIndexOfNode(node_pointer_type node) const override;
    const c8& getValue(node_pointer_type currNode) const override;
    c8& getValue(node_pointer_type currNode) override;

    //non const iterable implementation
    void eraseNode(node_pointer_type i_valueNode) override;
    node_pointer_type addNode(add_type i_value) override;

    const Allocator<RopeNode>& m_alloc;
    const Allocator<RopeLeave>& m_allocLeave;
};

}
}

#include "YTL/container/detail/cRope.tpp"
