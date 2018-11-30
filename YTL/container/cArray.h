
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

#include "System/memory/cSystemAllocator.h"
#include <string>
#include <time.h>
#include <stdio.h>
#include "Utils/engineTypes.h"
#include <math.h>
#include <stdlib.h>
#include <cstring>
#include "YTL/mpl/cTemplateHelper.h"
#include "YTL/container/cIterableBase.h"
#include <vector>
#include "YTL/container/cContainerView.h"

namespace yame
{
namespace container
{

template<typename T>
using array_view = cRandomAccessView<T&>;
template<typename T>
using const_array_view = cConstRandomAccessView<T&>;

template<typename T, typename A = memory::cTypedSystemAllocator<T> >
class cArray : public detail::cRandomAccessIterableBaseImpl<T&>
{
    static_assert(mpl::is_const<T>::value == false && mpl::is_reference<T>::value == false, "Const/ reference types are not supported");

    friend inline bool operator==(const cArray& i_lhs, const cArray& i_rhs)
    {
        return (i_lhs.m_size != i_rhs.m_size) ? false : memcmp(i_lhs.getRootNode(),i_lhs.getRootNode(),i_lhs.m_size*sizeof(T))==0;
    }
    friend inline bool operator!=(const cArray& i_lhs, const cArray& i_rhs)
    {
        return (i_lhs.m_size != i_rhs.m_size) ? true : memcmp(i_lhs.getRootNode(),i_lhs.getRootNode(),i_lhs.m_size*sizeof(T))!=0;
    }

public:
    typedef typename detail::cRandomAccessIterableBaseImpl<T&> iterable_type;
    typedef typename detail::cRandomAccessIterableBaseImpl<T&>::iterator_type iterator_type;
    typedef typename detail::cRandomAccessIterableBaseImpl<T&>::const_iterator_type const_iterator_type;
    typedef typename detail::cRandomAccessIterableBaseImpl<T&>::reverse_iterator_type reverse_iterator_type;
    typedef typename detail::cRandomAccessIterableBaseImpl<T&>::const_reverse_iterator_type const_reverse_iterator_type;
    typedef typename detail::cRandomAccessIterableBaseImpl<T&>::iterable_public_interface iterable_public_interface;
    typedef typename detail::cConstRandomAccessIterableBaseImpl<T&>::iterable_public_interface const_iterable_public_interface;
    typedef typename detail::cRandomAccessIterableBaseImpl<T&>::pointer_type pointer_type;
    typedef typename detail::cRandomAccessIterableBaseImpl<T&>::node_pointer_type node_pointer_type;
    typedef typename detail::cRandomAccessIterableBaseImpl<T&>::add_type add_type;
    static const detail::ReferenceCategory category = iterator_type::category;
    typedef typename mpl::drop_reference<T>::type& lreference;
    typedef const typename mpl::drop_reference<T>::type& const_lreference;
    typedef typename mpl::drop_reference<T>::type&& rreference;
    typedef typename mpl::drop_reference<T>::type value_type;
    typedef T particle;
    static const size_t npos=-1;

    using detail::cRandomAccessIterableBaseImpl<T&>::begin;
    using detail::cRandomAccessIterableBaseImpl<T&>::end;
    using detail::cRandomAccessIterableBaseImpl<T&>::rbegin;
    using detail::cRandomAccessIterableBaseImpl<T&>::rend;
    using detail::cRandomAccessIterableBaseImpl<T&>::cbegin;
    using detail::cRandomAccessIterableBaseImpl<T&>::cend;
    using detail::cRandomAccessIterableBaseImpl<T&>::crbegin;
    using detail::cRandomAccessIterableBaseImpl<T&>::crend;
    using detail::cRandomAccessIterableBaseImpl<T&>::getSize;
    using detail::cRandomAccessIterableBaseImpl<T&>::empty;
    using detail::cRandomAccessIterableBaseImpl<T&>::add;
    using detail::cRandomAccessIterableBaseImpl<T&>::erase;

    cArray();
    cArray(const cArray& other);
    cArray(cArray&& other);
    cArray(const std::initializer_list<T>& i_initList);
    cArray(const_lreference value, size_t length=1);
    cArray(rreference value, size_t length=1);
    cArray(const value_type* data, size_t length);
    cArray(const std::vector<T>& other);
    cArray(const array_view<T>& i_arrayView);
    cArray(const const_array_view<T>& i_arrayView);
    virtual ~cArray();
    cArray& operator=(const cArray& other);
    cArray& operator=(cArray&& other);
    cArray<T,A>& operator+=(const cArray<T,A>& other);
    virtual const_lreference operator[](const size_t index) const;
    virtual lreference operator[](const size_t index);
    virtual lreference at(const size_t index);
    cArray subArr(const size_t initPos, const size_t length) const;
    const value_type* getArray() const;
    value_type* getArray();
    void reset(T value=0);
    void reset(const T& value);
    void reset(T&& value=0);
    inline iterator_type insert(iterator_type itPos, const_iterator_type itOtherInit, const_iterator_type itOtherEnd);
    template<typename ... TT>
    inline iterator_type insert(iterator_type itPos, TT&& ... i_args);
    template<typename ... TT>
    lreference push_back(TT&& ... i_args);
    template<typename ... TT>
    lreference push_front(TT&& ... i_args);
    lreference front();
    lreference back();
    inline const_lreference front() const;
    inline const_lreference back() const;
    void resize(size_t size);
    void pop();
    void clear();
    iterator_type erase(size_t initPos, size_t length=1);
    size_t getCapacity() const;
    size_t getSize() const override;
    void reserve(size_t newCap);
    cArray<size_t> find(const cArray<T>& other) const;
    size_t find_first_of(const_lreference other) const;
    size_t find_last_of(const_lreference other) const;
    void replace(size_t prevPos, size_t newPos);
    cArray<cArray<T>> tokenize(const cArray<T>& separator) const;
    static bool equal(const cArray& arrayA, const cArray& arrayB);
    bool equal(const cArray& other) const;
    static size_t copyBlock(const value_type* source, value_type* dest, size_t numBits, u8 offset, u8 endianess);

protected:
    virtual value_type* acquireMem(size_t size);
    virtual void releaseMem(value_type* node);
    inline value_type* allocate(size_t size);
    inline value_type* reallocate(value_type* ptr, size_t oldSize, size_t newSize);
    inline void deallocate(value_type* node, bool destroy=true);
    virtual bool isInBounds(const value_type* address) const;
    inline void construct(const value_type* address);
    template<typename TT, typename ... TTs>
    inline void construct(const value_type* address, TT&& i_arg, TTs&& ... i_args);
    inline void destruct(value_type* initAddress, value_type* addressEnd);
    template<typename TT>
    inline void assign(TT&& val, value_type* address);
    template<typename ... TT>
    inline lreference _insert(size_t pos, TT&& ... other);
    inline value_type* getAddressAtIndex(size_t index) const;

    //const iterable implementation
    node_pointer_type getFirstElem() const override;
    node_pointer_type getLastElem() const override;
    node_pointer_type getNextElem(node_pointer_type currNode) const override;
    node_pointer_type getPrevElem(node_pointer_type currNode) const override;
    node_pointer_type shiftNodeByIndex(node_pointer_type node, int index) const override;
    size_t getIndexOfNode(node_pointer_type node) const override;
    const_lreference getValue(node_pointer_type i_currNode) const override;
    lreference getValue(node_pointer_type i_currNode) override;

    //non const iterable implementation
    void eraseNode(node_pointer_type i_valueNode) override;
    node_pointer_type addNode(add_type i_value) override;

    size_t      m_capacity;
    const A&    m_alloc;
    size_t      m_size;
};

typedef cArray<c8> arrayC8;
typedef cArray<u8> arrayU8;
typedef cArray<u16> arrayU16;
typedef cArray<u32> arrayU32;
typedef cArray<f32> arrayF32;
typedef cArray<f64> arrayF64;
typedef cArray<s8> arrayS8;
typedef cArray<s16> arrayS16;
typedef cArray<s32> arrayS32;

}
}

template<typename T, typename A>
inline yame::container::cArray<T,A> operator+(const yame::container::cArray<T,A>& i_lhs, const yame::container::cArray<T,A>& i_rhs);
template<typename T, typename A>
inline yame::container::cArray<T,A>& operator<<(yame::container::cArray<T,A>& o_array, const yame::container::cArray<T,A>& i_array);
template<typename T, typename A>
inline yame::container::cArray<T,A>& operator>>(const yame::container::cArray<T,A>& i_array, yame::container::cArray<T,A>& o_array);
template<typename T, typename A>
inline yame::container::cArray<T,A>& operator<<(yame::container::cArray<T,A>& o_array, const typename yame::container::cArray<T,A>::particle& i_value);
template<typename T, typename A>
inline yame::container::cArray<T,A>& operator>>(const typename yame::container::cArray<T,A>::particle& i_value, yame::container::cArray<T,A>& o_array);

#include "detail/cArray.tpp"
