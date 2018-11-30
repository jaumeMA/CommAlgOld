#pragma once

#include "Math/detail/cMathTypeConceptHelper.h"

namespace yame
{
namespace math
{

template<set_type Im, set_type Dom>
struct FunctionSpaceSetTraits
{
    typedef decltype(underlying_function_type(std::declval<Im>(),std::declval<Dom>())) underlying_type;
    typedef decltype(underlying_function_extension_type(std::declval<Im>(),std::declval<Dom>())) extended_structure;

	static void init(underlying_type& o_value);
	static void init(underlying_type& o_value, const underlying_type& i_value);
	static void deinit(underlying_type& o_value);
    static void assign(underlying_type& o_value, const underlying_type& i_value);
    static bool cmp(const underlying_type& i_lhs, const underlying_type& i_rhs);
};

template<group_type Im, set_type Dom>
struct FunctionSpaceGroupTraits
{
    typedef FunctionSpaceSetTraits<Im,Dom> set_traits;
	typedef typename set_traits::underlying_type underlying_type;
	static underlying_type neutral_element;
	static constexpr bool is_commutative = Im::isCommutative();

	static void sum(underlying_type& res, const underlying_type& i_lhs,const underlying_type& i_rhs);
	static void inv(underlying_type& res, const underlying_type& value);
};

template<ring_type Im, set_type Dom>
struct FunctionSpaceModuleTraits
{
    typedef FunctionSpaceGroupTraits<Im,Dom> group_traits;
	typedef typename group_traits::underlying_type underlying_type;
    typedef Im ring;
	static constexpr bool is_leftModule = Im::isLeftModule();
	static constexpr bool is_rightModule = Im::isRightModule();

    static void modProd(underlying_type& o_res, const ring& i_lhs, const underlying_type& i_rhs);
    static void modProd(underlying_type& o_res, const underlying_type& i_lhs, const ring& i_rhs);
};

}
}

#include "Math/detail/cFunctionSpaceTraits.tpp"
