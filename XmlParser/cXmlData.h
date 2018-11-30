#pragma once

#include "YTL/container/cString.h"
#include "XmlParser/cProperty.h"

namespace yame
{
namespace xml
{

class cXmlData
{
public:
    cXmlData() = default;
    cXmlData(const container::string& i_tag, const container::cArray<xml::cProperty>& i_props, const container::string& i_content, size_t i_depth);

    container::const_string_view getTag() const;
    container::const_array_view<xml::cProperty> getProperties() const;
    container::const_string_view getContents() const;
    size_t getDepth() const;

private:
    mutable container::string m_tag;
    mutable container::cArray<xml::cProperty> m_properties;
    mutable container::string m_content;
    size_t m_depth;
};

}
}
