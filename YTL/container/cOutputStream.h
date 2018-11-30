#pragma once

namespace yame
{
namespace container
{

template<typename Traits>
class output_stream
{
    typedef typename Traits::source source;
    static const size_t npos = -1;

public:
    typedef typename Traits::particle particle;
    typedef typename Traits::particle_set particle_set;

    output_stream() = default;

    output_stream& write(const particle_set& i_content);
    output_stream& operator<<(const particle_set& i_content);

protected:
    void init();

private:
    virtual source& get_output_data() = 0;
    virtual const source& get_output_data() const = 0;

    size_t m_pos = npos;
};

}
}
