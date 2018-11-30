
namespace yame
{
namespace math
{
namespace detail
{

template<group_traits_type Traits, typename FinalObject>
FinalObject& cGroupImpl<Traits,FinalObject>::operator+=(const cGroupImpl<Traits,FinalObject>& other)
{
    FinalObject res;

    Traits::sum(res.getValue(), this->getValue(), other.getValue());

    this->setValue(res.getValue());

    return static_cast<FinalObject&>(*this);
}
template<group_traits_type Traits, typename FinalObject>
FinalObject& cGroupImpl<Traits,FinalObject>::operator-=(const cGroupImpl<Traits,FinalObject>& other)
{
    FinalObject res;
    FinalObject subs;

    Traits::inv(subs.getValue(), other.getValue());
    Traits::sum(res.getValue(), this->getValue(),subs.getValue());

    this->setValue(res.getValue());

    return static_cast<FinalObject&>(*this);
}
template<group_traits_type Traits, typename FinalObject>
FinalObject cGroupImpl<Traits,FinalObject>::neutral_element()
{
    return FinalObject(Traits::neutral_element);
}

template<ring_traits_type Traits, typename FinalObject>
FinalObject& cRingImpl<Traits,FinalObject>::operator*=(const cRingImpl<Traits,FinalObject>& other)
{
    FinalObject res;

    Traits::prod(res.getValue(), this->getValue(), other.getValue());

    this->setValue(res.getValue());

    return static_cast<FinalObject&>(*this);
}
template<ring_traits_type Traits, typename FinalObject>
FinalObject cRingImpl<Traits,FinalObject>::neutral_element()
{
    return FinalObject(Traits::neutral_element);
}

template<division_ring_traits_type Traits, typename FinalObject>
FinalObject& cDivisionRingImpl<Traits,FinalObject>::operator/=(const cDivisionRingImpl<Traits,FinalObject>& other) const
{
    FinalObject res;

    Traits::inv(res, other.getValue());

    Traits::ring_traits::prod(res.getValue(), this->getValue(), res.getValue());

    this->setValue(res.getValue());

    return static_cast<FinalObject&>(*this);
}

template<module_traits_type Traits, typename FinalObject>
FinalObject cModuleImpl<Traits,FinalObject>::prod(const ring& other) const
{
    FinalObject res;

    Traits::modProd(res.getValue(), this->getValue(), other.getValue());

    return res;
}

template<typename Traits, typename FinalObject>
FinalObject& cAlgebraImpl<Traits,FinalObject>::operator*=(const cAlgebraImpl<Traits,FinalObject>& other)
{
    FinalObject res;

    Traits::algProd(res.getValue(), this->getValue(), other.getValue());

    this->setValue(res.getValue());

    return static_cast<FinalObject&>(*this);
}

template<typename Traits, typename FinalObject>
FinalObject operator*(const cModuleImpl<Traits,FinalObject>& i_lhs, const typename Traits::ring& i_rhs)
{
    return i_lhs.prod(i_rhs);
}
template<typename Traits, typename FinalObject>
FinalObject operator*(const typename Traits::ring& i_lhs, const cModuleImpl<Traits,FinalObject>& i_rhs)
{
    return i_rhs.prod(i_lhs);
}
template<typename Traits, typename FinalObject>
const cModuleImpl<Traits,FinalObject>& operator*=(const cModuleImpl<Traits,FinalObject>& i_lhs, const typename Traits::ring& i_rhs)
{
    return i_lhs = i_lhs.prod(i_rhs);
}
template<typename Traits, typename FinalObject>
const cModuleImpl<Traits,FinalObject>& operator*=(const typename Traits::ring& i_lhs, const cModuleImpl<Traits,FinalObject>& i_rhs)
{
    return i_rhs = i_rhs.prod(i_lhs);
}

}
}
}
