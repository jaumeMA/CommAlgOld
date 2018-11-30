
namespace yame
{
namespace math
{
namespace detail
{

template<typename Traits, typename FinalObject>
cauchy_sequence<Traits,FinalObject>::cauchy_sequence(const underlying_type& i_centerValue)
{
    FinalObject::traits::init(m_targetValue,i_centerValue);
    FinalObject::traits::init(m_currValue,i_centerValue);
}
template<typename Traits, typename FinalObject>
cauchy_sequence<Traits,FinalObject>::~cauchy_sequence()
{
    FinalObject::traits::deinit(m_currValue);
    FinalObject::traits::deinit(m_targetValue);
}
template<typename Traits, typename FinalObject>
void cauchy_sequence<Traits,FinalObject>::operator++()
{
    Traits::get_next_elem(m_currValue, m_targetValue);
}
template<typename Traits, typename FinalObject>
void cauchy_sequence<Traits,FinalObject>::begin(double i_distance)
{
    Traits::get_point_at_distance(m_currValue,m_targetValue,i_distance);
}
template<typename Traits, typename FinalObject>
bool cauchy_sequence<Traits,FinalObject>::end() const
{
    return m_targetValue == m_currValue;
}
template<typename Traits, typename FinalObject>
FinalObject cauchy_sequence<Traits,FinalObject>::operator*() const
{
    return FinalObject(m_currValue);
}

template<metric_space_traits_type Traits, typename Set>
float cMetricSpaceImpl<Traits,Set>::distance(const cMetricSpaceImpl<Traits,Set>& other) const
{
    return Traits::distance(this->getValue(), other.getValue());
}
template<metric_space_traits_type Traits, typename Set>
typename cMetricSpaceImpl<Traits,Set>::vicinity cMetricSpaceImpl<Traits,Set>::get_vicinity() const
{
    return vicinity(this->getValue());
}
template<metric_space_traits_type Traits, typename Set>
float cMetricSpaceImpl<Traits,Set>::getEpsilon()
{
    return 0.0005f;
}

}
}
}
