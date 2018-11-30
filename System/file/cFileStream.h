#pragma once

#include "YTL/container/cStream.h"
#include "YTL/container/cString.h"
#include "System/file/IFile.h"

namespace yame
{
namespace io
{

struct FileStreamTraits
{
    typedef char particle;
    typedef container::string particle_set;
    typedef IFile source;

    inline static container::string read(IFile& i_file, size_t i_pos, size_t i_size);
    inline static size_t write(IFile& i_file, const container::string& i_content, size_t i_pos);
};

class file_stream : public container::stream<FileStreamTraits>
{
    typedef container::stream<FileStreamTraits> base;

public:
    file_stream(IFile& i_file);
    bool open(const IFileFlags& i_openFlags);
    void close();
    container::string get_line();
    IFileFlags getFlags() const;

private:
    IFile& get_input_data() override;
    const IFile& get_input_data() const override;
    IFile& get_output_data() override;
    const IFile& get_output_data() const override;

    IFile& m_file;
    IFileFlags m_flags;
};

}
}
