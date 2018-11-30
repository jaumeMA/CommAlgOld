
#include "Math/cMath.h"

namespace yame
{
namespace math
{

template<group_type V, int N>
inline typename cVectorSpaceGroupTraits<V,N>::underlying_type CreateVectorSpaceGroupNeutralElement()
{
    typename cVectorSpaceGroupTraits<V,N>::underlying_type res;

    for(size_t index=0;index<N;++index)
    {
        res[index] = V::group::neutral_element();
    }

    return res;
}

template<group_type V, int N, int M>
inline typename cMatrixVectorSpaceGroupTraits<V,N,M>::underlying_type CreateMatrixSpaceGroupNeutralElement()
{
    typename cMatrixVectorSpaceGroupTraits<V,N,M>::underlying_type res;

    for(size_t indexI=0;indexI<N;++indexI)
    {
        for(size_t indexJ=0;indexJ<M;++indexJ)
        {
            res[indexI][indexJ] = V::group::neutral_element();
        }
    }

    return res;
}

template<group_type V, int N>
const typename cVectorSpaceGroupTraits<V,N>::underlying_type cVectorSpaceGroupTraits<V,N>::neutral_element = CreateVectorSpaceGroupNeutralElement<V,N>();

template<group_type V, int N, int M>
const typename cMatrixVectorSpaceGroupTraits<V,N,M>::underlying_type cMatrixVectorSpaceGroupTraits<V,N,M>::neutral_element = CreateVectorSpaceGroupNeutralElement<V,N>();

template<typename Traits>
typename VectorSpaceExtendedAccess<Traits>::particle& VectorSpaceExtendedAccess<Traits>::operator[](size_t i_index)
{
    underlying_type& thisValue = this->getValue();

    return thisValue[i_index];
}
template<typename Traits>
const typename VectorSpaceExtendedAccess<Traits>::particle& VectorSpaceExtendedAccess<Traits>::operator[](size_t i_index) const
{
    const underlying_type& thisValue = this->getValue();

    return thisValue[i_index];
}
template<typename Traits>
template<int ... Indexs>
typename VectorSpaceExtendedAccess<Traits>::particle& VectorSpaceExtendedAccess<Traits>::get()
{
    underlying_type& thisValue = this->getValue();

    return thisValue.get<Indexs...>();
}
template<typename Traits>
template<int ... Indexs>
const typename VectorSpaceExtendedAccess<Traits>::particle& VectorSpaceExtendedAccess<Traits>::get() const
{
    const underlying_type& thisValue = this->getValue();

    return thisValue.get<Indexs...>();
}

template<set_type V, int N>
void cVectorSpaceSetTraits<V,N>::init(underlying_type& o_value)
{
    //done by default constructor of V
}
template<set_type V, int N>
void cVectorSpaceSetTraits<V,N>::init(underlying_type& o_value, const underlying_type& i_value)
{
    for(size_t index=0;index<N;++index)
    {
        o_value[index] = i_value[index];
    }
}
template<set_type V, int N>
template<typename ... Args>
void cVectorSpaceSetTraits<V,N>::init(underlying_type& o_value, Args&& ... i_args)
{
    std::initializer_list<V> vals({ V(mpl::forward<Args>(i_args)) ... });

    typename std::initializer_list<V>::const_iterator itVal = vals.begin();
    for(size_t index=0;itVal!=vals.end()&&index<N;++itVal,++index)
    {
        o_value[index] = *itVal;
    }
}
template<set_type V, int N>
void cVectorSpaceSetTraits<V,N>::deinit(underlying_type& o_value)
{
    //done by destructor of V
}
template<set_type V, int N>
void cVectorSpaceSetTraits<V,N>::assign(underlying_type& o_value, const underlying_type& i_value)
{
    for(size_t index=0;index<N;++index)
    {
        o_value[index] = i_value[index];
    }
}
template<set_type V, int N>
bool cVectorSpaceSetTraits<V,N>::cmp(const underlying_type& i_lhs, const underlying_type& i_rhs)
{
    for(size_t index=0;index<N;++index)
    {
        if(i_lhs[index] != i_rhs[index])
        {
            return false;
        }
    }

    return true;
}

template<group_type V, int N>
void cVectorSpaceGroupTraits<V,N>::sum(underlying_type& o_res, const underlying_type& i_lhs,const underlying_type& i_rhs)
{
    for(size_t index=0;index<N;++index)
    {
        o_res[index] = i_lhs[index] + i_rhs[index];
    }
}
template<group_type V, int N>
void cVectorSpaceGroupTraits<V,N>::inv(underlying_type& o_res, const underlying_type& i_vector)
{
    for(size_t index=0;index<N;++index)
    {
        o_res[index] = -i_vector[index];
    }
}

template<ring_type F, group_type V, int N>
typename cVectorSpaceModuleTraits<F,V,N>::underlying_type cVectorSpaceModuleTraits<F,V,N>::base(size_t i_index)
{
    underlying_type res = 0;

    for(size_t index=0;index<N;++index)
    {
        res[index] = V::group::neutral_element();
    }

    return res;
}
template<ring_type F, group_type V, int N>
void cVectorSpaceModuleTraits<F,V,N>::modProd(underlying_type& o_res, const ring& i_lhs, const underlying_type& i_rhs)
{
    for(size_t index=0;index<N;++index)
    {
        o_res[index] = i_lhs * i_rhs[index];
    }
}
template<ring_type F, group_type V, int N>
void cVectorSpaceModuleTraits<F,V,N>::modProd(underlying_type& o_res, const underlying_type& i_lhs, const ring& i_rhs)
{
    for(size_t index=0;index<N;++index)
    {
        o_res[index] = i_lhs[index] * i_rhs;
    }
}

template<set_type V, int N>
double cVectorSpaceMetricSpaceTraits<V,N>::distance(const typename cVectorSpaceSetTraits<V,N>::underlying_type& i_lhs, const typename cVectorSpaceSetTraits<V,N>::underlying_type& i_rhs)
{
    if(N == 1)
    {
        return i_lhs[0].distance(i_rhs[0]);
    }
    else
    {
        double res = 0.f;

        for(size_t index=0;index<N;++index)
        {
            const double currDistance = i_lhs[index].distance(i_rhs[index]);

            res += currDistance * currDistance;
        }

        return math::fastSqrt(res);
    }
}

template<set_type V, int N, int M>
void cMatrixVectorSpaceSetTraits<V,N,M>::init(underlying_type& o_value)
{
    for(size_t indexI=0;indexI<N;++indexI)
    {
        for(size_t indexJ=0;indexJ<M;++indexJ)
        {
            V::Traits::init(o_value[indexI][indexJ]);
        }
    }
}
template<set_type V, int N, int M>
void cMatrixVectorSpaceSetTraits<V,N,M>::init(underlying_type& o_value, const underlying_type& i_value)
{
    for(size_t indexI=0;indexI<N;++indexI)
    {
        for(size_t indexJ=0;indexJ<M;++indexJ)
        {
            o_value[indexI][indexJ] = i_value[indexI][indexJ];
        }
    }
}
template<set_type V, int N, int M>
template<typename ... Args>
void cMatrixVectorSpaceSetTraits<V,N,M>::init(underlying_type& o_value, Args&& ... i_args)
{
    std::initializer_list<V> vals({ mpl::forward<Args>(i_args) ... });

    for(size_t indexI=0;indexI<N;++indexI)
    {
        for(size_t indexJ=0;indexJ<M;++indexJ)
        {
            V::Traits::init(o_value[indexI][indexJ], vals[indexI*N + indexJ]);
        }
    }
}
template<set_type V, int N, int M>
void cMatrixVectorSpaceSetTraits<V,N,M>::deinit(underlying_type& o_value)
{
    for(size_t indexI=0;indexI<N;++indexI)
    {
        for(size_t indexJ=0;indexJ<M;++indexJ)
        {
            V::Traits::deinit(o_value[indexI][indexJ]);
        }
    }
}
template<set_type V, int N, int M>
void cMatrixVectorSpaceSetTraits<V,N,M>::assign(underlying_type& o_value, const underlying_type& i_value)
{
    for(size_t indexI=0;indexI<N;++indexI)
    {
        for(size_t indexJ=0;indexJ<M;++indexJ)
        {
            o_value[indexI][indexJ] = i_value[indexI][indexJ];
        }
    }
}
template<set_type V, int N, int M>
const typename cMatrixVectorSpaceSetTraits<V,N,M>::particle& cMatrixVectorSpaceSetTraits<V,N,M>::access(const underlying_type& i_value, size_t i_index)
{
    return i_value[i_index];
}
template<set_type V, int N, int M>
typename cMatrixVectorSpaceSetTraits<V,N,M>::particle& cMatrixVectorSpaceSetTraits<V,N,M>::access(underlying_type& i_value, size_t i_index)
{
    return i_value[i_index];
}
template<set_type V, int N, int M>
bool cMatrixVectorSpaceSetTraits<V,N,M>::cmp(const underlying_type& i_lhs, const underlying_type& i_rhs)
{
    for(size_t indexI=0;indexI<N;++indexI)
    {
        for(size_t indexJ;indexJ<M;++indexJ)
        {
            if(i_lhs[indexI][indexJ] != i_rhs[indexI][indexJ])
            {
                return false;
            }
        }
    }

    return true;
}

template<group_type V, int N, int M>
void cMatrixVectorSpaceGroupTraits<V,N,M>::sum(underlying_type& o_res, const underlying_type& i_lhs,const underlying_type& i_rhs)
{
    for(size_t indexI=0;indexI<N;++indexI)
    {
        for(size_t indexJ=0;indexJ<M;++indexJ)
        {
            o_res[indexI][indexJ] = i_lhs[indexI][indexJ] + i_rhs[indexI][indexJ];
        }
    }
}
template<group_type V, int N,int M>
void cMatrixVectorSpaceGroupTraits<V,N,M>::inv(underlying_type& o_res, const underlying_type& i_vector)
{
    for(size_t indexI=0;indexI<N;++indexI)
    {
        for(size_t indexJ=0;indexJ<M;++indexJ)
        {
            o_res[indexI][indexJ] = -i_vector[indexI][indexJ];
        }
    }
}

template<ring_type F, group_type V, int N,int M>
void cMatrixVectorSpaceModuleTraits<F,V,N,M>::modProd(underlying_type& o_res, const ring& i_lhs, const underlying_type& i_rhs)
{
    for(size_t indexI=0;indexI<N;++indexI)
    {
        for(size_t indexJ=0;indexJ<M;++indexJ)
        {
            o_res[indexI][indexJ] = i_lhs * i_rhs[indexI][indexJ];
        }
    }
}
template<ring_type F, group_type V, int N,int M>
void cMatrixVectorSpaceModuleTraits<F,V,N,M>::modProd(underlying_type& o_res, const underlying_type& i_lhs, const ring& i_rhs)
{
    for(size_t indexI=0;indexI<N;++indexI)
    {
        for(size_t indexJ=0;indexJ<M;++indexJ)
        {
            o_res[indexI][indexJ] = i_lhs[indexI][indexJ] * i_rhs;
        }
    }
}

}
}
