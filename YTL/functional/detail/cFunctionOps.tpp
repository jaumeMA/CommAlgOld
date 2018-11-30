
#include "YTL/functional/cComposedCallable.h"

namespace yame
{
namespace ytl
{
namespace detail
{

template<typename T, typename Return, typename ... Types>
function<Return(Types...)> get_nested_func(T&& i_callable, ...)
{
    return i_callable;
}
template<typename T, typename Return, typename ... Types>
requires (mpl::is_base_of<composed_callable_tag, T>::value)
T get_nested_func(const functor_impl<T,Return,Types...>& i_functor)
{
    return i_functor.getCallable();
}

}

template<typename Return>
yame::ytl::curried_function<Return>& operator<<(yame::ytl::curried_function<Return>& i_lhs, const yame::ytl::function<Return()>& i_rhs)
{
    return i_lhs = i_rhs;
}

template<typename Return, typename Type, typename ... Types>
yame::ytl::curried_function<Return,Type,Types...>& operator<<(yame::ytl::curried_function<Return,Type,Types...>& i_lhs, const yame::ytl::function<Return(Type,Types...)>& i_rhs)
{
    return i_lhs =  [i_rhs](Type&& i_val) -> yame::ytl::curried_function<Return,Types...>
    {
        yame::ytl::curried_function<Return,Types...> nestedCurriedFunction;

        return nestedCurriedFunction << i_rhs.template specializeAt<0>(i_val);
    };
}

template<typename Return, typename ... Types>
yame::ytl::function<Return(Types...)> operator*(const Return& i_lhs, const yame::ytl::function<Return(Types...)>& i_rhs)
{
    if(i_rhs != null_ptr)
    {
        auto transformReturn = [i_lhs](Return i_srhs) { return i_lhs * i_srhs; };

        return yame::ytl::composed_callable<decltype(transformReturn),Types...>(transformReturn, i_rhs);
    }
    else
    {
        return null_ptr;
    }
}

template<typename Return, typename ... Types>
yame::ytl::function<Return(Types...)> operator*(const yame::ytl::function<Return(Types...)>& i_lhs, const Return& i_rhs)
{
    if(i_lhs != null_ptr)
    {
        auto transformReturn = [i_rhs](Return i_slhs) { return i_slhs * i_rhs; };

        return yame::ytl::composed_callable<decltype(transformReturn),Types...>(transformReturn, i_rhs);
    }
    else
    {
        return null_ptr;
    }
}

template<typename Return, typename ... Types>
yame::ytl::function<Return(Types...)> operator+(const Return& i_lhs, const yame::ytl::function<Return(Types...)>& i_rhs)
{
    if(i_rhs != null_ptr)
    {
        auto transformReturn = [i_lhs](Return i_srhs) { return i_lhs + i_srhs; };

        return yame::ytl::composed_callable<decltype(transformReturn),Types...>(transformReturn, i_rhs);
    }
    else
    {
        return null_ptr;
    }
}

template<typename Return, typename ... Types>
yame::ytl::function<Return(Types...)> operator+(const yame::ytl::function<Return(Types...)>& i_lhs, const Return& i_rhs)
{
    if(i_lhs != null_ptr)
    {
        auto transformReturn = [i_rhs](Return i_slhs) { return i_slhs + i_rhs; };

        return yame::ytl::composed_callable<decltype(transformReturn),Types...>(transformReturn, i_rhs);
    }
    else
    {
        return null_ptr;
    }
}

template<typename Return, typename ... Types>
yame::ytl::function<Return(Types...)> operator-(const Return& i_lhs, const yame::ytl::function<Return(Types...)>& i_rhs)
{
    if(i_rhs != null_ptr)
    {
        auto transformReturn = [i_lhs](Return i_srhs) { return i_lhs - i_srhs; };

        return yame::ytl::composed_callable<decltype(transformReturn),Types...>(transformReturn, i_rhs);
    }
    else
    {
        return null_ptr;
    }
}

template<typename Return, typename ... Types>
yame::ytl::function<Return(Types...)> operator-(const yame::ytl::function<Return(Types...)>& i_lhs, const Return& i_rhs)
{
    if(i_lhs != null_ptr)
    {
        auto transformReturn = [i_rhs](Return i_slhs) { return i_slhs - i_rhs; };

        return yame::ytl::composed_callable<decltype(transformReturn),Types...>(transformReturn, i_rhs);
    }
    else
    {
        return null_ptr;
    }
}

template<typename Return, typename ... Types>
yame::ytl::function<Return(Types...)> operator/(const Return& i_lhs, const yame::ytl::function<Return(Types...)>& i_rhs)
{
    if(i_rhs != null_ptr)
    {
        auto transformReturn = [i_lhs](Return i_srhs) { return i_lhs / i_srhs; };

        return yame::ytl::composed_callable<decltype(transformReturn),Types...>(transformReturn, i_rhs);
    }
    else
    {
        return null_ptr;
    }
}

template<typename Return, typename ... Types>
yame::ytl::function<Return(Types...)> operator/(const yame::ytl::function<Return(Types...)>& i_lhs, const Return& i_rhs)
{
    if(i_lhs != null_ptr)
    {
        auto transformReturn = [i_rhs](Return i_slhs) { return i_slhs / i_rhs; };

        return yame::ytl::composed_callable<decltype(transformReturn),Types...>(transformReturn, i_rhs);
    }
    else
    {
        return null_ptr;
    }
}

}
}
