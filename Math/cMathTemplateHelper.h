#pragma once

#include "YTL/mpl/cTemplateHelper.h"

namespace yame
{
namespace math
{
namespace detail
{

constexpr bool is_prime_loop(int i, int k)
{
    return (k*k > i) ? true : (i%k == 0) ? false : is_prime_loop(i, k + 1);
}
constexpr bool isPrime(int i)
{
    return is_prime_loop(i, 2);
}
constexpr int next_prime(int k)
{
    return isPrime(k) ? k : next_prime(k + 1);
}
constexpr int get_prime_loop(int i, int k)
{
    return (i == 0) ? k : (i % 2) ? get_prime_loop(i-1, next_prime(k + 1)) : get_prime_loop(i/2, get_prime_loop(i/2, k));
}

}

template<int Component, int Index, typename T>
T add_delta_at(const T& i_value, const T& i_delta)
{
    if(Component == Index)
    {
        return i_value + i_delta;
    }
    else
    {
        return i_value;
    }
}

template<typename,bool>
struct _inherit_extended_structure;

template<typename Traits>
struct _inherit_extended_structure<Traits,true>
{
    typedef typename Traits::extended_structure type;
};

template<typename Traits>
struct _inherit_extended_structure<Traits,false>
{
    //empty struct
    struct _foo
    {};

    typedef _foo type;
};

template<typename Traits>
struct inherit_extended_structure
{
    YTL_HAS_DEFINED_SYMBOL_DECL(TTraits,extended_structure)

    typedef typename _inherit_extended_structure<Traits,YTL_HAS_DEFINED_SYMBOL(Traits,extended_structure)>::type type;
};

constexpr int get_nth_prime(int i)
{
    return detail::get_prime_loop(i, 2);
}

}
}
