#pragma once

#include "Math/cVectorFunction.h"
#include "YTL/functional/detail/cFunctionTemplateHelper.h"
#include "Math/detail/cMathTypeConceptHelper.h"
#include "Math/detail/cVectorTemplateHelper.h"
#include "Math/cFunctionSpace.h"

namespace yame
{
namespace math
{
namespace detail
{

template<typename ...>
struct _is_composition;

template<>
struct _is_composition<>
{
    static const bool value = true;
};

template<typename Type,typename ... TTypes>
struct _is_composition<Type,TTypes...>
{
    static const bool value = is_function_space<Type>::value && _is_composition<TTypes...>::value;
};

template<vector_space_type Dom, typename ... Types>
struct is_composition
{
    static_assert(mpl::get_num_types<Types...>::value == Dom::dimension(), "Mismatch between construct args and types");

    static const bool value = _is_composition<typename mpl::drop_reference<Types>::type...>::value;
};

template<ring_type Im, vector_space_type Dom>
struct _specialization
{
    struct specializer
    {
    private:
        template<typename ... Types>
        using nested_vector_subspace = typename vector_subspace<typename resolve_underlying_vector_space<Dom>::type,mpl::get_num_types<Types...>::value - mpl::get_num_of_types_of<mpl::is_place_holder,Types...>::value>::type;

    public:
        template<typename ... Args>
        inline detail::vector_function<Im,nested_vector_subspace<Args...>> operator()(const cFunctionSpace<Im,Dom>& i_func, Args&& ... i_args) const
        {
            typedef cFunctionSpace<Im,nested_vector_subspace<Args...>> ret_func_type;
            typedef typename ret_func_type::underlying_type nested_ret_func_type;

            const vector_function<Im,Dom>& nestedFunc = i_func.getValue();

            const typename vector_function<Im,Dom>::func_ptr_base* nestedFuncPtr = nestedFunc.getFuncPtr();

            nested_ret_func_type specFunc;

            nestedFuncPtr->specialize(specFunc.getArena(), nestedFunc.getArenaSize(), mpl::forward<Args>(i_args)...);

            return specFunc;
        }
    };
    struct composer
    {
    private:
        template<typename Type, typename ... Types, typename ... TTypes>
        inline ytl::agnostic_composed_callable<Type(TTypes...)> _invoke(const ytl::function<Type(Types...)>& i_func, const ytl::function<typename mpl::drop_constness<typename mpl::drop_reference<Types>::type>::type(TTypes...)>& ... i_funcs) const
        {
            return ytl::agnostic_composed_callable<Type(TTypes...)>(ytl::pass_by_value_functor<Type(Types...)>(i_func),i_funcs ...);
        }
    public:
        template<typename DDom, typename ... DDoms>
        requires ( is_vector_space<DDom>::value && mpl::are_same_type<DDom,DDoms...>::value && Dom::dimension() == mpl::get_num_types<DDoms...>::value + 1 )
        inline detail::vector_function<Im,DDom> operator()(const cFunctionSpace<Im,Dom>& i_func, const cFunctionSpace<typename Dom::particle,DDom>& other, const cFunctionSpace<typename Dom::particle,DDoms>& ... others) const
        {
            return _invoke(i_func.getValue(),other.getValue(),others.getValue() ...);
        }
    };
};

template<ring_type Im, vector_space_type Dom>
_specialization<Im,Dom> underlying_specialization_type(const detail::vector_function<Im,Dom>&);

}

template<ring_type Im, vector_space_type Dom>
using specialization = detail::_specialization<Im,Dom>;

}
}
