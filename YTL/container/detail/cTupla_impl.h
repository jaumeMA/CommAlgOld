
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
#include "YTL/container/cIterableBase.h"
#include "YTL/container/cContainerView.h"
#include <initializer_list>

#define SIGNATURE(T,dim) typename yame::mpl::create_function_signature<T,dim>::template is<>::type
#define BASE_TYPE(T) T
#define DEFINE_BASE_TYPE(T) typedef BASE_TYPE(T) base_type
#define PRIMITIVE_TYPE(T) typename yame::mpl::get_primitive_type<T,yame::mpl::has_base_type<T>::value>::type
#define DEFINE_PRIMITIVE_TYPE(T) typedef PRIMITIVE_TYPE(T) primitive_type
#define DIM_TUPLE(rank,ranks) mpl::get_num_ranks<rank,ranks...>::value
#define PROD_RANKS(rank,ranks) mpl::Prod<rank,ranks...>::value
#define SQUARE_TUPLE(T,rank,m) mpl::append_dim<T,m,rank,yame::container::cTupla>::template is<>::type

#define PROD_TUPLE(T,rank,ranks,rankOther,ranksOther) cTupla<T,rank,ranks...,rankOther,ranksOther...>
#define SUM_TUPLE(T,rank,ranks,rankOther,ranksOther) cTupla<T,rank+rankOther,ranks...>
#define SUB_TUPLE(T,ranks) \
    typename mpl::get_sub_tuple<T,container::detail::cSubTuple,ranks...>::type

#define FUNCTION_CALL(T,rank) typename function_call<T,rank>::template is<>

namespace yame
{
namespace container
{
namespace detail
{
template<typename T, int ... ranks>
class cTupla_impl;

template<typename T, int ... ranks>
class cSubTuple;

template<typename T, int rank, int ... ranks>
class cSubTuple<T,rank,ranks...>
{
public:
    cSubTuple<T,ranks...>   m_subTuple[rank];
    static const u16        m_rank=mpl::Prod<rank,ranks...>::value;
    DEFINE_PRIMITIVE_TYPE(T);
    DEFINE_BASE_TYPE(T);

public:
    void reference_tuple(const cSubTuple<T,rank,ranks...>& other);
    void reference_tuple(void* ref);

public:
    cSubTuple<T,rank,ranks...>();
    cSubTuple<T,rank,ranks...>(const cSubTuple<T,rank,ranks...>& other)=delete;
    cSubTuple<T,rank,ranks...>(const cTupla_impl<T,rank,ranks...>& other);
    cSubTuple<T,rank,ranks...>(void *ref);
    SUB_TUPLE(T,ranks)& operator[](u8 index);
    SUB_TUPLE(T,ranks) operator[](u8 index) const;
    cSubTuple<T,rank,ranks...>& operator=(const cSubTuple<T,rank,ranks...>& other);
    void print() const;
    int getRank(int dim, int counter);
    static u16 getSubDimAt(u16 index, u16 *indexs);
};

template<typename T>
class cSubTuple<T>
{
public:
    T                       *m_ref;
    DEFINE_PRIMITIVE_TYPE(T);
    DEFINE_BASE_TYPE(T);

public:
    void reference_tuple(const cSubTuple<T>& other);
    void reference_tuple(void* ref);
    cSubTuple<T>();
    cSubTuple<T>(const cSubTuple<T>& other)=delete;
    explicit cSubTuple<T>(void *ref);
    operator T&();
    operator const T&() const;
    cSubTuple<T>& operator=(const cSubTuple<T>& other);
    cSubTuple<T>& operator=(const T& other);
    static int getSubDimAt(u16 index, u16 *indexs);
    int getRank(int dim, int counter);
};

//specialization for nested tuples
template<typename T, int rank, int ... ranks>
class cSubTuple<cSubTuple<T,rank,ranks...> >
{
public:
    cSubTuple<T,rank,ranks...>   m_subTuple;
    DEFINE_PRIMITIVE_TYPE(T);
    DEFINE_BASE_TYPE(T);

public:
    void reference_tuple(const cSubTuple<cSubTuple<T,rank,ranks...> >& other);
    void reference_tuple(void* ref);
public:
    cSubTuple();
    cSubTuple(const cSubTuple<cSubTuple<T,rank,ranks...> >& other)=delete;
    explicit cSubTuple(void *ref);
    operator cSubTuple<T,rank,ranks...>&();
    operator const cSubTuple<T,rank,ranks...>&() const;
    SUB_TUPLE(T,ranks)& operator[](u8 index);
    SUB_TUPLE(T,ranks) operator[](u8 index) const;
    cSubTuple<cSubTuple<T,rank,ranks...> >& operator=(const cSubTuple<cSubTuple<T,rank,ranks...> >& other);
    cSubTuple<cSubTuple<T,rank,ranks...> >& operator=(const cSubTuple<T,rank,ranks...>& other);
    static int getSubDimAt(u16 index, u16 *indexs);
    int getRank(int dim, int counter);
};


template<typename T, int totalRank>
class cTuplaStorage
{
public:
    DEFINE_PRIMITIVE_TYPE(T);

    __attribute__((aligned(16*sizeof(u8)))) primitive_type   m_tupla[totalRank*mpl::total_rank<T>::value];

    inline primitive_type* getTupla()
    {
        return m_tupla;
    }
    inline const primitive_type* getTupla() const
    {
        return m_tupla;
    }
    inline bool isAddressInside(const void* i_address, size_t i_bottomOffset, size_t i_topOffset) const
    {
        const primitive_type* address = reinterpret_cast<const primitive_type*>(i_address);

        return address >= (m_tupla + i_bottomOffset) && address < (m_tupla + totalRank - i_topOffset);
    }
};

template<typename T, int rank, int ... ranks>
class cTupla_impl<T,rank,ranks...> : public cTuplaStorage<T,PROD_RANKS(rank,ranks)>, public detail::cConstRandomAccessIterableBaseImpl<T&>
{
    typedef typename detail::cConstRandomAccessIterableBaseImpl<T&>::node_pointer_type node_pointer_type;

    using cTuplaStorage<T,PROD_RANKS(rank,ranks)>::isAddressInside;

public:
    typedef T value_type;
    typedef detail::cConstRandomAccessIterableBaseImpl<T&> iterable_type;
    typedef typename detail::cConstRandomAccessIterableBaseImpl<T&>::iterator_type iterator_type;
    typedef typename detail::cConstRandomAccessIterableBaseImpl<T&>::const_iterator_type const_iterator_type;
    typedef typename detail::cConstRandomAccessIterableBaseImpl<T&>::iterable_public_interface iterable_public_interface;
    typedef typename detail::cConstRandomAccessIterableBaseImpl<T&>::reference reference;
    typedef typename detail::cConstRandomAccessIterableBaseImpl<T&>::const_reference const_reference;
    static const detail::ReferenceCategory category = iterator_type::category;
    typedef typename cTuplaStorage<T,PROD_RANKS(rank,ranks)>::primitive_type primitive_type;

    cTupla_impl();
    cTupla_impl(T *values);
    template<typename ... Args>
    cTupla_impl(const T& arg, Args&& ... i_args);
    template<typename ... Args>
    cTupla_impl(T&& arg, Args&& ... i_args);
    cTupla_impl(const cTupla_impl<T,rank,ranks...>& other);
    cTupla_impl(const cSubTuple<T,rank,ranks...>& other);
    template<typename TT>
    cTupla_impl(const std::initializer_list<TT>& i_tupleList);
    ~cTupla_impl();
    SUB_TUPLE(T,ranks)& operator[](u8 index);
    const SUB_TUPLE(T,ranks)& operator[](u8 index) const;
    cTupla_impl<T,rank,ranks...>& operator=(const cTupla_impl<T,rank,ranks...>& other);
    bool operator==(const cTupla_impl<T,rank,ranks...>& other) const;
    bool operator!=(const cTupla_impl<T,rank,ranks...>& other) const;
    primitive_type at(u16 numIndex, u16 *indexs) const;
    primitive_type& at(u16 numIndex, u16 *indexs);
    u8 getDim();
    u16 getTotalRank();
    u8 getRank(int dim);
    const primitive_type* getAsPtr() const;
    template<int Index, int ... Indexs>
    T& get();
    template<int Index, int ... Indexs>
    const T& get() const;

    //const iterable implementation
    size_t getSize() const override;
    node_pointer_type getFirstElem() const override;
    node_pointer_type getLastElem() const override;
    node_pointer_type getNextElem(node_pointer_type currNode) const override;
    node_pointer_type getPrevElem(node_pointer_type currNode) const override;
    const_reference getValue(node_pointer_type) const override;
    reference getValue(node_pointer_type) override;
    node_pointer_type shiftNodeByIndex(node_pointer_type currNode, int index) const override;
    size_t getIndexOfNode(node_pointer_type node) const override;


private:
    //this var is useful when applying SFINAE to this class and for recursion over type T
    DEFINE_BASE_TYPE(T);

    void reference_tuple();

protected:
    static const u16       m_rank=PROD_RANKS(rank,ranks);
    static const u8        m_dim=DIM_TUPLE(rank,ranks);
    cSubTuple<T,ranks...>   m_subTuple[rank];
};

template<typename T>
class cTupla_impl<T,1> : public cTuplaStorage<T,1>, public detail::cConstRandomAccessIterableBaseImpl<T&>
{
friend class cMatrix;

    typedef typename detail::cConstRandomAccessIterableBaseImpl<T&>::node_pointer_type node_pointer_type;

    using cTuplaStorage<T,1>::isAddressInside;

public:
    static const u16    m_rank=1;
    typedef T value_type;
    typedef detail::cConstRandomAccessIterableBaseImpl<T&> iterable_type;
    typedef typename detail::cConstRandomAccessIterableBaseImpl<T&>::iterator_type iterator_type;
    typedef typename detail::cConstRandomAccessIterableBaseImpl<T&>::const_iterator_type const_iterator_type;
    typedef typename detail::cConstRandomAccessIterableBaseImpl<T&>::iterable_public_interface iterable_public_interface;
    typedef typename detail::cConstRandomAccessIterableBaseImpl<T&>::reference reference;
    typedef typename detail::cConstRandomAccessIterableBaseImpl<T&>::const_reference const_reference;
    static const detail::ReferenceCategory category = iterator_type::category;
    typedef typename cTuplaStorage<T,1>::primitive_type primitive_type;

public:
    cTupla_impl();
    cTupla_impl(T value);
    cTupla_impl(const cTupla_impl<T,1>& other);
    template<typename TT>
    cTupla_impl(const std::initializer_list<TT>& i_tupleList);
    T& operator[](u16 index);
    const T& operator[](u16 index) const;
    operator T&();
    operator const T&() const;
    cTupla_impl<T,1>& operator=(const cTupla_impl<T,1>& other);
    bool operator==(const cTupla_impl<T,1>& other) const;
    bool operator!=(const cTupla_impl<T,1>& other) const;
    primitive_type at(u16 numIndex, u16 *indexs) const;
    primitive_type& at(u16 numIndex, u16 *indexs);
    u8 getDim();
    u16 getTotalRank();
    u8 getRank(int dim);
    const primitive_type* getAsPtr() const;
    template<int Index>
    T& get();
    template<int Index>
    const T& get() const;

    //const iterable implementation
    size_t getSize() const override;
    node_pointer_type getFirstElem() const override;
    node_pointer_type getLastElem() const override;
    node_pointer_type getNextElem(node_pointer_type currNode) const override;
    node_pointer_type getPrevElem(node_pointer_type currNode) const override;
    const_reference getValue(node_pointer_type) const override;
    reference getValue(node_pointer_type) override;
    node_pointer_type shiftNodeByIndex(node_pointer_type currNode, int index) const override;
    size_t getIndexOfNode(node_pointer_type node) const override;

private:
    DEFINE_BASE_TYPE(T);

    void reference_tuple();

    cSubTuple<T>          m_subTuple;
};

}

template<typename T>
using tupla_view = cRandomAccessView<T&>;
template<typename T>
using const_tupla_view = cConstRandomAccessView<T&>;

}

namespace mpl
{
template<typename T, int ... ranks>
struct type_size<container::detail::cSubTuple<T,ranks...> >
{
    static const size_t value = Prod<ranks...>::value * type_size<T>::value;
};

template<typename T, int ... ranks>
struct total_dim<container::detail::cSubTuple<T,ranks...> >
{
    static const size_t value = get_num_ranks<ranks...>::value + total_dim<T>::value;
};

template<typename T, int ... ranks>
struct total_rank<container::detail::cSubTuple<T,ranks...> >
{
    static const size_t value = Prod<ranks...>::value * total_rank<T>::value;
};
}
}

#include "cTupla_impl.tpp"
