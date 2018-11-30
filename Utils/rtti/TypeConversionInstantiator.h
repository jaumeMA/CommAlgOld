#pragma once

#include "Utils/rtti/TypeConversion.h"
#include "YTL/container/cParameterPack.h"

namespace yame
{
namespace rtti
{
namespace detail
{

template<typename ...>
struct TypeConversionExpansion;

template<>
struct TypeConversionExpansion<>
{
    static constexpr bool _expansor(bool i_value = true)
    {
        return i_value;
    }
};

template<typename T, typename ... TT>
struct TypeConversionExpansion<T,TT...>
{
    static constexpr bool _expansor(bool i_value = T::_expansion)
    {
        return  TypeConversionExpansion<TT...>::_expansor() && i_value;
    }
};

template<typename,typename ...>
struct TypeDescent;

template<typename Joint,typename T>
struct TypeDescent<Joint,T,container::parameter_pack<>>
{
    static constexpr bool _expansor()
    {
        return true;
    }
};

template<typename Joint,typename T, typename ... TT>
struct TypeDescent<Joint,T,container::parameter_pack<TT...>>
{
    static constexpr bool _expansor()
    {
        return TypeConversionExpansion<rtti_conversor<T,TT,Joint>...>::_expansor() &&
                TypeConversionExpansion<rtti_conversor<TT,T,Joint>...>::_expansor() &&
                TypeDescent<Joint,T,TT...>::_expansor();
    }
};

template<typename Joint>
struct TypeDescent<Joint,container::parameter_pack<>>
{
    static constexpr bool _expansor()
    {
        return true;
    }
};

template<typename Joint,typename T,typename ... TT>
struct TypeDescent<Joint,container::parameter_pack<T,TT...>>
{
    static constexpr bool _expansor()
    {
        return TypeConversionExpansion<rtti_conversor<Joint,T>>::_expansor() &&
                TypeDescent<Joint,container::parameter_pack<TT...>>::_expansor() &&
                TypeDescent<Joint,T>::_expansor();
    }
};

template<typename Joint,typename T>
struct TypeDescent<Joint,T>
{
    static constexpr bool _expansor()
    {
        return TypeDescent<Joint,typename T::rtti_base>::_expansor();
    }
};

template<typename Joint,typename T1, typename T2, typename ... TT2>
struct TypeDescent<Joint,T1,T2,TT2...>
{
    static constexpr bool _expansor()
    {
        return TypeConversionExpansion<rtti_conversor<T1,T2,Joint>>::_expansor() &&
                TypeConversionExpansion<rtti_conversor<T2,T1,Joint>>::_expansor() &&
                TypeDescent<Joint,T1,typename T2::rtti_base>::_expansor() &&
                TypeDescent<Joint,T2,typename T1::rtti_base>::_expansor() &&
                TypeDescent<Joint,T1,TT2...>::_expansor();
    }
};

template<typename Joint,typename...>
struct TypeConversionCombinator;

template<typename Joint>
struct TypeConversionCombinator<Joint>
{
    static constexpr bool _expansor()
    {
        return true;
    }
};

template<typename Joint, typename T, typename ... TT>
struct TypeConversionCombinator<Joint,T,TT...>
{
    static constexpr bool _expansor()
    {
        return TypeDescent<Joint,T,TT...>::_expansor() &&
                TypeConversionCombinator<Joint,TT...>::_expansor();
    }
};

template<typename Type, typename ...>
struct TypeConversionInstantiator;

template<typename Type, typename ... Bases>
struct TypeConversionInstantiator
{
    //expand from parent to child (up/down, cross casting)
    template<bool _foo = TypeConversionExpansion<rtti_conversor<Type,Bases>...>::_expansor() && TypeConversionCombinator<Type,Bases...>::_expansor()>
    static constexpr bool _expansor()
    {
        return _foo;
    }
};

}
}
}
