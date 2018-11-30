#pragma once

#include "YTL/functional/cFunction.h"

namespace yame
{
namespace ytl
{

template<typename Return,typename ... Types>
class agnostic_composed_callable;

template<typename Return,typename ... Types>
class agnostic_composed_callable<Return(Types...)>
{
public:
    template<typename Callable,typename ... IntermediateTypes>
    agnostic_composed_callable(const Callable& i_transformReturn, const function<IntermediateTypes(Types...)>& ... i_baseFunctions);
    Return operator()(Types ... i_args) const;
    size_t getNumIntermediateTypes() const;
    template<typename ... IntermediateTypes>
    const detail::function_impl_base<Return,container::parameter_pack<IntermediateTypes...>>* getTransform() const;
    template<int Index,typename ... IntermediateTypes>
    const detail::function_impl_base<Return,container::parameter_pack<Types...>>* getNestedCallable() const;

private:
    ytl::function<Return(Types...)> m_composedCallable;
    size_t m_intermediateDimension;
};

}

namespace mpl
{

template<typename Return,typename ... Types>
struct is_local_invariant<ytl::agnostic_composed_callable<Return(Types...)>>
{
    static const bool value = false;
};

}
}

#include "detail/cAgnosticCallable.tpp"
