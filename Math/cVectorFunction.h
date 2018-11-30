#pragma once

#include "YTL/mpl/cFunctionTemplateHelper.h"
#include "YTL/functional/cFunction.h"
#include "Math/cVectorSpace.h"
#include "Math/detail/cStdMathOps.h"
#include <utility>

#define DECLARE_MATH_FUNCTION(FUNC_NAME, NESTED_FUNC_NAME) \
namespace yame \
{ \
namespace math \
{ \
template<ring_type Im, vector_space_type Dom> \
class FUNC_NAME##_function_t : public detail::vector_function<Im,Dom> \
{ \
public: \
FUNC_NAME##_function_t() \
: detail::vector_function<Im,Dom>(&NESTED_FUNC_NAME) \
{} \
}; \
static FUNC_NAME##_function_t<Real,R1> FUNC_NAME##r = FUNC_NAME##_function_t<Real,R1>(); \
static FUNC_NAME##_function_t<Complex,C1> FUNC_NAME##c = FUNC_NAME##_function_t<Complex,C1>(); \
} \
}

namespace yame
{
namespace math
{
namespace detail
{

template<ring_type Im, vector_space_type Dom>
class vector_function : public mpl::homogeneous_callable<ytl::function,Im,typename Dom::particle,Dom::dimension()>::type
{
public:
    typedef typename mpl::homogeneous_callable<ytl::function,Im,typename Dom::particle,Dom::dimension()>::type base_function;
    typedef typename base_function::func_ptr_base func_ptr_base;
    using base_function::base_function;
    using base_function::operator();
    using base_function::eval;
    using base_function::clone;

    vector_function() = default;
    vector_function(const base_function& i_base);
	Im eval(const Dom& i_point) const;
    static vector_function<Im,Dom> clone(const func_ptr_base* i_funcPtr);

private:
    template<int ... Indexs>
    inline Im _eval(const mpl::sequence<Indexs...>&, const Dom& i_point) const;
};

template<typename Return,typename ... Types>
ytl::detail::free_function_impl<Return,Types...> free_function_type(const ytl::function<Return(Types...)>&);

template<ring_type Im, vector_space_type Dom>
using free_function_impl = decltype(free_function_type(std::declval<vector_function<Im,Dom>>()));

template<typename Functor,typename Return,typename ... Types>
ytl::detail::functor_impl<Functor,Return,Types...> functor_type(const ytl::function<Return(Types...)>&);

template<typename Functor,typename Im, vector_space_type Dom>
using functor_impl = decltype(functor_type<Functor>(std::declval<vector_function<Im,Dom>>()));

}

template<ring_type Im, vector_space_type Dom>
using constant_function = ytl::constant_function<typename detail::vector_function<Im,Dom>::base_function>;

template<ring_type Im, vector_space_type Dom>
using projection = ytl::identity_function<typename detail::vector_function<Im,Dom>::base_function>;

template<ring_type Im, vector_space_type Dom>
detail::vector_function<Im,Dom> underlying_function_type(const Im&, const Dom&);

}
}

DECLARE_MATH_FUNCTION(sin, yame::math::sin);
DECLARE_MATH_FUNCTION(cos, yame::math::cos);
DECLARE_MATH_FUNCTION(tan, yame::math::tan);
DECLARE_MATH_FUNCTION(exp, yame::math::exp);
DECLARE_MATH_FUNCTION(log, yame::math::log);

#include "Math/detail/cVectorFunction.tpp"
