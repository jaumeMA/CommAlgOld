#pragma once

namespace yame
{
namespace ytl
{
namespace detail
{

template<typename Variant, typename ... Types>
class variant_interface;

template<typename Variant, typename Type, typename ... Types>
class variant_interface<Variant, Type, Types...> : public variant_interface < Variant, Types... >
{
    typedef typename mpl::drop_constness<typename mpl::drop_reference<Type>::type>::type rawType;

public:
    using variant_interface<Variant, Types...>::variant_interface;
    using variant_interface<Variant, Types...>::operator=;
    using variant_interface<Variant, Types...>::operator==;
    using variant_interface<Variant, Types...>::operator!=;
    using variant_interface<Variant, Types...>::get;
    using variant_interface<Variant, Types...>::extract;
    using variant_interface<Variant, Types...>::empty;
    using variant_interface<Variant, Types...>::is;
    using variant_interface<Variant, Types...>::which;
    using variant_interface<Variant, Types...>::reset;
    using variant_interface<Variant, Types...>::swap;
    using variant_interface<Variant, Types...>::apply_visitor;

    variant_interface() = default;
    variant_interface(const rawType& other);
    variant_interface(rawType& other);
    variant_interface(rawType&& other);
    variant_interface& operator=(const rawType& other);
    variant_interface& operator=(rawType& other);
    variant_interface& operator=(rawType&& other);
    bool operator==(const rawType& other) const;
    bool operator==(rawType&& other) const;
};

template<typename Variant>
class variant_interface < Variant > : public Variant
{
public:
    using Variant::operator=;
    using Variant::operator==;
    using Variant::operator!=;
    using Variant::get;
    using Variant::extract;
    using Variant::empty;
    using Variant::is;
    using Variant::which;
    using Variant::reset;
    using Variant::swap;
    using Variant::apply_visitor;

    variant_interface() = default;
};

}
}
}

#include "YTL/types/variant/detail/variant_interface.tpp"
