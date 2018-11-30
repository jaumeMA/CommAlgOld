#pragma once

#include "YTL/container/cArray.h"

namespace yame
{
namespace container
{

template<typename Traits>
class input_stream
{
    typedef typename Traits::source source;
    static const size_t npos = -1;

public:
    typedef typename Traits::particle particle;
    typedef typename Traits::particle_set particle_set;

    input_stream() = default;

    input_stream& read(size_t i_numChars);
    input_stream& operator>>(particle_set& o_res);

protected:
    void init();

private:
    virtual source& get_input_data() = 0;
    virtual const source& get_input_data() const = 0;

    size_t m_pos = npos;
};

}
}

#include "YTL/container/detail/cInputStream.tpp"
