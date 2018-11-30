#include "cXMLwriter.h"
#include "XmlParser/cXmlIterable.h"

namespace yame
{
namespace xml
{

cXMLwriter::cXMLwriter(const cXMLdoc& xmlDoc)
: m_xmlDoc(xmlDoc)
{

}
void cXMLwriter::operator >>(io::file_stream& fileStreamer)
{
      //first of all print the verion
    container::string str = "<?xml ";

    str += m_xmlDoc.getHeader() + "?>";

    fileStreamer << str;

      //then the rest of the document
    writeFormattedOutput(fileStreamer,m_xmlDoc.getXmlTree());

    return;
}

void cXMLwriter::writeFormattedOutput(io::file_stream& fileStreamer, cXmlTree<>& xmlTree) const
{
    container::string str;
    cXmlIterable<> xmlIterable(xmlTree);

    if(!xmlIterable.empty())
    {
        fileStreamer << "\n";

        auto preffixConstructor = [](size_t i_depth) -> container::string
        {
            container::string res;

            for(size_t depthIndex=0;depthIndex<i_depth;++depthIndex)
            {
                res += '\t';
            }

            return res;
        };

        cXmlIterable<>::const_iterator_type itXml = xmlIterable.cbegin();
        for(;itXml!=xmlIterable.cend();++itXml)
        {
            char tagStr[256];
            container::string tag;
            u32 internalCounter = 0;
            memset(tagStr,0,sizeof(c8)*256);

            const cXmlData currXmlNode = *itXml;
            const container::string preffix = preffixConstructor(currXmlNode.getDepth());

            //remove internal counter
            const container::const_string_view currNodeTag = currXmlNode.getTag();

            if(currNodeTag.subView(0,6) == "__info")
            {
                fileStreamer << preffix + "<!--" + currXmlNode.getContents() + "-->\n";
            }
            else
            {
//                container::Scan(currNodeTag,"%s__%d",tagStr,&internalCounter);
                tag = tagStr;
                fileStreamer << preffix + "<" + tag;

                container::const_array_view<xml::cProperty> properties = currXmlNode.getProperties();

                container::const_array_view<xml::cProperty>::const_iterator_type itProperty = properties.cbegin();
                for(;itProperty<properties.cend();++itProperty)
                {
                    //recover quotation marks for properties
                    fileStreamer << " " + (*itProperty).m_id + "=\"" + (*itProperty).m_val + "\"";
                }

                container::const_string_view content = currXmlNode.getContents();

                if(content.empty())
                {
                    fileStreamer << "/>\n";
                }
                else
                {
                    fileStreamer << ">" + content;

                    fileStreamer << preffix + "</" + tag + ">\n";
                }
            }
        }
    }

    return;
}

}
}
