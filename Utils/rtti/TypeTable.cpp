#include "TypeTable.h"

namespace yame
{
namespace rtti
{

ytl::optional<std::ptrdiff_t> TypeTable::relate(const TypeId& i_lhs, const TypeId& i_rhs)
{
    container::cUnorderedMap<TypeInfo,TypeId>::const_iterator_type itTable = m_typeTable.search(i_lhs);

    if(itTable != m_typeTable.end())
    {
        const TypeInfo& currTypeInfo = itTable->second;

        const TypeInfo::iterable_interface& bases = currTypeInfo.getBases();

        TypeInfo::const_iterator_type itTypeInfo = yame::find(bases.begin(),bases.end(),i_rhs);

        if(itTypeInfo != bases.end())
        {
            return itTypeInfo->offset;
        }
        else
        {
            return ytl::none;
        }
    }
    else
    {
        return ytl::none;
    }
}

}
}
