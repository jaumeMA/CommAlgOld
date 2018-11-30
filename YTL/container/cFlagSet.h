#pragma once

#include "YTL/mpl/cTemplateHelper.h"

namespace flag_set_operators
{

template<typename Enum>
struct expansor_tag
{
    friend constexpr bool flag_set_checker(const expansor_tag<Enum>&);
};

template<typename Enum>
struct expansor
{
    friend constexpr bool flag_set_checker(const expansor_tag<Enum>&)
    {
        return true;
    }
};

}

namespace yame
{
namespace ytl
{

template<typename Enum, typename UnderlyingType = void>
class flag_set : private flag_set_operators::expansor<Enum>
{
    typedef typename mpl::static_if<mpl::is_same_type<UnderlyingType,void>::value==false,UnderlyingType,size_t>::type underlying_type;
    static_assert(mpl::is_integral<underlying_type>::value, "You shall provide an integral type as non void underlying type");

public:
    flag_set();
    flag_set(const Enum& i_flag);
    flag_set operator|(const Enum& i_flag) const;
    flag_set& operator|=(const Enum& i_flag);
    flag_set operator&(const Enum& i_flag) const;
    flag_set& operator&=(const Enum& i_flag);
    flag_set operator^(const Enum& i_flag) const;
    flag_set& operator^=(const Enum& i_flag);
    flag_set operator|(const flag_set& other) const;
    flag_set& operator|=(const flag_set& other);
    flag_set operator&(const flag_set& other) const;
    flag_set& operator&=(const flag_set& other);
    flag_set operator^(const flag_set& other) const;
    flag_set& operator^=(const flag_set& other);
    flag_set operator!();
    bool operator==(const flag_set& other) const;
    bool operator!=(const flag_set& other) const;
    void set(const Enum& i_flag);
    bool test(const Enum& i_flag) const;
    bool empty() const;
    underlying_type getValue() const;

private:
    flag_set(const underlying_type& i_flags);

    underlying_type m_flags;
};

}
}

template<typename Enum, bool _foo = flag_set_checker(flag_set_operators::expansor_tag<Enum>{})>
yame::ytl::flag_set<Enum> operator|(const Enum& i_lhs, const Enum& i_rhs)
{
    yame::ytl::flag_set<Enum> res(i_lhs);

    return res | i_rhs;
}


#include "YTL/container/detail/cFlagSet.tpp"
