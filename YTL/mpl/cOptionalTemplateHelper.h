#pragma once

#include "YTL/mpl/cTemplateHelper.h"

namespace yame
{
namespace mpl
{

template<typename>
struct optional;

template<typename>
struct is_optional;

template<typename T>
struct is_optional<optional<T>>
{
    static const bool value = true;
};
template<typename T>
struct is_optional<optional<T>&>
{
    static const bool value = true;
};
template<typename T>
struct is_optional<optional<T>&&>
{
    static const bool value = true;
};
template<typename T>
struct is_optional
{
    static const bool value = false;
};
template<typename T, typename TT>
struct constrCond
{
    typedef typename std::remove_reference<typename std::remove_const<TT>::type>::type type;
    static const bool value = !is_optional<TT>::value && std::is_base_of<type,T>::value;
};

}
}
