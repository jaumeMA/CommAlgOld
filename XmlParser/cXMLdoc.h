
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

#include "container/cString.h"
#include "cXmlTree.h"

namespace yame
{
namespace xml
{

class cXMLdoc
{
private:
    typedef cXmlValue<>::MapClass MapClass;

    container::string   m_header;
    cXmlTree<>          m_xmlTree;
    u16                 m_size;
    u16                 m_currDepth;
    container::string   m_currTag;
    MapClass*           m_pCurrTree;

public:
    cXMLdoc();
    ~cXMLdoc();
    void startTag(const container::string& tag, const container::cArray<cProperty>& attr);
    void endTag();
    void addContent(const container::string& content);
    void setHeader(const container::string& header);
    void addInfo(const container::string& info);
    const container::string& getHeader() const;
    cXmlTree<>& getXmlTree();
    const container::string& getCurrentTag() const;
    const container::string& getCurrentContent() const;
    const container::cArray<cProperty>& getCurrentProperties() const;
};

}
}
