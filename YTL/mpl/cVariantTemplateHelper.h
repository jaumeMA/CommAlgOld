#pragma once

#include "YTL/mpl/cTemplateHelper.h"

namespace yame
{
namespace mpl
{

template<typename ...>
class variant;

namespace detail
{
template<typename ... Types>
class variant_impl;

template<typename T>
struct is_variant;

template<typename ... Types>
struct is_variant < variant_impl<Types...> >
{
    static const bool value = true;
};

template<typename T>
struct is_variant
{
    static const bool value = false;
};

}
}
