
#include "System/cException.h"

#define STORAGE_ADDRESS(_STORAGE) reinterpret_cast<char*>(const_cast<decltype(_STORAGE)*>(&_STORAGE))

namespace yame
{
namespace container
{

template<typename Type, typename ... Types>
template<typename TType, typename ... TTypes>
requires ( mpl::get_num_types<Types...>::value == mpl::get_num_types<TTypes...>::value )
parameter_pack<Type,Types...>::parameter_pack(TType&& i_val, TTypes&& ... i_vals)
{
    size_t thisTypeIndex = 1;

    construct<Type>(STORAGE_ADDRESS(m_storage), mpl::forward<TType>(i_val)) && ( construct<Types>(STORAGE_ADDRESS(m_storage) + data_offset::at(thisTypeIndex++), mpl::forward<TTypes>(i_vals)) && ... );
}
template<typename Type, typename ... Types>
parameter_pack<Type,Types...>::parameter_pack(const parameter_pack<Type,Types...>& other)
{
    size_t thisTypeIndex = 1;
    size_t otherTypeIndex = 1;

    construct<Type>(STORAGE_ADDRESS(m_storage), mpl::forward<Type>(get<Type>(STORAGE_ADDRESS(other.m_storage) ))) && ( construct<Types>(STORAGE_ADDRESS(m_storage) + data_offset::at(thisTypeIndex++), mpl::forward<Types>(get<Types>(STORAGE_ADDRESS(other.m_storage) + data_offset::at(otherTypeIndex++)))) && ... );
}
template<typename Type, typename ... Types>
parameter_pack<Type,Types...>::parameter_pack(parameter_pack<Type,Types...>&& other)
{
    size_t thisTypeIndex = 1;
    size_t otherTypeIndex = 1;

    construct<Type>(STORAGE_ADDRESS(m_storage), mpl::move(extract<Type>(STORAGE_ADDRESS(other.m_storage) ))) && ( construct<Types>(STORAGE_ADDRESS(m_storage) + data_offset::at(thisTypeIndex++), mpl::move(extract<Types>(STORAGE_ADDRESS(other.m_storage) + data_offset::at(otherTypeIndex++)))) && ... );
}
template<typename Type, typename ... Types>
template<typename TType, typename ... TTypes>
requires ( mpl::get_num_types<Types...>::value == mpl::get_num_types<TTypes...>::value )
parameter_pack<Type,Types...>::parameter_pack(const parameter_pack<TType,TTypes...>& other)
{
    size_t thisTypeIndex = 1;
    size_t otherTypeIndex = 1;

    construct<Type>(STORAGE_ADDRESS(m_storage),mpl::forward<TType>(get<TType>(STORAGE_ADDRESS(other.m_storage)))) && ( construct<Types>(STORAGE_ADDRESS(m_storage) + data_offset::at(thisTypeIndex++), mpl::forward<TTypes>(get<TTypes>(STORAGE_ADDRESS(other.m_storage) + data_offset::at(otherTypeIndex++)))) && ... );
}
template<typename Type, typename ... Types>
template<typename TType, typename ... TTypes>
requires ( mpl::get_num_types<Types...>::value == mpl::get_num_types<TTypes...>::value )
parameter_pack<Type,Types...>::parameter_pack(parameter_pack<TType,TTypes...>&& other)
{
    size_t thisTypeIndex = 1;
    size_t otherTypeIndex = 1;

    construct<Type>(STORAGE_ADDRESS(m_storage),mpl::move(extract<TType>(STORAGE_ADDRESS(other.m_storage)))) && ( construct<Types>(STORAGE_ADDRESS(m_storage) + data_offset::at(thisTypeIndex++), mpl::move(extract<TTypes>(STORAGE_ADDRESS(other.m_storage) + data_offset::at(otherTypeIndex++)))) && ... );
}
template<typename Type, typename ... Types>
parameter_pack<Type,Types...>::~parameter_pack()
{
    size_t thisTypeIndex = 1;

    destruct<Type>(STORAGE_ADDRESS(m_storage)) && ( destruct<Types>(STORAGE_ADDRESS(m_storage) + data_offset::at(thisTypeIndex++)) && ... );
}
template<typename Type, typename ... Types>
parameter_pack<Type,Types...>& parameter_pack<Type,Types...>::operator=(const parameter_pack<Type,Types...>& other)
{
    size_t thisTypeIndex = 1;
    size_t otherTypeIndex = 1;

    assign<Type>(STORAGE_ADDRESS(m_storage),mpl::forward<Type>(get<Type>(STORAGE_ADDRESS(other.m_storage)))) && ( assign<Types>(STORAGE_ADDRESS(m_storage) + data_offset::at(thisTypeIndex++), mpl::forward<Types>(get<Types>(STORAGE_ADDRESS(other.m_storage) + data_offset::at(otherTypeIndex++)))) && ... );

    return *this;
}
template<typename Type, typename ... Types>
parameter_pack<Type,Types...>& parameter_pack<Type,Types...>::operator=(parameter_pack<Type,Types...>&& other)
{
    size_t thisTypeIndex = 1;
    size_t otherTypeIndex = 1;

    assign<Type>(STORAGE_ADDRESS(m_storage),mpl::move(extract<Type>(STORAGE_ADDRESS(other.m_storage)))) && ( assign<Types>(STORAGE_ADDRESS(m_storage) + data_offset::at(thisTypeIndex++), mpl::move(extract<Types>(STORAGE_ADDRESS(other.m_storage) + data_offset::at(otherTypeIndex++)))) && ... );

    return *this;
}
template<typename Type, typename ... Types>
template<int _pos>
typename ytl::embedded_type<typename mpl::nth_type_of<_pos,Type,Types...>::type>::cref_type parameter_pack<Type,Types...>::getValue() const
{
    typedef typename mpl::nth_type_of<_pos,Type,Types...>::type nth_type;

    return get<nth_type>(STORAGE_ADDRESS(m_storage) + data_offset::at(_pos));
}
template<typename Type, typename ... Types>
template<int _pos>
typename ytl::embedded_type<typename mpl::nth_type_of<_pos,Type,Types...>::type>::ref_type parameter_pack<Type,Types...>::getValue()
{
    typedef typename mpl::nth_type_of<_pos,Type,Types...>::type nth_type;

    return get<nth_type>(STORAGE_ADDRESS(m_storage) + data_offset::at(_pos));
}
template<typename Type, typename ... Types>
template<int ... _pos>
void parameter_pack<Type,Types...>::setValues(typename mpl::nth_type_of<_pos,Type,Types...>::type ... args)
{
    ( assign<typename mpl::nth_type_of<_pos,Type,Types...>::type>(STORAGE_ADDRESS(m_storage) + data_offset::at(_pos), mpl::forward<typename mpl::nth_type_of<_pos,Type,Types...>::type>(args)) && ... );
}
template<typename Type, typename ... Types>
template<int _pos>
bool parameter_pack<Type,Types...>::setValue(typename mpl::nth_type_of<_pos,Type,Types...>::type val)
{
    typedef typename mpl::nth_type_of<_pos,Type,Types...>::type nth_type;

    return assign<nth_type>(STORAGE_ADDRESS(m_storage) + data_offset::at(_pos), mpl::forward<nth_type>(val));
}

}
}
