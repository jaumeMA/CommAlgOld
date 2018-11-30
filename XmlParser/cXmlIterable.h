
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

#include "YTL/container/cMultiMapIterable.h"
#include "XmlParser/cXmlTree.h"
#include "XmlParser/cXmlData.h"

namespace yame
{
namespace xml
{

template<template<typename> class A = memory::cTypedSystemAllocator>
class cXmlIterable : public container::cMultiMapIterable<typename cXmlTree<A>::key_type,typename cXmlTree<A>::value_type,cXmlData>
{
    typedef typename container::cMultiMapIterable<typename cXmlTree<A>::key_type,typename cXmlTree<A>::value_type,cXmlData>::inner_value_type inner_value_type;

public:
	typedef typename cXmlTree<A>::MapNode MapNode;
    typedef typename container::cMultiMapIterable<typename cXmlTree<A>::key_type,typename cXmlTree<A>::value_type,cXmlData>::iterator_type iterator_type;
    typedef typename container::cMultiMapIterable<typename cXmlTree<A>::key_type,typename cXmlTree<A>::value_type,cXmlData>::const_iterator_type const_iterator_type;
    typedef typename container::cMultiMapIterable<typename cXmlTree<A>::key_type,typename cXmlTree<A>::value_type,cXmlData>::reverse_iterator_type reverse_iterator_type;
    typedef typename container::cMultiMapIterable<typename cXmlTree<A>::key_type,typename cXmlTree<A>::value_type,cXmlData>::const_reverse_iterator_type const_reverse_iterator_type;
	typedef typename container::cMultiMapIterable<typename cXmlTree<A>::key_type,typename cXmlTree<A>::value_type,cXmlData>::value_type value_type;
	typedef typename container::cMultiMapIterable<typename cXmlTree<A>::key_type,typename cXmlTree<A>::value_type,cXmlData>::reference reference;
	typedef typename container::cMultiMapIterable<typename cXmlTree<A>::key_type,typename cXmlTree<A>::value_type,cXmlData>::const_reference const_reference;

    using container::cMultiMapIterable<typename cXmlTree<A>::key_type,typename cXmlTree<A>::value_type,cXmlData>::begin;
    using container::cMultiMapIterable<typename cXmlTree<A>::key_type,typename cXmlTree<A>::value_type,cXmlData>::end;
    using container::cMultiMapIterable<typename cXmlTree<A>::key_type,typename cXmlTree<A>::value_type,cXmlData>::cbegin;
    using container::cMultiMapIterable<typename cXmlTree<A>::key_type,typename cXmlTree<A>::value_type,cXmlData>::cend;
    using container::cMultiMapIterable<typename cXmlTree<A>::key_type,typename cXmlTree<A>::value_type,cXmlData>::getSize;
    using container::cMultiMapIterable<typename cXmlTree<A>::key_type,typename cXmlTree<A>::value_type,cXmlData>::empty;

    cXmlIterable(const cXmlTree<A>& i_xmlTree);

private:
    static cXmlData fromXmlValueDataToXmlData(container::cPair<typename cXmlTree<A>::key_type,typename cXmlTree<A>::value_type>& i_val);
};

typedef container::multimap_view<cXmlData> xml_view;

}
}

#include "detail/cXmlIterable.tpp"

