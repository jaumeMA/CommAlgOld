#pragma once

#include "YTL/container/cTupla.h"
#include "YTL/functional/cFunctionOps.h"
#include "Math/cVectorFunctionSpace.h"
#include "Math/detail/cMathDerivativeHelper.h"

namespace yame
{
namespace math
{

template<int ... Components, ring_type Im, vector_space_type Dom>
inline container::cTupla<cFunctionSpace<Im,Dom>,mpl::get_num_ranks<Components...>::value> derivative(const cFunctionSpace<Im,Dom>& i_function);

template<ring_type Im, vector_space_type Dom>
inline cFunctionSpace<Im,Dom> derivative(const Dom& i_direction, const cFunctionSpace<Im,Dom>& i_function);

}
}

#include "Math/detail/cMathOps.tpp"
