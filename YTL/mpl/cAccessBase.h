#pragma once

#include "YTL/mpl/cTemplateHelper.h"

#define YTL_PUBLISH_ACCESS(_KEY,_DERIVED,_BASE) \
friend _BASE& __access_private_base(_DERIVED& i_object, const _KEY&) \
{ \
    return static_cast<_BASE&>(i_object); \
} \
friend const _BASE& __const_access_private_base(const _DERIVED& i_object, const _KEY&) \
{ \
    return static_cast<const _BASE&>(i_object); \
}

namespace yame
{
namespace ytl
{

template<typename Key, typename T>
decltype(__access_private_base(std::declval<T&>(),std::declval<Key>())) access(T& i_object)
{
    return __access_private_base(i_object,Key());
}
template<typename Key, typename T>
decltype(__const_access_private_base(std::declval<const T&>(),std::declval<Key>())) access(const T& i_object)
{
    return __const_access_private_base(i_object,Key());
}

}
}
