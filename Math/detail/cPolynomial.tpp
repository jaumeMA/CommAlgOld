
namespace yame
{
namespace math
{

template<typename T, template<typename> class A>
cPolynomialValue<T,A>::cPolynomialValue()
: MapValue(T::group::neutral_element())
{}

template<typename T, template<typename> class A>
template<typename ... Args>
typename cPolynomialValue<T,A>::underlying_type& cPolynomialValue<T,A>::getAt(s16 key, Args&& ... keys)
{
    if(this->m_pNodeLocation->m_value.first == 0 && mpl::are_ranks_equal_to<0>(key, keys...))
    {
        return this->m_value;
    }
    else
    {
        MapValue& foundValue = MapClass::operator[](key);

        return foundValue.getAt(mpl::forward<Args>(keys)...);
    }
}
template<typename T, template<typename> class A>
template<typename ... Args>
const typename cPolynomialValue<T,A>::underlying_type& cPolynomialValue<T,A>::getAt(s16 key, Args&& ... keys) const
{
    if(this->m_pNodeLocation->m_value.first == 0 && mpl::are_ranks_equal_to<0>(key, keys...))
    {
        return this->m_value;
    }
    else
    {
        const MapValue& foundValue = MapClass::operator[](key);

        return foundValue.template MapValue::getAt(mpl::forward<Args>(keys)...);
    }
}
template<typename T, template<typename> class A>
cPolynomialValue<T,A>& cPolynomialValue<T,A>::operator=(const cPolynomialValue<T,A>& other)
{
    MapValue::operator=(other);

    return *this;
}
template<typename T, template<typename> class A>
cPolynomialValue<T,A>& cPolynomialValue<T,A>::operator=(cPolynomialValue<T,A>&& other)
{
    MapValue::operator=(mpl::move(other));

    return *this;
}
template<typename T, template<typename> class A>
cPolynomialValue<T,A> cPolynomialValue<T,A>::operator+(const T& i_value) const
{
    cPolynomialValue<T,A> res(this->m_value);

    res.m_value += i_value;

    return res;
}
template<typename T, template<typename> class A>
cPolynomialValue<T,A>& cPolynomialValue<T,A>::operator+=(const T& i_value)
{
    this->m_value += i_value;

    return *this;
}
template<typename T, template<typename> class A>
cPolynomialValue<T,A> cPolynomialValue<T,A>::operator-() const
{
    const cPolynomialValue<T,A> res(-this->m_value);

    return res;
}
template<typename T, template<typename> class A>
cPolynomialValue<T,A> cPolynomialValue<T,A>::operator-(const T& i_value) const
{
    cPolynomialValue<T,A> res(this->m_value);

    res -= i_value;

    return res;
}
template<typename T, template<typename> class A>
cPolynomialValue<T,A>& cPolynomialValue<T,A>::operator-=(const T& i_value)
{
    this->m_value -= i_value;

    return *this;
}
template<typename T, template<typename> class A>
cPolynomialValue<T,A> cPolynomialValue<T,A>::operator*(const T& i_value) const
{
    cPolynomialValue<T,A> res(this->m_value);

    res *= i_value;

    return res;
}
template<typename T, template<typename> class A>
cPolynomialValue<T,A>& cPolynomialValue<T,A>::operator*=(const T& i_value)
{
    this->m_value *= i_value;

    return *this;
}

}
}
