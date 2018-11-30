#include "System/file/cFileStream.h"

namespace yame
{
namespace io
{

container::string FileStreamTraits::read(IFile& i_file, size_t i_pos, size_t i_size)
{
    return i_file.read(i_size);
}
size_t FileStreamTraits::write(IFile& i_file, const container::string& i_content, size_t i_pos)
{
    return i_file.write(i_content);
}

file_stream::file_stream(IFile& i_file)
: m_file(i_file)
{
}
bool file_stream::open(const IFileFlags& i_openFlags)
{
    m_flags = i_openFlags | IFileFlag::MemoryMapped;

    m_file.open(m_flags);
}
void file_stream::close()
{
    m_file.close();
}
IFileFlags file_stream::getFlags() const
{
    return m_flags;
}
container::string file_stream::get_line()
{
    return m_file.read_until('\n');
}
IFile& file_stream::get_input_data()
{
    return m_file;
}
const IFile& file_stream::get_input_data() const
{
    return m_file;
}
IFile& file_stream::get_output_data()
{
    return m_file;
}
const IFile& file_stream::get_output_data() const
{
    return m_file;
}

}
}
