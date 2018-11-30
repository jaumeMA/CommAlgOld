
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

#include "YTL/container/cPair.h"
#include "YTL/functional/cFunction.h"
#include "YTL/container/cMultiMapBalancer.h"
#include "System/memory/cSystemAllocator.h"

namespace yame
{
namespace container
{
namespace detail
{
template<typename,typename,template<typename,typename,template<typename>class,template<typename,typename>class> class,template<typename> class,template<typename,typename>class>
class cMultiMapImpl;

template<typename,typename,template<typename,typename,template<typename>class,template<typename,typename>class> class,template<typename> class,template<typename,typename>class,typename = void>
class cMultiMapValue;
}
}
//for avoiding problems regarding not complete types while checking copy cinstructibility, do it here
namespace mpl
{
template<typename T>
struct is_copy_constructible;

template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A,template<typename,typename> class B, typename V>
struct is_copy_constructible<container::detail::cMultiMapValue<T,R,Map,A,B,V>>
{
static const bool value = is_copy_constructible<T>::value;
};
}

namespace container
{
namespace detail
{

template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
class cMultiMapValue : public Map<typename mpl::static_if<mpl::is_same_type<V,void>::value,cMultiMapValue<T,R,Map,A,B,void>,V>::type,R,A,B>
{
public:
    typedef typename mpl::static_if<mpl::is_same_type<V,void>::value,cMultiMapValue<T,R,Map,A,B,void>,V>::type Value;
    typedef Map<Value,R,A,B> MapClass;
    typedef T underlying_type;
    typedef typename MapClass::MapNode MapNode;
    typedef typename MapClass::const_T const_T;
    typedef typename MapClass::const_R const_R;
    typedef typename MapClass::lvaluereference lvaluereference;
    typedef typename MapClass::const_lvaluereference const_lvaluereference;
    typedef typename MapClass::lkeyreference lkeyreference;
    typedef typename MapClass::const_lkeyreference const_lkeyreference;
    typedef typename MapClass::rvaluereference rvaluereference;
    typedef typename MapClass::rkeyreference rkeyreference;
    typedef typename MapClass::value_type value_type;
    typedef typename MapClass::const_reference const_reference;
    typedef typename MapClass::key_type key_type;
    typedef typename MapClass::iterator_type iterator_type;
    typedef typename MapClass::const_iterator_type const_iterator_type;
    typedef typename MapClass::iterable_private_interface iterable_private_interface;
    typedef typename MapClass::lendable_tag lendable_tag;

    using MapClass::operator[];

    cMultiMapValue(const T& i_value);
    cMultiMapValue(T&& i_value);
    Value* getHolder();
    const Value* getHolder() const;
    MapNode* getNodeLocation() const;
    cMultiMapValue(const cMultiMapValue& other);
    cMultiMapValue(cMultiMapValue&& other);
    cMultiMapValue<T,R,Map,A,B,V>& operator=(const cMultiMapValue& other);
    cMultiMapValue<T,R,Map,A,B,V>& operator=(const T& i_val);
    cMultiMapValue<T,R,Map,A,B,V>& operator=(cMultiMapValue&& other);
    cMultiMapValue<T,R,Map,A,B,V>& operator=(T&& i_val);
    inline size_t getTotalSize(const ytl::function<bool(const_reference)>& filter = null_ptr) const;
    inline T& getValue();
    inline const T& getValue() const;
    inline operator T&();
    inline operator const T&() const;
	inline T& getAt();
	inline const T& getAt() const;
	template<typename ... Args>
	inline T& getAt(R key, Args&& ... keys);
	template<typename ... Args>
	inline const T& getAt(R key, Args&& ... keys) const;
	inline const iterable_private_interface& acces_private_interface() const;
    void setHolder(Value* pParent);
    void setNodeLocation(MapNode* i_loc);
    void setReferences();

protected:
    Value*  m_pHolder = NULL;
    MapNode* m_pNodeLocation = NULL;
    T m_value;
};

template<typename V,typename R,template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B>
class cMultiMapImpl : public Map<V,R,A,B>
{
public:
    typedef Map<V,R,A,B> MapClass;
    typedef typename V::underlying_type underlying_type;
    typedef typename MapClass::MapNode MapNode;
    typedef typename MapClass::const_T const_T;
    typedef typename MapClass::const_R const_R;
    typedef typename MapClass::lvaluereference lvaluereference;
    typedef typename MapClass::const_lvaluereference const_lvaluereference;
    typedef typename MapClass::lkeyreference lkeyreference;
    typedef typename MapClass::const_lkeyreference const_lkeyreference;
    typedef typename MapClass::rvaluereference rvaluereference;
    typedef typename MapClass::rkeyreference rkeyreference;
    typedef typename MapClass::value_type value_type;
    typedef typename MapClass::const_reference const_reference;
    typedef typename MapClass::key_type key_type;
    typedef typename MapClass::iterator_type iterator_type;
    typedef typename MapClass::const_iterator_type const_iterator_type;
    typedef typename MapClass::iterable_private_interface iterable_private_interface;
    typedef typename MapClass::lendable_tag lendable_tag;

    cMultiMapImpl() = default;
    cMultiMapImpl(const cMultiMapImpl&);
    cMultiMapImpl& operator=(const cMultiMapImpl&);
    size_t getTotalSize(const ytl::function<bool(const_reference)>& filter = null_ptr) const;
	template<typename ... Args>
	underlying_type& getAt(R key, Args&& ... keys);
	template<typename ... Args>
	const underlying_type& getAt(R key, Args&& ... keys) const;

private:
    void setReferences();
};

}

template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A = memory::cTypedSystemAllocator>
using cMultiMap = detail::cMultiMapImpl<detail::cMultiMapValue<T,R,Map,A,RecursiveBalancer<Map>::template Balancer>,R,Map,A,MultiBalancer<Map>::template Balancer>;

template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
void traverse(const detail::cMultiMapValue<T,R,Map,A,B,V>& i_node, const ytl::function<void(const R&, const T&)>& i_sink);

}
}

#include "detail/cMultiMap.tpp"
