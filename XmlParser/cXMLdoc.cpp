#include "cXMLdoc.h"


namespace yame
{
namespace xml
{

cXMLdoc::cXMLdoc()
: m_pCurrTree(NULL)
{
    //by default
    m_header = "version=\"1.0\" encoding=\"UTF-8\"";
}
cXMLdoc::~cXMLdoc()
{
}
void cXMLdoc::setHeader(const container::string& header)
{
    m_header = header;
}
void cXMLdoc::addInfo(const container::string& info)
{
    container::cArray<cProperty> attr;

    //internal trick for handling infos
    const size_t mapSize = (m_pCurrTree) ? (*m_pCurrTree).getSize() : m_xmlTree.getSize();
    container::string innerTag = container::string::Format("__info%d",mapSize);

    cXmlValue<>& currValue = (m_pCurrTree) ? (*m_pCurrTree)[innerTag] : m_xmlTree[innerTag];

    currValue = cXmlValue<>(innerTag,attr);
    currValue.append(info);

    return;
}
const container::string& cXMLdoc::getHeader() const
{
    return m_header;
}
void cXMLdoc::startTag(const container::string& tag, const container::cArray<cProperty>& attr)
{
    container::string localTag = tag;

    //add internal counter (to be removed in writing step)
    const size_t mapSize = (m_pCurrTree) ? (*m_pCurrTree).getSize() : m_xmlTree.getSize();
    cXmlValue<>& currValue = (m_pCurrTree) ? (*m_pCurrTree)[localTag] : m_xmlTree[localTag];

    localTag += container::string::Format("__%d",mapSize);

    currValue = cXmlValue<>(tag,attr);
    m_pCurrTree = &currValue;

    m_currTag = tag;

    return;
}
void cXMLdoc::endTag()
{
    if(cXmlValue<> *pCurrNode = static_cast<cXmlValue<> *>(m_pCurrTree))
    {
        if(m_pCurrTree = pCurrNode->getHolder())
        {
            cXmlValue<> *pParentNode = static_cast<cXmlValue<> *>(m_pCurrTree);
            m_currTag = pParentNode->getTag();
        }
        else
        {
            m_currTag.clear();
        }
    }
    else
    {
        m_currTag.clear();
    }
}
void cXMLdoc::addContent(const container::string& content)
{
    cXmlValue<> *pCurrNode = static_cast<cXmlValue<> *>(m_pCurrTree);

    ASSERT(pCurrNode, "Null parent tree node!");

    pCurrNode->append(content);
}
cXmlTree<>& cXMLdoc::getXmlTree()
{
    return m_xmlTree;
}
const container::string& cXMLdoc::getCurrentTag() const
{
    return m_currTag;
}
const container::string& cXMLdoc::getCurrentContent() const
{
    cXmlValue<> *pCurrNode = static_cast<cXmlValue<> *>(m_pCurrTree);

    ASSERT(pCurrNode, "Null parent tree node!");

    return pCurrNode->getContent();
}
const container::cArray<cProperty>& cXMLdoc::getCurrentProperties() const
{
    cXmlValue<> *pCurrNode = static_cast<cXmlValue<> *>(m_pCurrTree);

    ASSERT(pCurrNode, "Null parent tree node!");

    return pCurrNode->getProperties();
}

}
}
