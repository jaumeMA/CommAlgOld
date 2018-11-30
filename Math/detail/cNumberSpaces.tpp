
namespace yame
{

template<int p>
container::string format(const math::Zp<p>& i_value)
{
    return container::string::Format("%d", i_value.get_raw());
}
}
