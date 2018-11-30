#pragma once

#include "rttiDefs.h"
#include "YTL/container/cArray.h"
#include "YTL/container/cString.h"
#include "YTL/container/cPair.h"
#include "YTL/container/cIterator.h"

namespace yame
{
namespace rtti
{

class TypeInfo;

}

//JAUME: there is a known issue about expansion templates and copy constructibility
namespace mpl
{
template<>
struct is_copy_constructible<rtti::TypeInfo>
{
static const bool value = true;
};
}

namespace rtti
{

class TypeInfo
{
    typedef container::cArray<const TypeInfo*> base_container;
    typedef container::cArray<container::cPair<container::string,const TypeInfo*>> member_container;
    typedef yame::container::cConstRandomAccessIntraIterable<const TypeInfo*&,const TypeInfo&> const_base_iterable;
    typedef yame::container::cConstRandomAccessIntraIterable<container::cPair<container::string,const TypeInfo*>&,container::cPair<container::string,const TypeInfo&>> const_member_iterable;

public:
    typedef yame::container::const_array_view<const TypeInfo&> const_base_view;
    typedef yame::container::cConstRandomAccessView<container::cPair<container::string,const TypeInfo&>> const_member_view;
    typedef typename const_base_view::const_iterator_type base_iterator;
    typedef typename const_member_view::const_iterator_type member_iterator;


    TypeInfo(const container::string& i_name, TypeId i_id, const container::cArray<const TypeInfo*>& i_bases, const yame::container::cArray<yame::container::cPair<yame::container::string,const TypeInfo*>>& i_members);
    TypeInfo(const TypeInfo& other);
    TypeInfo(TypeInfo&& other);

    container::const_string_view getName() const;
    TypeId getId() const;
    const_base_view getBases() const;
    const_member_view getMembers() const;

    TypeInfo& operator=(const TypeInfo& other);
    TypeInfo& operator=(TypeInfo&& other);

private:
    container::string m_name;
    TypeId m_id;
    container::string m_typeName;
    base_container m_bases;
    const_base_iterable m_baseIterable;
    member_container m_members;
    const_member_iterable m_memberIterable;
};

}

container::string format(const rtti::TypeInfo& i_poly);

}
