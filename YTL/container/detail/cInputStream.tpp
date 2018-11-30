
namespace yame
{
namespace container
{

template<typename Traits>
input_stream<Traits>& input_stream<Traits>::read(size_t i_numChars)
{
    particle_set res = Traits::read(get_input_data(),m_pos,i_numChars);

    m_pos += res.getSize();

    return *this;
}
template<typename Traits>
input_stream<Traits>& input_stream<Traits>::operator>>(particle_set& o_res)
{
    o_res = Traits::read(get_input_data(),m_pos,o_res.getCapacity());

    m_pos += o_res.getSize();

    return *this;
}
template<typename Traits>
void input_stream<Traits>::init()
{
    m_pos = 0;
}

}
}
