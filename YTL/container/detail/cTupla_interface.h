
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

#include "cTupla_impl.h"

namespace yame
{
namespace container
{
namespace detail
{

template<typename T, int rank, int ... ranks>
class cTupla_interface : public cTupla_impl<T,rank,ranks...>
{
    typedef cTupla_impl<T,rank,ranks...> base_type;

public:
    typedef typename base_type::value_type value_type;
    typedef typename base_type::iterator_type iterator_type;
    typedef typename base_type::const_iterator_type const_iterator_type;
    typedef typename base_type::iterable_public_interface iterable_public_interface;
    static const detail::ReferenceCategory category = base_type::category;

    cTupla_interface(){};

    using base_type::base_type;
    using base_type::operator[];
    using base_type::operator=;
    using base_type::operator==;
    using base_type::operator!=;
    using base_type::at;
    using base_type::getDim;
    using base_type::getTotalRank;
    using base_type::getRank;
    using base_type::getAsPtr;

    using base_type::begin;
    using base_type::end;
    using base_type::cbegin;
    using base_type::cend;
    using base_type::getSize;
    using base_type::empty;
};
}
}
}
