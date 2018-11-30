
#include "YTL/types/variant/detail/variant_impl.h"

namespace yame
{
namespace ytl
{
namespace detail
{

template<typename Variant, typename Type, typename ... Types>
variant_interface<Variant, Type, Types...>::variant_interface(rawType& other)
{
    Variant::template construct<Type>(other);
}

template<typename Variant, typename Type, typename ... Types>
variant_interface<Variant, Type, Types...>::variant_interface(const rawType& other)
{
    Variant::template construct<Type>(other);
}

template<typename Variant, typename Type, typename ... Types>
variant_interface<Variant, Type, Types...>::variant_interface(rawType&& other)
{
    Variant::template construct<Type>(std::move(other));
}

template<typename Variant, typename Type, typename ... Types>
variant_interface<Variant, Type, Types...>& variant_interface<Variant, Type, Types...>::operator=(rawType& other)
{
    Variant::template assign<Type>(other);

    return *this;
}

template<typename Variant, typename Type, typename ... Types>
variant_interface<Variant, Type, Types...>& variant_interface<Variant, Type, Types...>::operator=(const rawType& other)
{
    Variant::template assign<Type>(other);

    return *this;
}

template<typename Variant, typename Type, typename ... Types>
variant_interface<Variant, Type, Types...>& variant_interface<Variant, Type, Types...>::operator=(rawType&& other)
{
    Variant::template assign<Type, rawType&&>(mpl::move(other));

    return *this;
}

template<typename Variant, typename Type, typename ... Types>
bool variant_interface<Variant, Type, Types...>::operator==(const rawType& other) const
{
    return Variant::template compare<Type>(other);
}

template<typename Variant, typename Type, typename ... Types>
bool variant_interface<Variant, Type, Types...>::operator==(rawType&& other) const
{
    return Variant::template compare<Type>(mpl::move(other));
}

}
}
}
