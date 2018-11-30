#include "cXMLreader.h"
#include "container/cIterator.h"

extern void lexParse(yame::xml::cXMLreader *caller, FILE *in);

namespace yame
{
namespace xml
{

cXMLreader::cXMLreader(const container::string& filename, cDefaultHandler *handler)
: m_xmlDoc()
{
    m_filename = filename;
    m_initialized = false;
    m_opened = false;
    m_fp = NULL;
    m_state = ParserState::EPS_NONE;
    m_pHandler = handler;

    Init();
}
cXMLreader::~cXMLreader()
{
    deInit();
}
void cXMLreader::Init()
{
    m_fp = fopen(m_filename.getStr(),"r");

    m_initialized = (m_fp) ? true : false;
}
void cXMLreader::deInit()
{
    if(m_fp)
    {
        fclose(m_fp);
    }

}
cXMLdoc& cXMLreader::parse()
{
    //lex parse
    lexParse(this,m_fp);

    return m_xmlDoc;
}
void cXMLreader::_version(const container::string& version)
{
    container::string localVersion = container::string::Format("version=\"%s\"",version.getStr());

    //version case
    m_state = ParserState::EPS_VERSION;

    m_xmlDoc.setHeader(localVersion);

    return;
}
void cXMLreader::_version(const container::string& version, const container::string& encoding)
{
    container::string localVersion = container::string::Format("version=\"%s\" enconding=\"%s\"",version.getStr(),encoding.getStr());

    //version case
    m_state = ParserState::EPS_VERSION;

    m_xmlDoc.setHeader(localVersion);

    return;
}
void cXMLreader::_version(const container::string& version, const container::string& encoding, const container::string& standalone)
{
    container::string localVersion = container::string::Format("version=\"%s\" enconding=\"%s\" standalone=\"%s\"",
                                                            version.getStr(),encoding.getStr(),standalone.getStr());

    //version case
    m_state = ParserState::EPS_VERSION;

    m_xmlDoc.setHeader(localVersion);

    if(m_pHandler)
    {
        m_pHandler->VersionHandler(localVersion);
    }

    return;
}
void cXMLreader::_info(const container::string& node)
{
    if(node.getSize())
    {
        m_xmlDoc.addInfo(node);

        if(m_pHandler)
        {
            m_pHandler->InfoHandler(node);
        }
    }
}
void cXMLreader::_startTag(const container::string& tag, const container::cArray<cProperty>& properties)
{
    if(tag.getSize())
    {
        m_xmlDoc.startTag(tag,properties);
        //start case
        m_state =ParserState::EPS_START;

        if(m_pHandler)
        {
            m_pHandler->StartHandler(tag,properties);
        }
    }
    else
    {
        //raise error
    }
}
void cXMLreader::_contentTag(const container::string& node)
{
    //just in case, store any content before the <\ tag
    m_xmlDoc.addContent(node);

    m_state = ParserState::EPS_CONTENT;
}
void cXMLreader::_stopTag(const container::string& tag)
{
    if(tag == m_xmlDoc.getCurrentTag())
    {
        //stop case
        m_state = ParserState::EPS_STOP;

        if(m_pHandler)
        {
            //inform about content and end of tag
            m_pHandler->ContentHandler(m_xmlDoc.getCurrentContent());
            m_pHandler->EndHandler(tag);
        }

        m_xmlDoc.endTag();
    }
    else
    {
        container::string error = container::string::Format("Ep, hem intentat xapar %s quan hi havia %s\n",tag.getStr(),m_xmlDoc.getCurrentTag().getStr());
        //raise error
        m_pHandler->ErrorHanlder(error);
    }
}

}
}
