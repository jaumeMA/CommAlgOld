
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

#include "optional/optional.h"
#include "container/cParameterPack.h"

namespace yame
{
namespace ytl
{
namespace detail
{

template<typename ... Types>
struct function_parameter_pack : container::parameter_pack<ytl::optional<Types>...>
{
    function_parameter_pack<Types...>();
    function_parameter_pack<Types...>(Types ... vals);
    function_parameter_pack<Types...>(const function_parameter_pack<Types...>& other);
    function_parameter_pack<Types...>(const container::parameter_pack<Types...>& other);
    template<int ... posTypes>
    void specialize(typename mpl::nth_type_of<posTypes,Types...>::type ... args);
    template<int ... posTypes>
    void specialize(mpl::sequence<posTypes...> rankSequence, typename mpl::nth_type_of<posTypes,Types...>::type ... args);
};

}
}
}

#include "functional/detail/cFunctionParameterPack.tpp"
