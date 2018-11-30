
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

#include "detail/cTupla_interface.h"

#define BASE_TUPLE(T,...) yame::mpl::inherit_tupla<T,yame::container::cTupla,##__VA_ARGS__>::template is<>::type

namespace yame
{
namespace container
{
template<typename T, int rank, int ... ranks>
class cTupla : public detail::cTupla_interface<T,rank,ranks...>
{
    static_assert(mpl::is_const<T>::value == false && mpl::is_reference<T>::value == false, "Const/ reference types are not supported");

public:
    cTupla();

    using detail::cTupla_interface<T,rank,ranks...>::cTupla_interface;
    using detail::cTupla_interface<T,rank,ranks...>::operator[];
    using detail::cTupla_interface<T,rank,ranks...>::operator=;
    using detail::cTupla_interface<T,rank,ranks...>::operator==;
    using detail::cTupla_interface<T,rank,ranks...>::operator!=;
    using detail::cTupla_interface<T,rank,ranks...>::at;
    using detail::cTupla_interface<T,rank,ranks...>::getDim;
    using detail::cTupla_interface<T,rank,ranks...>::getTotalRank;
    using detail::cTupla_interface<T,rank,ranks...>::getRank;
    using detail::cTupla_interface<T,rank,ranks...>::getAsPtr;
};
}
}

#include "detail/cTupla.tpp"
