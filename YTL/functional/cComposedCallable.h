#pragma once

#include "YTL/functional/cFunctionStorage.h"

namespace yame
{
namespace ytl
{
namespace detail
{

template<typename Return, typename ... Types>
struct function_impl_base;

template<int,typename, typename ...>
class composed_callable_func;
class composed_callable_func_tag
{};

template<int Pos,typename ... CallableTypes,typename ... NestedCallableTypes>
class composed_callable_func<Pos,container::parameter_pack<CallableTypes...>,NestedCallableTypes...> : protected mpl::static_if<Pos+1<mpl::get_num_types<CallableTypes...>::value,composed_callable_func<Pos+1,container::parameter_pack<CallableTypes...>,NestedCallableTypes...>,composed_callable_func_tag>::type
{
    static const size_t num_callables = mpl::get_num_types<CallableTypes...>::value;
    typedef typename mpl::static_if<Pos+1<num_callables,composed_callable_func<Pos+1,container::parameter_pack<CallableTypes...>,NestedCallableTypes...>,composed_callable_func_tag>::type base_type;

    template<int Size>
    struct proper_function_storage_size
    {
        template<int _Size>
        struct retrieve_size
        {
            static const int value = _Size;
        };

        static const int value = mpl::static_if<(Size / num_callables)>= 4, retrieve_size<Size>, proper_function_storage_size<Size + 4>>::type::value;
    };

public:
    typedef typename mpl::nth_type_of<Pos,CallableTypes...>::type callable_type;
    typedef detail::function_storage<proper_function_storage_size<k_embeddedStorageSize>::value,callable_type(NestedCallableTypes...)> nested_function;
    typedef const nested_function& const_nested_function_reference;

    template<typename Arg, typename ... Args>
    composed_callable_func(const Arg& i_callable, const Args& ... i_callables);
    composed_callable_func(const composed_callable_func& i_other);
    composed_callable_func(composed_callable_func&& i_other);
    ~composed_callable_func();
    const_nested_function_reference getNestedCallable() const;

private:
    nested_function m_nestedCallable;
};


struct composed_callable_tag
{};

};

template<typename Callable, typename ... NestedCallableTypes>
class composed_callable : detail::composed_callable_tag, protected detail::composed_callable_func<0,typename mpl::function_signature<Callable>::callable_args_pack,NestedCallableTypes...>
{
    typedef typename mpl::function_signature<Callable>::callable callable;
    typedef typename mpl::function_signature<Callable>::callable_return_type callable_return_type;
    typedef typename mpl::function_signature<Callable>::callable_args_pack callable_args;
    typedef detail::composed_callable_func<0,callable_args,NestedCallableTypes...> base_type;
    static const size_t num_callables = mpl::get_parameter_pack_num_types<callable_args>::value;

public:
    template<typename ... Args>
    composed_callable(const Callable& i_callableTransform, const Args& ... i_args);
    composed_callable(const composed_callable<Callable,NestedCallableTypes...>& other);
    composed_callable(composed_callable<Callable,NestedCallableTypes...>&& other);
    callable_return_type operator()(NestedCallableTypes ... i_args) const;
    callable_return_type eval(NestedCallableTypes ... i_args) const;
    template<int Index>
    const detail::function_impl_base<typename detail::composed_callable_func<Index,callable_args,NestedCallableTypes...>::callable_type,container::parameter_pack<NestedCallableTypes...>>* getNestedCallable() const;
    const Callable& getTransform() const;

private:
    template<int ... Index>
    inline callable_return_type execute(const mpl::sequence<Index...>&, NestedCallableTypes ... i_args) const;

    Callable m_returnTransform;
};

}
}

#include "YTL/functional/detail/cComposedCallable.tpp"
