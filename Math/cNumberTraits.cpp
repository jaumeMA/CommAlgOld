
#include "Math/cNumberTraits.h"
#include "Math/cMath.h"

namespace yame
{
namespace math
{

typename RationalGroupTraits::underlying_type RationalGroupTraits::neutral_element = {};
RationalGroupTraits::element_initializer::element_initializer()
{
    mpq_init(RationalGroupTraits::neutral_element);
    mpq_set_ui(RationalGroupTraits::neutral_element, 0, 1);
}
typename RationalRingTraits::underlying_type RationalRingTraits::neutral_element = {};
RationalRingTraits::element_initializer::element_initializer()
{
    mpq_init(RationalRingTraits::neutral_element);
    mpq_set_ui(RationalRingTraits::neutral_element, 1, 1);
}


void RationalSetTraits::init(underlying_type& o_value)
{
    mpq_init(o_value);
}
void RationalSetTraits::init(underlying_type& o_value, const underlying_type& i_value)
{
    mpq_init(o_value);
    mpq_set(o_value, i_value);
}
void RationalSetTraits::init(underlying_type& o_value, int i_numerator, unsigned int i_denominator)
{
    mpq_init(o_value);
    mpq_set_si(o_value, i_numerator, i_denominator);
}
void RationalSetTraits::deinit(underlying_type& o_value)
{
    mpq_clear(o_value);
}
void RationalSetTraits::assign(underlying_type& o_value, const underlying_type& i_value)
{
    mpq_set(o_value, i_value);
}
void RationalSetTraits::assign(underlying_type& o_value, const rationalPair& i_value)
{
    mpq_set_si(o_value, i_value[0], i_value[1]);
}
bool RationalSetTraits::cmp(const underlying_type& i_lhs, const underlying_type& i_rhs)
{
    return mpq_cmp(i_lhs, i_rhs) == 0;
}

void RationalGroupTraits::sum(underlying_type& res, const underlying_type& i_lhs,const underlying_type& i_rhs)
{
    mpq_add(res, i_lhs, i_rhs);
}
void RationalGroupTraits::inv(underlying_type& res, const underlying_type& value)
{
    mpq_neg(res, value);
}

void RationalRingTraits::prod(underlying_type& res, const underlying_type& i_lhs, const underlying_type& i_rhs)
{
    mpq_mul(res, i_lhs, i_rhs);
}

void RationalDivisionRingTraits::inv(underlying_type& res, const underlying_type& value)
{
    mpq_inv(res, value);
}

}
}
