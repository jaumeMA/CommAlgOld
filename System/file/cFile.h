#pragma once

#include "YTL/container/cString.h"
#include "System/file/cFileDefs.h"

namespace yame
{
namespace io
{

class file
{
public:
    file(const container::string& i_fileName, const FileFlags& i_flags);
    file(int i_fileDescriptor, const FileFlags& i_mode);
    file(const file&) = delete;
    file(file&& other);
    ~file();
    container::string read(size_t i_numChars);
    size_t write(const container::string& i_content);
    size_t seek(size_t i_pos);
    file& operator=(const file&) = delete;
    file& operator=(file&& other);

private:
    container::string fromFlagToString(const FileFlags& i_mode) const;

    FILE* m_file;
};

}
}
