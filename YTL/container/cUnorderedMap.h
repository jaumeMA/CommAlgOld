
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

#include "cQueue.h"
#include "cString.h"
#include "cPair.h"
#include "cIterableBase.h"
#include "cContainerView.h"
#include <functional>

#define MAX_SIZE_TABLE 1024
#define DEFAULT_LOAD_FACTOR 1

namespace yame
{
namespace container
{
//empty implementation
template<typename R>
struct Hash
{
    Hash(){};
    inline size_t operator()(const R& i_key) const
    {
        return hasher(i_key);
    }

    std::hash<R> hasher;
};

template<typename T>
struct Hash<T*>
{
    inline size_t operator()(const T* pointer) const
    {
        uintptr_t addr = reinterpret_cast<uintptr_t>(pointer);

        addr = addr >> 3;

#if SIZE_MAX < UINTPTR_MAX
        /* size_t is not large enough to hold the pointer’s memory address */
        addr %= SIZE_MAX; /* truncate the address so it is small enough to fit in a size_t */
#endif
        return addr;
    }
};
//specializations
template<>
struct Hash<string>
{
public:
    inline size_t operator()(const string& keyStr) const
    {
        const char *key = keyStr.getStr();
        size_t len = keyStr.getSize();
        size_t hash, i;

        for(hash = i = 0; i < len; ++i)
        {
            hash += key[i];
            hash += (hash << 10);
            hash ^= (hash >> 6);
        }
        hash += (hash << 3);
        hash ^= (hash >> 11);
        hash += (hash << 15);

        return hash;
    }
};

namespace detail
{

template<typename R,typename T>
class cUnorderedNode
{
public:
    typedef typename cArray<cPair<R,T>>::iterator_type iterator_type;
    typedef typename cArray<cPair<R,T>>::const_iterator_type const_iterator_type;
    typedef cPair<R,T>& lvaluereference;
    typedef const cPair<R,T>& const_lvaluereference;

    cUnorderedNode() = default;
    cUnorderedNode(const R& key, const T& value);
    inline void push(const R& key, const T& value);
    inline void push_front(const R& key, const T& value);
    inline void pop();
    inline void pop_back();
    inline lvaluereference front();
    inline lvaluereference back();
    inline const_lvaluereference front() const;
    inline const_lvaluereference back() const;
    inline iterator_type find(const R& key);
    inline const_iterator_type find(const R& key) const;
    inline iterator_type erase(iterator_type it);
    inline iterator_type erase(cPair<R,T>* i_node);
    inline size_t getSize() const;
    inline bool empty() const;
    inline cPair<R,T>* getFirstElem() const;
    inline cPair<R,T>* getLastElem() const;
    inline cPair<R,T>* getNextElem(cPair<R,T>* currNode) const;
    inline cPair<R,T>* getPrevElem(cPair<R,T>* currNode) const;
    inline iterator_type begin();
    inline const_iterator_type begin() const;
    inline iterator_type end();
    inline const_iterator_type cend() const;

private:
    cArray<cPair<R,T>>  m_bucket;
};


template<typename T, typename R, typename N, typename hash, template<typename> class A>
class cUnorderedMapImpl : public detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>
{
    static_assert(mpl::is_const<T>::value == false && mpl::is_reference<T>::value == false, "Const/ reference types are not supported");

    struct iterator_pair
    {
        iterator_pair(cUnorderedNode<R,T>* i_currNode, cPair<R,T>* i_currPair)
        : currNode(i_currNode)
        , currPair(i_currPair)
        {}

        cUnorderedNode<R,T>* currNode;
        cPair<R,T>* currPair;
    };

    typedef typename detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>::node_pointer_type node_pointer_type;

public:
    typedef N MapNode;
    typedef typename A<MapNode>::allocator Allocator;
    typedef typename mpl::add_constness<T>::type const_T;
    typedef typename mpl::add_constness<R>::type const_R;
    typedef typename mpl::drop_reference<T>::type& lvaluereference;
    typedef const typename mpl::drop_reference<T>::type& const_lvaluereference;
    typedef typename mpl::drop_reference<R>::type& lkeyreference;
    typedef const typename mpl::drop_reference<R>::type& const_lkeyreference;
    typedef typename mpl::drop_reference<T>::type&& rvaluereference;
    typedef typename mpl::drop_reference<R>::type&& rkeyreference;
    typedef typename mpl::drop_reference<T>::type value_type;
    typedef const value_type const_value_type;
    typedef typename mpl::drop_reference<R>::type key_type;
    typedef const key_type const_key_type;
    typedef typename detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>::iterator_type iterator_type;
    typedef typename detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>::const_iterator_type const_iterator_type;
    typedef typename detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>::iterable_private_interface iterable_private_interface;
    typedef typename detail::cBidirectionalIterableBaseImpl<cPair<const R,T>&>::pointer_type pointer_type;

    cUnorderedMapImpl<T,R,N,hash,A>();
    cUnorderedMapImpl<T,R,N,hash,A>(const_lkeyreference key, const_lvaluereference value);
    ~cUnorderedMapImpl<T,R,N,hash,A>() = default;
    inline lvaluereference insert(const_lkeyreference key, const_lvaluereference val);
    inline lvaluereference insert(const_lkeyreference key, rvaluereference val);
    inline size_t erase(const_lkeyreference key);
    inline void clear();
    lvaluereference operator[](const_lkeyreference key);
    const_lvaluereference operator[](const_lkeyreference key) const;
    inline size_t getSize() const;
    inline bool isEmpty() const;
    iterator_type search(const_lkeyreference key) const;
    void setMaxLoadFactor(float i_maxLoadFactor);
    float getMaxLoadFactor() const;
    float getLoadFactor() const;

private:
    inline static iterator_pair* create_iterator(cUnorderedNode<R,T>* i_currNode, cPair<R,T>* i_currPair);
    inline static void delete_iterator(iterator_pair* i_pair);

    //const iterable implementation
    node_pointer_type getFirstElem() const override;
    node_pointer_type getLastElem() const override;
    node_pointer_type getNextElem(node_pointer_type currNode) const override;
    node_pointer_type getPrevElem(node_pointer_type currNode) const override;
    node_pointer_type acquireNode(node_pointer_type i_node) const override;
    void releaseNode(node_pointer_type i_node) const override;
    const cPair<const R,T>& getValue(node_pointer_type currNode) const override;
    cPair<const R,T>& getValue(node_pointer_type currNode) override;

    //non const iterable implementation
    void eraseNode(node_pointer_type i_pair) override;
    node_pointer_type addNode(typename mpl::PairType<const R,T>::type i_value) override;
    inline void reHash();

    cArray<cUnorderedNode<R,T>,Allocator> m_records;
    hash                        m_hash;
    size_t                     m_numItems;
    size_t                      m_maxNumBuckets;
    float                       m_maxLoadFactor;
};
}

template<typename T, typename R, typename hash = Hash<R>, template<typename> class A = memory::cTypedSystemAllocator>
using cUnorderedMap = detail::cUnorderedMapImpl<T,R,detail::cUnorderedNode<R,T>,hash,A>;

template<typename T, typename R>
using unordered_map_view = cBidirectionalView<cPair<const R,T>&>;
template<typename T, typename R>
using const_unordered_map_view = cConstBidirectionalView<cPair<const R,T>&>;

}
}

#include "detail/cUnorderedMap.tpp"
