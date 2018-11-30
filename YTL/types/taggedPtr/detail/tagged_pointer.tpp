
#include "System/cException.h"

#define TAG_POINTER(_PTR,_TAG) reinterpret_cast<size_t&>(_PTR) |= _TAG;
#define TAGGED_POINTER(_PTR,_TAG) (reinterpret_cast<size_t>(_PTR) & _TAG)

namespace yame
{
namespace ytl
{

template<typename T>
tagged_pointer<T>::tagged_pointer()
: m_data(NULL)
{
}
template<typename T>
tagged_pointer<T>::tagged_pointer(T* i_data)
: m_data(i_data)
{
}
template<typename T>
tagged_pointer<T>::tagged_pointer(T* i_data, short i_tag)
: m_data(i_data)
{
    ASSERT(i_tag <= tag_mask, "You shall provide a tag less than tag mask");

    TAG_POINTER(m_data,i_tag);
}
template<typename T>
template<typename TT>
tagged_pointer<T>::tagged_pointer(tagged_pointer<TT> other)
: m_data(NULL)
{
    static_assert(std::is_base_of<T,TT>::value, "You shall provider inherited type");

    m_data = other.m_data;
}
template<typename T>
tagged_pointer<T>& tagged_pointer<T>::operator=(const tagged_pointer& other)
{
    m_data = other.m_data;

    return *this;
}
template<typename T>
template<typename TT>
tagged_pointer<T>& tagged_pointer<T>::operator=(const tagged_pointer<TT>& other)
{
    static_assert(std::is_base_of<T,TT>::value, "You shall provider inherited type");

    m_data = other.m_data;

    return *this;
}
template<typename T>
short tagged_pointer<T>::get_tag() const
{
    return TAGGED_POINTER(m_data,tag_mask);
}
template<typename T>
bool tagged_pointer<T>::is_tagged() const
{
    return TAGGED_POINTER(m_data,tag_mask) != 0;
}
template<typename T>
typename tagged_pointer<T>::value_pointer tagged_pointer<T>::operator->()
{
    return untagged_pointer();
}
template<typename T>
typename tagged_pointer<T>::value_const_pointer tagged_pointer<T>::operator->() const
{
    return untagged_pointer();
}
template<typename T>
typename tagged_pointer<T>::value_reference tagged_pointer<T>::operator*()
{
    if (value_pointer pData = untagged_pointer())
    {
        return *pData;
    }
    else
    {
        return *reinterpret_cast<value_pointer>(0xDEAD);
    }
}
template<typename T>
typename tagged_pointer<T>::value_const_reference tagged_pointer<T>::operator*() const
{
    if (value_pointer pData = untagged_pointer())
    {
        return *pData;
    }
    else
    {
        return *reinterpret_cast<value_const_pointer>(0xDEAD);
    }
}
template<typename T>
typename tagged_pointer<T>::value_pointer tagged_pointer<T>::get_pointer()
{
    return untagged_pointer();
}
template<typename T>
typename tagged_pointer<T>::value_const_pointer tagged_pointer<T>::get_pointer() const
{
    return untagged_pointer();
}
template<typename T>
tagged_pointer<T> tagged_pointer<T>::extract()
{
    value_pointer res = m_data;

    m_data = 0;

    return res;
}
template<typename T>
typename tagged_pointer<T>::value_pointer tagged_pointer<T>::extract_pointer()
{
    value_pointer res = untagged_pointer();

    m_data = 0;

    return res;
}
template<typename T>
bool tagged_pointer<T>::empty() const
{
    return m_data == NULL;
}
template<typename T>
void tagged_pointer<T>::destroy()
{
    if(m_data != NULL)
    {
        value_pointer res = untagged_pointer();

        if(TAGGED_POINTER(m_data,tag_mask))
        {
            delete res;
        }
        else
        {
            res->~T();
        }

        m_data = NULL;
    }
}
template<typename T>
bool tagged_pointer<T>::operator==(const tagged_pointer& other) const
{
    return m_data == other.m_data;
}
template<typename T>
bool tagged_pointer<T>::operator!=(const tagged_pointer& other) const
{
    return m_data != other.m_data;
}
template<typename T>
tagged_pointer<T>::operator bool() const
{
    return m_data != NULL;
}
template<typename T>
typename tagged_pointer<T>::value_pointer tagged_pointer<T>::untagged_pointer()
{
    value_pointer _data = m_data;

    if(TAGGED_POINTER(m_data,tag_mask))
    {
        static size_t _dataValue = 0;

        //we have to untag and tag
        memcpy(&_dataValue, &m_data, sizeof(value_pointer));
        _data = value_pointer(_dataValue & bitMask);
    }

    return _data;
}
template<typename T>
typename tagged_pointer<T>::value_const_pointer tagged_pointer<T>::untagged_pointer() const
{
    value_pointer _data = m_data;

    if(TAGGED_POINTER(m_data,tag_mask))
    {
        static size_t _dataValue = 0;

        //we have to untag and tag
        memcpy(&_dataValue, &m_data, sizeof(value_pointer));
        _data = value_pointer(_dataValue & bitMask);
    }

    return _data;
}

template<typename T>
tagged_pointer<T> make_tagged_pointer(short i_tag)
{
	return tagged_pointer<T>(new T(), i_tag);
}
template<typename T, typename ... Args>
tagged_pointer<T> make_tagged_pointer(Args&& ... i_val, short i_tag)
{
	return tagged_pointer<T>(new T(std::forward<Args>(i_val) ...), i_tag);
}
template<typename TT, typename T>
tagged_pointer<TT> dynamic_tagged_cast(const tagged_pointer<T>& i_ptr)
{
	TT* pData = dynamic_cast<TT*>(i_ptr.untagged_pointer());

	return (pData) ? tagged_pointer<TT>(pData,i_ptr.get_tag()) : tagged_pointer<TT>();
}

}
}
