
#include "YTL/functional/cComposedCallable.h"
#include "Math/cMathTemplateHelper.h"
#include "Math/detail/cMetricSpaceImpl.h"
#include "Math/cVectorOps.h"

namespace yame
{
namespace math
{

template<int ... Components, ring_type Im, vector_space_type Dom>
container::cTupla<cFunctionSpace<Im,Dom>,mpl::get_num_ranks<Components...>::value> derivative(const cFunctionSpace<Im,Dom>& i_function)
{
    static_assert(mpl::get_num_ranks<Components...>::value <= Dom::dimension(), "Deriving more than can be done");
    static_assert(mpl::check_monotonic_range<Components...>::cond, "You shall provide a monotonically increasing components");

    return { cFunctionSpace<Im,Dom>(detail::derivative_helper::derivative<Components>(i_function.get_nested_function())) ... };
}

template<ring_type Im, vector_space_type Dom>
cFunctionSpace<Im,Dom> derivative(const Dom& i_direction, const cFunctionSpace<Im,Dom>& i_function)
{
    typedef typename mpl::create_range_rank<0,Dom::dimension()>::type rangeSeq;

    return detail::derivative_helper::derivative(rangeSeq(), i_direction, i_function.get_nested_function());
}

}
}
