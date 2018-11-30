#include "YTL/container/cIterableBase.h"

#define NEXT_VALID_ENTRY(IN,OUT) \
ASSERT(this->m_iterable, "Unconsisent situation"); \
 \
if(IN != this->m_pHead) \
{ \
    OUT = this->m_iterable->getNextElem(IN); \
     \
    while(OUT != this->m_pHead) \
    { \
        if(this->m_filter == null_ptr || this->m_filter.eval(this->m_iterable->getValue(OUT))) \
        { \
            break; \
        } \
     \
        OUT = this->m_iterable->getNextElem(OUT); \
    } \
} \
else \
{ \
    OUT = IN; \
}

#define PREV_VALID_ENTRY(IN,OUT) \
ASSERT(this->m_iterable, "Unconsisent situation"); \
 \
if(IN != this->m_pHead) \
{ \
    OUT = this->m_iterable->getPrevElem(IN); \
     \
    while(OUT != this->m_pHead) \
    { \
        if(this->m_filter == null_ptr || this->m_filter.eval(this->m_iterable->getValue(OUT))) \
        { \
            break; \
        } \
     \
        OUT = this->m_iterable->getPrevElem(OUT); \
    } \
} \
else \
{ \
    IN = OUT; \
}

namespace yame
{
namespace container
{
namespace detail
{
namespace impl
{

//iterator base

template<typename T, template<class> class Iterator, typename Iterable, template<typename> class AgnosticTag, typename DirectionTag, ReferenceCategory _category>
typename cIteratorImpl<IteratorTraits<T,Iterator,Iterable,AgnosticTag<DirectionTag>,_category>>::node_pointer_type const cIteratorImpl<IteratorTraits<T,Iterator,Iterable,AgnosticTag<DirectionTag>,_category>>::m_pHead = reinterpret_cast<node_pointer_type const>(0xc1c1);

template<typename T, template<class> class Iterator, typename Iterable, template<typename> class AgnosticTag, typename DirectionTag, ReferenceCategory _category>
cIteratorImpl<IteratorTraits<T,Iterator,Iterable,AgnosticTag<DirectionTag>,_category>>::cIteratorImpl(const cIteratorImpl<IteratorTraits<T,Iterator,Iterable,AgnosticTag<DirectionTag>,_category>>& iter)
: m_pCurrentNode(m_pHead)
, m_filter(iter.m_filter)
, m_iterable(iter.m_iterable)
{
    m_pCurrentNode = m_iterable->acquireNode(iter.m_pCurrentNode);
}
template<typename T, template<class> class Iterator, typename Iterable, template<typename> class AgnosticTag, typename DirectionTag, ReferenceCategory _category>
cIteratorImpl<IteratorTraits<T,Iterator,Iterable,AgnosticTag<DirectionTag>,_category>>::cIteratorImpl(cIteratorImpl<IteratorTraits<T,Iterator,Iterable,AgnosticTag<DirectionTag>,_category>>&& iter)
: m_pCurrentNode(iter.m_pCurrentNode)
, m_filter(mpl::move(iter.m_filter))
, m_iterable(iter.m_iterable)
{
    iter.m_pCurrentNode = m_pHead;
}
template<typename T, template<class> class Iterator, typename Iterable, template<typename> class AgnosticTag, typename DirectionTag, ReferenceCategory _category>
cIteratorImpl<IteratorTraits<T,Iterator,Iterable,AgnosticTag<DirectionTag>,_category>>::cIteratorImpl(iterable_private_interface& iter)
: m_iterable(&iter)
, m_filter(null_ptr)
, m_pCurrentNode(m_pHead)
{
}
template<typename T, template<class> class Iterator, typename Iterable, template<typename> class AgnosticTag, typename DirectionTag, ReferenceCategory _category>
cIteratorImpl<IteratorTraits<T,Iterator,Iterable,AgnosticTag<DirectionTag>,_category>>::cIteratorImpl(iterable_private_interface& iter, const ytl::function<bool(const_reference)>& filter)
: m_iterable(&iter)
, m_filter(filter)
, m_pCurrentNode(m_pHead)
{
}
template<typename T, template<class> class Iterator, typename Iterable, template<typename> class AgnosticTag, typename DirectionTag, ReferenceCategory _category>
template<typename DDirectionTag, ReferenceCategory __category>
cIteratorImpl<IteratorTraits<T,Iterator,Iterable,AgnosticTag<DirectionTag>,_category>>::cIteratorImpl(const cIteratorImpl<IteratorTraits<T,Iterator,Iterable,AgnosticTag<DDirectionTag>,__category>>& other)
: m_pCurrentNode(m_pHead)
, m_filter(other.m_filter)
, m_iterable(other.m_iterable)
{
    //apply some category conditions
    static_assert( _category == __category ||
                  (_category == ReferenceCategory::ConstReference && __category == ReferenceCategory::NonConstReference) ||
                  (_category == ReferenceCategory::Value && (__category == ReferenceCategory::NonConstReference || __category == ReferenceCategory::ConstReference)), "Trying to construct iterator with invalid type category");

    m_pCurrentNode = m_iterable->acquireNode(other.m_pCurrentNode);
}
template<typename T, template<class> class Iterator, typename Iterable, template<typename> class AgnosticTag, typename DirectionTag, ReferenceCategory _category>
template<typename DDirectionTag, ReferenceCategory __category>
cIteratorImpl<IteratorTraits<T,Iterator,Iterable,AgnosticTag<DirectionTag>,_category>>::cIteratorImpl(cIteratorImpl<IteratorTraits<T,Iterator,Iterable,AgnosticTag<DDirectionTag>,__category>>&& other)
: m_pCurrentNode(other.m_pCurrentNode)
, m_filter(mpl::move(other.m_filter))
, m_iterable(other.m_iterable)
{
    //apply some category conditions
    static_assert( _category == __category ||
                  (_category == ReferenceCategory::ConstReference && __category == ReferenceCategory::NonConstReference) ||
                  (_category == ReferenceCategory::Value && (__category == ReferenceCategory::NonConstReference || __category == ReferenceCategory::ConstReference)), "Trying to construct iterator with invalid type category");

    other.m_pCurrentNode = m_pHead;
}
template<typename T, template<class> class Iterator, typename Iterable, template<typename> class AgnosticTag, typename DirectionTag, ReferenceCategory _category>
cIteratorImpl<IteratorTraits<T,Iterator,Iterable,AgnosticTag<DirectionTag>,_category>>::~cIteratorImpl()
{
    ASSERT(m_iterable, "Unconsisent situation");

    m_iterable->releaseNode(m_pCurrentNode);
}
template<typename T, template<class> class Iterator, typename Iterable, template<typename> class AgnosticTag, typename DirectionTag, ReferenceCategory _category>
typename cIteratorImpl<IteratorTraits<T,Iterator,Iterable,AgnosticTag<DirectionTag>,_category>>::iterator_type& cIteratorImpl<IteratorTraits<T,Iterator,Iterable,AgnosticTag<DirectionTag>,_category>>::operator=(const cIteratorImpl<IteratorTraits<T,Iterator,Iterable,AgnosticTag<DirectionTag>,_category>>& other)
{
    ASSERT(m_iterable, "Unconsisent situation");
    ASSERT(other.m_iterable, "Unconsisent situation");

    m_iterable->releaseNode(m_pCurrentNode);

    m_iterable = other.m_iterable;

    m_filter = other.m_filter;

    m_pCurrentNode = m_iterable->acquireNode(other.m_pCurrentNode);

    return static_cast<typename Traits::iterator_type&>(*this);
}
template<typename T, template<class> class Iterator, typename Iterable, template<typename> class AgnosticTag, typename DirectionTag, ReferenceCategory _category>
void cIteratorImpl<IteratorTraits<T,Iterator,Iterable,AgnosticTag<DirectionTag>,_category>>::setCurrentNode(node_pointer_type currNode)
{
    ASSERT(m_iterable, "Unconsisent situation");

    m_pCurrentNode = (currNode == m_pHead || m_filter == null_ptr || m_filter.eval(m_iterable->getValue(currNode))) ? currNode : m_pHead;
}
template<typename T, template<class> class Iterator, typename Iterable, template<typename> class AgnosticTag, typename DirectionTag, ReferenceCategory _category>
typename cIteratorImpl<IteratorTraits<T,Iterator,Iterable,AgnosticTag<DirectionTag>,_category>>::node_pointer_type cIteratorImpl<IteratorTraits<T,Iterator,Iterable,AgnosticTag<DirectionTag>,_category>>::getCurrentNode() const
{
    return m_pCurrentNode;
}
template<typename T, template<class> class Iterator, typename Iterable, template<typename> class AgnosticTag, typename DirectionTag, ReferenceCategory _category>
typename IteratorTraits<T,Iterator,Iterable,AgnosticTag<DirectionTag>,_category>::iterator_type cIteratorImpl<IteratorTraits<T,Iterator,Iterable,AgnosticTag<DirectionTag>,_category>>::operator++(int)
{
    typename Traits::iterator_type res(static_cast<typename Traits::iterator_type&>(*this));

    NEXT_VALID_ENTRY(m_pCurrentNode, m_pCurrentNode)

    return res;
}

//input iterator

template<typename Traits>
typename cInputIteratorImpl<Traits>::iterator_type& cInputIteratorImpl<Traits>::operator++()
{
    NEXT_VALID_ENTRY(this->m_pCurrentNode, this->m_pCurrentNode);

    return static_cast<typename Traits::iterator_type&>(*this);
}
template<typename Traits>
typename cInputIteratorImpl<Traits>::reference cInputIteratorImpl<Traits>::operator*()
{
    ASSERT(this->m_pCurrentNode!=this->m_pHead,"Dereferencing the head node!");

    return this->m_iterable->getValue(this->m_pCurrentNode);
}
template<typename Traits>
bool cInputIteratorImpl<Traits>::operator==(const cInputIteratorImpl<Traits>& other) const
{
    return this->m_pCurrentNode == other.m_pCurrentNode;
}
template<typename Traits>
bool cInputIteratorImpl<Traits>::operator!=(const cInputIteratorImpl<Traits>& other) const
{
    return this->m_pCurrentNode != other.m_pCurrentNode;
}
template<typename Traits>
typename cInputIteratorImpl<Traits>::pointer_type cInputIteratorImpl<Traits>::operator->()
{
    ASSERT(this->m_pCurrentNode!=this->m_pHead,"Dereferencing the head node!");

    reference tmp = this->m_iterable->getValue(this->m_pCurrentNode);

    return &tmp;
}


//output iterator

template<typename Traits>
typename cOutputIteratorImpl<Traits>::iterator_type& cOutputIteratorImpl<Traits>::operator++()
{
    NEXT_VALID_ENTRY(this->m_pCurrentNode, this->m_pCurrentNode);

    return static_cast<typename Traits::iterator_type&>(*this);
}
template<typename Traits>
typename cOutputIteratorImpl<Traits>::iterator_type& cOutputIteratorImpl<Traits>::operator*()
{
    return static_cast<typename Traits::iterator_type&>(*this);
}
template<typename Traits>
typename cOutputIteratorImpl<Traits>::iterator_type& cOutputIteratorImpl<Traits>::operator=(const value_type& value)
{
    ASSERT(this->m_iterable, "Unconsisent situation");
    ASSERT(this->m_pCurrentNode!=this->m_pHead,"Dereferencing the head node!");

    this->m_iterable->setValue(this->m_pCurrentNode, value);

    return static_cast<typename Traits::iterator_type&>(*this);
}

//bidirectional iterator

template<typename Traits>
typename cBidirectionalIteratorImpl<Traits>::iterator_type cBidirectionalIteratorImpl<Traits>::operator++(int)
{
    constexpr bool isReverseIterator = is_reversed_iterator<typename Traits::tag>::value;

    typename Traits::iterator_type res(static_cast<typename Traits::iterator_type&>(*this));

    if(isReverseIterator)
    {
        PREV_VALID_ENTRY(this->m_pCurrentNode, this->m_pCurrentNode);
    }
    else
    {
        NEXT_VALID_ENTRY(this->m_pCurrentNode, this->m_pCurrentNode);
    }

    return res;
}
template<typename Traits>
typename cBidirectionalIteratorImpl<Traits>::iterator_type& cBidirectionalIteratorImpl<Traits>::operator++()
{
    constexpr bool isReverseIterator = is_reversed_iterator<typename Traits::tag>::value;

    if(isReverseIterator)
    {
        PREV_VALID_ENTRY(this->m_pCurrentNode, this->m_pCurrentNode);
    }
    else
    {
        NEXT_VALID_ENTRY(this->m_pCurrentNode, this->m_pCurrentNode);
    }

    return static_cast<typename Traits::iterator_type&>(*this);
}
template<typename Traits>
typename cBidirectionalIteratorImpl<Traits>::iterator_type& cBidirectionalIteratorImpl<Traits>::operator--()
{
    constexpr bool isReverseIterator = is_reversed_iterator<typename Traits::tag>::value;

    if(isReverseIterator)
    {
        NEXT_VALID_ENTRY(this->m_pCurrentNode, this->m_pCurrentNode);
    }
    else
    {
        PREV_VALID_ENTRY(this->m_pCurrentNode, this->m_pCurrentNode);
    }

    return static_cast<typename Traits::iterator_type&>(*this);
}
template<typename Traits>
typename cBidirectionalIteratorImpl<Traits>::iterator_type cBidirectionalIteratorImpl<Traits>::operator--(int)
{
    typename Traits::iterator_type res(static_cast<typename Traits::iterator_type&>(*this));

    constexpr bool isReverseIterator = is_reversed_iterator<typename Traits::tag>::value;

    if(isReverseIterator)
    {
        NEXT_VALID_ENTRY(this->m_pCurrentNode, this->m_pCurrentNode);
    }
    else
    {
        PREV_VALID_ENTRY(this->m_pCurrentNode, this->m_pCurrentNode);
    }

    return res;
}

//random access iterator

template<typename Traits>
typename cRandomAccessIteratorImpl<Traits>::difference_type cRandomAccessIteratorImpl<Traits>::operator-(const cRandomAccessIteratorImpl<Traits>& other) const
{
    ASSERT(this->m_iterable, "Unconsisent situation");
    ASSERT(this->m_iterable == other.m_iterable, "You must compare iterators from the same iterable");

    return this->m_iterable->getIndexOfNode(this->m_pCurrentNode) - this->m_iterable->getIndexOfNode(other.m_pCurrentNode);
}
template<typename Traits>
typename cRandomAccessIteratorImpl<Traits>::iterator_type cRandomAccessIteratorImpl<Traits>::operator-(difference_type i_index) const
{
    ASSERT(this->m_iterable, "Unconsisent situation");

    typename Traits::iterator_type res(*this);

    if(i_index > 0)
    {
        res.setCurrentNode(this->m_iterable->shiftNodeByIndex(res.m_pCurrentNode,-i_index));
    }

    return res;
}
template<typename Traits>
typename cRandomAccessIteratorImpl<Traits>::iterator_type& cRandomAccessIteratorImpl<Traits>::operator-=(difference_type i_index) const
{
    ASSERT(this->m_iterable, "Unconsisent situation");

    if(i_index > 0)
    {
        this->setCurrentNode(this->m_iterable->shiftNodeByIndex(this->m_pCurrentNode,-i_index));
    }

    return static_cast<typename Traits::iterator_type&>(*this);
}
template<typename Traits>
typename cRandomAccessIteratorImpl<Traits>::iterator_type cRandomAccessIteratorImpl<Traits>::operator+(difference_type i_index) const
{
    ASSERT(this->m_iterable, "Unconsisent situation");

    typename Traits::iterator_type res(*this);

    if(i_index > 0)
    {
        res.setCurrentNode(this->m_iterable->shiftNodeByIndex(res.m_pCurrentNode,i_index));
    }

    return res;
}
template<typename Traits>
typename cRandomAccessIteratorImpl<Traits>::iterator_type& cRandomAccessIteratorImpl<Traits>::operator+=(difference_type i_index) const
{
    ASSERT(this->m_iterable, "Unconsisent situation");

    if(i_index > 0)
    {
        this->setCurrentNode(this->m_iterable->shiftNodeByIndex(this->m_pCurrentNode,i_index));
    }

    return static_cast<typename Traits::iterator_type&>(*this);
}
template<typename Traits>
typename cRandomAccessIteratorImpl<Traits>::reference cRandomAccessIteratorImpl<Traits>::operator[](size_t i_index)
{
    ASSERT(this->m_iterable, "Unconsisent situation");

    return this->m_iterable->getValue(this->m_iterable->shiftNodeByIndex(this->m_pCurrentNode,i_index));
}
template<typename Traits>
bool cRandomAccessIteratorImpl<Traits>::operator<(const cRandomAccessIteratorImpl<Traits>& other) const
{
    return (other - *this) < 0;
}
template<typename Traits>
bool cRandomAccessIteratorImpl<Traits>::operator>(const cRandomAccessIteratorImpl<Traits>& other) const
{
    return (other - *this) > 0;
}
template<typename Traits>
bool cRandomAccessIteratorImpl<Traits>::operator<=(const cRandomAccessIteratorImpl<Traits>& other) const
{
    return (other - *this) <= 0;
}
template<typename Traits>
bool cRandomAccessIteratorImpl<Traits>::operator>=(const cRandomAccessIteratorImpl<Traits>& other) const
{
    return (other - *this) >= 0;
}

}
}
}
}

/*template<typename T, typename R>
void sort(cIteratorImpl<T> beginItem, cIteratorImpl<T> endItem)
{
    T pivot;
    u32 pivotInd;

    if(beginItem == endItem)
    {
        return;
    }
    else
    {
        u32 rangeSize = endItem - beginItem;
        pivotInd = rand()%rangeSize;
        pivot = (*(beginItem+pivotInd)).second;

        //divide and conquer subranges
        cIteratorImpl<T> rangeAA, rangeAB, rangeBA, rangeBB;

        //initialize
        if(pivotInd == 0)
        {
            rangeAA = beginItem+1;
            rangeAB = endItem;
            rangeBA = endItem;
            rangeBB = endItem;
        }
        else if(pivotInd == rangeSize)
        {
            rangeAA = beginItem;
            rangeAB = endItem-1;
            rangeBA = endItem;
            rangeBB = endItem;
        }
        else
        {
            rangeAA = beginItem;
            rangeAB = beginItem+pivotInd;
            rangeBA = beginItem+pivotInd+1;
            rangeBB = endItem;
        }

        for(u16 i=0;i<rangeSize;i++)
        {
            if(i != pivotInd)
            {
                if((*(beginItem+i)).second < pivot && i > pivotInd)
                {
                    //swap elemtnts
                    swapItems((*(beginItem+i)).second,(*(beginItem+pivotInd)).second);
                }
                else if((*(beginItem+i)).second > pivot && i < pivotInd)
                {
                    //swap elemtnts
                    swapItems((*(beginItem+i)).second,(*(beginItem+pivotInd)).second);
                }
            }
        }

        sort(rangeAA,rangeAB);
        sort(rangeBA,rangeBB);
    }

    return;
}

template<typename T, typename TT>
T find(T beginItem, T endItem, const TT& val)
{
    T res;

    for(T currItem=beginItem; currItem!=endItem;currItem++)
    {
        if((*currItem).second == val)
        {
            res = currItem;

            break;
        }
    }

    return res;
}*/
