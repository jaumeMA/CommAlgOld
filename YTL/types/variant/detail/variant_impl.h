
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
#include "YTL/mpl/cVariantTemplateHelper.h"

namespace yame
{
namespace ytl
{

template<typename ... Types>
class variant_impl
{
public:
    variant_impl();
    void construct(const variant_impl<Types...>& other);
    void construct(variant_impl<Types...>&& other);
    template<typename Type, typename TType>
    requires (mpl::is_variant<TType>::value== false)
    void construct(TType&& other);
    void destroy();
    ~variant_impl();
    variant_impl<Types...>& operator=(const variant_impl<Types...>& other);
    variant_impl<Types...>& operator=(variant_impl<Types...>&& other);
    template<typename Type, typename TType>
    requires (mpl::is_variant<TType>::value == false)
    variant_impl<Types...>& assign(TType&& val);
    bool operator==(const variant_impl<Types...>& other) const;
    bool operator==(variant_impl<Types...>&& other) const;
    bool operator!=(const variant_impl<Types...>& other) const;
    bool operator!=(variant_impl<Types...>&& other) const;
    template<typename Type, typename TType>
    requires (mpl::is_variant<TType>::value == false)
    bool compare(TType&& other) const;
    template<typename TType>
    typename embedded_type<TType>::ref_type get() const;
    template<typename TType>
    TType extract();
    template<typename TType>
    bool is() const;
    bool empty() const;
    uint8_t which() const;
    void reset();
    void swap(variant_impl<Types...>& other);
    template<typename Visitor>
    typename Visitor::result_type apply_visitor(const Visitor& visitor);
    template<typename Visitor>
    typename Visitor::result_type apply_visitor(const Visitor& visitor) const;

private:
    static const int _numTypes = mpl::get_num_types<Types...>::value;
    typedef typename mpl::max_size_of<embedded_type<Types>...>::type dominantType;

    typedef typename std::aligned_storage<sizeof(dominantType), std::alignment_of<dominantType>::value>::type data_type;
    data_type	m_storage;
    uint8_t		m_currentType;
};

}
}
}

#include "YTL/types/variant/detail/variant_impl.tpp"
