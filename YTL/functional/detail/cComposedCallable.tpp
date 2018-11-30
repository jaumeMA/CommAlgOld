
#include "YTL/functional/detail/cFunctionImpl.h"

namespace yame
{
namespace ytl
{
namespace detail
{
template<int Pos,typename ... CallableTypes,typename ... NestedCallableTypes>
template<typename Arg, typename ... Args>
composed_callable_func<Pos,container::parameter_pack<CallableTypes...>,NestedCallableTypes...>::composed_callable_func(const Arg& i_callable, const Args& ... i_callables)
: base_type(i_callables ...)
{
    m_nestedCallable.clone(*i_callable.getFuncPtr());
}
template<int Pos,typename ... CallableTypes,typename ... NestedCallableTypes>
composed_callable_func<Pos,container::parameter_pack<CallableTypes...>,NestedCallableTypes...>::composed_callable_func(const composed_callable_func& other)
: base_type(static_cast<const base_type&>(other))
, m_nestedCallable(other.getNestedCallable())
{
}
template<int Pos,typename ... CallableTypes,typename ... NestedCallableTypes>
composed_callable_func<Pos,container::parameter_pack<CallableTypes...>,NestedCallableTypes...>::composed_callable_func(composed_callable_func&& other)
: base_type(static_cast<base_type&&>(other))
, m_nestedCallable(mpl::move(other.getNestedCallable()))
{
}
template<int Pos,typename ... CallableTypes,typename ... NestedCallableTypes>
composed_callable_func<Pos,container::parameter_pack<CallableTypes...>,NestedCallableTypes...>::~composed_callable_func()
{
    m_nestedCallable.destroy();
}
template<int Pos,typename ... CallableTypes,typename ... NestedCallableTypes>
typename composed_callable_func<Pos,container::parameter_pack<CallableTypes...>,NestedCallableTypes...>::const_nested_function_reference composed_callable_func<Pos,container::parameter_pack<CallableTypes...>,NestedCallableTypes...>::getNestedCallable() const
{
    return m_nestedCallable;
}

}


template<typename Callable, typename ... NestedCallableTypes>
template<typename ... Args>
composed_callable<Callable,NestedCallableTypes...>::composed_callable(const Callable& i_callableTransform, const Args& ... i_args)
: base_type(i_args ...)
, m_returnTransform(i_callableTransform)
{
}
template<typename Callable, typename ... NestedCallableTypes>
composed_callable<Callable,NestedCallableTypes...>::composed_callable(const composed_callable<Callable,NestedCallableTypes...>& other)
: base_type(static_cast<const base_type&>(other))
, m_returnTransform(other.m_returnTransform)
{
}
template<typename Callable, typename ... NestedCallableTypes>
composed_callable<Callable,NestedCallableTypes...>::composed_callable(composed_callable<Callable,NestedCallableTypes...>&& other)
: base_type(mpl::move(static_cast<base_type&&>(other)))
, m_returnTransform(mpl::move(other.m_returnTransform))
{
}
template<typename Callable, typename ... NestedCallableTypes>
typename composed_callable<Callable,NestedCallableTypes...>::callable_return_type composed_callable<Callable,NestedCallableTypes...>::operator()(NestedCallableTypes ... i_args) const
{
    typedef typename mpl::create_range_rank<0,num_callables>::type rangeSeq;

    return execute(rangeSeq(), mpl::forward<NestedCallableTypes>(i_args)...);
}
template<typename Callable, typename ... NestedCallableTypes>
template<int ... Index>
typename composed_callable<Callable,NestedCallableTypes...>::callable_return_type composed_callable<Callable,NestedCallableTypes...>::execute(const mpl::sequence<Index...>&, NestedCallableTypes ... i_args) const
{
    return m_returnTransform( detail::composed_callable_func<Index,callable_args,NestedCallableTypes...>::getNestedCallable().getFuncPtr()->operator()(mpl::forward<NestedCallableTypes>(i_args) ...) ...);
}
template<typename Callable, typename ... NestedCallableTypes>
template<int Index>
const detail::function_impl_base<typename detail::composed_callable_func<Index,typename composed_callable<Callable,NestedCallableTypes...>::callable_args,NestedCallableTypes...>::callable_type,container::parameter_pack<NestedCallableTypes...>>* composed_callable<Callable,NestedCallableTypes...>::getNestedCallable() const
{
    static_assert(Index < num_callables, "Index out of bounds");

    return detail::composed_callable_func<Index,callable_args,NestedCallableTypes...>::getNestedCallable().getFuncPtr();
}
template<typename Callable, typename ... NestedCallableTypes>
const Callable& composed_callable<Callable,NestedCallableTypes...>::getTransform() const
{
    return m_returnTransform;
}

}
}
