
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
#include "cXMLdoc.h"
#include "cDefaultHandler.h"

#define MAX_CHARS_PER_LINE 256

extern int xml_parse();

namespace yame
{
namespace xml
{

enum class ParserState
{
    EPS_NONE,
    EPS_VERSION,
    EPS_INFO,
    EPS_START,
    EPS_CONTENT,
    EPS_STOP,
    EPS_END
};

class cXMLreader
{
    friend int xml_parse();

public:
    //try to figure out how to call private methods from ypp file
    void _version(const container::string& version);
    void _version(const container::string& version, const container::string& encoding);
    void _version(const container::string& version, const container::string& encoding, const container::string& standalone);
    void _info(const container::string& node);
    void _startTag(const container::string& tag, const container::cArray<cProperty>& properties);
    void _contentTag(const container::string& node);
    void _stopTag(const container::string& node);

    cXMLreader(const container::string& xmlFile, cDefaultHandler *handler=NULL);
    ~cXMLreader();
    cXMLdoc& parse();

private:
    void Init();
    void deInit();

    container::string   m_filename;
    cXMLdoc             m_xmlDoc;
    FILE*               m_fp;
    bool                m_initialized;
    bool                m_opened;
    cDefaultHandler*    m_pHandler;
    ParserState         m_state;
    container::string   m_currTag;
};

}
}
