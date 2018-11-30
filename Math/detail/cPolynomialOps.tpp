
#include "Math/cPolynomialIterable.h"
#include "YTL/container/cAlgorithm.h"

// NOTE: in certain functions inside this class we use move semantics over temporary variables for avoiding
// RVo optimization since it really causes side effects (basically it avoids to set references properly when
// copy constructing and we dont really want to put setReferences per intermidiate copy since it would go
// really against performance)

namespace yame
{
namespace math
{
namespace detail
{

template<template<typename> class A, typename T>
polynomial<T,A> createPolyFromMonomial(const cMonomial<T>& i_monomial)
{
    polynomial<T,A> res;

    typename math::cMonomial<T>::power_index_view indexView = i_monomial.getPoweredIndexes();

    if(indexView.empty() == false)
    {
        typedef typename math::cMonomial<T>::power_index_view::const_reverse_iterator_type const_reverse_iterator_type;
        const const_reverse_iterator_type itEnd = indexView.crend();
        const_reverse_iterator_type itCurr = find_if(indexView.crbegin(),itEnd,ytl::function<bool(const s16&)>([](const s16& i_index){ return i_index!= 0; }));

        if(itCurr != itEnd)
        {
            typedef typename cPolynomial<T>::value_type value_type;

            ytl::function<value_type*(const_reverse_iterator_type)> getCoeff = [&res,&getCoeff,&itEnd](const_reverse_iterator_type itCurrIndex)
            {
                const size_t currPowerIndex = *itCurrIndex;

                return (++itCurrIndex != itEnd) ? &((*getCoeff.eval(itCurrIndex))[currPowerIndex]) : &res[currPowerIndex];
            };

            if(value_type* currNode = getCoeff.eval(itCurr))
            {
                *currNode = i_monomial.getCoeff();
            }
        }
        else
        {
            res[0] = i_monomial.getCoeff();
        }
    }

    return res;
}

template<size_t Component, typename T, template<typename> class A>
cPolynomialValue<T,A> derivative(const cPolynomialValue<T,A>& i_subPoly, size_t currDepth)
{
    cPolynomialValue<T,A> res;

    //divide and conquer
    typename cPolynomialValue<T,A>::const_iterator_type itDegree = i_subPoly.cbegin();
    for(;itDegree!=i_subPoly.cend();++itDegree)
    {
        const container::cPair<const s16,cPolynomialValue<T,A>>& node = *itDegree;

        if(Component == currDepth)
        {
            if(node.first > 0)
            {
                res[node.first - 1] = PolynomialModuleTraits<T,A>::mult(node.second, T(node.first));
            }
        }
        else
        {
            //keep diggin
            res[node.first] = derivative<Component>(node.second,currDepth + 1);
        }
    }

    return res;
}

template<size_t Component, typename T, template<typename> class A>
container::detail::cMultiMapImpl<cPolynomialValue<T,A>,s16,container::cAVLtree,A,container::MultiBalancer<container::cAVLtree>::Balancer> derivative(const container::detail::cMultiMapImpl<cPolynomialValue<T,A>,s16,container::cAVLtree,A,container::MultiBalancer<container::cAVLtree>::Balancer>& i_subPoly)
{
    container::detail::cMultiMapImpl<cPolynomialValue<T,A>,s16,container::cAVLtree,A,container::MultiBalancer<container::cAVLtree>::Balancer> res;

    //divide and conquer
    typename container::detail::cMultiMapImpl<cPolynomialValue<T,A>,s16,container::cAVLtree,A,container::MultiBalancer<container::cAVLtree>::Balancer>::const_iterator_type itDegree = i_subPoly.cbegin();
    for(;itDegree!=i_subPoly.cend();++itDegree)
    {
        const container::cPair<const s16,cPolynomialValue<T,A>>& node = *itDegree;

        if(Component == 0)
        {
            if(node.first > 0)
            {
                res[node.first - 1] = PolynomialModuleTraits<T,A>::mult(node.second, T(node.first));
            }
        }
        else
        {
            //keep diggin
            res[node.first] = derivative<Component>(node.second,1);
        }
    }

    return res;
}

template<template<typename> class A, typename Im, typename Dom>
template<size_t Component,typename Foo>
void taylor<A,Im,Dom>::expansion<Component,Foo>::is(const cFunctionSpace<Im,Dom>& i_function, const ytl::function<void(const cFunctionSpace<Im,Dom>&, const container::cTupla<size_t,Dom::dimension()>&)>& o_sink, const container::cTupla<size_t,Dom::dimension()>& i_indexes, int currOrder)
{
    container::cTupla<size_t,s_dimension> localIndexes = i_indexes;
    localIndexes[Component]++;
    cFunctionSpace<Im,Dom> partialDerivative = derivative<Component>(i_function);

    //send current derivative
    o_sink.eval(partialDerivative, localIndexes);

    //in case you can, send next order
    if(currOrder < k_maxDerivativeOrder)
    {
        taylor<A,Im,Dom>::expansion<Component,Foo>::is(partialDerivative,o_sink,localIndexes,currOrder+1);
    }

    //in case you can, and over current order, send next component
    taylor<A,Im,Dom>::expansion<Component+1,Foo>::is(i_function,o_sink,i_indexes,currOrder);
}

template<template<typename> class A, typename Im, typename Dom>
template<typename Foo>
void taylor<A,Im,Dom>::expansion<Dom::dimension()-1,Foo>::is(const cFunctionSpace<Im,Dom>& i_function, const ytl::function<void(const cFunctionSpace<Im,Dom>&, const container::cTupla<size_t,Dom::dimension()>&)>& o_sink, container::cTupla<size_t,Dom::dimension()> i_localIndexes, int currOrder)
{
    i_localIndexes[s_dimension-1]++;
    cFunctionSpace<Im,Dom> partialDerivative = derivative<s_dimension-1>(i_function);

    //send current derivative
    o_sink.eval(partialDerivative, i_localIndexes);

    //in case you can, send next order
    if(currOrder < k_maxDerivativeOrder)
    {
        taylor<A,Im,Dom>::expansion<s_dimension-1,Foo>::is(partialDerivative, o_sink, i_localIndexes,currOrder+1);
    }
}

}

template<typename T, template<typename> class A>
yame::container::cArray<yame::math::polynomial<T,A>> grobnerBase(const yame::container::cArray<yame::math::polynomial<T,A>>& i_ideal, const ytl::function<bool(yame::math::cMonomial<T>,yame::math::cMonomial<T>)>& i_compOp)
{
    typedef typename yame::math::cPolynomialIterable<T,A>::const_iterator_type const_poly_iterator;

    yame::container::cArray<yame::math::polynomial<T,A>> res = i_ideal;
    size_t sSize = i_ideal.getSize();
    container::cArray<u16> indexJ;//fill it from 1 to s
    container::cArray<u16> indexI;//fill it from 1 to s, this is more than we need, but its ok
    cMonomial<T> jLeadingMonomial, iLeadingMonomial;

    indexJ.resize(sSize+1);
    for(size_t currIndex=0;currIndex < sSize; currIndex++) indexJ[currIndex] = currIndex;
    indexI.resize(sSize);
    for(size_t currIndex=0;currIndex < sSize-1; currIndex++) indexI[currIndex] = currIndex;

    for(size_t j=0;j<sSize;++j)
    {
        const polynomial<T,A>& polyJ = res[indexJ[j]];

        {
            yame::math::cPolynomialIterable<T,A> itPolyJ(polyJ);

            const_poly_iterator itJLeadingTerm = yame::max(itPolyJ, i_compOp);

            if(itJLeadingTerm == itPolyJ.cend())
            {
                continue;
            }

            jLeadingMonomial = *itJLeadingTerm;
        }

        for(size_t i=0;i<j;++i)
        {
            const polynomial<T,A>& polyI = res[indexI[i]];

            {
                yame::math::cPolynomialIterable<T,A> itPolyI(polyI);

                const_poly_iterator itILeadingTerm = yame::max(itPolyI, i_compOp);

                if(itILeadingTerm == itPolyI.cend())
                {
                    continue;
                }

                iLeadingMonomial = *itILeadingTerm;
            }

            yame::container::cArray<s16> maxExp;
            container::cBidirectionalIntraIterable<s16&> itMaxExp(maxExp);
            typename math::cMonomial<T>::power_index_view iIndexView = iLeadingMonomial.getPoweredIndexes();
            typename math::cMonomial<T>::power_index_view jIndexView = jLeadingMonomial.getPoweredIndexes();
            yame::merge(iIndexView.cbegin(),iIndexView.cend(),jIndexView.cbegin(),jIndexView.cend(),itMaxExp, yame::ytl::function<s16(ytl::optional<const s16&>,ytl::optional<const s16&>)>([](ytl::optional<const s16&> i_lhs, ytl::optional<const s16&> i_rhs) { if(i_lhs && i_rhs) return yame::max(*i_lhs,*i_rhs); else if(i_lhs) return *i_lhs; else return *i_rhs; }));

            yame::container::cArray<s16> sumExp;
            container::cBidirectionalIntraIterable<s16&> itSumExp(sumExp);
            yame::merge(iIndexView.cbegin(),iIndexView.cend(),jIndexView.cbegin(),jIndexView.cend(),itSumExp, yame::ytl::function<s16(ytl::optional<const s16&>,ytl::optional<const s16&>)>([](ytl::optional<const s16&> i_lhs, ytl::optional<const s16&> i_rhs) -> s16 { if(i_lhs && i_rhs) return *i_lhs + *i_rhs; else if(i_lhs) return *i_lhs; else return *i_rhs; }));

            if(yame::equal(itMaxExp, itSumExp) == true)
            {
                continue;
            }

            //look for a k in (1,#res) such that (i,k) and (k,j) are not present in indexJ, IndexI
            bool divCond = false;
            for(size_t k=0;!divCond && k<sSize;k++)
            {
                if(k<i && k<j)
                {
                    yame::math::cPolynomialIterable<T,A> polyK(res[k]);

                    const_poly_iterator itKLeadingTerm = yame::max(polyK, i_compOp);

                    if(itKLeadingTerm == polyK.cend())
                    {
                        continue;
                    }

                    math::cMonomial<T> kLeadingMonomial = *itKLeadingTerm;
                    container::cArray<s16> subsExp;
                    container::cBidirectionalIntraIterable<s16&> itSubsExp(subsExp);

                    typename math::cMonomial<T>::power_index_view kPoweredIndexes = kLeadingMonomial.getPoweredIndexes();
                    //yame::merge(itMaxExp.cbegin(), itMaxExp.cend(), kPoweredIndexes.cbegin(), kPoweredIndexes.cend(), itSubsExp, ytl::function<s16(ytl::optional<const s16&>,ytl::optional<const s16&>)>([](ytl::optional<const s16&> i_indexA, ytl::optional<const s16&> i_indexB){ s16 res = 0; if(i_indexA)res=*i_indexA; if(i_indexB)res-=*i_indexB; return res; }));

                    //check that LCM is divisible by f_k
                    if(yame::find_if(itSubsExp, ytl::function<bool(const s16&)>([](const s16& i_index){ return i_index < 0; })) == itSubsExp.end())
                    {
                        divCond = true;
                    }
                }
            }

            if(divCond)
            {
                //do division of s-polys
                yame::container::cArray<s16> gamma;
                yame::merge(iIndexView.cbegin(),iIndexView.cend(),jIndexView.cbegin(),jIndexView.cend(),gamma, yame::ytl::function<s16(ytl::optional<const s16&>,ytl::optional<const s16&>)>([](ytl::optional<const s16&> i_lhs, ytl::optional<const s16&> i_rhs) { if(i_lhs && i_rhs) return yame::max(*i_lhs,*i_rhs); else if(i_lhs) return *i_lhs; else return *i_rhs; }));
                //define S-poly with i and j polys
                yame::math::polynomial<T,A> sPoly = *(yame::math::cMonomial<T>(T::ring::neutral_element(), gamma) / iLeadingMonomial) * polyI - *(yame::math::cMonomial<T>(T::ring::neutral_element(), gamma) / jLeadingMonomial) * polyJ;
                yame::container::cArray<yame::math::polynomial<T,A>> div = sPoly / res;
                const yame::math::polynomial<T,A>& remain = div[0];

                if(yame::math::cPolynomialIterable<T,A>(remain).empty() == false)
                {
                    //here we are altering the collection of polynomials with potential reallocations, destroy before iterables and iterators bound to it
                    res.push_back(remain);

					indexJ.push_back(res.getSize() - 1);
					indexI.push_back(res.getSize() - 1);

					sSize++;
                }
            }
        }
    }

    return res;
}

template<size_t ... Components, typename T, template<typename> class A>
container::cTupla<polynomial<T,A>, mpl::get_num_ranks<Components...>::value> derivative(const polynomial<T,A>& i_poly)
{
    return { polynomial<T,A>(detail::derivative<Components>(i_poly.get_raw())) ...};
}

template<typename Im, typename Dom, template<typename> class A = memory::cTypedSystemAllocator>
requires requires { math::is_ring<Im>::value; math::is_vector_space<Dom>::value; math::is_metric_space<Dom>::value; }
polynomial<Im,A> taylorSeries(const cFunctionSpace<Im,Dom>& i_function, const Dom& i_point)
{
    static const size_t s_dimension = Dom::dimension();
    polynomial<Im,A> res;
    res.at(0) = i_function.eval(i_point);
    container::cTupla<size_t,s_dimension> indices;

    ytl::function<void(const cFunctionSpace<Im,Dom>&,const container::cTupla<size_t,s_dimension>&)> derivativeSink =
    [&res,&i_point](const cFunctionSpace<Im,Dom>& i_derivative, const container::cTupla<size_t,s_dimension>& i_indexes) -> void
    {
        Im derivativeValue = i_derivative.eval(i_point);

        if(derivativeValue != Im::group::neutral_element())
        {
            container::cArray<size_t> factorials;
            factorials.reserve(s_dimension);
            bool firstMonomial = true;
            polynomial<Im,A> currPoly;

            for(size_t index=0;index<i_indexes.getSize();++index)
            {
                polynomial<Im,A> polyRoot;
                polyRoot.at(0) = -i_point[index];
                polyRoot.incognita(index) = Im::ring::neutral_element();
                currPoly = (firstMonomial) ? polyRoot ^ i_indexes[index] : currPoly * (polyRoot ^ i_indexes[index]);
                firstMonomial = false;
            }

            yame::transform(i_indexes,factorials,ytl::function<size_t(const size_t&)>(&math::factorial));

            res += currPoly * derivativeValue * (Im::ring::neutral_element() / Im(yame::prod(factorials)));
        }
    };

    detail::taylor<A,Im,Dom>::template expansion<0,void>::is(i_function, derivativeSink, indices,0);

    return res;
}

template<typename T>
yame::math::polynomial<T> operator+(const yame::math::cMonomial<T>& i_lhs, const yame::math::cMonomial<T>& i_rhs)
{
    return yame::math::detail::createPolyFromMonomial<yame::memory::cTypedSystemAllocator>(i_lhs) + yame::math::detail::createPolyFromMonomial<yame::memory::cTypedSystemAllocator>(i_rhs);
}
template<typename T, template<typename> class A>
yame::math::polynomial<T,A> operator+(const yame::math::polynomial<T,A>& i_lhs, const yame::math::cMonomial<T>& i_rhs)
{
    return i_lhs + yame::math::detail::createPolyFromMonomial<A>(i_rhs);
}
template<typename T, template<typename> class A>
const yame::math::polynomial<T,A>& operator+=(yame::math::polynomial<T,A>& i_lhs, const yame::math::cMonomial<T>& i_rhs)
{
    i_lhs += yame::math::detail::createPolyFromMonomial<A>(i_rhs);

    return i_lhs;
}
template<typename T, template<typename> class A>
yame::math::polynomial<T,A> operator+(const yame::math::cMonomial<T>& i_lhs, const yame::math::polynomial<T,A>& i_rhs)
{
    return yame::math::detail::createPolyFromMonomial<A>(i_lhs) + i_rhs;
}
template<typename T, template<typename> class A>
yame::math::polynomial<T,A> operator-(const yame::math::polynomial<T,A>& i_lhs, const yame::math::cMonomial<T>& i_rhs)
{
    return i_lhs - yame::math::detail::createPolyFromMonomial<A>(i_rhs);
}
template<typename T, template<typename> class A>
const yame::math::polynomial<T,A>& operator-=(yame::math::polynomial<T,A>& i_lhs, const yame::math::cMonomial<T>& i_rhs)
{
    i_lhs -= yame::math::detail::createPolyFromMonomial<A>(i_rhs);

    return i_lhs;
}
template<typename T, template<typename> class A>
yame::math::polynomial<T,A> operator-(const yame::math::cMonomial<T>& i_lhs, const yame::math::polynomial<T,A>& i_rhs)
{
    return yame::math::detail::createPolyFromMonomial<A>(i_lhs) - i_rhs;
}
template<typename T, template<typename> class A>
yame::math::polynomial<T,A> operator*(const yame::math::polynomial<T,A>& i_lhs, const yame::math::cMonomial<T>& i_rhs)
{
    return i_lhs * yame::math::detail::createPolyFromMonomial<A>(i_rhs);
}
template<typename T, template<typename> class A>
const yame::math::polynomial<T,A>& operator*=(yame::math::polynomial<T,A>& i_lhs, const yame::math::cMonomial<T>& i_rhs)
{
    i_lhs *= yame::math::detail::createPolyFromMonomial<A>(i_rhs);

    return i_lhs;
}
template<typename T, template<typename> class A>
yame::math::polynomial<T,A> operator*(const yame::math::cMonomial<T>& i_lhs, const yame::math::polynomial<T,A>& i_rhs)
{
    return yame::math::detail::createPolyFromMonomial<A>(i_lhs) * i_rhs;
}

template<typename T, template<typename> class A>
yame::container::cArray<yame::math::polynomial<T,A>> operator/(yame::math::polynomial<T,A> i_lhs, const yame::container::cArray<yame::math::polynomial<T,A>>& i_rhs)
{
    typedef typename yame::math::cPolynomialIterable<T,A>::const_iterator_type const_poly_iterator;

    yame::container::cArray<yame::math::polynomial<T,A>> res;
    const size_t rhsSize = i_rhs.getSize();
    res.resize(rhsSize + 1);
    yame::math::cPolynomialIterable<T,A> lhsIterable(i_lhs);

    while(lhsIterable.empty() == false)
    {
        bool foundDivisor = false;
        const_poly_iterator itDividenLeadingTerm = yame::max(lhsIterable, yame::ytl::function<bool(yame::math::cMonomial<T>,yame::math::cMonomial<T>)>(&yame::math::cMonomial<T>::lexMoreComparison));

        if(itDividenLeadingTerm == lhsIterable.cend())
        {
            break;
        }

        yame::math::cMonomial<T> leadingDividenMonomial = *itDividenLeadingTerm;

        for(size_t divisorIndex=0;divisorIndex<rhsSize && !foundDivisor;divisorIndex++)
        {
            const yame::math::polynomial<T,A>& currDivisor = i_rhs[divisorIndex];
            yame::math::cPolynomialIterable<T,A> rhsIterable(currDivisor);
            const_poly_iterator itDivisorLeadingTerm = yame::max(rhsIterable,yame::ytl::function<bool(yame::math::cMonomial<T>,yame::math::cMonomial<T>)>(&yame::math::cMonomial<T>::lexMoreComparison));

            if(itDivisorLeadingTerm == rhsIterable.cend())
            {
                continue;
            }

            if(yame::ytl::optional<yame::math::cMonomial<T>> partialDivOpt = leadingDividenMonomial / *itDivisorLeadingTerm)
            {
                const yame::math::cMonomial<T>& partiaDiv = *partialDivOpt;
                res[divisorIndex+1] += partiaDiv;
                i_lhs -= partiaDiv * currDivisor;
                foundDivisor = true;
            }
        }

        if(!foundDivisor)
        {
            res[0] += leadingDividenMonomial;
            i_lhs -= leadingDividenMonomial;
        }
    }

    return res;
}

template<typename T, template<typename> class A>
inline yame::math::polynomial<T,A> operator^(yame::math::polynomial<T,A> i_lhs, size_t i_rhs)
{
    yame::math::polynomial<T,A> res;
    res[0] = T::ring::neutral_element();

    for(size_t prodCounter=0;prodCounter<i_rhs;prodCounter++)
    {
        res *= i_lhs;
    }

    return res;
}

}

template<typename T, template<typename> class A>
container::string format(const math::polynomial<T,A>& i_value)
{
    container::string res;
    bool firstItem = true;
    yame::math::cPolynomialIterable<T,A> polyIterable(i_value);

    typename yame::math::cPolynomialIterable<T,A>::const_iterator_type itPoly = polyIterable.cbegin();
    for(;itPoly!=polyIterable.cend();++itPoly)
    {
        res += (firstItem) ? format(*itPoly) : " + " + format(*itPoly);
        firstItem = false;
    }

    return res;
}

}
