
#include "Math/cMath.h"

namespace yame
{
namespace math
{

template<typename T, int N>
vector<T,N> normalize(const vector<T,N>& i_value)
{
    vector<T,N> res;
    T invNorma = T::group::neutral_element;
    T sum = T::group::neutral_element;

    for(size_t index=0;index<N;++index)
    {
        res[index] = i_value[index];
        sum += res[index] * res[index];
    }

    invNorma = invSqrt(sum);

    res *= invNorma;

    return res;
}

template<typename T, int N>
T operator*(const yame::math::vector<T,N>& i_lhs, const yame::math::vector<T,N>& i_rhs)
{
    T res = T::group::neutral_element();

    for(size_t index=0;index<N;++index)
    {
        res += i_lhs[index] * i_rhs[index];
    }

    return res;
}
template<typename T, int N>
yame::math::vector<T,yame::mpl::NoverM<N,2>::value> operator^(const yame::math::vector<T,N>& i_lhs, const yame::math::vector<T,N>& i_rhs)
{
    const size_t numCombs = yame::math::factorial(N)/2;
    size_t comb[numCombs*N] = { 0 };
    yame::math::vector<T,yame::mpl::NoverM<N,2>::value> res;

    yame::math::combination(N,2,1,1,yame::math::E_TYPE_COMBINATORICS::ETC_NO_REPETITIION,&(comb[0]));

    for(size_t index=0;index<numCombs;++index)
    {
        const size_t subIndex = (comb[index * 2] == 1) ? (comb[index * 2 + 1] == 2) ? 3 : 2 : 1;
        res[subIndex - 1] = yame::math::parity(index) * (i_lhs[comb[index * 2]-1] * i_rhs[comb[index * 2 + 1] - 1] - i_lhs[comb[index * 2 + 1] - 1] * i_rhs[comb[index * 2] - 1]);
    }

    return res;
}

}
}
