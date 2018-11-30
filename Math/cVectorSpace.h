#pragma once

#include "Math/cVectorSpaceTraits.h"
#include "Math/cNumberSpaces.h"
#include "Math/cSet.h"
#include "Math/detail/cVectorSpaceImpl.h"
#include "Math/detail/cMetricSpaceImpl.h"

namespace yame
{
namespace math
{

template<vector_space_traits_type Traits>
class cVectorSpace : public detail::cVectorSpaceImpl<Traits, cVectorSpace<Traits>>, public cSet<typename Traits::set_traits>
{
    typedef typename detail::cVectorSpaceImpl<Traits, cVectorSpace<Traits>>::group group;

public:
    typedef Traits traits;
	typedef typename Traits::particle particle;
	typedef typename Traits::underlying_type underlying_type;
	typedef cSet<typename Traits::set_traits> set;
	using cSet<typename Traits::set_traits>::cSet;
	using cSet<typename Traits::set_traits>::operator=;
	using cSet<typename Traits::set_traits>::operator==;
	using cSet<typename Traits::set_traits>::operator!=;

	cVectorSpace(const cVectorSpace&) = default;
    cVectorSpace& operator=(const cVectorSpace&) = default;
    static inline constexpr int dimension()
    {
        return Traits::Dimension;
    }
};

template<ring_type F, group_type V, int N>
struct cMetricVectorSpaceTraits : cVectorSpaceTraits<F,V,N>, cVectorSpaceMetricSpaceTraits<V,N>
{
public:
    typedef typename cVectorSpaceTraits<F,V,N>::set_traits set_traits;
    typedef cVectorSpaceTraits<F,V,N> vector_space_traits;
    typedef cVectorSpaceMetricSpaceTraits<V,N> metric_space_traits;
	typedef typename cVectorSpaceTraits<F,V,N>::particle particle;
	typedef typename cVectorSpaceTraits<F,V,N>::underlying_type underlying_type;

    static const int Dimension = N;
};

template<int N>
using RealVectorSpaceTraits = cMetricVectorSpaceTraits<Real,Real,N>;
template<int N>
using ComplexVectorSpaceTraits = cMetricVectorSpaceTraits<Complex,Complex,N>;
template<int N, int M>
using RealMatrixTraits = cMatrixVectorSpaceTraits<Real,Real,N,M>;
template<int N, int M>
using ComplexMatrixTraits = cMatrixVectorSpaceTraits<Complex,Complex,N,M>;

template<vector_metric_space_traits_type Traits>
class cMetricVectorSpace : public detail::cVectorSpaceImpl<typename Traits::vector_space_traits,cMetricVectorSpace<Traits>>, public detail::cMetricSpaceImpl<typename Traits::metric_space_traits,cMetricVectorSpace<Traits>>, public cSet<typename Traits::set_traits>
{
    typedef typename detail::cVectorSpaceImpl<typename Traits::vector_space_traits, cMetricVectorSpace<Traits>>::group group;

public:
    typedef Traits traits;
	typedef typename Traits::particle particle;
	typedef typename Traits::underlying_type underlying_type;
	typedef cSet<typename Traits::set_traits> set_traits;
	using cSet<typename Traits::set_traits>::cSet;
	using cSet<typename Traits::set_traits>::operator=;
	using cSet<typename Traits::set_traits>::operator==;
	using cSet<typename Traits::set_traits>::operator!=;

	cMetricVectorSpace(const cMetricVectorSpace&) = default;
    cMetricVectorSpace& operator=(const cMetricVectorSpace&) = default;
    static inline constexpr int dimension()
    {
        return Traits::Dimension;
    }
};

//general definitions
typedef cMetricVectorSpace<RealVectorSpaceTraits<1>> R1;
typedef cMetricVectorSpace<RealVectorSpaceTraits<2>> R2;
typedef cMetricVectorSpace<RealVectorSpaceTraits<3>> R3;
template<int N>
using Rn = cMetricVectorSpace<RealVectorSpaceTraits<N>>;

typedef cMetricVectorSpace<ComplexVectorSpaceTraits<1>> C1;
typedef cMetricVectorSpace<ComplexVectorSpaceTraits<2>> C2;
typedef cMetricVectorSpace<ComplexVectorSpaceTraits<3>> C3;
template<int N>
using Cn = cMetricVectorSpace<ComplexVectorSpaceTraits<N>>;

template<typename T, int N>
using vector = cMetricVectorSpace<cMetricVectorSpaceTraits<T,T,N>>;
template<typename T, int N, int M>
using matrix = cMetricVectorSpace<cMatrixVectorSpaceTraits<T,T,N,M>>;

}

template<typename T, int N>
inline container::string format(const math::vector<T,N>& i_value);

template<typename T, int N, int M>
inline container::string format(const math::matrix<T,N,M>& i_value);

}

#include "Math/detail/cVectorSpace.tpp"
