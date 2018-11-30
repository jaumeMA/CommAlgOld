#pragma once

#include "YTL/container/cInputStream.h"
#include "YTL/container/cOutputStream.h"

namespace yame
{
namespace container
{

template<typename Traits>
class stream : public input_stream<Traits>, public output_stream<Traits>
{
    typedef typename Traits::source source;
    typedef input_stream<Traits> input_base;
    typedef output_stream<Traits> output_base;
};

}
}

#include "YTL/container/detail/cStream.tpp"
