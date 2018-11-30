#pragma once

#include "YTL/mpl/cStaticCounter.h"
#include "Math/detail/cAlgebraTemplateHelper.h"

#define REGISTER_DERIVATIVE(helper_name) \
namespace yame \
{ \
namespace math \
{ \
namespace detail \
{ \
template<> \
struct derivative_register<derivative_helper::get_next_count()> \
{ \
    typedef derivative_helper_wrapper<helper_name> type; \
}; \
 \
} \
} \
}

namespace yame
{
namespace math
{
namespace detail
{

template<typename T>
struct derivative_helper_wrapper : T
{

using T::get_custom_derivative;

template<int Component, typename TT>
inline static mpl::null_ptr_type get_custom_derivative(TT&&,...)
{
    return null_ptr;
}

};

template<int>
struct derivative_helper_caller;

template<int Case>
struct derivative_helper_caller
{

template<int Component,ring_type Im, vector_space_type Dom, int ... Indexs>
inline static vector_function<Im,Dom> derivative(const mpl::sequence<Indexs...>& i_seq, const vector_function<Im,Dom>& i_function);

};

template<>
struct derivative_helper_caller<0>
{

template<int Component, ring_type Im, vector_space_type Dom, int ... Indexs>
requires (is_metric_space<Im>::value && is_metric_space<typename Dom::particle>::value)
inline static vector_function<Im,Dom> derivative(const mpl::sequence<Indexs...>&, const vector_function<Im,Dom>& i_function);

};

template<size_t>
struct derivative_register;

struct derivative_helper : mpl::static_counter<derivative_helper>
{

template<int Component,ring_type Im, vector_space_type Dom, int currCase = mpl::static_counter<derivative_helper>::get_curr_count()>
static vector_function<Im,Dom> derivative(const vector_function<Im,Dom>& i_function);

template<ring_type Im, vector_space_type Dom, int ... Indexs>
static vector_function<Im,Dom> derivative(const mpl::sequence<Indexs...>& i_seq, const vector<Im,Dom::dimension()>& i_direction, const vector_function<Im,Dom>& i_function);
};

struct composite_derivative_helper
{

template<int Component,ring_type Im, vector_space_type Dom,int DDimension,int ... DerivativeIndexs,typename Return,typename ... Types>
inline static vector_function<Im,Dom> derivative(const mpl::sequence<DerivativeIndexs...>&, const ytl::agnostic_composed_callable<Return(Types...)>& i_callable);
template<int Component,ring_type Im, vector_space_type Dom,int DDimension,typename Return,typename ... Types>
inline static vector_function<Im,Dom> derivative(const ytl::agnostic_composed_callable<Return(Types...)>& i_callable);
template<int Component,ring_type Im, vector_space_type Dom,int ... MaxComponent,typename Return,typename ... Types>
inline  static vector_function<Im,Dom> derivative(const mpl::sequence<MaxComponent...>&, const ytl::agnostic_composed_callable<Return(Types...)>& i_callable);
template<int Component, ring_type Im, vector_space_type Dom>
inline static vector_function<Im,Dom> get_custom_derivative(const vector_function<Im,Dom>& i_function);

};

struct identity_derivative_helper
{

template<int Component, ring_type Im, vector_space_type Dom>
inline static vector_function<Im,Dom> get_custom_derivative(const vector_function<Im,Dom>& i_function);

};

struct constant_derivative_helper
{

template<int Component, ring_type Im, vector_space_type Dom>
inline static vector_function<Im,Dom> get_custom_derivative(const vector_function<Im,Dom>& i_function);

};

struct neg_derivative_helper
{

template<int Component, ring_type Im, vector_space_type Dom>
inline static vector_function<Im,Dom> get_custom_derivative(const vector_function<Im,Dom>& i_function);

};

struct sum_derivative_helper
{

template<int Component,ring_type Im, vector_space_type Dom>
inline static vector_function<Im,Dom> get_custom_derivative(const vector_function<Im,Dom>& i_function);

};

struct subs_derivative_helper
{

template<int Component, ring_type Im, vector_space_type Dom>
inline static vector_function<Im,Dom> get_custom_derivative(const vector_function<Im,Dom>& i_function);

};

struct prod_derivative_helper
{

template<int Component, ring_type Im, vector_space_type Dom>
inline static vector_function<Im,Dom> get_custom_derivative(const vector_function<Im,Dom>& i_function);

};

struct div_derivative_helper
{

template<int Component, ring_type Im, vector_space_type Dom>
inline static vector_function<Im,Dom> get_custom_derivative(const vector_function<Im,Dom>& i_function);

};

struct sin_derivative_helper
{

template<int Component>
inline static vector_function<Real,R1> get_custom_derivative(const vector_function<Real,R1>& i_function);
template<int Component>
inline static vector_function<Complex,C1> get_custom_derivative(const vector_function<Complex,C1>& i_function);

};
struct cos_derivative_helper
{

template<int Component>
inline static vector_function<Real,R1> get_custom_derivative(const vector_function<Real,R1>& i_function);
template<int Component>
inline static vector_function<Complex,C1> get_custom_derivative(const vector_function<Complex,C1>& i_function);

};

struct tan_derivative_helper
{

template<int Component>
inline static vector_function<Real,R1> get_custom_derivative(const vector_function<Real,R1>& i_function);
template<int Component>
inline static vector_function<Complex,C1> get_custom_derivative(const vector_function<Complex,C1>& i_function);

};

struct exp_derivative_helper
{

template<int Component>
inline static vector_function<Real,R1> get_custom_derivative(const vector_function<Real,R1>& i_function);
template<int Component>
inline static vector_function<Complex,C1> get_custom_derivative(const vector_function<Complex,C1>& i_function);

};

struct log_derivative_helper
{

template<int Component>
inline static vector_function<Real,R1> get_custom_derivative(const vector_function<Real,R1>& i_function);
template<int Component>
inline static vector_function<Complex,C1> get_custom_derivative(const vector_function<Complex,C1>& i_function);

};

}
}
}

REGISTER_DERIVATIVE(yame::math::detail::composite_derivative_helper)
REGISTER_DERIVATIVE(yame::math::detail::identity_derivative_helper)
REGISTER_DERIVATIVE(yame::math::detail::constant_derivative_helper)
REGISTER_DERIVATIVE(yame::math::detail::neg_derivative_helper)
REGISTER_DERIVATIVE(yame::math::detail::sum_derivative_helper)
REGISTER_DERIVATIVE(yame::math::detail::subs_derivative_helper)
REGISTER_DERIVATIVE(yame::math::detail::prod_derivative_helper)
REGISTER_DERIVATIVE(yame::math::detail::div_derivative_helper)
REGISTER_DERIVATIVE(yame::math::detail::sin_derivative_helper)
REGISTER_DERIVATIVE(yame::math::detail::cos_derivative_helper)
REGISTER_DERIVATIVE(yame::math::detail::tan_derivative_helper)
REGISTER_DERIVATIVE(yame::math::detail::exp_derivative_helper)
REGISTER_DERIVATIVE(yame::math::detail::log_derivative_helper)

#include "cMathDerivativeHelper.tpp"
