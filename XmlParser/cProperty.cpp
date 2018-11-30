#include "cProperty.h"

namespace yame
{
namespace xml
{

cProperty::cProperty()
{
}
cProperty::cProperty(const container::string& id, const container::string& val)
{
    m_id = id;
    m_val = val;
}
bool cProperty::operator==(const cProperty& other) const
{
    return m_id==other.m_id && m_val==other.m_val;
}

}
}
