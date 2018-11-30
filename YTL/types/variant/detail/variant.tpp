
#include "YTL/types/variant/detail/variant_interface.h"

namespace yame
{
namespace ytl
{

//variant impl
template<typename ... Types>
variant<Types...>::variant()
{
}

template<typename ... Types>
variant<Types...>::variant(const variant<Types...>& other)
{
    detail::variant_impl<Types...>::construct(other);
}

template<typename ... Types>
variant<Types...>::variant(variant<Types...>& other)
{
    detail::variant_impl<Types...>::construct(other);
}

//JAUME: we use foo variable (not needed to be set since its defaulted to NULL) for using substitution for avoiding this constructor to be expanded in funcs like is_convertible or is_constructible
template<typename ... Types>
template<typename TType>
requires (mpl::is_among_constructible_types<TType,Types...>::value == false)
variant<Types...>::variant(TType&& other)
{
    detail::variant_interface<detail::variant_impl<Types...>,Types...>::construct(std::forward<TType>(other));
}

template<typename ... Types>
variant<Types...>::~variant()
{

}

template<typename ... Types>
variant<Types...>& variant<Types...>::operator=(const variant<Types...>& other)
{
    detail::variant_impl<Types...>::operator=(other);

    return *this;
}

}
}
