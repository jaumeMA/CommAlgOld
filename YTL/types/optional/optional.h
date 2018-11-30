
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
#include "YTL/mpl/cOptionalTemplateHelper.h"

namespace yame
{
namespace ytl
{

template<typename T>
class optional
{
template<typename TT>
friend class optional;

//typedef typename embedded_type<T>::internal_type argument_type;

public:
    //publish relevant inner types
    typedef const typename embedded_type<T>::raw_type& reference_const_type;
    typedef typename embedded_type<T>::raw_type& reference_type;
    typedef typename embedded_type<T>::raw_type&& rreference_type;
    typedef typename embedded_type<T>::raw_type* pointer_type;
    typedef const typename embedded_type<T>::raw_type* pointer_const_type;

    optional();
    optional(mpl::none_t);
    optional(const optional<T>& other);
    optional(optional<T>&& other);
    optional(reference_type val);
    optional(reference_const_type val);
    optional(rreference_type val);
    template<typename TT>
    optional(const optional<TT>& other);
    template<typename TT>
    optional(optional<TT>&& other);
    template<typename TT>
    optional<T>(bool cond, TT&& val);
    template<typename TT>
    requires (mpl::is_optional<TT>::value == false)
    explicit optional(TT&& val);
    ~optional();
    optional<T>& operator=(const optional<T>& other);
    optional<T>& operator=(optional<T>& other);
    optional<T>& operator=(optional<T>&& other);
    template<typename TT>
    optional<T>& operator=(const optional<TT>& other);
    template<typename TT>
    optional<T>& operator=(optional<TT>& other);
    template<typename TT>
    optional<T>& operator=(optional<TT>&& other);
    optional<T>& operator=(mpl::none_t);
    optional<T>& operator=(reference_type val);
    optional<T>& operator=(reference_const_type val);
    optional<T>& operator=(rreference_type val);
    template<typename TT>
    optional<T>& operator=(TT&& val);
    bool isInitialized() const;
    operator bool() const;
    bool operator!() const;
    typename embedded_type<T>::cref_type get() const;
    typename embedded_type<T>::ref_type get();
    typename embedded_type<T>::cref_type get_value_or(typename embedded_type<T>::cref_type default_value) const;
    typename embedded_type<T>::ref_type get_value_or(typename embedded_type<T>::ref_type default_value);
    typename embedded_type<T>::cpointer_type get_ptr() const;
    typename embedded_type<T>::pointer_type get_ptr();
    T extract();
    typename embedded_type<T>::ref_type operator*() const;
    typename embedded_type<T>::ref_type operator*();
    typename embedded_type<T>::cpointer_type operator->() const;
    typename embedded_type<T>::pointer_type operator->();
    void swap(const optional<T>& other);

private:
    bool m_is_initialized;
    typedef typename std::aligned_storage<sizeof(embedded_type<T>), std::alignment_of<embedded_type<T>>::value>::type data_type;
    data_type m_storage;
};

}
}

#include "YTL/types/optional/detail/optional.tpp"
