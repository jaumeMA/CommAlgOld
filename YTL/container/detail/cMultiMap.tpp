
#include "YTL/container/detail/cMultiMapOps.h"

namespace yame
{
namespace container
{
namespace detail
{

template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
cMultiMapValue<T,R,Map,A,B,V>::cMultiMapValue(const T& i_value)
: MapClass()
, m_value(i_value)
, m_pHolder(NULL)
, m_pNodeLocation(NULL)
{
}
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
cMultiMapValue<T,R,Map,A,B,V>::cMultiMapValue(T&& i_value)
: MapClass()
, m_value(mpl::move(i_value))
, m_pHolder(NULL)
, m_pNodeLocation(NULL)
{
}
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
cMultiMapValue<T,R,Map,A,B,V>::cMultiMapValue(const cMultiMapValue& other)
: MapClass(other)
, m_value(other.m_value)
, m_pHolder(NULL)//holder is always set on insert node operation, not n construction
, m_pNodeLocation(NULL)
{
}
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
cMultiMapValue<T,R,Map,A,B,V>::cMultiMapValue(cMultiMapValue&& other)
: MapClass(mpl::move(other))
, m_value(mpl::move(other.m_value))
, m_pHolder(NULL)//holder is always set on insert node operation, not n construction
, m_pNodeLocation(NULL)
{
}
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
typename cMultiMapValue<T,R,Map,A,B,V>::Value* cMultiMapValue<T,R,Map,A,B,V>::getHolder()
{
    return m_pHolder;
}
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
const typename cMultiMapValue<T,R,Map,A,B,V>::Value* cMultiMapValue<T,R,Map,A,B,V>::getHolder() const
{
    return m_pHolder;
}
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
typename cMultiMapValue<T,R,Map,A,B,V>::MapNode* cMultiMapValue<T,R,Map,A,B,V>::getNodeLocation() const
{
    return m_pNodeLocation;
}
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
void cMultiMapValue<T,R,Map,A,B,V>::setHolder(Value* pParent)
{
    if(m_pHolder != pParent)
    {
        m_pHolder = pParent;
    }

    return;
}
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
cMultiMapValue<T,R,Map,A,B,V>& cMultiMapValue<T,R,Map,A,B,V>::operator=(const cMultiMapValue& other)
{
    m_value = other.m_value;

    MapClass::operator=(other);

    return *this;
}
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
cMultiMapValue<T,R,Map,A,B,V>& cMultiMapValue<T,R,Map,A,B,V>::operator=(const T& i_val)
{
    m_value = i_val;

    return *this;
}
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
cMultiMapValue<T,R,Map,A,B,V>& cMultiMapValue<T,R,Map,A,B,V>::operator=(cMultiMapValue&& other)
{
    m_value = mpl::move(other.m_value);

    MapClass::operator=(mpl::move(other));

    setReferences();

    return *this;
}
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
cMultiMapValue<T,R,Map,A,B,V>& cMultiMapValue<T,R,Map,A,B,V>::operator=(T&& i_val)
{
    m_value = mpl::move(i_val);

    setReferences();

    return *this;
}
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
T& cMultiMapValue<T,R,Map,A,B,V>::getValue()
{
    return m_value;
}
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
const T& cMultiMapValue<T,R,Map,A,B,V>::getValue() const
{
    return m_value;
}
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
cMultiMapValue<T,R,Map,A,B,V>::operator T&()
{
    return m_value;
}
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
cMultiMapValue<T,R,Map,A,B,V>::operator const T&() const
{
    return m_value;
}
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
size_t cMultiMapValue<T,R,Map,A,B,V>::getTotalSize(const ytl::function<bool(const_reference)>& filter) const
{
    size_t res = 0;

    const_iterator_type itPoliThis = this->cbegin();

    for(;itPoliThis!=this->cend();itPoliThis++)
    {
        if(filter == null_ptr || filter(*itPoliThis))
        {
            res++;
        }

        res += itPoliThis->second.getTotalSize();
    }

    return res;
}
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
void cMultiMapValue<T,R,Map,A,B,V>::setNodeLocation(MapNode* i_loc)
{
    ASSERT(i_loc, "You shall provide non null pointer to map node");

    m_pNodeLocation = i_loc;
}
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
void cMultiMapValue<T,R,Map,A,B,V>::setReferences()
{
    MapNode* currNode = static_cast<MapNode*>(this->getFirstElem());

    while(currNode!=this->getHead())
    {
        currNode->m_value.second.setHolder(static_cast<Value*>(this));
        currNode->m_value.second.setNodeLocation(currNode);
        currNode->m_value.second.setReferences();

        currNode = static_cast<MapNode*>(this->getNextElem(currNode));
    }
}
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
T& cMultiMapValue<T,R,Map,A,B,V>::getAt()
{
    return m_value;
}
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
const T& cMultiMapValue<T,R,Map,A,B,V>::getAt() const
{
    return m_value;
}
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
template<typename ... Args>
T& cMultiMapValue<T,R,Map,A,B,V>::getAt(R key, Args&& ... keys)
{
    cMultiMapValue<T,R,Map,A,B,V>& foundValue = MapClass::operator[](key);

    return foundValue.getAt(mpl::forward<Args>(keys)...);
}
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
template<typename ... Args>
const T& cMultiMapValue<T,R,Map,A,B,V>::getAt(R key, Args&& ... keys) const
{
    const cMultiMapValue<T,R,Map,A,B,V>& foundValue = MapClass::operator[](key);

    return foundValue.getAt(mpl::forward<Args>(keys)...);
}
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
const typename cMultiMapValue<T,R,Map,A,B,V>::iterable_private_interface& cMultiMapValue<T,R,Map,A,B,V>::acces_private_interface() const
{
    return static_cast<const iterable_private_interface&>(*this);
}

template<typename V, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A,template<typename,typename> class B>
cMultiMapImpl<V,R,Map,A,B>::cMultiMapImpl(const cMultiMapImpl& other)
: MapClass(other)
{
    setReferences();
}
template<typename V, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A,template<typename,typename> class B>
cMultiMapImpl<V,R,Map,A,B>& cMultiMapImpl<V,R,Map,A,B>::operator=(const cMultiMapImpl& other)
{
    MapClass::operator=(other);

    setReferences();

    return *this;
}
template<typename V, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A,template<typename,typename> class B>
size_t cMultiMapImpl<V,R,Map,A,B>::getTotalSize(const ytl::function<bool(const_reference)>& filter) const
{
    size_t res = 0;

    const_iterator_type itPoliThis = this->cbegin();

    for(;itPoliThis!=this->cend();itPoliThis++)
    {
        if(filter == null_ptr || filter.eval(*itPoliThis))
        {
            res++;
        }

        res += itPoliThis->second.getTotalSize(filter);
    }

    return res;
}
template<typename V, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A,template<typename,typename> class B>
template<typename ... Args>
typename cMultiMapImpl<V,R,Map,A,B>::underlying_type& cMultiMapImpl<V,R,Map,A,B>::getAt(R key, Args&& ... keys)
{
    V& foundValue = MapClass::operator[](key);

    return foundValue.getAt(mpl::forward<Args>(keys)...);
}
template<typename V, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A,template<typename,typename> class B>
template<typename ... Args>
const typename cMultiMapImpl<V,R,Map,A,B>::underlying_type& cMultiMapImpl<V,R,Map,A,B>::getAt(R key, Args&& ... keys) const
{
    const V& foundValue = MapClass::operator[](key);

    return foundValue.getAt(mpl::forward<Args>(keys)...);
}
template<typename V, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A,template<typename,typename> class B>
void cMultiMapImpl<V,R,Map,A,B>::setReferences()
{
    MapNode* currNode = static_cast<MapNode*>(this->getFirstElem());
    const auto headNode = this->getHead();

    while(currNode != headNode)
    {
        currNode->m_value.second.setNodeLocation(currNode);
        currNode->m_value.second.setReferences();

        currNode = static_cast<MapNode*>(this->getNextElem(currNode));
    }
}

}

template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
void traverse(const detail::cMultiMapValue<T,R,Map,A,B,V>& i_node, const ytl::function<void(const R&, const T&)>& i_sink)
{
    typedef detail::cMultiMapValue<T,R,Map,A,B,V> MapValue;
    typedef typename MapValue::MapNode MapNode;

    const MapValue* currNode = &i_node;

    while(currNode)
    {
        if(const MapNode* currMapNode = currNode->getNodeLocation())
        {
            i_sink.eval(currMapNode->m_value.first,currMapNode->m_value.second.getValue());

            currNode = currMapNode->m_value.second.getHolder();
        }
        else
        {
            currNode = NULL;
        }
    }
}

}
}
