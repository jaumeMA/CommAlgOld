#pragma once

#include "Utils/rtti/TypeInfo.h"
#include "YTL/container/cUnorderedMap.h"
#include "YTL/optional/optional.h"

namespace yame
{
namespace rtti
{

class TypeTable
{
public:
    typedef container::cUnorderedMap<TypeInfo,TypeId>::iterator_type iterator_type;
    typedef container::cUnorderedMap<TypeInfo,TypeId>::const_iterator_type const_iterator_type;

    TypeTable() = default;
    template<typename T>
    static size_t addType();
    template<typename T,typename TT>
    static bool addTypeBinding();
    static ytl::optional<std::ptrdiff_t> relate(const TypeId& i_lhs, const TypeId& i_rhs);

private:
    static container::cUnorderedMap<TypeInfo,TypeId> m_typeTable;
};


}
}

#include "Utils/rtti/detail/TypeTable.tpp"
