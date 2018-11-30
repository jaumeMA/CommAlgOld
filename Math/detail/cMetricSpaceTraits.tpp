
#include "Math/cMath.h"

namespace yame
{
namespace math
{

template<int p>
float ZpMetricSpaceTraits<p>::distance(const typename ZpSetTraits<p>::underlying_type& i_lhs, const typename ZpSetTraits<p>::underlying_type& i_rhs)
{
    return (i_lhs - i_rhs) % p;
}

}
}
