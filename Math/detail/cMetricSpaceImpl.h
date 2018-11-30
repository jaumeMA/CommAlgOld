#pragma once

#include "Math/detail/cSetInterface.h"
#include "Math/detail/cMathTraitsConceptHelper.h"
#include "Math/detail/cMathTags.h"

namespace yame
{
namespace math
{
namespace detail
{

template<typename Traits, typename FinalObject>
struct cauchy_sequence
{
    typedef typename Traits::underlying_type underlying_type;

public:
    cauchy_sequence(const underlying_type& i_centerValue);
    ~cauchy_sequence();
    void operator++();
    void begin(double i_distance);
    bool end() const;
    FinalObject operator*() const;

private:
    underlying_type m_targetValue;
    underlying_type m_currValue;
};

template<metric_space_traits_type Traits, typename FinalObject>
class cMetricSpaceImpl : virtual public detail::ISet<typename Traits::set_traits>, public metric_space_tag
{
public:
    typedef cauchy_sequence<Traits,FinalObject> vicinity;

    float distance(const cMetricSpaceImpl<Traits,FinalObject>& other) const;
    vicinity get_vicinity() const;
    static float getEpsilon();
};

}

template<metric_space_traits_type Traits, typename FinalObject>
detail::cMetricSpaceImpl<Traits,FinalObject> underlying_metric_space_type(const detail::cMetricSpaceImpl<Traits,FinalObject>&);

}
}

#include "Math/detail/cMetricSpaceImpl.tpp"
