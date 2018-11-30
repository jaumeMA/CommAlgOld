#include "cMappedFile.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

namespace yame
{
namespace io
{

const size_t mapped_file::s_pageSize = sysconf(_SC_PAGE_SIZE);

mapped_file::mapped_file(int i_fileDescriptor, const FileFlags& i_flags, size_t i_numPagesPerLoad)
: m_fileAddr(NULL)
, m_localPos(0)
, m_globalPos(0)
, m_pagesPerLoad(i_numPagesPerLoad)
, m_flags(i_flags)
, m_fileDescriptor(-1)
{
    ASSERT(m_pagesPerLoad > 0, "You shall provide a non zero value for pages to load");

    container::string flagStr = fromFlagsToFlagStr();

    if(FILE* filePtr = fdopen(i_fileDescriptor,flagStr.getStr()))
    {
        m_fileDescriptor = fileno(filePtr);
    }

    m_fileSize = LoadFileAtOffset(0);
}
mapped_file::mapped_file(const container::string& i_fileName, const FileFlags& i_flags, size_t i_numPagesPerLoad)
: m_fileAddr(NULL)
, m_localPos(0)
, m_globalPos(0)
, m_pagesPerLoad(i_numPagesPerLoad)
, m_flags(i_flags)
, m_fileDescriptor(-1)
{
    ASSERT(m_pagesPerLoad > 0, "You shall provide a non zero value for pages to load");

    m_fileDescriptor = open(i_fileName.getStr(),fromFlagsToFdFlags());

    m_fileSize = LoadFileAtOffset(0);
}
mapped_file::mapped_file(mapped_file&& other)
: m_fileAddr(mpl::move(other.m_fileAddr))
, m_fileDescriptor(other.m_fileDescriptor)
, m_localPos(mpl::move(other.m_localPos))
, m_globalPos(mpl::move(other.m_globalPos))
, m_pagesPerLoad(mpl::move(other.m_pagesPerLoad))
, m_flags(mpl::move(other.m_flags))
{
    other.m_fileAddr = NULL;
    other.m_localPos = 0;
    other.m_globalPos = 0;
}
mapped_file::~mapped_file()
{
    if(m_fileAddr)
    {
        munmap(m_fileAddr,m_pagesPerLoad);
    }
}
mapped_file& mapped_file::operator=(mapped_file&& other)
{
    if(m_fileAddr)
    {
        munmap(m_fileAddr,m_pagesPerLoad);
    }

    m_fileAddr = mpl::move(other.m_fileAddr);
    m_fileDescriptor = other.m_fileDescriptor;
    m_localPos = other.m_localPos;
    m_globalPos = other.m_globalPos;
    m_pagesPerLoad = other.m_pagesPerLoad;
    m_flags = other.m_flags;

    other.m_fileAddr = NULL;
    other.m_localPos = 0;
    other.m_globalPos = 0;

    return *this;
}
size_t mapped_file::LoadFileAtOffset(size_t i_offset)
{
    if(m_fileDescriptor != -1)
    {
        struct stat sb;

        if(fstat(m_fileDescriptor,&sb) != -1)
        {
            if(S_ISREG (sb.st_mode))
            {
                m_localPos = 0;

                if(m_fileAddr = static_cast<char *>(mmap(m_fileAddr,m_pagesPerLoad * s_pageSize,fromFlagsToMMapFlags(),MAP_SHARED,m_fileDescriptor,i_offset)))
                {
                    return sb.st_size;
                }
            }
            else
            {
                FAIL("Your filename does not belong to a regular file");
            }
        }

        close (m_fileDescriptor);
    }
    else
    {
        FAIL("Filename not found");
    }

    return 0;
}
container::string mapped_file::read(size_t i_numChars)
{
    const size_t loadFileSize = m_pagesPerLoad * s_pageSize;
    container::string res;
    size_t firstPart = 0;

    do
    {
        firstPart = (m_localPos + i_numChars > loadFileSize) ? loadFileSize - m_localPos : i_numChars;
        res += container::string(m_fileAddr[m_localPos],firstPart);
        m_localPos += firstPart;
        i_numChars -= firstPart;
        LoadFileAtOffset(m_localPos);
        m_globalPos += firstPart;
    }
    while(firstPart < i_numChars);

    return res;
}
size_t mapped_file::write(const container::string& i_content)
{
    const size_t loadFileSize = m_pagesPerLoad * s_pageSize;
    size_t writtenChars = 0;
    size_t numChars = i_content.getSize();
    size_t firstPart = 0;

    do
    {
        firstPart = (m_localPos + numChars > loadFileSize) ? loadFileSize - m_localPos : numChars;
        memcpy(m_fileAddr,&i_content[m_localPos],firstPart);
        m_localPos += firstPart;
        numChars -= firstPart;
        LoadFileAtOffset(m_localPos);
        m_globalPos += numChars;
        writtenChars += numChars;
    }
    while(firstPart < numChars);

    return writtenChars;
}
size_t mapped_file::seek(size_t i_pos)
{
    LoadFileAtOffset(i_pos);

    m_globalPos = i_pos;

    return m_globalPos;
}
int mapped_file::fromFlagsToFdFlags() const
{
    if(m_flags.test(FileFlag::Read))
    {
        return O_RDONLY;
    }
    else if(m_flags.test(FileFlag::Write))
    {
        return O_WRONLY;
    }
    else if(m_flags.test(FileFlag::Append))
    {
        return O_APPEND;
    }
    else if(m_flags.test(FileFlag::Create))
    {
        return O_CREAT;
    }
    else
    {
        return 0;
    }
}
int mapped_file::fromFlagsToMMapFlags() const
{
    int res = 0;

    if(m_flags.test(FileFlag::Read))
    {
        res |= PROT_READ;
    }

    if(m_flags.test(FileFlag::Write))
    {
        res |= PROT_WRITE;
    }

    return res;
}
container::string mapped_file::fromFlagsToFlagStr() const
{
    if(m_flags.test(FileFlag::Read))
    {
        if(m_flags.test(FileFlag::Write))
        {
            if(m_flags.test(FileFlag::Create))
            {
                return "w+";
            }
            else
            {
                return "r+";
            }
        }
        else if(m_flags.test(FileFlag::Append))
        {
            return "a+";
        }
        else
        {
            return "r";
        }
    }
    else if(m_flags.test(FileFlag::Append))
    {
        return "a";
    }
    else
    {
        return "";
    }
}

}
}
