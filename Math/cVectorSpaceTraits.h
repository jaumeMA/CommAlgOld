#pragma once

#include "YTL/container/cTupla.h"
#include "YTL/container/cString.h"
#include "Math/detail/cSetInterface.h"
#include "Math/detail/cMathTypeConceptHelper.h"

namespace yame
{
namespace math
{

template<typename Traits>
struct VectorSpaceExtendedAccess : virtual public detail::ISet<Traits>
{
    typedef typename Traits::underlying_type underlying_type;
    typedef typename Traits::particle particle;

    inline particle& operator[](size_t i_index);
    inline const particle& operator[](size_t i_index) const;
    template<int ... Indexs>
    particle& get();
    template<int ... Indexs>
    const particle& get() const;
};


template<set_type V, int N>
struct cVectorSpaceSetTraits
{
    typedef VectorSpaceExtendedAccess<cVectorSpaceSetTraits<V,N>> extended_structure;
    typedef container::cTupla<V,N> underlying_type;
    typedef V particle;
    static const size_t dimension = N;

    static void init(underlying_type& o_value);
    static void init(underlying_type& o_value, const underlying_type& i_value);
    template<typename ... Args>
    static void init(underlying_type& o_value, Args&& ... i_args);
    static void deinit(underlying_type& o_value);
    static void assign(underlying_type& o_value, const underlying_type& i_value);
    static bool cmp(const underlying_type& i_lhs, const underlying_type& i_rhs);
    static container::string format(const underlying_type& i_value);
};

template<group_type V, int N>
struct cVectorSpaceGroupTraits
{
    typedef cVectorSpaceSetTraits<V,N> set_traits;
	typedef typename set_traits::underlying_type underlying_type;
	typedef typename set_traits::particle particle;
	static const underlying_type neutral_element;
	static const bool is_commutative = true;

	static void sum(underlying_type& o_res, const underlying_type& i_lhs,const underlying_type& i_rhs);
	static void inv(underlying_type& o_res, const underlying_type& i_vector);
};

template<ring_type F, group_type V, int N>
struct cVectorSpaceModuleTraits
{
    typedef cVectorSpaceGroupTraits<V,N> group_traits;
	typedef typename group_traits::underlying_type underlying_type;
	typedef typename group_traits::particle particle;
    typedef F ring;
	static const bool is_leftModule = true;
	static const bool is_rightModule = true;

    static underlying_type base(size_t i_index);
    static void modProd(underlying_type& o_res, const ring& i_lhs, const underlying_type& i_rhs);
    static void modProd(underlying_type& o_res, const underlying_type& i_lhs, const ring& i_rhs);
    static const size_t dimension = N;
};

template<ring_type F, group_type V, int N>
struct cVectorSpaceTraits
{
public:
    typedef cVectorSpaceModuleTraits<F,V,N> module_traits;
    typedef typename module_traits::group_traits group_traits;
    typedef typename group_traits::set_traits set_traits;
	typedef typename module_traits::underlying_type underlying_type;
	typedef typename module_traits::particle particle;
    typedef F field;

    static const int Dimension = N;
};

template<set_type V, int N>
struct cVectorSpaceMetricSpaceTraits
{
    typedef typename cVectorSpaceSetTraits<V,N>::underlying_type underlying_type;
    typedef cVectorSpaceSetTraits<V,N> set_traits;

    static double distance(const typename cVectorSpaceSetTraits<V,N>::underlying_type& i_lhs, const typename cVectorSpaceSetTraits<V,N>::underlying_type& i_rhs);
};

template<set_type V, int N, int M>
struct cMatrixVectorSpaceSetTraits
{
    typedef container::cTupla<V,N,M> underlying_type;
    typedef V particle;
    static const size_t dimension = N * M;

    static const particle& access(const underlying_type& i_value, size_t i_index);
    static particle& access(underlying_type& i_value, size_t i_index);
    static void init(underlying_type& o_value);
    static void init(underlying_type& o_value, const underlying_type& i_value);
    template<typename ... Args>
    static void init(underlying_type& o_value, Args&& ... i_args);
    static void deinit(underlying_type& o_value);
    static void assign(underlying_type& o_value, const underlying_type& i_value);
    static bool cmp(const underlying_type& i_lhs, const underlying_type& i_rhs);
    static container::string format(const underlying_type& i_value);
};

template<group_type V, int N, int M>
struct cMatrixVectorSpaceGroupTraits
{
    typedef cMatrixVectorSpaceSetTraits<V,N,M> set_traits;
	typedef typename set_traits::underlying_type underlying_type;
	typedef typename set_traits::particle particle;
	static const underlying_type neutral_element;
	static const bool is_commutative = true;

	static void sum(underlying_type& o_res, const underlying_type& i_lhs,const underlying_type& i_rhs);
	static void inv(underlying_type& o_res, const underlying_type& i_vector);
};

template<ring_type F, group_type V, int N, int M>
struct cMatrixVectorSpaceModuleTraits
{
    typedef cMatrixVectorSpaceGroupTraits<V,N,M> group_traits;
	typedef typename group_traits::underlying_type underlying_type;
	typedef typename group_traits::particle particle;
    typedef F ring;
	static const ring neutral_element;
	static const bool is_leftModule = true;
	static const bool is_rightModule = true;

    static void modProd(underlying_type& o_res, const ring& i_lhs, const underlying_type& i_rhs);
    static void modProd(underlying_type& o_res, const underlying_type& i_lhs, const ring& i_rhs);
};

template<ring_type F, group_type V, int N, int M>
struct cMatrixVectorSpaceTraits
{
public:
    typedef cMatrixVectorSpaceModuleTraits<F,V,N,M> module_traits;
    typedef typename module_traits::group_traits group_traits;
    typedef typename group_traits::set_traits set_traits;
	typedef typename module_traits::underlying_type underlying_type;
	typedef typename module_traits::particle particle;
    typedef F ring;
};

}
}

#include "Math/detail/cVectorSpaceTraits.tpp"
