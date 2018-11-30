
/*This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#pragma once

#include "YTL/variant/detail/variant_impl.h"

namespace yame
{
namespace ytl
{
template<typename ... Types>
class variant : public detail::variant_interface<detail::variant_impl<Types...>, Types...>
{
    static_assert(mpl::get_num_types<Types...>::value > 0, "You have to provide at least one type to variant");
    static_assert(mpl::get_num_types<Types...>::value < 255, "You cannot provide more than 255 types to a variant!");
    static_assert(!mpl::has_duplicate_types<typename std::decay<Types>::type...>::value, "You cannot create a variant type with duplicated types (decay is applied to types)!");

public:
    using detail::variant_interface<detail::variant_impl<Types...>, Types...>::variant_interface;
    using detail::variant_interface<detail::variant_impl<Types...>, Types...>::operator=;
    using detail::variant_interface<detail::variant_impl<Types...>, Types...>::operator==;
    using detail::variant_interface<detail::variant_impl<Types...>, Types...>::operator!=;
    using detail::variant_interface<detail::variant_impl<Types...>, Types...>::get;
    using detail::variant_interface<detail::variant_impl<Types...>, Types...>::extract;
    using detail::variant_interface<detail::variant_impl<Types...>, Types...>::empty;
    using detail::variant_interface<detail::variant_impl<Types...>, Types...>::is;
    using detail::variant_interface<detail::variant_impl<Types...>, Types...>::which;
    using detail::variant_interface<detail::variant_impl<Types...>, Types...>::reset;
    using detail::variant_interface<detail::variant_impl<Types...>, Types...>::swap;
    using detail::variant_interface<detail::variant_impl<Types...>, Types...>::apply_visitor;

    variant();
    variant(const variant<Types...>& other);
    variant(variant<Types...>& other);
    template<typename TType>
    requires (mpl::is_among_constructible_types<TType,Types...>::value == false)
    variant(TType&& other);
    ~variant();
    variant<Types...>& operator=(const variant<Types...>& other);
};
}
}

#include "YTL/types/variant/detail/variant.tpp"
