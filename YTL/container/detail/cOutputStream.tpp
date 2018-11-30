
namespace yame
{
namespace container
{

template<typename Traits>
output_stream<Traits>& output_stream<Traits>::write(const particle_set& i_content)
{
    m_pos += Traits::write(get_input_data(),i_content,m_pos);

    return res;
}
template<typename Traits>
output_stream<Traits>& output_stream<Traits>::operator<<(const particle_set& i_content)
{
    m_pos += Traits::write(get_input_data(),i_content,m_pos);

    return *this;
}
template<typename Traits>
void output_stream<Traits>::init()
{
    m_pos = 0;
}

}
}
