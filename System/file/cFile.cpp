#include "cFile.h"
#include <cstdio>

namespace yame
{
namespace io
{

file::file(int i_fileDescriptor, const FileFlags& i_mode)
{
    container::string modeStr = fromFlagToString(i_mode);

    m_file = fdopen(i_fileDescriptor,modeStr.getStr());
}
file::file(const container::string& i_fileName, const FileFlags& i_mode)
{
    container::string modeStr = fromFlagToString(i_mode);

    m_file = fopen(i_fileName.getStr(),modeStr.getStr());
}
file::file(file&& other)
{
    m_file = other.m_file;
    other.m_file = NULL;
}
file::~file()
{
    if(m_file)
    {
        fclose(m_file);
        m_file = NULL;
    }
}
container::string file::read(size_t i_numChars)
{
    container::string res;

    res.resize(i_numChars);

    size_t numReadChars = fread(res.getStr(),sizeof(char),i_numChars,m_file);

    if(numReadChars != i_numChars)
    {
        res.resize((numReadChars != -1) ? numReadChars : 0);
    }

    return res;
}
size_t file::write(const container::string& i_content)
{
    return fwrite(i_content.getStr(),sizeof(char),i_content.getSize(),m_file);
}
size_t file::seek(size_t i_pos)
{
    fseek(m_file,i_pos,0);
}
file& file::operator=(file&& other)
{
    if(m_file)
    {
        fclose(m_file);
    }

    m_file = other.m_file;
    other.m_file = NULL;

    return *this;
}
container::string file::fromFlagToString(const FileFlags& i_mode) const
{


    return "r";
}

}
}
