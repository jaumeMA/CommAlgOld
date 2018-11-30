
namespace yame
{
namespace math
{

template<typename T, template<typename> class A>
const typename PolynomialGroupTraits<T,A>::underlying_type PolynomialGroupTraits<T,A>::neutral_element = typename PolynomialGroupTraits<T,A>::underlying_type();

template<typename Traits>
size_t PolynomialSetExtended<Traits>::getNumMonomials() const
{
    const underlying_type& thisValue = this->getValue();

    return thisValue.getTotalSize([](const container::cPair<key_type,value_type>& i_pair){ return i_pair.second.getValue() != field_type(); });
}
template<typename Traits>
template<typename ... Degrees>
typename PolynomialSetExtended<Traits>::field_type& PolynomialSetExtended<Traits>::at(Degrees ... degrees)
{
    underlying_type& thisValue = this->getValue();

    return thisValue.getAt(degrees...);
}
template<typename Traits>
template<typename ... Degrees>
const typename PolynomialSetExtended<Traits>::field_type& PolynomialSetExtended<Traits>::at(Degrees ... degrees) const
{
    const underlying_type& thisValue = this->getValue();

    return thisValue.getAt(degrees...);
}
template<typename Traits>
typename PolynomialSetExtended<Traits>::field_type& PolynomialSetExtended<Traits>::incognita(size_t index)
{
    underlying_type& thisValue = this->getValue();

    value_type* currValueNode = NULL;

    for(size_t currCounter=0;;++currCounter)
    {
        if(currCounter==index)
        {
            currValueNode = (currValueNode) ? &(currValueNode->operator[](1)) : &thisValue[1];

            return currValueNode->getValue();
        }
        else
        {
            currValueNode = (currValueNode) ? &(currValueNode->operator[](0)) : &thisValue[0];
        }
    }

    //for code path completion
    return *reinterpret_cast<field_type*>(0xDEAD);
}
template<typename Traits>
const typename PolynomialSetExtended<Traits>::field_type& PolynomialSetExtended<Traits>::incognita(size_t index) const
{
    const underlying_type& thisValue = this->getValue();
    const value_type* currValueNode = &thisValue[1];

    for(size_t currCounter=0;;++currCounter)
    {
        if(currCounter==index)
        {
            return currValueNode->getValue();
        }
        else
        {
            currValueNode = &(currValueNode[1]);
        }
    }

    //for code path completion
    return *reinterpret_cast<const field_type*>(0xDEAD);
}
template<typename Traits>
typename PolynomialSetExtended<Traits>::value_type& PolynomialSetExtended<Traits>::operator[](size_t i_index)
{
    underlying_type& thisValue = this->getValue();

    return thisValue[i_index];
}
template<typename Traits>
const typename PolynomialSetExtended<Traits>::value_type& PolynomialSetExtended<Traits>::operator[](size_t i_index) const
{
    const underlying_type& thisValue = this->getValue();

    return thisValue[i_index];
}
template<typename Traits>
const typename PolynomialSetExtended<Traits>::underlying_type& PolynomialSetExtended<Traits>::get_raw() const
{
    return this->getValue();
}

template<set_type T, template<typename> class A>
void PolynomialSetTraits<T,A>::init(underlying_type& o_value)
{
    //already handled by underlying traits
}
template<set_type T, template<typename> class A>
void PolynomialSetTraits<T,A>::init(underlying_type& o_value, const underlying_type& i_value)
{
    //holded by multimap
    o_value = i_value;
}
template<set_type T, template<typename> class A>
void PolynomialSetTraits<T,A>::init(underlying_type& o_value, underlying_type&& i_value)
{
    //holded by multimap
    o_value = mpl::move(i_value);
}
template<set_type T, template<typename> class A>
void PolynomialSetTraits<T,A>::deinit(underlying_type& o_value)
{
    //already handled by underlying traits
}
template<set_type T, template<typename> class A>
void PolynomialSetTraits<T,A>::assign(underlying_type& o_value, const underlying_type& i_value)
{
    //holded by multimap
    o_value = i_value;
}
template<set_type T, template<typename> class A>
void PolynomialSetTraits<T,A>::assign(underlying_type& o_value, underlying_type&& i_value)
{
    //holded by multimap
    o_value = mpl::move(i_value);
}
template<set_type T, template<typename> class A>
bool PolynomialSetTraits<T,A>::cmp(const underlying_type& i_lhs, const underlying_type& i_rhs)
{
    //holded by multimap
    return i_lhs == i_rhs;
}
template<set_type T, template<typename> class A>
const typename PolynomialSetTraits<T,A>::particle& PolynomialSetTraits<T,A>::access(const underlying_type& i_value, size_t i_index)
{
    return i_value.at(i_index);
}
template<set_type T, template<typename> class A>
typename PolynomialSetTraits<T,A>::particle& PolynomialSetTraits<T,A>::access(underlying_type& i_value, size_t i_index)
{
    return i_value.at(i_index);
}

template<group_type T, template<typename> class A>
typename PolynomialGroupTraits<T,A>::value_type PolynomialGroupTraits<T,A>::sum(const_lvaluereference i_lhs, const_lvaluereference i_rhs)
{
    value_type res = i_lhs;

    res += i_rhs.getValue();

    //iterate over keys on rhs
    typename underlying_type::const_iterator_type itRhs = i_rhs.cbegin();

    for(;itRhs!=i_rhs.cend();itRhs++)
    {
        res[(*itRhs).first] = sum(res[(*itRhs).first], i_rhs[(*itRhs).first]);
    }

    return res;
}
template<group_type T, template<typename> class A>
typename PolynomialGroupTraits<T,A>::value_type PolynomialGroupTraits<T,A>::subs(const_lvaluereference i_lhs, const_lvaluereference i_rhs)
{
    value_type res = i_lhs;

    res -= i_rhs.getValue();

    //iterate over keys on rhs
    typename underlying_type::const_iterator_type itRhs = i_rhs.cbegin();

    for(;itRhs!=i_rhs.cend();itRhs++)
    {
        res[(*itRhs).first] = subs(res[(*itRhs).first], i_rhs[(*itRhs).first]);
    }

    return res;
}
template<group_type T, template<typename> class A>
typename PolynomialGroupTraits<T,A>::value_type PolynomialGroupTraits<T,A>::inv(const_lvaluereference i_value)
{
    value_type res = -i_value;

    //iterate over keys on rhs
    typename underlying_type::const_iterator_type itValue = i_value.cbegin();

    for(;itValue!=i_value.cend();++itValue)
    {
        res[(*itValue).first] = inv((*itValue).second);
    }

    return res;
}
template<group_type T, template<typename> class A>
void PolynomialGroupTraits<T,A>::sum(underlying_type& o_res, const underlying_type& i_lhs,const underlying_type& i_rhs)
{
    o_res = i_lhs;

    typename cPolynomial<T,A>::const_iterator_type itPoliRhs = i_rhs.cbegin();

    for(;itPoliRhs!=i_rhs.cend();++itPoliRhs)
    {
        o_res[(*itPoliRhs).first] = sum(o_res[(*itPoliRhs).first], (*itPoliRhs).second);
    }
}
template<group_type T, template<typename> class A>
void PolynomialGroupTraits<T,A>::inv(underlying_type& o_res, const underlying_type& value)
{
    typename cPolynomial<T,A>::const_iterator_type itPoli = value.cbegin();

    for(;itPoli!=value.cend();++itPoli)
    {
        o_res[(*itPoli).first] = inv((*itPoli).second);
    }
}

template<ring_type T, template<typename> class A>
typename PolynomialModuleTraits<T,A>::value_type PolynomialModuleTraits<T,A>::mult(const_lvaluereference i_lhs, const T& coeff)
{
    value_type res = i_lhs * coeff;

    //iterate over keys on lhs
    typename underlying_type::const_iterator_type itLhs = i_lhs.cbegin();

    for(;itLhs!=i_lhs.cend();itLhs++)
    {
        value_type& resRef = res[(*itLhs).first];
        resRef = group_traits::sum(resRef,mult((*itLhs).second, coeff));
    }

    return res;
}
template<ring_type T, template<typename> class A>
typename PolynomialModuleTraits<T,A>::value_type PolynomialModuleTraits<T,A>::mult(const_lvaluereference i_lhs, const_lvaluereference i_rhs)
{
    value_type res = mult(i_rhs, i_lhs.getValue());

    //iterate over keys on lhs and rhs
    typename underlying_type::const_iterator_type itLhs = i_lhs.cbegin();

    for(;itLhs!=i_lhs.cend();itLhs++)
    {
        value_type& resRefL = res[(*itLhs).first];
        resRefL = group_traits::sum(resRefL,mult((*itLhs).second,i_rhs.getValue()));

        typename underlying_type::const_iterator_type itRhs = i_rhs.cbegin();

        for(;itRhs!=i_rhs.cend();itRhs++)
        {
            value_type& resRefLR = res[(*itLhs).first + (*itRhs).first];
            resRefLR = group_traits::sum(resRefLR,mult((*itLhs).second,(*itRhs).second));
        }
    }

    return res;
}
template<ring_type T, template<typename> class A>
void PolynomialModuleTraits<T,A>::modProd(underlying_type& o_res, const ring& i_lhs, const underlying_type& i_rhs)
{
    typename cPolynomial<T,A>::const_iterator_type itPoliRhs = i_rhs.cbegin();

    for(;itPoliRhs!=i_rhs.cend();++itPoliRhs)
    {
        o_res[((*itPoliRhs).first)] = mult((*itPoliRhs).second, i_lhs);
    }
}
template<ring_type T, template<typename> class A>
void PolynomialModuleTraits<T,A>::modProd(underlying_type& o_res, const underlying_type& i_lhs, const ring& i_rhs)
{
    typename cPolynomial<T,A>::const_iterator_type itPoliLhs = i_lhs.cbegin();

    for(;itPoliLhs!=i_lhs.cend();++itPoliLhs)
    {
        o_res[((*itPoliLhs).first)] = mult((*itPoliLhs).second, i_rhs);
    }
}

template<ring_type T, template<typename> class A>
void PolynomialAlgebraTraits<T,A>::algProd(underlying_type& o_res, const underlying_type& i_lhs, const underlying_type& i_rhs)
{
    typename underlying_type::const_iterator_type itPoliLhs = i_lhs.cbegin();

    for(;itPoliLhs!=i_lhs.cend();++itPoliLhs)
    {
        typename underlying_type::const_iterator_type itPoliRhs = i_rhs.cbegin();

        for(;itPoliRhs!=i_rhs.cend();++itPoliRhs)
        {
            value_type& resRef = o_res[((*itPoliLhs).first+(*itPoliRhs).first)];
            resRef = group_traits::sum(resRef,module_traits::mult((*itPoliLhs).second, (*itPoliRhs).second));
        }
    }
}

}
}
