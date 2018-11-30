
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

#include "YTL/mpl/cTemplateHelper.h"
#include <cstdint>

namespace yame
{
namespace mpl
{

//helper for catching sub parameter packs
template<int, template<typename...> class, int, typename ...>
struct _get_sub_pack;

template<int targetPos, template<typename ...> class pack, int currPos, typename Type, typename ... Types>
struct _get_sub_pack<targetPos,pack,currPos,Type,Types...>
{
    typedef typename _get_sub_pack<targetPos,pack,currPos+1,Types...>::type type;
};

template<int targetPos, template<typename ...> class pack, typename Type, typename ... Types>
struct _get_sub_pack<targetPos,pack,targetPos,Type,Types...>
{
    typedef pack<Type,Types...> type;
};

template<int targetPos, template<typename ...> class pack, typename ... Types>
struct get_sub_pack
{
    static const int numTypes = mpl::get_num_types<Types...>::value;
    //STATIC_ASSERT(numTypes > targetPos,"Attempting to acces parameter pack out of bounds");
    typedef typename _get_sub_pack<targetPos,pack,0,Types...>::type type;
};

template<typename>
struct getValueType;

template<typename T>
struct getValueType<T&>
{
    typedef typename mpl::add_constness<T&>::type type;
};

template<typename T>
struct getValueType<T&&>
{
    typedef T&& type;
};

template<typename T>
struct getValueType
{
    typedef const T& type;
};

template<typename ...>
struct get_total_size;

template<>
struct get_total_size<>
{
    static const size_t value = 0;
};

template<typename T, typename ... TT>
struct get_total_size<T,TT...>
{
    static const size_t value = sizeof(T) + get_total_size<TT...>::value;
};

template<typename ...>
struct get_total_alignment;

template<>
struct get_total_alignment<>
{
    static const size_t value = 1;
};

template<typename T, typename ... TT>
struct get_total_alignment<T,TT...>
{
    typedef uint8_t type[get_total_size<T,TT...>::value];
    static const size_t value = std::alignment_of<type>::value;
    //static const size_t value = std::alignment_of<T>::value * get_total_alignment<TT...>::value;
};

template<typename>
struct _acc_sizeof;

template<int ... Indexs>
struct _acc_sizeof<sequence<Indexs...>>
{
    static const size_t value[get_num_ranks<Indexs...>::value];

    constexpr _acc_sizeof() = default;

    static constexpr size_t at(size_t i_index)
    {
        return (i_index == 0) ? 0 : value[i_index-1];
    }
};

template<int ... Indexs>
const size_t _acc_sizeof<sequence<Indexs...>>::value[get_num_ranks<Indexs...>::value] = { Indexs ... };

template<typename ... Types>
struct acc_sizeof
{
    typedef _acc_sizeof<typename acc_sequence<sizeof(Types) ...>::type> type;
};

}
}
