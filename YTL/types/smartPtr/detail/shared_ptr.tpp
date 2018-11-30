
#include "Utils/cMacroHelper.h"

namespace yame
{
namespace ytl
{

template<typename T>
shared_ptr<T>::shared_ptr()
: m_refCounter(NULL)
, m_data(NULL)
, m_deleter(NULL)
{
}
template<typename T>
shared_ptr<T>::shared_ptr(T* i_data, ISmartPtrDeleter* i_refDeleter)
: m_refCounter(NULL)
, m_data(i_data)
, m_deleter(i_refDeleter)
{
    if (m_data)
    {
        if(m_refCounter = make_tagged_pointer<shared_reference_counter>(0))
        {
            m_refCounter->incrementReference();
        }
    }
}
template<typename T>
shared_ptr<T>::shared_ptr(const shared_ptr& other)
: m_refCounter(NULL)
, m_data(NULL)
, m_deleter(other.m_deleter)
{
    m_data = other.m_data;

    if(m_refCounter = other.m_refCounter)
    {
        m_refCounter->incrementReference();
    }
}
template<typename T>
shared_ptr<T>::shared_ptr(shared_ptr&& other)
: m_refCounter(NULL)
, m_data(NULL)
, m_deleter(other.m_deleter)
{
    std::swap(m_data,other.m_data);
    std::swap(m_refCounter,other.m_refCounter);
}
template<typename T>
template<typename TT>
shared_ptr<T>::shared_ptr(const shared_ptr<TT>& other)
: m_refCounter(NULL)
, m_data(NULL)
, m_deleter(other.m_deleter)
{
    static_assert(std::is_base_of<T,TT>::value, "You shall provide a base class of T");

    if(m_data = dynamic_cast<T*>(other.m_data))
    {
        if(m_refCounter = other.m_refCounter)
        {
            m_refCounter->incrementReference();
        }
    }
}
template<typename T>
template<typename TT>
shared_ptr<T>::shared_ptr(shared_ptr<TT>&& other)
: m_refCounter(NULL)
, m_data(NULL)
, m_deleter(other.m_deleter)
{
    static_assert(std::is_base_of<T,TT>::value, "You shall provide a base class of T");

    if(m_data = dynamic_cast<T*>(other.m_data))
    {
        std::swap(m_refCounter,other.m_refCounter);
    }

    other.m_data = NULL;
}
template<typename T>
shared_ptr<T>::~shared_ptr()
{
    if(m_refCounter)
    {
        m_refCounter->decrementReference();

        clearIfCounterVoid();
    }
}
template<typename T>
shared_ptr<T>& shared_ptr<T>::operator=(const std::nullptr_t&)
{
    if (m_refCounter)
    {
        m_refCounter->decrementReference();

        clearIfCounterVoid();
    }

    return *this;
}
template<typename T>
shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr& other)
{
    if(m_refCounter)
    {
        m_refCounter->decrementReference();

        clearIfCounterVoid();
    }

    m_deleter = other.m_deleter;
    m_data = other.m_data;

    if(m_refCounter = other.m_refCounter)
    {
        m_refCounter->incrementReference();
    }

    return *this;
}
template<typename T>
shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr&& other)
{
    if(m_refCounter)
    {
        m_refCounter->decrementReference();

        clearIfCounterVoid();
    }

    m_deleter = other.m_deleter;
    m_data = other.m_data;
    other.m_data = NULL;
    m_refCounter = other.m_refCounter;
    other.m_refCounter = NULL;

    return *this;
}
template<typename T>
template<typename TT>
shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr<TT>& other)
{
    if(m_refCounter)
    {
        m_refCounter->decrementReference();

        clearIfCounterVoid();
    }

    if(m_data = dynamic_cast<T*>(other.m_data))
    {
        m_deleter = other.m_deleter;
        m_refCounter = other.m_refCounter;

        if(m_refCounter)
        {
            m_refCounter->incrementReference();
        }
    }

    return *this;
}
template<typename T>
template<typename TT>
shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr<TT>&& other)
{
    if(m_refCounter)
    {
        m_refCounter->decrementReference();

        clearIfCounterVoid();
    }

    if(m_data = dynamic_cast<T*>(other.m_data))
    {
        m_deleter = other.m_deleter;
    }

    other.m_data = NULL;
    m_refCounter = other.m_refCounter;
    other.m_refCounter = NULL;

    return *this;
}
template<typename T>
bool shared_ptr<T>::operator==(std::nullptr_t) const
{
    return m_data == NULL;
}
template<typename T>
bool shared_ptr<T>::operator==(const shared_ptr<T>& other) const
{
    if (m_data && other.m_data)
    {
        return m_data == other.m_data;
    }
    else
    {
        return false;
    }
}
template<typename T>
bool shared_ptr<T>::operator!=(std::nullptr_t) const
{
    return m_data != NULL;
}
template<typename T>
bool shared_ptr<T>::operator!=(const shared_ptr<T>& other)
{
    if (m_data && other.m_data)
    {
        return m_data != other.m_data;
    }
    else
    {
        return true;
    }
}
template<typename T>
T* shared_ptr<T>::operator->()
{
    return m_data;
}
template<typename T>
const T* shared_ptr<T>::operator->() const
{
    return m_data;
}
template<typename T>
T& shared_ptr<T>::operator*()
{
    if(m_data == NULL)
    {
        MAKE_IT_CRASH
    }

    return *m_data;
}
template<typename T>
const T& shared_ptr<T>::operator*() const
{
    if(m_data == NULL)
    {
        MAKE_IT_CRASH
    }

    return *m_data;
}
template<typename T>
shared_ptr<T>::operator bool() const
{
    return m_data != NULL;
}
template<typename T>
void shared_ptr<T>::clear()
{
    if (m_refCounter)
    {
        m_refCounter->decrementReference();

        clearIfCounterVoid();
    }
}
template<typename T>
shared_ptr<T>::shared_ptr(T* i_data, tagged_reference_counter i_refCounter)
: m_data(i_data)
, m_refCounter(NULL)
, m_deleter(NULL)
{
    if(m_refCounter = i_refCounter)
    {
        m_refCounter->incrementReference();
    }
}
template<typename T>
shared_ptr<T>::shared_ptr(T* i_data, shared_reference_counter* i_refCounter)
: m_data(i_data)
, m_refCounter(NULL)
, m_deleter(NULL)
{
    if(m_refCounter = tagged_pointer<shared_reference_counter>(i_refCounter,0x01))
    {
        m_refCounter->incrementReference();
    }
}
template<typename T>
void shared_ptr<T>::clearIfCounterVoid()
{
    if(m_refCounter->hasReferences() == false)
    {
        const bool isTagged = m_refCounter.is_tagged();
        shared_reference_counter* refCounter = m_refCounter.extract_pointer();

        if (isTagged == false)
        {
            delete refCounter;
        }
        else
        {
            refCounter->~shared_reference_counter();
        }

        if(m_deleter)
        {
            m_deleter->Deallocate(const_cast<typename std::remove_const<T>::type*>(m_data));
        }
        else
        {
            ::delete(m_data);
        }

        m_data = NULL;
    }
}

template<typename T, typename ... Args>
shared_ptr<T> make_shared_reference(Args&& ... i_args);

}
}
