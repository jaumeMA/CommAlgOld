#pragma once

#include "Utils/rtti/TypeConversionInstantiator.h"
#include "Utils/rtti/TypePrimeId.h"
#include "YTL/container/cPair.h"
#include "Utils/cMacroHelper.h"
#include <math.h>
#include <cstdlib>
#include <cmath>

#define __RETURN_NULL_IF_NOT_DIVISIBLE_BIG_NUMBERS(_DIV_0,_DIV_1,_TOLERANCE,_PTR) \
    const unsigned long long __rtti_integral_part = (unsigned long long)_DIV_0; \
    const double __rtti_fractional_part = _DIV_0 - __rtti_integral_part; \
    const double __rtti_div_number = _DIV_1 * __rtti_fractional_part; \
    const double __rtti_fraction = __rtti_div_number - (unsigned long long)__rtti_div_number + 1; \
    if(char(__rtti_fraction - _TOLERANCE) < char(__rtti_fraction + _TOLERANCE)) \
    { \
        return reinterpret_cast<TT*>(reinterpret_cast<char_type>(_PTR) + yame::rtti::detail::type_relator<non_const_T,non_const_TT>::relate(yame::rtti::detail::rtti_conversor_tag<non_const_T,non_const_TT>{})); \
    } \
    else \
    { \
        return NULL; \
    }

#define __RETURN_NULL_IF_NOT_DIVISIBLE(_DIV,_TOLERANCE,_PTR) \
    const double __rtti_fraction = _DIV - (unsigned long long)_DIV + 1; \
    if(char(__rtti_fraction - _TOLERANCE) < char(__rtti_fraction + _TOLERANCE)) \
    { \
        return reinterpret_cast<TT*>(reinterpret_cast<char_type>(_PTR) + yame::rtti::detail::type_relator<non_const_T,non_const_TT>::relate(yame::rtti::detail::rtti_conversor_tag<non_const_T,non_const_TT>{})); \
    } \
    else \
    { \
        return NULL; \
    }

#define PUBLISH_RTTI_BASE_TYPE(_TYPE) \
public: \
    template<typename ...> \
    friend struct yame::rtti::detail::get_bases_prime_id; \
    template<typename Joint,typename __T1, typename __T2, typename ... __TT2> \
    friend bool yame::rtti::detail::TypeDescent<Joint,__T1,__T2,__TT2...>::_expansor(); \
    typedef yame::container::parameter_pack<> rtti_base; \
    template<typename __TT, typename __T> \
    friend __TT* yame::rtti::dynamicCast(__T* i_ptr); \
    template<typename __T, typename __TT> \
    friend yame::rtti::detail::rtti_conversor_tag<__T,__TT> yame::rtti::detail::relateTag(const yame::rtti::detail::rtti_conversor_tag<__T,__TT>& other); \
    static_assert(yame::rtti::detail::get_bases_prime_id<_TYPE,_TYPE::rtti_base>::expand, "Inavlid prime id"); \
    virtual yame::container::cPair<const char*,size_t> _rtti_layout() const \
    { \
        return yame::container::make_pair(reinterpret_cast<const char*>(this),sizeof(_TYPE)); \
    } \
    virtual yame::rtti::detail::prod_uint512 _rtti_prime_id() const \
    { \
        return yame::rtti::detail::get_bases_prime_id<_TYPE,_TYPE::rtti_base>::value; \
    } \

#define PUBLISH_RTTI_TYPE_BASES(_TYPE,...) \
public: \
    template<typename ...> \
    friend struct yame::rtti::detail::get_bases_prime_id; \
    template<typename Joint,typename __T1, typename __T2, typename ... __TT2> \
    friend bool yame::rtti::detail::TypeDescent<Joint,__T1,__T2,__TT2...>::_expansor(); \
    template<typename __TT, typename __T> \
    friend __TT* yame::rtti::dynamicCast(__T* i_ptr); \
    template<typename __T, typename __TT> \
    friend yame::rtti::detail::rtti_conversor_tag<__T,__TT> yame::rtti::detail::relateTag(const yame::rtti::detail::rtti_conversor_tag<__T,__TT>& other); \
    static_assert(yame::rtti::detail::TypeConversionInstantiator<_TYPE,__VA_ARGS__>::template _expansor<>(), "Invalid rtti type instantiation"); \
    typedef yame::container::parameter_pack<__VA_ARGS__> rtti_base; \
    static_assert(yame::rtti::detail::get_bases_prime_id<_TYPE,_TYPE::rtti_base>::expand, "Inavlid prime id"); \
    virtual yame::container::cPair<const char*,size_t> _rtti_layout() const override\
    { \
        return yame::container::make_pair(reinterpret_cast<const char*>(this),sizeof(_TYPE)); \
    } \
    virtual yame::rtti::detail::prod_uint512 _rtti_prime_id() const override\
    { \
        return yame::rtti::detail::get_bases_prime_id<_TYPE,_TYPE::rtti_base>::value; \
    } \

namespace yame
{
namespace rtti
{

template<typename TT, typename T>
TT* staticCast(T* i_ptr)
{
    return static_cast<TT*>(i_ptr);
}

template<typename TT, typename T>
TT* dynamicCast(T* i_ptr)
{
    typedef typename mpl::static_if<mpl::is_const<T>::value,const char*,char*>::type char_type;
    typedef typename mpl::drop_constness<T>::type non_const_T;
    typedef typename mpl::drop_constness<TT>::type non_const_TT;

    static const double s_tolerance = 1e-9;
    const detail::prod_uint512 Tprime_id = i_ptr->_rtti_prime_id();

    static const unsigned long long k_maxNumberValue = 1e13;

    const double __rtti_div = yame::rtti::detail::get_bases_prime_id<non_const_TT,typename non_const_TT::rtti_base>::inverse(Tprime_id);

    const double __rtti_fraction = __rtti_div - (unsigned long long)__rtti_div + 1;
    if(char(__rtti_fraction - s_tolerance) < char(__rtti_fraction + s_tolerance))
    {
        return reinterpret_cast<TT*>(reinterpret_cast<char_type>(i_ptr) + yame::rtti::detail::type_relator<non_const_T,non_const_TT>::relate(yame::rtti::detail::rtti_conversor_tag<non_const_T,non_const_TT>{})); \
    }
    else
    {
        return NULL;
    }
}

}
}
