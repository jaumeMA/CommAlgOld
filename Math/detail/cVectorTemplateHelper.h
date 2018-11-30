#pragma once

#include "Math/cVectorSpace.h"

namespace yame
{
namespace math
{

template<vector_space_type,int>
struct vector_subspace;

template<vector_space_type VectorSpace, int Dimension>
requires (Dimension <= VectorSpace::dimension())
struct vector_subspace<VectorSpace,Dimension>
{
private:
    template<ring_type F, group_type V, int N, template<typename> class FinalObject>
    static FinalObject<cVectorSpaceTraits<F,V,Dimension>> _resolve_vector_space(const detail::cVectorSpaceImpl<cVectorSpaceTraits<F,V,N>,FinalObject<cVectorSpaceTraits<F,V,N>>>&);

public:
    typedef decltype(_resolve_vector_space(std::declval<VectorSpace>())) type;
};

template<vector_space_type , typename ...>
struct compose_vector_space;

template<vector_space_type VectorSpace, typename Type, typename ... Types>
requires (mpl::homogeneous_types<Type,Types...>::value)
struct compose_vector_space<VectorSpace,Type,Types...>
{
private:
    template<template<ring_type,group_type,int> typename FinalObjectTraits, ring_type F, group_type V, int N, template<vector_space_traits_type> typename FinalObject>
    requires (is_vector_space<FinalObject<FinalObjectTraits<F,V,N>>>::value)
    static FinalObject<FinalObjectTraits<F,Type,mpl::get_num_types<Types...>::value + 1>> _resolve_vector_space(const detail::cVectorSpaceImpl<cVectorSpaceTraits<F,V,N>,FinalObject<FinalObjectTraits<F,V,N>>>&);

public:
    typedef decltype(_resolve_vector_space(std::declval<VectorSpace>())) type;
};

template<vector_space_type VectorSpace>
struct resolve_underlying_vector_space
{
private:
    template<vector_space_traits_type Traits, typename FinalObject>
    static cVectorSpace<Traits> _resolve_underlying_vector_space(const detail::cVectorSpaceImpl<Traits,FinalObject>&);
public:
    typedef decltype(_resolve_underlying_vector_space(std::declval<VectorSpace>())) type;
};

}
}
