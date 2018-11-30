#pragma once

#include "Math/cPolynomialAlgebra.h"
#include "Math/cMonomial.h"
#include "Math/cVectorSpace.h"
#include "Math/cMathOps.h"

namespace yame
{
namespace math
{
namespace detail
{

template<template<typename> class A, typename T>
inline polynomial<T,A> createPolyFromMonomial(const cMonomial<T>& i_monomial);

template<size_t Component, typename T, template<typename> class A>
cPolynomialValue<T,A> derivative(const cPolynomialValue<T,A>& i_subPoly, size_t currDepth);

template<size_t Component, typename T, template<typename> class A>
container::detail::cMultiMapImpl<cPolynomialValue<T,A>,s16,container::cAVLtree,A,container::MultiBalancer<container::cAVLtree>::Balancer> derivative(const container::detail::cMultiMapImpl<cPolynomialValue<T,A>,s16,container::cAVLtree,A,container::MultiBalancer<container::cAVLtree>::Balancer>& i_subPoly);

template<template<typename> class A, typename Im, typename Dom>
struct taylor
{
static const size_t s_dimension = Dom::dimension();

template<size_t,typename=void>
struct expansion;

template<size_t Component,typename Foo>
struct expansion
{
static void is(const cFunctionSpace<Im,Dom>& i_function, const ytl::function<void(const cFunctionSpace<Im,Dom>&, const container::cTupla<size_t,Dom::dimension()>&)>& o_sink, const container::cTupla<size_t,Dom::dimension()>& i_indexes, int currOrder);
};

template<typename Foo>
struct expansion<Dom::dimension()-1,Foo>
{
static void is(const cFunctionSpace<Im,Dom>& i_function, const ytl::function<void(const cFunctionSpace<Im,Dom>&, const container::cTupla<size_t,Dom::dimension()>&)>& o_sink, container::cTupla<size_t,Dom::dimension()> i_localIndexes, int currOrder);
};

static const size_t k_maxDerivativeOrder = 5;
};

}

template<typename T, template<typename> class A>
inline yame::container::cArray<yame::math::polynomial<T,A>> grobnerBase(const yame::container::cArray<yame::math::polynomial<T,A>>& i_ideal, const ytl::function<bool(yame::math::cMonomial<T>,yame::math::cMonomial<T>)>& i_compOp = &cMonomial<T>::lexMoreComparison);

template<size_t ... Components, typename T, template<typename> class A>
inline container::cTupla<polynomial<T,A>, mpl::get_num_ranks<Components...>::value> derivative(const polynomial<T,A>& i_poly);

template<typename Im, typename Dom, template<typename> class A = memory::cTypedSystemAllocator>
requires requires { math::is_ring<Im>::value; math::is_vector_space<Dom>::value; math::is_metric_space<Dom>::value; }
polynomial<Im,A> taylorSeries(const cFunctionSpace<Im,Dom>& i_function, const Dom& i_point);

template<typename T>
inline yame::math::polynomial<T> operator+(const yame::math::cMonomial<T>& i_lhs, const yame::math::cMonomial<T>& i_rhs);
template<typename T, template<typename> class A>
inline yame::math::polynomial<T,A> operator+(const yame::math::polynomial<T,A>& i_lhs, const yame::math::cMonomial<T>& i_rhs);
template<typename T, template<typename> class A>
inline const yame::math::polynomial<T,A>& operator+=(yame::math::polynomial<T,A>& i_lhs, const yame::math::cMonomial<T>& i_rhs);
template<typename T, template<typename> class A>
inline yame::math::polynomial<T,A> operator+(const yame::math::cMonomial<T>& i_lhs, const yame::math::polynomial<T,A>& i_rhs);
template<typename T, template<typename> class A>
inline yame::math::polynomial<T,A> operator-(const yame::math::polynomial<T,A>& i_lhs, const yame::math::cMonomial<T>& i_rhs);
template<typename T, template<typename> class A>
inline const yame::math::polynomial<T,A>& operator-=(yame::math::polynomial<T,A>& i_lhs, const yame::math::cMonomial<T>& i_rhs);
template<typename T, template<typename> class A>
inline yame::math::polynomial<T,A> operator-(const yame::math::cMonomial<T>& i_lhs, const yame::math::polynomial<T,A>& i_rhs);
template<typename T, template<typename> class A>
inline yame::math::polynomial<T,A> operator*(const yame::math::polynomial<T,A>& i_lhs, const yame::math::cMonomial<T>& i_rhs);
template<typename T, template<typename> class A>
inline const yame::math::polynomial<T,A>& operator*=(yame::math::polynomial<T,A>& i_lhs, const yame::math::cMonomial<T>& i_rhs);
template<typename T, template<typename> class A>
inline yame::math::polynomial<T,A> operator*(const yame::math::cMonomial<T>& i_lhs, const yame::math::polynomial<T,A>& i_rhs);
template<typename T, template<typename> class A>
inline yame::container::cArray<yame::math::polynomial<T,A>> operator/(yame::math::polynomial<T,A> i_lhs, const yame::container::cArray<yame::math::polynomial<T,A>>& i_rhs);
template<typename T, template<typename> class A>
inline yame::math::polynomial<T,A> operator^(yame::math::polynomial<T,A> i_lhs, size_t i_rhs);

}

template<typename T, template<typename> class A>
container::string format(const math::polynomial<T,A>& i_value);

}

#include "Math/detail/cPolynomialOps.tpp"
