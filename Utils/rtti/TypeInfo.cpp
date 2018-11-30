#include "Utils/rtti/TypeInfo.h"

namespace yame
{
namespace rtti
{

TypeInfo::TypeInfo(const container::string& i_name, TypeId i_id, const container::cArray<const TypeInfo*>& i_bases, const yame::container::cArray<yame::container::cPair<yame::container::string,const TypeInfo*>>& i_members)
: m_name(i_name)
, m_id(i_id)
, m_bases(i_bases)
, m_baseIterable(m_bases, [](const TypeInfo* i_typeInfo) -> const TypeInfo& { return *i_typeInfo; })
, m_members(i_members)
, m_memberIterable(m_members, [](const container::cPair<container::string,const TypeInfo*>& i_pairView) -> container::cPair<container::string,const TypeInfo&> { return container::make_pair<container::string,const TypeInfo&>(i_pairView.first,*i_pairView.second); })
{
}
TypeInfo::TypeInfo(const TypeInfo& other)
: m_name(other.m_name)
, m_id(other.m_id)
, m_bases(other.m_bases)
, m_baseIterable(m_bases, [](const TypeInfo* i_typeInfo) -> const TypeInfo& { return *i_typeInfo; })
, m_members(other.m_members)
, m_memberIterable(m_members, [](const container::cPair<container::string,const TypeInfo*>& i_pairView) -> container::cPair<container::string,const TypeInfo&> { return container::make_pair<container::string,const TypeInfo&>(i_pairView.first,*i_pairView.second); })
{
}
TypeInfo::TypeInfo(TypeInfo&& other)
: m_name(mpl::move(other.m_name))
, m_id(mpl::move(other.m_id))
, m_bases(mpl::move(other.m_bases))
, m_baseIterable(m_bases, [](const TypeInfo* i_typeInfo) -> const TypeInfo& { return *i_typeInfo; })
, m_members(mpl::move(other.m_members))
, m_memberIterable(m_members, [](const container::cPair<container::string,const TypeInfo*>& i_pairView) -> container::cPair<container::string,const TypeInfo&> { return container::make_pair<container::string,const TypeInfo&>(i_pairView.first,*i_pairView.second); })
{
}
container::const_string_view TypeInfo::getName() const
{
    return m_name;
}
TypeId TypeInfo::getId() const
{
    return m_id;
}
TypeInfo::const_base_view TypeInfo::getBases() const
{
    return m_baseIterable;
}
TypeInfo::const_member_view TypeInfo::getMembers() const
{
    return m_memberIterable;
}
TypeInfo& TypeInfo::operator=(const TypeInfo& other)
{
    m_name = other.m_name;
    m_id = other.m_id;
    m_bases = other.m_bases;
    m_members = other.m_members;

    return *this;
}
TypeInfo& TypeInfo::operator=(TypeInfo&& other)
{
    m_name = mpl::move(other.m_name);
    m_id = mpl::move(other.m_id);
    m_bases = mpl::move(other.m_bases);
    m_members = mpl::move(other.m_members);

    return *this;
}

}

container::string format(const rtti::TypeInfo& i_typeInfo)
{
    container::string res;

    res += i_typeInfo.getName();
    res += "\n";
    res += "Bases:\n";
    rtti::TypeInfo::const_base_view typeInfoBases = i_typeInfo.getBases();
    rtti::TypeInfo::base_iterator itBase = typeInfoBases.cbegin();
    for(;itBase!=typeInfoBases.cend();++itBase)
    {
        res += format(*itBase);
        res += "\n";
    }

    res += "Members:\n";
    rtti::TypeInfo::const_member_view typeInfoMembers = i_typeInfo.getMembers();
    rtti::TypeInfo::member_iterator itMember = typeInfoMembers.cbegin();
    for(;itMember!=typeInfoMembers.cend();++itMember)
    {
        res += "Name: " + (*itMember).first + ", \n";
        res += format((*itMember).second);
        res += "\n";
    }

    return res;
}

}
