
/*This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#pragma once

#include "YTL/container/cIterable.h"
#include <initializer_list>
#include "YTL/types/optional/optional.h"

namespace yame
{
namespace container
{
namespace detail
{

template<typename IteratorIn>
inline bool equal(IteratorIn& itInitA, IteratorIn& itEndA, IteratorIn& itInitB, IteratorIn& itEndB, const ytl::function<bool(typename IteratorIn::const_reference,typename IteratorIn::const_reference)>& i_eqOp)
{
    bool res = true;

    for(;itInitA!=itEndA && itInitB!=itEndB;++itInitA,++itInitB)
    {
        if(i_eqOp.eval(*itInitA,*itInitB) == false)
        {
            res = false;
            break;
        }
    }

    return res;
}
template<typename Traits>
inline bool equal(impl::cRandomAccessIteratorImpl<Traits>& itInitA, impl::cRandomAccessIteratorImpl<Traits>& itEndA, impl::cRandomAccessIteratorImpl<Traits>& itInitB, impl::cRandomAccessIteratorImpl<Traits>& itEndB, const ytl::function<bool(typename Traits::const_reference,typename Traits::const_reference)>& i_eqOp)
{
    bool res = true;

    for(;itInitA!=itEndA && itInitB!=itEndB;++itInitA,++itInitB)
    {
        if(i_eqOp.eval(*itInitA,*itInitB) == false)
        {
            res = false;
            break;
        }
    }

    return res;
}
template<typename Traits>
inline bool equal(impl::cRandomAccessIteratorImpl<Traits>& itInitA, impl::cRandomAccessIteratorImpl<Traits>& itEndA, typename Traits::const_pointer_type itInitB, typename Traits::const_pointer_type itEndB, const ytl::function<bool(typename Traits::const_reference,typename Traits::const_reference)>& i_eqOp)
{
    bool res = true;

    for(;itInitA!=itEndA && itInitB!=itEndB;++itInitA,++itInitB)
    {
        if(i_eqOp.eval(*itInitA,*itInitB) == false)
        {
            res = false;
            break;
        }
    }

    return res;
}
template<typename T, typename TT>
inline bool equal(const TT* itInitA, const TT* itEndA, cRandomAccessIterator<T>& itInitB, cRandomAccessIterator<T>& itEndB, const ytl::function<bool(const TT&,const TT&)>& i_eqOp)
{
    bool res = true;

    for(;itInitA!=itEndA && itInitB!=itEndB;++itInitA,++itInitB)
    {
        if(i_eqOp.eval(*itInitA,*itInitB) == false)
        {
            res = false;
            break;
        }
    }

    return res;
}

template<typename Iterator>
inline Iterator cmp(Iterator& itInit, Iterator& itEnd, const ytl::function<bool(typename Iterator::const_reference,typename Iterator::const_reference)>& i_compOp)
{
    ASSERT(itInit!=itEnd, "You shall provide a non empty iterator pair");

    Iterator itFound = itInit++;

    for(;itInit!=itEnd;++itInit)
    {
        if(i_compOp.eval(*itInit, *itFound))
        {
            itFound = itInit;
        }
    }

    return itFound;
}

template<typename Iterator>
inline typename Iterator::value_type apply(Iterator& itInit, Iterator& itEnd, const ytl::function<typename Iterator::value_type(typename Iterator::const_reference,typename Iterator::const_reference)>& i_op)
{
    ASSERT(itInit!=itEnd, "You shall provide a non empty iterator pair");

    typename Iterator::value_type cmpValue = *itInit++;

    for(;itInit!=itEnd;++itInit)
    {
        cmpValue = i_op.eval(cmpValue,*itInit);
    }

    return cmpValue;
}

template<typename Traits>
inline void sort(impl::cRandomAccessIteratorImpl<Traits>& itInit, impl::cRandomAccessIteratorImpl<Traits>& itEnd, const ytl::function<bool(typename Traits::const_reference,typename Traits::const_reference)>& i_cmp)
{
    if(itInit == itEnd)
    {
        return;
    }
    else
    {
        const u32 rangeSize = itEnd - itInit;
        size_t pivotInd = rangeSize / 2;
        typename Traits::value_type pivot = *(itInit+pivotInd);
        size_t upperIndex = 0;
        size_t lowerIndex = rangeSize-1;
        bool done = false;

        while(!done)
        {
            while(i_cmp.eval(*(itInit+upperIndex), pivot)) upperIndex++;
            while(i_cmp.eval(pivot,*(itInit+lowerIndex))) lowerIndex--;

            if(upperIndex >= lowerIndex)
            {
                done = true;
                pivotInd = lowerIndex;
            }
            else
            {
                //swap elemtnts
                mpl::swapItems(*(itInit+upperIndex),*(itInit+lowerIndex));
            }
        }

        //divide and conquer subranges
        impl::cRandomAccessIteratorImpl<Traits> rangeAA = (pivotInd == 0) ? itInit+1 : (pivotInd == rangeSize) ? itInit : itInit;
        impl::cRandomAccessIteratorImpl<Traits> rangeAB = (pivotInd == 0) ? itEnd : (pivotInd == rangeSize) ? itEnd-1 : itInit+pivotInd;
        impl::cRandomAccessIteratorImpl<Traits> rangeBA = (pivotInd == 0) ? itEnd : (pivotInd == rangeSize) ? itEnd : itInit+pivotInd+1;
        impl::cRandomAccessIteratorImpl<Traits> rangeBB = (pivotInd == 0) ? itEnd : (pivotInd == rangeSize) ? itEnd : itEnd;

        if(rangeAA != rangeAB)
        {
            sort(rangeAA,rangeAB,i_cmp);
        }
        if(rangeBA != rangeBB)
        {
            sort(rangeBA,rangeBB,i_cmp);
        }
    }

    return;
}

template<typename IteratorIn, typename IteratorOut>
inline IteratorOut merge(IteratorIn& itInitA, IteratorIn& itEndA, IteratorIn& itInitB, IteratorIn& itEndB, cIterableBase<IteratorOut>& iterableOut, const ytl::function<typename IteratorIn::value_type(ytl::optional<typename IteratorIn::const_reference>,ytl::optional<typename IteratorIn::const_reference>)>& i_merge)
{
    bool done = false;
    IteratorOut itOut = iterableOut.begin();

    while(!done)
    {
        if(itInitA != itEndA && itInitB != itEndB)
        {
            itOut = iterableOut.add(i_merge.eval(*(itInitA),*(itInitB)));

            ++itInitA;
            ++itInitB;
        }
        else if(itInitA != itEndA)
        {
            itOut = iterableOut.add(i_merge.eval(*(itInitA),none));

            ++itInitA;
        }
        else if(itInitB != itEndB)
        {
            itOut = iterableOut.add(i_merge.eval(none,*(itInitB)));

            ++itInitB;
        }
        else
        {
            done = true;
        }
    }

    return itOut;
}

template<typename Iterator>
inline Iterator find_if(Iterator& itInit, Iterator& itEnd, ytl::function<bool(typename Iterator::const_reference)> i_findOp)
{
    Iterator currIt = itInit;

    for(;currIt!=itEnd;++currIt)
    {
        if(i_findOp.eval(*currIt))
        {
            return static_cast<Iterator&>(currIt);
        }
    }

    return static_cast<Iterator&>(itEnd);
}

template<typename Iterator>
inline typename Iterator::difference_type distance(Iterator& itInit, Iterator& itEnd)
{
    typename Iterator::difference_type dist = 0;

    for(Iterator currIt=itInit;currIt!=itEnd;++currIt,++dist);

    return dist;
}

template<typename T>
inline typename cRandomAccessIterator<T>::difference_type distance(cRandomAccessIterator<T>& itInit, cRandomAccessIterator<T>& itEnd)
{
    return itEnd - itInit;
}

template<typename Iterator>
inline Iterator initialize(Iterator& itInit, Iterator& itEnd, const std::initializer_list<typename Iterator::value_type>& i_initializerList)
{
    const typename Iterator::value_type* currElem = i_initializerList.begin();

    for(;currElem!=i_initializerList.end() && itInit!=itEnd;++currElem,++itInit)
    {
        *itInit = *currElem;
    }

    return static_cast<Iterator&>(itInit);
}

template<typename IteratorIn, typename IteratorOut>
inline IteratorOut assign(container::detail::cIterableBase<IteratorOut>& iterableOut, IteratorIn& itInit, IteratorIn& itEnd)
{
    IteratorOut itOut = iterableOut.begin();

    for(;itInit!=itEnd;++itInit)
    {
        itOut = iterableOut.add(*itInit);
    }

    return itOut;
}

template<typename Iterator>
inline Iterator assign(container::detail::cIterableBase<Iterator>& iterableOut, const std::initializer_list<typename Iterator::value_type>& i_initializerList)
{
    typedef typename container::detail::cIterableBase<Iterator>::add_type add_type;

    Iterator itOut = iterableOut.begin();

    typename Iterator::value_type* currElem = const_cast<typename Iterator::value_type*>(i_initializerList.begin());

    for(;currElem!=i_initializerList.end();++currElem)
    {
        itOut = iterableOut.add(mpl::forward<add_type>(*currElem));
    }

    return itOut;
}

template<typename IteratorIn, typename IteratorOutput>
inline IteratorOutput transform(IteratorIn& itInitInput, IteratorIn& itEndInput, cIterableBase<IteratorOutput>& iterableOutput, const ytl::function<typename IteratorIn::value_type(typename IteratorIn::const_reference)>& i_transformOp)
{
    IteratorOutput itOut = iterableOutput.begin();

    for(;itInitInput!=itEndInput;++itInitInput)
    {
        itOut = iterableOutput.add(i_transformOp(*itInitInput));
    }

    return itOut;
}

}
}

template<typename Iterator>
inline bool equal(Iterator itInitA, Iterator itEndA, Iterator itInitB, Iterator itEndB, typename mpl::enable_if<container::detail::impl::is_inputable_iterator<Iterator>::value>::type* = NULL)
{
    return container::detail::equal(itInitA,itEndA, itInitB, itEndB,ytl::function<bool(typename Iterator::const_reference, typename Iterator::const_reference)>([](typename Iterator::const_reference i_lhs,typename Iterator::const_reference i_rhs){ return i_lhs == i_rhs;}));
}
template<typename Iterator>
inline bool equal(Iterator itInitA, Iterator itEndA, Iterator itInitB, Iterator itEndB, const ytl::function<bool(typename Iterator::const_reference,typename Iterator::const_reference)>& i_eqOp, typename mpl::enable_if<container::detail::impl::is_inputable_iterator<Iterator>::value>::type* = NULL)
{
    return container::detail::equal(itInitA,itEndA, itInitB, itEndB,i_eqOp);
}
template<typename Traits>
inline bool equal(container::detail::impl::cRandomAccessIteratorImpl<Traits> itInitA, container::detail::impl::cRandomAccessIteratorImpl<Traits> itEndA, container::detail::impl::cRandomAccessIteratorImpl<Traits> itInitB, container::detail::impl::cRandomAccessIteratorImpl<Traits> itEndB)
{
    return container::detail::equal(itInitA,itEndA, itInitB, itEndB,ytl::function<bool(typename Traits::const_reference,typename Traits::const_reference)>([](typename Traits::const_reference i_lhs,typename Traits::const_reference i_rhs){ return i_lhs == i_rhs;}));
}
template<typename Traits>
inline bool equal(container::detail::impl::cRandomAccessIteratorImpl<Traits> itInitA, container::detail::impl::cRandomAccessIteratorImpl<Traits> itEndA, typename Traits::const_pointer_type itInitB, typename Traits::const_pointer_type itEndB)
{
    return container::detail::equal(itInitA,itEndA, itInitB, itEndB,ytl::function<bool(typename Traits::const_reference,typename Traits::const_reference)>([](typename Traits::const_reference i_lhs,typename Traits::const_reference i_rhs){ return i_lhs == i_rhs;}));
}
template<typename Traits>
inline bool equal(typename Traits::const_pointer_type itInitA, typename Traits::const_pointer_type itEndA, container::detail::impl::cRandomAccessIteratorImpl<Traits> itInitB, container::detail::impl::cRandomAccessIteratorImpl<Traits> itEndB)
{
    return container::detail::equal(itInitB, itEndB,itInitA,itEndA,ytl::function<bool(typename Traits::const_reference,typename Traits::const_reference)>([](typename Traits::const_reference i_lhs,typename Traits::const_reference i_rhs){ return i_lhs == i_rhs;}));
}
template<typename Traits>
inline bool equal(container::detail::impl::cRandomAccessIteratorImpl<Traits> itInitA, container::detail::impl::cRandomAccessIteratorImpl<Traits> itEndA, container::detail::impl::cRandomAccessIteratorImpl<Traits> itInitB, container::detail::impl::cRandomAccessIteratorImpl<Traits> itEndB, const ytl::function<bool(typename Traits::const_reference,typename Traits::const_reference)>& i_eqOp)
{
    return container::detail::equal(itInitA,itEndA, itInitB, itEndB,i_eqOp);
}
template<typename Traits>
inline bool equal(container::detail::impl::cRandomAccessIteratorImpl<Traits> itInitA, container::detail::impl::cRandomAccessIteratorImpl<Traits> itEndA, typename Traits::const_pointer_type itInitB, typename Traits::const_pointer_type itEndB, const ytl::function<bool(typename Traits::const_reference,typename Traits::const_reference)>& i_eqOp)
{
    return container::detail::equal(itInitA,itEndA, itInitB, itEndB,i_eqOp);
}
template<typename Traits>
inline bool equal(typename Traits::const_pointer_type itInitA, typename Traits::const_pointer_type itEndA, container::detail::impl::cRandomAccessIteratorImpl<Traits> itInitB, container::detail::impl::cRandomAccessIteratorImpl<Traits> itEndB, const ytl::function<bool(typename Traits::const_reference,typename Traits::const_reference)>& i_eqOp)
{
    return container::detail::equal(itInitB,itEndB,itInitA,itEndA,i_eqOp);
}
template<typename Iterator>
inline bool equal(const container::detail::cConstIterableBase<Iterator>& iterableA, const container::detail::cConstIterableBase<Iterator>& iterableB)
{
    typename Iterator::const_iterator_type itInitA = iterableA.cbegin();
    typename Iterator::const_iterator_type itEndA = iterableA.cend();

    typename Iterator::const_iterator_type itInitB = iterableB.cbegin();
    typename Iterator::const_iterator_type itEndB = iterableB.cend();

    return container::detail::equal(itInitA,itEndA, itInitB, itEndB,ytl::function<bool(typename Iterator::const_reference, typename Iterator::const_reference)>([](typename Iterator::const_reference i_lhs,typename Iterator::const_reference i_rhs){ return i_lhs == i_rhs;}));
}
template<typename Iterator>
inline bool equal(const container::detail::cConstIterableBase<Iterator>& iterableA, const container::detail::cConstIterableBase<Iterator>& iterableB, const ytl::function<bool(typename Iterator::const_reference,typename Iterator::const_reference)>& i_eqOp)
{
    typename Iterator::const_iterator_type itInitA = iterableA.cbegin();
    typename Iterator::const_iterator_type itEndA = iterableA.cend();

    typename Iterator::const_iterator_type itInitB = iterableB.cbegin();
    typename Iterator::const_iterator_type itEndB = iterableB.cend();

    return container::detail::equal(itInitA,itEndA, itInitB, itEndB,i_eqOp);
}

template<typename Iterator>
inline Iterator min(Iterator itInit, Iterator itEnd, const ytl::function<bool(typename Iterator::const_reference,typename Iterator::const_reference)>& i_lessOp, typename mpl::enable_if<container::detail::impl::is_inputable_iterator<Iterator>::value>::type* = NULL)
{
    return container::detail::cmp(itInit,itEnd,i_lessOp);
}

template<typename Iterator>
inline Iterator max(Iterator itInit, Iterator itEnd, const ytl::function<bool(typename Iterator::const_reference,typename Iterator::const_reference)>& i_moreOp, typename mpl::enable_if<container::detail::impl::is_inputable_iterator<Iterator>::value>::type* = NULL)
{
    return container::detail::cmp(itInit,itEnd,i_moreOp);
}

template<typename Iterator>
inline Iterator min(Iterator itInit, Iterator itEnd, typename mpl::enable_if<container::detail::impl::is_inputable_iterator<Iterator>::value>::type* = NULL)
{
    return container::detail::cmp(itInit,itEnd,ytl::function<bool(typename Iterator::const_reference, typename Iterator::const_reference)>([](typename Iterator::const_reference i_lhs,typename Iterator::const_reference i_rhs){ return i_lhs < i_rhs;}));
}

template<typename Iterator>
inline Iterator max(Iterator itInit, Iterator itEnd, typename mpl::enable_if<container::detail::impl::is_inputable_iterator<Iterator>::value>::type* = NULL)
{
    return container::detail::cmp(itInit,itEnd,ytl::function<bool(typename Iterator::const_reference, typename Iterator::const_reference)>([](typename Iterator::const_reference i_lhs,typename Iterator::const_reference i_rhs){ return i_lhs > i_rhs;}));
}

template<typename Iterator>
inline typename Iterator::const_iterator_type min(const container::detail::cConstIterableBase<Iterator>& iterable, const ytl::function<bool(typename Iterator::const_reference,typename Iterator::const_reference)>& i_lessOp)
{
    typename Iterator::const_iterator_type itInit = iterable.cbegin();
    typename Iterator::const_iterator_type itEnd = iterable.cend();

    return container::detail::cmp(itInit,itEnd,i_lessOp);
}

template<typename Iterator>
inline typename Iterator::const_iterator_type max(const container::detail::cConstIterableBase<Iterator>& iterable, const ytl::function<bool(typename Iterator::const_reference,typename Iterator::const_reference)>& i_moreOp)
{
    typename container::detail::cIterableBase<Iterator>::const_iterator_type itInit = iterable.cbegin();
    typename container::detail::cIterableBase<Iterator>::const_iterator_type itEnd = iterable.cend();

    return container::detail::cmp(itInit,itEnd,i_moreOp);
}

template<typename Iterator>
inline typename Iterator::const_iterator_type min(const container::detail::cConstIterableBase<Iterator>& iterable)
{
    typename Iterator::const_iterator_type itInit = iterable.cbegin();
    typename Iterator::const_iterator_type itEnd = iterable.cend();

    return container::detail::cmp(itInit,itEnd,ytl::function<bool(typename Iterator::const_reference, typename Iterator::const_reference)>([](typename Iterator::const_reference i_lhs,typename Iterator::const_reference i_rhs){ return i_lhs < i_rhs;}));
}

template<typename Iterator>
inline typename Iterator::const_iterator_type max(const container::detail::cConstIterableBase<Iterator>& iterable)
{
    typename Iterator::const_iterator_type itInit = iterable.cbegin();
    typename Iterator::const_iterator_type itEnd = iterable.cend();

    return container::detail::cmp(itInit,itEnd,ytl::function<bool(typename Iterator::const_reference, typename Iterator::const_reference)>([](typename Iterator::const_reference i_lhs,typename container::detail::cIterableBase<Iterator>::const_reference i_rhs){ return i_lhs > i_rhs;}));
}
template<typename Iterator>
inline typename Iterator::value_type apply(const container::detail::cConstIterableBase<Iterator>& iterable, const ytl::function<typename Iterator::value_type(typename Iterator::const_reference,typename Iterator::const_reference)>& i_op)
{
    typename Iterator::const_iterator_type itInit = iterable.cbegin();
    typename Iterator::const_iterator_type itEnd = iterable.cend();

    return container::detail::apply(itInit,itEnd,i_op);
}
template<typename T>
inline typename T::value_type apply(T itInit, T itEnd, const ytl::function<typename T::value_type(typename T::const_reference,typename T::const_reference)>& i_op, typename mpl::enable_if<container::detail::impl::is_inputable_iterator<T>::value>::type* = NULL)
{
    return container::detail::apply(itInit,itEnd,i_op);
}
template<typename Iterator>
inline typename Iterator::value_type sum(const container::detail::cConstIterableBase<Iterator>& iterable)
{
    typename Iterator::const_iterator_type itInit = iterable.cbegin();
    typename Iterator::const_iterator_type itEnd = iterable.cend();

    return container::detail::apply(itInit,itEnd,ytl::function<typename Iterator::value_type(typename Iterator::const_reference,typename Iterator::const_reference)>([](typename Iterator::const_reference i_lhs,typename Iterator::const_reference i_rhs) { return i_lhs + i_rhs;}));
}
template<typename Iterator>
inline typename Iterator::value_type sum(Iterator itInit, Iterator itEnd, typename mpl::enable_if<container::detail::impl::is_inputable_iterator<Iterator>::value>::type* = NULL)
{
    return container::detail::apply(itInit,itEnd,ytl::function<typename Iterator::value_type(typename Iterator::const_reference, typename Iterator::const_reference)>([](typename Iterator::const_reference i_lhs,typename Iterator::const_reference i_rhs){ return i_lhs + i_rhs;}));
}
template<typename Iterator>
inline typename Iterator::value_type prod(const container::detail::cConstIterableBase<Iterator>& iterable)
{
    typename Iterator::const_iterator_type itInit = iterable.cbegin();
    typename Iterator::const_iterator_type itEnd = iterable.cend();

    return container::detail::apply(itInit,itEnd,ytl::function<typename Iterator::value_type(typename Iterator::const_reference,typename Iterator::const_reference)>([](typename Iterator::const_reference i_lhs,typename Iterator::const_reference i_rhs) { return i_lhs * i_rhs;}));
}
template<typename Iterator>
inline typename Iterator::value_type prod(Iterator itInit, Iterator itEnd, typename mpl::enable_if<container::detail::impl::is_inputable_iterator<Iterator>::value>::type* = NULL)
{
    return container::detail::apply(itInit,itEnd,ytl::function<typename Iterator::value_type(typename Iterator::const_reference, typename Iterator::const_reference)>([](typename Iterator::const_reference i_lhs,typename Iterator::const_reference i_rhs){ return i_lhs * i_rhs;}));
}
template<typename Traits>
inline void sort(container::detail::impl::cRandomAccessIteratorImpl<Traits> itInit, container::detail::impl::cRandomAccessIteratorImpl<Traits> itEnd, const ytl::function<bool(typename Traits::const_reference,typename Traits::const_reference)>& i_lessOp)
{
    container::detail::sort(itInit,itEnd,i_lessOp);
}
template<typename Traits>
inline void sort(container::detail::impl::cRandomAccessIteratorImpl<Traits> itInit, container::detail::impl::cRandomAccessIteratorImpl<Traits> itEnd)
{
    container::detail::sort(itInit,itEnd,[](typename Traits::const_reference i_lhs,typename Traits::const_reference i_rhs){ return i_lhs < i_rhs;});
}
template<typename T>
inline void sort(const container::cConstRandomAccessIterable<T>& iterable, const ytl::function<bool(typename container::cRandomAccessIterable<T>::const_reference,typename container::cRandomAccessIterable<T>::const_reference)>& i_lessOp)
{
    typename container::cConstRandomAccessIterable<T>::const_iterator_type itInit = iterable.cbegin();
    typename container::cConstRandomAccessIterable<T>::const_iterator_type itEnd = iterable.cend();

    container::detail::sort(itInit,itEnd,i_lessOp);

    return;
}
template<typename T>
inline void sort(container::cConstRandomAccessIterable<T>& iterable)
{
    typename container::cConstRandomAccessIterable<T>::iterator_type itInit = iterable.begin();
    typename container::cConstRandomAccessIterable<T>::iterator_type itEnd = iterable.end();

    container::detail::sort(itInit,itEnd,ytl::function<bool(typename container::cRandomAccessIterable<T>::const_reference,typename container::cRandomAccessIterable<T>::const_reference)>([](typename container::cRandomAccessIterable<T>::const_reference i_lhs,typename container::cRandomAccessIterable<T>::const_reference i_rhs){ return i_lhs < i_rhs;}));

    return;
}
template<typename T, typename TT>
inline typename TT::iterator_type merge(T itInitA, T itEndA, T itInitB, T itEndB, TT itInitOut, const ytl::function<typename T::value_type(ytl::optional<typename T::const_reference>,ytl::optional<typename T::const_reference>)>& i_moreOp)
{
    return container::detail::merge(itInitA, itEndA, itInitB, itEndB, itInitOut, i_moreOp);
}
template<typename Iterator, typename IIterator>
inline IIterator merge(const container::detail::cConstIterableBase<Iterator>& iterableA, const container::detail::cConstIterableBase<Iterator>& iterableB, container::detail::cIterableBase<IIterator>& iterableOut, const ytl::function<typename Iterator::value_type(ytl::optional<typename Iterator::const_reference>,ytl::optional<typename Iterator::const_reference>)>& i_moreOp)
{
    typename Iterator::const_iterator_type itInitA = iterableA.cbegin();
    typename Iterator::const_iterator_type itEndA = iterableA.cend();
    typename Iterator::const_iterator_type itInitB = iterableB.cbegin();
    typename Iterator::const_iterator_type itEndB = iterableB.cend();

    return container::detail::merge(itInitA, itEndA, itInitB, itEndB, iterableOut, i_moreOp);
}
template<typename Iterator>
inline Iterator find(Iterator itInit, Iterator itEnd, typename Iterator::const_reference i_val)
{
    return container::detail::find_if(itInit, itEnd, ytl::function<bool(typename Iterator::const_reference)>([&i_val](typename Iterator::const_reference i_currVal){ return i_currVal == i_val; }));
}
template<typename Iterator>
inline typename Iterator::const_iterator_type find(const container::detail::cIterableBase<Iterator>& iterable, typename Iterator::const_reference i_val)
{
    typename Iterator::const_iterator_type itInit = iterable.cbegin();
    typename Iterator::const_iterator_type itEnd = iterable.cend();

    return container::detail::find_if(itInit, itEnd, ytl::function<bool(typename Iterator::const_reference)>([&i_val](typename Iterator::const_reference i_currVal){ return i_currVal == i_val; }));
}
template<typename T>
inline T find_if(T itInit, T itEnd, const ytl::function<bool(typename T::const_reference)>& i_findOp)
{
    return container::detail::find_if(itInit, itEnd, i_findOp);
}
template<typename Iterator>
inline typename Iterator::iterator_type find_if(container::detail::cIterableBase<Iterator>& iterable, const ytl::function<bool(typename Iterator::const_reference)>& i_findOp)
{
    typename Iterator::iterator_type itInit = iterable.begin();
    typename Iterator::iterator_type itEnd = iterable.end();

    return container::detail::find_if(itInit, itEnd, i_findOp);
}
template<typename Iterator>
inline typename Iterator::const_iterator_type find_if(const container::detail::cIterableBase<Iterator>& iterable, const ytl::function<bool(typename Iterator::const_reference)>& i_findOp)
{
    typename Iterator::const_iterator_type itInit = iterable.cbegin();
    typename Iterator::const_iterator_type itEnd = iterable.cend();

    return container::detail::find_if(itInit, itEnd, i_findOp);
}
template<typename T>
inline typename T::difference_type distance(T itInit, T itEnd)
{
    return container::detail::distance(itInit, itEnd);
}
template<typename T>
inline T initialize(T itInit, T itEnd, const std::initializer_list<typename T::value_type>& i_initializerList)
{
    return container::detail::initialize(itInit, itEnd, i_initializerList);
}
template<typename Iterator>
inline typename Iterator::iterator_type initialize(container::detail::cConstIterableBase<Iterator>& iterable, const std::initializer_list<typename Iterator::value_type>& i_initializerList)
{
    typename Iterator::iterator_type itInit = iterable.begin();
    typename Iterator::iterator_type itEnd = iterable.end();

    return container::detail::initialize(itInit, itEnd, i_initializerList);
}
template<typename Iterator, typename T>
inline typename Iterator::iterator_type assign(container::detail::cIterableBase<Iterator>& iterableOut, T itInit, T itEnd)
{
    return container::detail::assign(iterableOut, itInit, itEnd);
}
template<typename Iterator>
inline typename Iterator::iterator_type assign(container::detail::cIterableBase<Iterator>& iterableOut, const container::detail::cConstIterableBase<Iterator>& iterableIn)
{
    typename Iterator::const_iterator_type itInit = iterableIn.cbegin();
    typename Iterator::const_iterator_type itEnd = iterableIn.cend();

    return container::detail::assign(iterableOut, itInit, itEnd);
}
template<typename Iterator>
inline typename Iterator::iterator_type assign(container::detail::cIterableBase<Iterator>& iterableOut, const std::initializer_list<typename Iterator::value_type>& i_initializerList)
{
    return container::detail::assign(iterableOut, i_initializerList);
}
template<typename T, typename TT>
inline typename TT::iterator_type transform(T itInitInput, T itEndInput, TT itInitOutput, const ytl::function<typename T::value_type(typename T::const_reference)>& i_transformOp)
{
    return container::detail::transform(itInitInput, itEndInput, itInitOutput, i_transformOp);
}
template<typename Iterator, typename IIterator>
inline IIterator transform(const container::detail::cConstIterableBase<Iterator>& iterableInput, container::detail::cIterableBase<IIterator>& iterableOutput, const ytl::function<typename Iterator::value_type(typename Iterator::const_reference)>& i_transformOp)
{
    typename Iterator::const_iterator_type itInitInput = iterableInput.cbegin();
    typename Iterator::const_iterator_type itEndInput = iterableInput.cend();

    return container::detail::transform(itInitInput, itEndInput, iterableOutput, i_transformOp);
}

}
