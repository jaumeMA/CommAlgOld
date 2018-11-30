
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

#include "YTL/container/cString.h"
#include "YTL/container/cRecursiveMap.h"
#include "YTL/container/cMultiMap.h"
#include "XmlParser/cProperty.h"

namespace yame
{
namespace xml
{

template<template<typename> class A = memory::cTypedSystemAllocator>
class cXmlValue;

}

namespace mpl
{
template<template<typename> class A>
struct is_copy_constructible<xml::cXmlValue<A>>
{
static const bool value = true;
};
}

namespace xml
{
namespace detail
{

struct xmlValueData
{
    container::string   m_tag;
    container::cArray<xml::cProperty>   m_properties;
    container::string   m_content;
};

}

template<template<typename> class A>
class cXmlValue : public container::detail::cMultiMapValue<detail::xmlValueData,container::string,container::cMap,A,container::RecursiveBalancer<container::cMap>::Balancer,cXmlValue<A>>
{
public:
    typedef container::detail::cMultiMapValue<detail::xmlValueData,container::string,container::cMap,A,container::RecursiveBalancer<container::cMap>::Balancer,cXmlValue<A>> MapValue;
    typedef typename MapValue::underlying_type underlying_type;
    typedef typename MapValue::MapClass MapClass;
    typedef typename MapValue::MapNode MapNode;
    typedef typename MapValue::const_T const_T;
    typedef typename MapValue::const_R const_R;
    typedef typename MapValue::lvaluereference lvaluereference;
    typedef typename MapValue::const_lvaluereference const_lvaluereference;
    typedef typename MapValue::lkeyreference lkeyreference;
    typedef typename MapValue::const_lkeyreference const_lkeyreference;
    typedef typename MapValue::rvaluereference rvaluereference;
    typedef typename MapValue::rkeyreference rkeyreference;
    typedef typename MapValue::value_type value_type;
    typedef typename MapValue::key_type key_type;
    typedef typename MapValue::iterator_type iterator_type;
    typedef typename MapValue::const_iterator_type const_iterator_type;
    typedef typename MapValue::iterable_private_interface iterable_private_interface;

    using MapValue::begin;
    using MapValue::cbegin;
    using MapValue::end;
    using MapValue::cend;
    using MapValue::operator=;
    using MapValue::operator[];
    using MapValue::operator underlying_type&;
    using MapValue::operator const underlying_type&;
    using MapValue::MapValue;
    using MapValue::getValue;
    using MapValue::getTotalSize;
    using MapValue::getHolder;
    using MapValue::getNodeLocation;
    using MapValue::getAt;

    cXmlValue();
    cXmlValue(container::string tag, container::cArray<cProperty> properties);
    bool operator==(const cXmlValue& other) const;
    const container::string& getTag() const;
    const container::string& getContent() const;
    void append(container::string content);
    const cProperty& getProperty(size_t i) const;
    const container::cArray<cProperty>& getProperties() const;
    size_t getNumProperties() const;
};

template<template<typename> class A = memory::cTypedSystemAllocator>
using cXmlTree = container::detail::cMultiMapImpl<cXmlValue<A>,container::string,container::cMap,A,container::MultiBalancer<container::cMap>::Balancer>;

}
}

#include "XmlParser/detail/cXmlTree.tpp"
