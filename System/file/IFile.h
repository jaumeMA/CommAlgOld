#pragma once

#include "YTL/container/cFlagSet.h"
#include "Utils/rtti/rtti.h"
#include "Utils/cInterfaceAccess.h"

namespace yame
{
namespace io
{

enum class IFileFlag
{
    MemoryMapped,
    Read,
    Write
};

typedef ytl::flag_set<IFileFlag> IFileFlags;

namespace detail
{

class IFileAccess
{
    virtual container::string read(size_t i_numChars) = 0;
    virtual container::string read_until(const char& i_delimiter) = 0;
    virtual size_t write(const container::string& i_content) = 0;
    virtual void seek(size_t i_pos) = 0;
};

}

class IFile /*: public utils::cInterfaceAccess<detail::IFileAccess>*/
{
    PUBLISH_RTTI_BASE_TYPE(IFile)

public:
    virtual ~IFile() = default;
    virtual void open(const IFileFlags&) = 0;
    virtual void close() = 0;
    virtual bool is_open() const = 0;
    virtual container::string read(size_t i_numChars) = 0;
    virtual container::string read_until(const char& i_delimiter) = 0;
    virtual size_t write(const container::string& i_content) = 0;
    virtual void seek(size_t i_pos) = 0;
};

}
}
