
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

#include "YTL/types/embedded_type/embedded_type.h"
#include "YTL/mpl/cParameterPackHelper.h"

namespace yame
{
namespace container
{

template<typename ...>
class parameter_pack;

template<>
class parameter_pack<>
{
public:
    parameter_pack() = default;
};

template<typename Type, typename ... Types>
class parameter_pack<Type,Types...>
{
    static const size_t s_num_types = mpl::get_num_types<Type,Types...>::value;

public:
    parameter_pack<Type,Types...>() = default;
    template<typename TType, typename ... TTypes>
    requires ( mpl::get_num_types<Types...>::value == mpl::get_num_types<TTypes...>::value )
    parameter_pack<Type,Types...>(TType&& i_val, TTypes&& ...vals);
    parameter_pack<Type,Types...>(const parameter_pack<Type,Types...>& other);
    parameter_pack<Type,Types...>(parameter_pack<Type,Types...>&& other);
    template<typename TType, typename ... TTypes>
    requires ( mpl::get_num_types<Types...>::value == mpl::get_num_types<TTypes...>::value )
    parameter_pack<Type,Types...>(const parameter_pack<TType,TTypes...>& other);
    template<typename TType, typename ... TTypes>
    requires ( mpl::get_num_types<Types...>::value == mpl::get_num_types<TTypes...>::value )
    parameter_pack<Type,Types...>(parameter_pack<TType,TTypes...>&& other);
    ~parameter_pack<Type,Types...>();
    parameter_pack<Type,Types...>& operator=(const parameter_pack<Type,Types...>& other);
    parameter_pack<Type,Types...>& operator=(parameter_pack<Type,Types...>&& other);
    template<int _pos>
    typename ytl::embedded_type<typename mpl::nth_type_of<_pos,Type,Types...>::type>::cref_type getValue() const;
    template<int _pos>
    typename ytl::embedded_type<typename mpl::nth_type_of<_pos,Type,Types...>::type>::ref_type getValue();
    template<int ... _pos>
    void setValues(typename mpl::nth_type_of<_pos,Type,Types...>::type ... args);
    template<int _pos>
    bool setValue(typename mpl::nth_type_of<_pos,Type,Types...>::type val);

private:
    typedef typename std::aligned_storage<mpl::get_total_size<Type,Types...>::value,mpl::get_total_alignment<Type,Types...>::value>::type data_type;

    template<typename TType, typename Arg>
    inline bool construct(void* i_address, Arg&& i_val)
    {
        return ytl::embedded_type<TType>::construct(i_address,i_val);
    }
    template<typename TType, typename Arg>
    inline bool assign(void* i_address, Arg&& i_val)
    {
        return ytl::embedded_type<TType>::assign(i_address,i_val);
    }
    template<typename TType>
    inline bool destruct(void* i_address)
    {
        return ytl::embedded_type<TType>::destroy(i_address);
    }
    template<typename TType>
    inline typename ytl::embedded_type<TType>::ref_type get(const void* i_address)
    {
        return reinterpret_cast<ytl::embedded_type<TType> *>(const_cast<void*>(i_address))->get();
    }
    template<typename TType>
    inline typename ytl::embedded_type<TType>::cref_type get(const void* i_address) const
    {
        return reinterpret_cast<ytl::embedded_type<TType> *>(const_cast<void*>(i_address))->get();
    }
    template<typename TType>
    inline typename ytl::embedded_type<TType>::rref_type extract(void* i_address)
    {
        return reinterpret_cast<ytl::embedded_type<TType> *>(i_address)->extract();
    }

    data_type m_storage;
    typedef typename mpl::acc_sizeof<Type,Types...>::type data_offset;
};

template<typename ... Types>
parameter_pack<Types&&...> make_parameter_pack(Types&& ... vals)
{
    return parameter_pack<Types&&...>(mpl::forward<Types>(vals)...);
}

}
}

#include "YTL/container/detail/cParameterPack.tpp"
