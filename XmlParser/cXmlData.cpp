#include "cXmlData.h"

namespace yame
{
namespace xml
{

cXmlData::cXmlData(const container::string& i_tag, const container::cArray<xml::cProperty>& i_props, const container::string& i_content, size_t i_depth)
: m_tag(i_tag)
, m_properties(i_props)
, m_content(i_content)
, m_depth(i_depth)
{
}
container::const_string_view cXmlData::getTag() const
{
    return m_tag;
}
container::const_array_view<xml::cProperty> cXmlData::getProperties() const
{
    return m_properties;
}
container::const_string_view cXmlData::getContents() const
{
    return m_content;
}
size_t cXmlData::getDepth() const
{
    return m_depth;
}

}
}
