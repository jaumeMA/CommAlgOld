#pragma once

#include "YTL/container/cParameterPack.h"

namespace yame
{
namespace mpl
{

template<template<typename...> class, typename, typename ...>
struct create_callable;

template<template<typename...> class Callable, typename Return, typename ... Types>
struct create_callable<Callable,Return,container::parameter_pack<Types...>>
{
    typedef Callable<Return(Types...)> type;
};

template<template<typename...> class Callable, typename Return, typename ... Types>
struct create_callable
{
    typedef Callable<Return(Types...)> type;
};

}
}
