
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

namespace yame
{
namespace ytl
{

//internal type
template<typename T>
struct embedded_type;

template<typename T>
struct embedded_type<T&>
{
public:
    //avoid right references
    typedef typename std::remove_cv<T>::type raw_type;
    typedef T internal_type;
    typedef T& ref_type;
    typedef const T& cref_type;
    typedef T& rref_type;
    typedef T* pointer_type;
    typedef const T* cpointer_type;

public:
    internal_type&			m_data;

public:
    embedded_type(internal_type& other);
    embedded_type(const embedded_type<T&>& other);
    //this is a ref, we do not allow assignments beyond construction
    embedded_type() = delete;
    embedded_type(internal_type&& other) = delete;
    embedded_type(embedded_type<T&>&& other) = delete;
    T& operator=(internal_type& other) = delete;
    T& operator=(internal_type&& other) = delete;
    T& operator=(embedded_type<T&>&& other) = delete;
    bool operator==(const embedded_type<T&>& other) const;
    bool operator==(embedded_type<T&>&& other) const;
    rref_type extract();
    ref_type get();
    pointer_type operator->();
    cpointer_type operator->() const;
    template<typename Type>
    static bool construct(void* address, Type&& val);
    static bool destroy(void* address);
    template<typename Type>
    static bool assign(void* address, Type&& val);
    static bool swap(void* addressA, internal_type& valA, void* addressB, internal_type& valB);
};

template<typename T>
struct embedded_type<T&&>
{
public:
    //avoid right references
    typedef typename std::remove_cv<T>::type raw_type;
    typedef T internal_type;
    typedef T&& ref_type;
    typedef T&& cref_type; //makes no sense this concept here
    typedef T&& rref_type;
    typedef T* pointer_type;
    typedef const T* cpointer_type;

public:
    internal_type&&			m_data;

public:
    embedded_type(T&& other);
    embedded_type(const embedded_type<T&&>& other);
    //this is a ref, we do not allow assignments beyond construction
    embedded_type() = delete;
    embedded_type(internal_type& other) = delete;
    embedded_type(embedded_type<T&&>&& other) = delete;
    T& operator=(internal_type& other) = delete;
    T& operator=(internal_type&& other) = delete;
    T& operator=(embedded_type<const T&>&& other) = delete;
    bool operator==(const embedded_type<T&&>& other) const;
    bool operator==(embedded_type<T&&>&& other) const;
    ref_type get();
    rref_type extract();
    pointer_type operator->();
    cpointer_type operator->() const;
    template<typename Type>
    static bool construct(void* address, Type&& val);
    static bool destroy(void* address);
    template<typename Type>
    static bool assign(void* address, Type&& val);
    static bool swap(void* addressA, internal_type&& valA, void* addressB, internal_type&& valB);
};

template<typename T>
struct embedded_type
{
private:
    static const bool copyConstr = std::is_copy_constructible<T>::value || std::is_pointer<T>::value;
    static const bool moveConstr = std::is_move_constructible<T>::value || std::is_pointer<T>::value;
    static_assert(copyConstr || moveConstr, "You cannot define a variant in which one of the types are not copy nor move constructible");

    template<typename _TypeTrue, typename _TypeFalse, bool _condition>
    struct getExternalTypeUponCondition;

    template<typename _TypeTrue, typename _TypeFalse>
    struct getExternalTypeUponCondition<_TypeTrue, _TypeFalse, true>
    {
        typedef _TypeTrue type;
    };

    template<typename _TypeTrue, typename _TypeFalse>
    struct getExternalTypeUponCondition<_TypeTrue, _TypeFalse, false>
    {
        typedef _TypeFalse type;
    };

public:
    //avoid deleted methods
    typedef typename std::remove_cv<T>::type raw_type;
    typedef T internal_type;
    typedef typename getExternalTypeUponCondition<T&, T&&, copyConstr>::type ref_type;
    typedef typename getExternalTypeUponCondition<const T&, const T&&, copyConstr>::type cref_type;
    typedef typename getExternalTypeUponCondition<T&&, T&, moveConstr>::type rref_type;
    typedef T* pointer_type;
    typedef const T* cpointer_type;

public:
    internal_type  m_data;

public:
    embedded_type(const internal_type& other);
    embedded_type(internal_type& other);
    embedded_type(internal_type&& other);
    embedded_type(const embedded_type<T>& other);
    template<typename TType>
    embedded_type(TType&& val);
    embedded_type& operator=(const internal_type& other);
    embedded_type& operator=(internal_type&& other);
    embedded_type& operator=(const embedded_type<T>& other);
    bool operator==(const embedded_type<T>& other) const;
    bool operator==(embedded_type<T>&& other) const;
    ref_type get();
    rref_type extract();
    pointer_type operator->();
    cpointer_type operator->() const;
    template<typename Type>
    static bool construct(void* address, Type&& val);
    static bool destroy(void* address);
    template<typename Type>
    static bool assign(void* address, Type&& val);
    static bool swap(void* addressA, internal_type& valA, void* addressB, internal_type& valB);
};

}
}

#include "YTL/types/embedded_type/detail/embedded_type.tpp"
