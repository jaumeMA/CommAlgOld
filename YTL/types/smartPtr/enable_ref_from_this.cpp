
#include "YTL/types/smartPtr/enable_ref_from_this.h"

namespace yame
{
namespace ytl
{

unique_reference_counter* enable_ref_from_this::getRefCounter()
{
    return &m_counter;
}
const unique_reference_counter* enable_ref_from_this::getRefCounter() const
{
    return &m_counter;
}
enable_ref_from_this::enable_ref_from_this()
{
}
enable_ref_from_this::enable_ref_from_this(const enable_ref_from_this& other)
{
}
enable_ref_from_this::~enable_ref_from_this()
{
}
enable_ref_from_this& enable_ref_from_this::operator=(const enable_ref_from_this& other)
{
    return *this;
}
enable_ref_from_this& enable_ref_from_this::ref_from_this()
{
    return *this;
}
const enable_ref_from_this& enable_ref_from_this::ref_from_this() const
{
    return *this;
}

}
}

