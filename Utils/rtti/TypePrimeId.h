#pragma once

#include "YTL/container/cParameterPack.h"
#include "Math/cPrimeNumbers.h"
#include "YTL/mpl/cStaticCounter.h"
#include "Utils/rtti/ProdBigNum.h"

namespace yame
{
namespace rtti
{
namespace detail
{

template<typename>
struct __rtti_tag
{
    typedef void type;
};

template<typename ... Bases>
struct _get_bases_prime_id;

typedef prod_num<unsigned long long,4> prod_uint512;
typedef prod_num<long double,4> prod_double512;

inline constexpr prod_uint512 unfoldPrimes()
{
    return prod_uint512(1,1,1,1);
}

template<typename Arg, typename ... Args>
inline constexpr prod_uint512 unfoldPrimes(Arg&& i_arg, Args&& ... i_args)
{
    return i_arg * unfoldPrimes(mpl::forward<Args>(i_args) ...);
}

template<typename ... T>
inline constexpr prod_uint512 getPrimeValue(const size_t& i_primeValue)
{
    return prod_uint512(i_primeValue,1,1,1) * unfoldPrimes(get_bases_prime_id<T,typename T::rtti_base>::value ...);
}

template<>
struct _get_bases_prime_id<>
{
    static const unsigned long long value = 1;
};

template<typename Base, typename ... Bases>
struct _get_bases_prime_id<Base,Bases...>
{
    static const unsigned long long value = static_cast<unsigned long long>(get_bases_prime_id<Base,typename Base::rtti_base>::value) * static_cast<unsigned long long>(_get_bases_prime_id<Bases...>::value);
};

template<typename Type, typename ... Bases>
struct get_bases_prime_id<Type,yame::container::parameter_pack<Bases...>>
{
    static constexpr prod_uint512 value = getPrimeValue<Bases...>(yame::math::getPrimeNumber(yame::mpl::static_counter<typename __rtti_tag<Type>::type>::get_next_count()));
    static const bool expand = (value.empty() == false);
    static const prod_double512 invValue;
    static inline long double inverse(const prod_uint512& i_value)
    {
        return i_value.m_value[0] * invValue.m_value[0] *
                i_value.m_value[1] * invValue.m_value[1] *
                i_value.m_value[2] * invValue.m_value[2] *
                i_value.m_value[3] * invValue.m_value[3];
    }
};

template<typename Type, typename ... Bases>
const prod_double512 get_bases_prime_id<Type,yame::container::parameter_pack<Bases...>>::invValue = get_bases_prime_id<Type,yame::container::parameter_pack<Bases...>>::value.inverse();

}
}
}
