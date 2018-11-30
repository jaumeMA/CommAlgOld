
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

namespace yame
{
namespace ytl
{
namespace detail
{
template<typename visitor, typename ... Types>
class variant_visitor_invoker
{
    static const size_t _numTypes = mpl::get_num_types<Types...>::value;

public:
    typedef typename visitor::result_type result_type;

    template<typename Type>
    inline static result_type inner_invoke(visitor& _visitor, const variant_impl<Types...>& var);

    template<typename Type>
    inline static result_type inner_invoke(visitor& _visitor, variant_impl<Types...>& var);

    template<typename Type>
    inline static result_type outer_invoke(visitor& _visitor, const variant_impl<Types...>& var);

    template<typename Type>
    inline static result_type outer_invoke(visitor& _visitor, variant_impl<Types...>& var);

public:
    //non const
    template<typename vvisitor>
    inline static result_type invoke(const uint8_t& currType, const visitor& _visitor, const variant_impl<Types...>& var, typename vvisitor::t_visitor* foo);

    template<typename vvisitor>
    inline static result_type invoke(const uint8_t& currType, const visitor& _visitor, const variant_impl<Types...>& var, ...);

    //const
    template<typename vvisitor>
    inline static result_type invoke(const uint8_t& currType, const visitor& _visitor, variant_impl<Types...>& var, typename vvisitor::t_visitor* foo);

    template<typename vvisitor>
    inline static result_type invoke(const uint8_t& currType, const visitor& _visitor, variant_impl<Types...>& var, ...);
};
}
}
}

#include "YTL/variant/detail/visitor_invoker.tpp"
