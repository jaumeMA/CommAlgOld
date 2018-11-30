#pragma once

#include "System/file/cFileStream.h"

namespace yame
{
namespace io
{

class file_istream : protected file_stream
{
public:
    using file_stream::file_stream;
    using file_stream::read;
    using file_stream::operator>>;
};

}
}
