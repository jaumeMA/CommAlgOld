#pragma once

#include "YTL/container/cFlagSet.h"

namespace yame
{
namespace io
{

enum class FileFlag
{
    Read = 0,
    Write,
    Append,
    Create
};

typedef ::yame::ytl::flag_set<FileFlag> FileFlags;

}
}
