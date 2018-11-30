
namespace yame
{
namespace ytl
{

template<typename Enum, typename UnderlyingType>
flag_set<Enum,UnderlyingType>::flag_set()
{
    m_flags = static_cast<underlying_type>(0);
}
template<typename Enum, typename UnderlyingType>
flag_set<Enum,UnderlyingType>::flag_set(const Enum& i_flag)
{
    m_flags = 1 << static_cast<underlying_type>(i_flag);
}
template<typename Enum, typename UnderlyingType>
flag_set<Enum,UnderlyingType>::flag_set(const underlying_type& i_flags)
: m_flags (i_flags)
{
}
template<typename Enum, typename UnderlyingType>
flag_set<Enum,UnderlyingType> flag_set<Enum,UnderlyingType>::operator|(const Enum& i_flag) const
{
    flag_set<Enum,UnderlyingType> res(m_flags | (1 << static_cast<underlying_type>(i_flag)));

    return res;
}
template<typename Enum, typename UnderlyingType>
flag_set<Enum,UnderlyingType>& flag_set<Enum,UnderlyingType>::operator|=(const Enum& i_flag)
{
    m_flags |= 1 << static_cast<underlying_type>(i_flag);

    return *this;
}
template<typename Enum, typename UnderlyingType>
flag_set<Enum,UnderlyingType> flag_set<Enum,UnderlyingType>::operator&(const Enum& i_flag) const
{
    flag_set<Enum,UnderlyingType> res(m_flags & (1 << static_cast<underlying_type>(i_flag)));

    return res;
}
template<typename Enum, typename UnderlyingType>
flag_set<Enum,UnderlyingType>& flag_set<Enum,UnderlyingType>::operator&=(const Enum& i_flag)
{
    m_flags &= 1 << static_cast<underlying_type>(i_flag);

    return *this;
}
template<typename Enum, typename UnderlyingType>
flag_set<Enum,UnderlyingType> flag_set<Enum,UnderlyingType>::operator^(const Enum& i_flag) const
{
    flag_set<Enum,UnderlyingType> res(m_flags ^ (1 << static_cast<underlying_type>(i_flag)));

    return res;
}
template<typename Enum, typename UnderlyingType>
flag_set<Enum,UnderlyingType>& flag_set<Enum,UnderlyingType>::operator^=(const Enum& i_flag)
{
    m_flags ^= 1 << static_cast<underlying_type>(i_flag);

    return *this;
}
template<typename Enum, typename UnderlyingType>
flag_set<Enum,UnderlyingType> flag_set<Enum,UnderlyingType>::operator|(const flag_set& other) const
{
    flag_set<Enum,UnderlyingType> res(m_flags | other.m_flags);

    return res;
}
template<typename Enum, typename UnderlyingType>
flag_set<Enum,UnderlyingType>& flag_set<Enum,UnderlyingType>::operator|=(const flag_set& other)
{
    m_flags |= other.m_flags;

    return *this;
}
template<typename Enum, typename UnderlyingType>
flag_set<Enum,UnderlyingType> flag_set<Enum,UnderlyingType>::operator&(const flag_set& other) const
{
    flag_set<Enum,UnderlyingType> res(m_flags & other.m_flags);

    return res;
}
template<typename Enum, typename UnderlyingType>
flag_set<Enum,UnderlyingType>& flag_set<Enum,UnderlyingType>::operator&=(const flag_set& other)
{
    m_flags &= other.m_flags;

    return *this;
}
template<typename Enum, typename UnderlyingType>
flag_set<Enum,UnderlyingType> flag_set<Enum,UnderlyingType>::operator^(const flag_set& other) const
{
    flag_set<Enum,UnderlyingType> res(m_flags ^ other.m_flags);

    return res;
}
template<typename Enum, typename UnderlyingType>
flag_set<Enum,UnderlyingType>& flag_set<Enum,UnderlyingType>::operator^=(const flag_set& other)
{
    m_flags ^= other.m_flags;

    return *this;
}
template<typename Enum, typename UnderlyingType>
flag_set<Enum,UnderlyingType> flag_set<Enum,UnderlyingType>::operator!()
{
    return !m_flags;
}
template<typename Enum, typename UnderlyingType>
bool flag_set<Enum,UnderlyingType>::operator==(const flag_set& other) const
{
    return m_flags == other.m_flags;
}
template<typename Enum, typename UnderlyingType>
bool flag_set<Enum,UnderlyingType>::operator!=(const flag_set& other) const
{
    return m_flags != other.m_flags;
}
template<typename Enum, typename UnderlyingType>
void flag_set<Enum,UnderlyingType>::set(const Enum& i_flag)
{
    m_flags |= 1 << static_cast<underlying_type>(i_flag);
}
template<typename Enum, typename UnderlyingType>
bool flag_set<Enum,UnderlyingType>::test(const Enum& i_flag) const
{
    bool res = (m_flags & (1 << static_cast<underlying_type>(i_flag))) != 0;

    return res;
}
template<typename Enum, typename UnderlyingType>
bool flag_set<Enum,UnderlyingType>::empty() const
{
    return m_flags == static_cast<underlying_type>(0);
}
template<typename Enum, typename UnderlyingType>
typename flag_set<Enum,UnderlyingType>::underlying_type flag_set<Enum,UnderlyingType>::getValue() const
{
    return m_flags;
}

}
}
