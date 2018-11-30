#pragma once

#include "YTL/container/cString.h"
#include "System/file/cFileDefs.h"
#include <sys/mman.h>

namespace yame
{
namespace io
{

class mapped_file
{
public:
    mapped_file(const container::string& i_fileName, const FileFlags& i_flags, size_t i_numPagesPerLoad = 1);
    mapped_file(int i_fileDescriptor, const FileFlags& i_flags, size_t i_numPagesPerLoad = 1);
    mapped_file(const mapped_file&) = delete;
    mapped_file(mapped_file&& other);
    ~mapped_file();
    container::string read(size_t i_numChars);
    size_t write(const container::string& i_content);
    size_t seek(size_t i_pos);
    mapped_file& operator=(const mapped_file&) = delete;
    mapped_file& operator=(mapped_file&& other);

private:
    size_t LoadFileAtOffset(size_t i_offset);
    int fromFlagsToFdFlags() const;
    int fromFlagsToMMapFlags() const;
    container::string fromFlagsToFlagStr() const;

    static const size_t s_pageSize;

    int m_fileDescriptor;
    char* m_fileAddr = NULL;
    size_t m_pagesPerLoad = 0;
    size_t m_localPos = 0;
    size_t m_globalPos = 0;
    size_t m_fileSize = 0;
    FileFlags m_flags;
};

}
}
