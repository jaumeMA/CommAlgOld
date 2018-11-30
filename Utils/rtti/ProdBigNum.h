#pragma once

#include <stdexcept>

namespace yame
{
namespace rtti
{
namespace detail
{

template<typename,typename>
struct get_bases_prime_id;

}

template<typename UnderlyingType, size_t Components>
requires ( mpl::is_numeric<UnderlyingType>::value && mpl::is_power_of_two<Components>::value )
struct prod_num;

template<typename UnderlyingType>
requires ( mpl::is_numeric<UnderlyingType>::value )
struct prod_num<UnderlyingType,1>
{
    template<typename UUnderlyingType, size_t CComponets>
    friend struct prod_num;

public:
    inline constexpr prod_num() = default;
    inline constexpr prod_num(const UnderlyingType& i_value)
    : m_value(i_value)
    {
    }
    inline constexpr const UnderlyingType& get() const
    {
        return m_value;
    }

    friend inline constexpr prod_num max(const prod_num& i_lhs, const prod_num& i_rhs)
    {
        return (i_lhs.m_value > i_rhs.m_value) ? i_lhs : i_rhs;
    }
    friend inline constexpr prod_num min(const prod_num& i_lhs, const prod_num& i_rhs)
    {
        return (i_lhs.m_value <= i_rhs.m_value) ? i_lhs : i_rhs;
    }
    friend inline constexpr prod_num operator*(const prod_num& i_lhs, const prod_num& i_rhs)
    {
        return i_lhs.m_value * i_rhs.m_value;
    }

private:
    template<int Component>
    inline constexpr UnderlyingType inner_get() const
    {
        return m_value;
    }

    UnderlyingType m_value = UnderlyingType(1);
};

template<typename UnderlyingType, size_t Components>
requires ( mpl::is_numeric<UnderlyingType>::value && mpl::is_power_of_two<Components>::value )
struct prod_num
{
    template<typename UUnderlyingType, size_t CComponents>
    friend class prod_num;
    template<typename,typename>
    friend struct detail::get_bases_prime_id;

    template<size_t Offset>
    inline constexpr prod_num(const mpl::numeric_type<Offset>&)
    : m_value{ UnderlyingType(1) }
    {
    }
    template<size_t Offset, size_t CComponents, size_t ... CCComponents>
    inline constexpr prod_num(const mpl::numeric_type<Offset>&, const prod_num<UnderlyingType,CComponents>& i_subBigNum, const prod_num<UnderlyingType,CCComponents>& ... i_subBigNums)
    : prod_num(mpl::numeric_type<Offset+CComponents>{},i_subBigNums...)
    {
        inner_construction<Offset>(typename mpl::create_range_rank<0,CComponents>::type{},i_subBigNum);
    }

public:
    constexpr prod_num()
    : m_value{ UnderlyingType(1) }
    {
        inner_construction(typename mpl::create_range_rank<0,Components>::type{},static_cast<UnderlyingType>(1));
    }
    template<typename ... Args>
    requires ( mpl::get_num_types<Args...>::value <= Components )
    constexpr prod_num(const Args& ... i_args)
    : m_value{ UnderlyingType(1) }
    {
        inner_construction(typename mpl::create_range_rank<0,mpl::get_num_types<Args...>::value>::type{},i_args...);
    }
    template<size_t CComponents, size_t ... CCComponents>
    requires ( CComponents + mpl::sum_ranks<CCComponents...>::value == Components )
    inline constexpr prod_num(const prod_num<UnderlyingType,CComponents>& i_subBigNum, const prod_num<UnderlyingType,CCComponents>& ... i_subBigNums)
    : prod_num(mpl::numeric_type<CComponents>{},i_subBigNums...)
    {
        inner_construction<0>(typename mpl::create_range_rank<0,CComponents>::type{},i_subBigNum);
    }
    inline constexpr bool empty() const
    {
        return inner_empty(typename mpl::create_range_rank<0,Components>::type{});
    }
    template<size_t Component>
    requires ( Component < Components )
    inline constexpr const UnderlyingType& get() const
    {
        return *(m_value + Component);
    }
    template<int InitComponent, int CComponents>
    requires ( (InitComponent + CComponents) <= Components )
    inline constexpr prod_num<UnderlyingType,CComponents> partial() const
    {
        return inner_partial<CComponents>(InitComponent,typename mpl::create_range_rank<0,CComponents>::type{});
    }
    constexpr UnderlyingType operator[](size_t i_component) const
    {
        return *(m_value + i_component);
    }
    inline constexpr prod_num<long double,Components> inverse() const
    {
        return inner_inverse(typename mpl::create_range_rank<0,Components>::type{});
    }

    friend inline constexpr prod_num max(const prod_num& i_lhs, const prod_num& i_rhs)
    {
        return (i_lhs.inner_sum(typename mpl::create_range_rank<0,Components>::type{}) > i_rhs.inner_sum(typename mpl::create_range_rank<0,Components>::type{})) ? i_lhs : i_rhs;
    }
    friend inline constexpr prod_num min(const prod_num& i_lhs, const prod_num& i_rhs)
    {
        return (i_lhs.inner_sum(typename mpl::create_range_rank<0,Components>::type{}) <= i_rhs.inner_sum(typename mpl::create_range_rank<0,Components>::type{})) ? i_lhs : i_rhs;
    }
    friend inline constexpr prod_num operator*(const prod_num& i_lhs, const prod_num& i_rhs)
    {
        return prod_num<UnderlyingType,Components>(min(i_lhs.partial<0,Components/2>(),i_lhs.partial<Components/2,Components/2>()) *
                                                   max(i_rhs.partial<0,Components/2>(),i_rhs.partial<Components/2,Components/2>()),
                                                   max(i_lhs.partial<0,Components/2>(),i_lhs.partial<Components/2,Components/2>()) *
                                                   min(i_rhs.partial<0,Components/2>(),i_rhs.partial<Components/2,Components/2>()));
    }

private:
    template<int ... Ranks>
    inline constexpr bool inner_construction(const mpl::sequence<Ranks...>&, const UnderlyingType& i_value)
    {
        return ( inner_set<Ranks>(i_value) && ...);
    }
    template<int ... Ranks>
    inline constexpr bool inner_construction(const mpl::sequence<Ranks...>&, const typename mpl::transform_index_type<Ranks>::template to<UnderlyingType>::type& ... i_values)
    {
        return ( inner_set<Ranks>(i_values) && ...);
    }
    template<int Offset, int ... Ranks>
    inline constexpr bool inner_construction(const mpl::sequence<Ranks...>&, const prod_num<UnderlyingType,mpl::get_num_ranks<Ranks...>::value>& other)
    {
        return ( inner_set<Ranks + Offset>(other.template inner_get<Ranks>()) && ...);
    }
    template<int CComponent>
    inline constexpr bool inner_set(const UnderlyingType& i_value)
    {
        *(m_value + CComponent) = i_value;
        return true;
    }
    template<int Component>
    inline constexpr UnderlyingType inner_get() const
    {
        return *(m_value + Component);
    }
    template<int ... Ranks>
    inline constexpr UnderlyingType inner_sum(const mpl::sequence<Ranks...>&) const
    {
        return ( inner_get<Ranks>() + ... );
    }
    template<int ... Ranks>
    inline constexpr bool inner_empty(const mpl::sequence<Ranks...>&) const
    {
        return ( _inner_empty<Ranks>() && ... );
    }
    template<int CComponent>
    inline constexpr bool _inner_empty() const
    {
        return inner_get<CComponent>() == static_cast<UnderlyingType>(1);
    }
    template<int CComponents, int ... Ranks>
    inline constexpr prod_num<UnderlyingType,CComponents> inner_partial(int i_initComponent, const mpl::sequence<Ranks...>&) const
    {
        return prod_num<UnderlyingType,CComponents>(m_value[i_initComponent + Ranks] ...);
    }
    template<int ... Ranks>
    inline constexpr prod_num<long double,Components> inner_inverse(const mpl::sequence<Ranks...>&) const
    {
        return prod_num<long double,Components>(UnderlyingType(1) / static_cast<long double>(m_value[Ranks]) ...);
    }

    UnderlyingType m_value[Components];
};

}
}
