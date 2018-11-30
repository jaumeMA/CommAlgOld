#pragma once

#include "cFunction.h"

namespace yame
{
namespace ytl
{

template<typename Return,typename ... Types>
class pass_by_value_functor;

template<typename Return,typename ... Types>
class pass_by_value_functor<Return(Types...)>
{
public:
    pass_by_value_functor(const function<Return(Types...)>& i_function);
    Return operator()(typename mpl::drop_constness<typename mpl::drop_reference<Types>::type>::type ...) const;
    const function<Return(Types...)>& getForwardedFunction() const;

private:
    function<Return(Types...)> m_nestedFunction;
};

}

namespace mpl
{

template<typename Return,typename ... Types>
struct is_local_invariant<ytl::pass_by_value_functor<Return,Types...>>
{
    static const bool value = false;
};

}
}

#include "detail/cPassByValueFunctor.tpp"
