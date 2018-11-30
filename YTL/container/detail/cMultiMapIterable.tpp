
#include "YTL/container/cAlgorithm.h"

namespace yame
{
namespace container
{

template<typename R, typename T, typename TT>
typename cMultiMapIterable<R,T,TT>::node_pointer_type cMultiMapIterable<R,T,TT>::getNextElemInCurrentIterable(const nested_iterable_interface* currIterableBackEnd, node_pointer_type i_currNode) const
{
    const inner_value_type& currMultiMapNode = currIterableBackEnd->getPrivateIterable().getValue(i_currNode);

    multimap_node& multimapNode = currMultiMapNode.second;

    if(multimapNode.empty() == false)
    {
        //we are the current node and we are not empty, consider the first of our children
        return multimapNode.acces_private_interface().getFirstElem();
    }
    else
    {
        node_pointer_type nextNode = detail::cBidirectionalIterableImpl<detail::ReversableIterableTraits<cPair<const R,T>&,TT,detail::cBidirectionalIterableBasePrivate,cConstBidirectionalIterable,1>>::getNextElemInCurrentIterable(currIterableBackEnd,i_currNode);

        if(nextNode != inner_iterator_type::m_pHead)
        {
            return nextNode;
        }
        else
        {
            multimap_node* currHolder = &multimapNode;

            while(multimap_node* multiMapParent = currHolder->getHolder())
            {
                node_pointer_type parentPrevNode = detail::cBidirectionalIterableImpl<detail::ReversableIterableTraits<cPair<const R,T>&,TT,detail::cBidirectionalIterableBasePrivate,cConstBidirectionalIterable,1>>::getNextElemInCurrentIterable(currIterableBackEnd,detail::sendNodeToIterator<node_pointer_type>(multiMapParent->getNodeLocation()));

                if(parentPrevNode != inner_iterator_type::m_pHead)
                {
                    return parentPrevNode;
                }

                currHolder = multiMapParent;
            }

            //in any other case, we have finished
            return inner_iterator_type::m_pHead;
        }
    }
}
template<typename R, typename T, typename TT>
typename cMultiMapIterable<R,T,TT>::node_pointer_type cMultiMapIterable<R,T,TT>::getPrevElemInCurrentIterable(const nested_iterable_interface* currIterableBackEnd, node_pointer_type i_currNode) const
{
    const inner_value_type& currMultiMapNode = currIterableBackEnd->getPrivateIterable().getValue(i_currNode);

    multimap_node& multimapNode = currMultiMapNode.second;

    if(multimapNode.empty() == false)
    {
        //we are the current node and we are not empty, consider the last of our children
        return multimapNode.acces_private_interface().getLastElem();
    }
    else
    {
        node_pointer_type prevNode = detail::cBidirectionalIterableImpl<detail::ReversableIterableTraits<cPair<const R,T>&,TT,detail::cBidirectionalIterableBasePrivate,cConstBidirectionalIterable,1>>::getPrevElemInCurrentIterable(currIterableBackEnd,i_currNode);

        if(prevNode != inner_iterator_type::m_pHead)
        {
            return prevNode;
        }
        else
        {
            multimap_node* currHolder = &multimapNode;

            while(multimap_node* multiMapParent = currHolder->getHolder())
            {
                node_pointer_type parentPrevNode = detail::cBidirectionalIterableImpl<detail::ReversableIterableTraits<cPair<const R,T>&,TT,detail::cBidirectionalIterableBasePrivate,cConstBidirectionalIterable,1>>::getPrevElemInCurrentIterable(currIterableBackEnd,detail::sendNodeToIterator<node_pointer_type>(multiMapParent->getNodeLocation()));

                if(parentPrevNode != inner_iterator_type::m_pHead)
                {
                    return parentPrevNode;
                }

                currHolder = multiMapParent;
            }

            //in any other case, we have finished
            return inner_iterator_type::m_pHead;
        }
    }
}
template<typename R, typename T, typename TT>
bool cMultiMapIterable<R,T,TT>::empty() const
{
    return cbegin() == cend();
}
template<typename R, typename T, typename TT>
size_t cMultiMapIterable<R,T,TT>::getSize() const
{
    return yame::distance(cbegin(), cend());
}

}
}
