
/*This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#pragma once

#include "cTemplateHelper.h"
#include "YTL/container/cParameterPack.h"

namespace yame
{

namespace ytl
{

template<typename ...>
struct function;

}

namespace mpl
{

template<typename ...>
struct is_local_invariant;

template<>
struct is_local_invariant<>
{
    static const bool value = true;
};

template<typename T>
struct is_local_invariant<T>
{
    static const bool value = is_trivially_copiable<T>::value;
};

template<typename T, typename ... TT>
struct is_local_invariant<T,TT...>
{
    static const bool value = is_trivially_copiable<T>::value && is_local_invariant<TT...>::value;
};

template<typename>
struct is_function;

template<typename>
struct is_function
{
    static const bool value = false;
};

template<typename Return,typename ... Types>
struct is_function<ytl::function<Return(Types...)>>
{
    static const bool value = true;
};

template<template <typename...> class M, typename Return, typename Type, int Dimension>
struct _homogeneous_callable;

template<template <typename...> class M, typename Return, typename Type>
struct _homogeneous_callable<M,Return,Type,0>
{
    typedef M<Return()> type;
};

template<template <typename...> class M, typename Return, typename Type, int Dimension>
struct _homogeneous_callable
{

template<typename ... TTypes>
static M<Return(TTypes...,typename add_reference<typename add_constness<Type>::type>::type)> addType(const M<Return(TTypes...)>&);

typedef decltype(addType(*reinterpret_cast<const typename _homogeneous_callable<M,Return,Type,Dimension-1>::type*>(0))) type;
};

template<template <typename...> class M, typename Return, typename Type, int Dimension>
struct homogeneous_callable
{
    typedef typename _homogeneous_callable<M,Return,Type,Dimension>::type type;
};

template<typename T>
struct is_valid_functor
{
    static const bool value = mpl::is_pointer<typename mpl::drop_constness<typename mpl::drop_reference<T>::type>::type>::value==false &&
                                mpl::is_function<typename mpl::drop_constness<typename mpl::drop_reference<T>::type>::type>::value==false;
};

template<typename T>
struct curry_function;

template<template<typename...> class M, typename Return, typename ... Types>
struct curry_function<M<Return(Types...)>>
{
private:
    template<typename ...>
    struct is;

    template<typename ... TTypes>
    struct is
    {
        typedef M<Return(TTypes...)> type;
    };

    template<typename TType, typename ... TTypes>
    struct is<TType,TTypes...>
    {
        typedef M<typename is<TTypes...>::type(TType)> type;
    };

public:
    typedef typename is<Types...>::type type;
};

template<typename>
struct function_signature;

template<typename Return,typename ... Types>
struct function_signature<ytl::function<Return(Types...)>>
{
public:
    typedef Return (ytl::function<Return(Types...)>::*callable)(Types...) const;
    typedef Return  callable_return_type;
    typedef ytl::function<Return(Types...)>callable_class;
    typedef container::parameter_pack<Types...> callable_args_pack;
};

template<typename T>
struct function_signature
{
private:
    typedef function_signature<decltype(&T::operator())> nested_traits;
public:
    typedef typename nested_traits::callable callable;
    typedef typename nested_traits::callable_return_type callable_return_type;
    typedef typename nested_traits::callable_class callable_class;
    typedef typename nested_traits::callable_args_pack callable_args_pack;
};

template<typename CallableReturn, typename CallableClass, typename ... CallableTypes>
struct function_signature<CallableReturn (CallableClass::*)(CallableTypes...) const>
{
    typedef CallableReturn (CallableClass::*callable)(CallableTypes...) const;
    typedef CallableReturn  callable_return_type;
    typedef CallableClass callable_class;
    typedef container::parameter_pack<CallableTypes...> callable_args_pack;
};

}
}
