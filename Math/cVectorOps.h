#pragma once

#include "Math/cVectorSpace.h"

namespace yame
{
namespace math
{

template<typename T, int N>
vector<T,N> normalize(const vector<T,N>& i_value);
template<typename T, int N>
T operator*(const yame::math::vector<T,N>& i_lhs, const yame::math::vector<T,N>& i_rhs);
template<typename T, int N>
yame::math::vector<T,yame::mpl::NoverM<N,2>::value> operator^(const yame::math::vector<T,N>& i_lhs, const yame::math::vector<T,N>& i_rhs);

}
}

#include "Math/cVectorOps.tpp"
