
#include "Math/cMath.h"
#include "YTL/functional/cFunctionOps.h"

namespace yame
{
namespace math
{

template<typename T, template<typename> class A>
cPolynomialIterable<T,A>::cPolynomialIterable(const polynomial<T,A>& i_polynomial)
: container::cMultiMapIterable<typename cPolynomial<T,A>::key_type,typename cPolynomial<T,A>::value_type,cMonomial<T>>(i_polynomial.getValue(), &cPolynomialIterable<T,A>::fromIterableNodeToMonomial)
, m_compare(null_ptr)
{
}
template<typename T, template<typename> class A>
cPolynomialIterable<T,A>::cPolynomialIterable(const cPolynomial<T,A>& i_polynomial)
: container::cMultiMapIterable<typename cPolynomial<T,A>::key_type,typename cPolynomial<T,A>::value_type,cMonomial<T>>(i_polynomial, &cPolynomialIterable<T,A>::fromIterableNodeToMonomial)
, m_compare(null_ptr)
{
}
template<typename T, template<typename> class A>
cPolynomialIterable<T,A>::cPolynomialIterable(const polynomial<T,A>& i_polynomial, const compare_func& i_compare)
: container::cMultiMapIterable<typename cPolynomial<T,A>::key_type,typename cPolynomial<T,A>::value_type,cMonomial<T>>(i_polynomial.getValue(), &cPolynomialIterable<T,A>::fromIterableNodeToMonomial)
, m_compare(i_compare)
{
}
template<typename T, template<typename> class A>
cPolynomialIterable<T,A>::cPolynomialIterable(const cPolynomial<T,A>& i_polynomial, const compare_func& i_compare)
: container::cMultiMapIterable<typename cPolynomial<T,A>::key_type,typename cPolynomial<T,A>::value_type,cMonomial<T>>(i_polynomial.getValue(), &cPolynomialIterable<T,A>::fromIterableNodeToMonomial)
, m_compare(i_compare)
{
}
template<typename T, template<typename> class A>
typename cPolynomialIterable<T,A>::iterator_type cPolynomialIterable<T,A>::begin(const ytl::function<bool(const_reference)>& filter)
{
    return container::cMultiMapIterable<typename cPolynomial<T,A>::key_type,typename cPolynomial<T,A>::value_type,cMonomial<T>>::begin( ytl::function<bool(const_reference)>([](const value_type& i_value) { return i_value.getCoeff() != T::group::neutral_element(); }) && filter);
}
template<typename T, template<typename> class A>
typename cPolynomialIterable<T,A>::const_iterator_type cPolynomialIterable<T,A>::cbegin(const ytl::function<bool(const_reference)>& filter) const
{
    return container::cMultiMapIterable<typename cPolynomial<T,A>::key_type,typename cPolynomial<T,A>::value_type,cMonomial<T>>::cbegin( ytl::function<bool(const_reference)>([](const value_type& i_value){ return i_value.getCoeff() != T::group::neutral_element(); }) && filter);
}
template<typename T, template<typename> class A>
typename cPolynomialIterable<T,A>::reverse_iterator_type cPolynomialIterable<T,A>::rbegin(const ytl::function<bool(const_reference)>& filter)
{
    return container::cMultiMapIterable<typename cPolynomial<T,A>::key_type,typename cPolynomial<T,A>::value_type,cMonomial<T>>::rbegin( ytl::function<bool(const_reference)>([](const value_type& i_value) { return i_value.getCoeff() != T::group::neutral_element(); }) && filter);
}
template<typename T, template<typename> class A>
typename cPolynomialIterable<T,A>::const_reverse_iterator_type cPolynomialIterable<T,A>::crbegin(const ytl::function<bool(const_reference)>& filter) const
{
    return container::cMultiMapIterable<typename cPolynomial<T,A>::key_type,typename cPolynomial<T,A>::value_type,cMonomial<T>>::crbegin( ytl::function<bool(const_reference)>([](const value_type& i_value){ return i_value.getCoeff() != T::group::neutral_element(); }) && filter);
}
template<typename T, template<typename> class A>
typename cPolynomialIterable<T,A>::node_pointer_type cPolynomialIterable<T,A>::getNextElem(node_pointer_type currNode) const
{
    if(m_compare != null_ptr)
    {
        const_reference currValue = this->getValue(currNode);
        typename base::pair_node* currPair = container::detail::receiveNodeFromIterator<typename base::pair_node*>(currNode);
        currPair->innerNode = currPair->backendInterface->getPrivateIterable().getFirstElem();

        node_pointer_type prevNode = inner_iterator_type::m_pHead;
        value_type prevValue, nextValue;

        while(currPair->innerNode != inner_iterator_type::m_pHead)
        {
            nextValue = this->getValue(currNode);

            if(m_compare.eval(currValue,nextValue) && (prevNode == inner_iterator_type::m_pHead || m_compare.eval(nextValue,prevValue)))
            {
                prevValue = this->getValue(currNode);
                prevNode = currPair->innerNode;
            }

            currPair->innerNode = base::getNextElemInCurrentIterable(currPair->backendInterface,currPair->innerNode);
        }

        if(prevNode != inner_iterator_type::m_pHead)
        {
            currPair->innerNode = prevNode;
            return container::detail::sendNodeToIterator<node_pointer_type>(currPair);
        }
        else
        {
            base::deletePair(currPair);
            return iterator_type::m_pHead;
        }
    }
    else
    {
        return base::getNextElem(currNode);
    }
}
template<typename T, template<typename> class A>
typename cPolynomialIterable<T,A>::node_pointer_type cPolynomialIterable<T,A>::getPrevElem(node_pointer_type currNode) const
{
    if(m_compare != null_ptr)
    {
        const_reference currValue = this->getValue(currNode);
        typename base::pair_node* currPair = container::detail::receiveNodeFromIterator<typename base::pair_node*>(currNode);
        currPair->innerNode = currPair->backendInterface->getPrivateIterable().getFirstElem();

        node_pointer_type prevNode = inner_iterator_type::m_pHead;
        value_type prevValue, nextValue;

        while(currPair->innerNode != inner_iterator_type::m_pHead)
        {
            nextValue = this->getValue(currNode);

            if(m_compare.eval(nextValue,currValue) && (prevNode == inner_iterator_type::m_pHead || m_compare.eval(prevValue,nextValue)))
            {
                prevValue = this->getValue(currNode);
                prevNode = currPair->innerNode;
            }

            currPair->innerNode = base::getNextElemInCurrentIterable(currPair->backendInterface,currPair->innerNode);
        }

        if(prevNode != inner_iterator_type::m_pHead)
        {
            currPair->innerNode = prevNode;
            return container::detail::sendNodeToIterator<node_pointer_type>(currPair);
        }
        else
        {
            base::deletePair(currPair);
            return iterator_type::m_pHead;
        }
    }
    else
    {
        return base::getPrevElem(currNode);
    }
}
template<typename T, template<typename> class A>
typename cPolynomialIterable<T,A>::node_pointer_type cPolynomialIterable<T,A>::getFirstElem() const
{
    if(m_compare != null_ptr)
    {
        node_pointer_type currElem = base::getFirstElem();

        if(currElem != iterator_type::m_pHead)
        {
            typename base::pair_node* currPair = container::detail::receiveNodeFromIterator<typename base::pair_node*>(currElem);
            node_pointer_type prevNode = currPair->innerNode;

            value_type prevValue = this->getValue(currElem);

            currPair->innerNode = base::getNextElemInCurrentIterable(currPair->backendInterface,currPair->innerNode);

            while(currPair->innerNode != inner_iterator_type::m_pHead)
            {
                if(m_compare.eval(this->getValue(currElem),prevValue))
                {
                    prevValue = this->getValue(currElem);
                    prevNode = currPair->innerNode;
                }

                currPair->innerNode = base::getNextElemInCurrentIterable(currPair->backendInterface,currPair->innerNode);
            }

            currPair->innerNode = prevNode;
            return container::detail::sendNodeToIterator<node_pointer_type>(currPair);
        }
        else
        {
            return iterator_type::m_pHead;
        }
    }
    else
    {
        return base::getFirstElem();
    }
}
template<typename T, template<typename> class A>
typename cPolynomialIterable<T,A>::node_pointer_type cPolynomialIterable<T,A>::getLastElem() const
{
    if(m_compare != null_ptr)
    {
        node_pointer_type currElem = base::getFirstElem();

        if(currElem != iterator_type::m_pHead)
        {
            typename base::pair_node* currPair = container::detail::receiveNodeFromIterator<typename base::pair_node*>(currElem);
            node_pointer_type prevNode = currPair->innerNode;

            value_type prevValue = this->getValue(currElem);

            currPair->innerNode = base::getNextElemInCurrentIterable(currPair->backendInterface,currPair->innerNode);

            while(currPair->innerNode != inner_iterator_type::m_pHead)
            {
                if(m_compare.eval(prevValue,this->getValue(currElem)))
                {
                    prevValue = this->getValue(currElem);
                    prevNode = currPair->innerNode;
                }

                currPair->innerNode = base::getNextElemInCurrentIterable(currPair->backendInterface,currPair->innerNode);
            }

            currPair->innerNode = prevNode;
            return container::detail::sendNodeToIterator<node_pointer_type>(currPair);
        }
        else
        {
            return iterator_type::m_pHead;
        }
    }
    else
    {
        return base::getLastElem();
    }
}
template<typename T, template<typename> class A>
cMonomial<T> cPolynomialIterable<T,A>::fromIterableNodeToMonomial(container::cPair<typename cPolynomial<T,A>::key_type,typename cPolynomial<T,A>::value_type>& i_val)
{
    cMonomial<T>res;

    //set monomial coefficient
    res.setCoeff(i_val.second.getValue());
    //res.insertPoweredIndex(0,i_val.first);

    //recover arrays of powered indexes
    auto traverseSink = [&res](const s16& i_key,const T& i_value)
    {
        res.insertPoweredIndex(0,i_key);
    };

    yame::container::traverse(i_val.second,ytl::function<void(const s16&,const T&)>(traverseSink));

    return res;
}

}
}
