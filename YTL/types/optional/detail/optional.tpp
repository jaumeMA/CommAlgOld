#include "System/cException.h"

namespace yame
{
namespace ytl
{

template<typename T>
optional<T>::optional()
: m_is_initialized(false)
{
}
template<typename T>
optional<T>::optional(mpl::none_t)
: m_is_initialized(false)
{
}
template<typename T>
optional<T>::optional(const optional<T>& other)
: m_is_initialized(other.m_is_initialized)
{
    if (other.m_is_initialized)
    {
        embedded_type<T> *_data = reinterpret_cast<embedded_type<T> *>(const_cast<data_type *>(&other.m_storage));

        ASSERT(_data, "Accessing null data!");

        new(&m_storage)embedded_type<T>(_data->get());
    }
}
template<typename T>
optional<T>::optional(optional<T>&& other)
: m_is_initialized(other.m_is_initialized)
{
    if (other.m_is_initialized)
    {
        embedded_type<T> *_data = reinterpret_cast<embedded_type<T> *>(&other.m_storage);

        ASSERT(_data, "Access null data!");

        new(&m_storage)embedded_type<T>(_data->extract());

        embedded_type<T>::destroy(&other.m_storage);

        other.m_is_initialized = false;
    }
}
template<typename T>
optional<T>::optional(reference_const_type val)
: m_is_initialized(true)
{
    new(&m_storage)embedded_type<T>(val);
}
template<typename T>
optional<T>::optional(reference_type val)
    : m_is_initialized(true)
{
    new(&m_storage)embedded_type<T>(val);
}
template<typename T>
optional<T>::optional(rreference_type val)
: m_is_initialized(true)
{
    new(&m_storage)embedded_type<T>(mpl::move(val));
}
template<typename T>
template<typename TT>
optional<T>::optional(const optional<TT>& other)
: m_is_initialized(other.m_is_initialized)
{
    if (other.m_is_initialized)
    {
        embedded_type<TT> *_data = reinterpret_cast<embedded_type<TT> *>(const_cast<typename optional<TT>::data_type *>(&other.m_storage));

        ASSERT(_data, "Accessing null data!");

        new(&m_storage)embedded_type<T>(_data->get());
    }
}
template<typename T>
template<typename TT>
optional<T>::optional(optional<TT>&& other)
    : m_is_initialized(other.m_is_initialized)
{
    if (other.m_is_initialized)
    {
        embedded_type<TT> *_data = reinterpret_cast<embedded_type<TT> *>(&other.m_storage);

        ASSERT(_data, "Accessing null data!");

        new(&m_storage)embedded_type<T>(_data->extract());

        embedded_type<TT>::destroy(&other.m_storage);

        other.m_is_initialized = false;
    }
}
template<typename T>
template<typename TT>
optional<T>::optional(bool cond, TT&& val)
: m_is_initialized(cond)
{
    if (cond)
    {
        embedded_type<T>::construct(&m_storage,mpl::forward<TT>(val));
    }
}
template<typename T>
template<typename TT>
requires (mpl::is_optional<TT>::value == false)
optional<T>::optional(TT&& val)
: m_is_initialized(true)
{
    embedded_type<T>::construct(&m_storage,mpl::forward<TT>(val));
}
template<typename T>
optional<T>::~optional()
{
    if (m_is_initialized)
    {
        embedded_type<T>::destroy(&m_storage);
    }
}
template<typename T>
optional<T>& optional<T>::operator=(const optional<T>& other)
{
    if (m_is_initialized)
    {
        if (other.isInitialized())
        {
            embedded_type<T> *_data = reinterpret_cast<embedded_type<T> *>(const_cast<optional<T>::data_type *>(&other.m_storage));

            ASSERT(_data, "Accessing null data!");

            embedded_type<T>::assign(&m_storage, _data->get());
        }
        else
        {
            embedded_type<T>::destroy(&m_storage);
            m_is_initialized = false;
        }
    }
    else if (other.isInitialized())
    {
        embedded_type<T> *_data = reinterpret_cast<embedded_type<T> *>(const_cast<optional<T>::data_type *>(&other.m_storage));

        ASSERT(_data, "Accessing null data!");

        embedded_type<T>::construct(&m_storage,_data->get());

        m_is_initialized = true;
    }

    return *this;
}
template<typename T>
optional<T>& optional<T>::operator=(optional<T>& other)
{
    if (m_is_initialized)
    {
        if (other.isInitialized())
        {
            embedded_type<T> *_data = reinterpret_cast<embedded_type<T> *>(&other.m_storage);

            ASSERT(_data, "Accessing null data!");

            embedded_type<T>::assign(&m_storage, _data->get());
        }
        else
        {
            embedded_type<T>::destroy(&m_storage);
            m_is_initialized = false;
        }
    }
    else if (other.isInitialized())
    {
        embedded_type<T> *_data = reinterpret_cast<embedded_type<T> *>(&other.m_storage);

        ASSERT(_data, "Accessing null data!");

        embedded_type<T>::construct(&m_storage,_data->get());

        m_is_initialized = true;
    }

    return *this;
}
template<typename T>
optional<T>& optional<T>::operator=(optional<T>&& other)
{
    if (m_is_initialized)
    {
        if (other.isInitialized())
        {
            embedded_type<T> *_data = reinterpret_cast<embedded_type<T> *>(&other.m_storage);

            ASSERT(_data, "Accessing null data!");

            embedded_type<T>::assign(&m_storage, _data->extract());

            embedded_type<T>::destroy(&other.m_storage);

            other.m_is_initialized = false;
        }
        else
        {
            embedded_type<T>::destroy(&m_storage);
            m_is_initialized = false;
        }
    }
    else if (other.isInitialized())
    {
        embedded_type<T> *_data = reinterpret_cast<embedded_type<T> *>(&other.m_storage);

        ASSERT(_data, "Accessing null data!");

        embedded_type<T>::construct(&m_storage,_data->extract());

        m_is_initialized = true;

        embedded_type<T>::destroy(&other.m_storage);

        other.m_is_initialized = false;
    }

    return *this;
}
template<typename T>
template<typename TT>
optional<T>& optional<T>::operator=(const optional<TT>& other)
{
    if (m_is_initialized)
    {
        if (other.isInitialized())
        {
            embedded_type<TT> *_data = reinterpret_cast<embedded_type<TT> *>(const_cast<typename optional<TT>::data_type *>(&other.m_storage));

            ASSERT(_data, "Accessing null data!");

            embedded_type<T>::assign(&m_storage, _data->get());
        }
        else
        {
            embedded_type<T>::destroy(&m_storage);
            m_is_initialized = false;
        }
    }
    else if (other.isInitialized())
    {
        embedded_type<TT> *_data = reinterpret_cast<embedded_type<TT> *>(const_cast<typename optional<TT>::data_type *>(&other.m_storage));

        ASSERT(_data, "Accessing null data!");

        new(&m_storage) embedded_type<T>(_data->get());

        m_is_initialized = true;
    }

    return *this;
}
template<typename T>
template<typename TT>
optional<T>& optional<T>::operator=(optional<TT>& other)
{
    if (m_is_initialized)
    {
        if (other.isInitialized())
        {
            embedded_type<TT> *_data = reinterpret_cast<embedded_type<TT> *>(&other.m_storage);

            ASSERT(_data, "Accessing null data!");

            embedded_type<T>::assign(&m_storage, _data->get());
        }
        else
        {
            embedded_type<T>::destroy(&m_storage);
            m_is_initialized = false;
        }
    }
    else if (other.isInitialized())
    {
        embedded_type<TT> *_data = reinterpret_cast<embedded_type<TT> *>(&other.m_storage);

        ASSERT(_data, "Accessing null data!");

        embedded_type<T>::construct(&m_storage,_data->get());

        m_is_initialized = true;
    }

    return *this;
}
template<typename T>
template<typename TT>
optional<T>& optional<T>::operator=(optional<TT>&& other)
{
    if (m_is_initialized)
    {
        if (other.isInitialized())
        {
            embedded_type<TT> *_data = reinterpret_cast<embedded_type<TT> *>(&other.m_storage);

            ASSERT(_data, "Accessing null data!");

            embedded_type<T>::assign(&m_storage, _data->extract());

            embedded_type<TT>::destroy(&other.m_storage);

            other.m_is_initialized = false;
        }
        else
        {
            embedded_type<T>::destroy(&m_storage);
            m_is_initialized = false;
        }
    }
    else if (other.isInitialized())
    {
        embedded_type<TT> *_data = reinterpret_cast<embedded_type<TT> *>(&other.m_storage);

        ASSERT(_data, "Accessing null data!");

        embedded_type<T>::construct(&m_storage,_data->extract());

        m_is_initialized = true;

        embedded_type<TT>::destroy(&other.m_storage);

        other.m_is_initialized = false;
    }

    return *this;
}
template<typename T>
optional<T>& optional<T>::operator=(mpl::none_t)
{
    if (m_is_initialized)
    {
        embedded_type<T>::destroy(&m_storage);

        m_is_initialized = false;
    }

    return *this;
}
template<typename T>
optional<T>& optional<T>::operator=(reference_type val)
{
    if (m_is_initialized)
    {
        embedded_type<T>::assign(&m_storage, val);
    }
    else
    {
        new(&m_storage) embedded_type<T>(val);

        m_is_initialized = true;
    }

    return *this;
}
template<typename T>
optional<T>& optional<T>::operator=(reference_const_type val)
{
    if (m_is_initialized)
    {
        embedded_type<T>::assign(&m_storage, val);
    }
    else
    {
        embedded_type<T>::construct(&m_storage,val);

        m_is_initialized = true;
    }

    return *this;
}
template<typename T>
optional<T>& optional<T>::operator=(rreference_type val)
{
    if (m_is_initialized)
    {
        embedded_type<T>::assign(&m_storage, mpl::move(val));
    }
    else
    {
        embedded_type<T>::construct(&m_storage,mpl::move(val));

        m_is_initialized = true;
    }

    return *this;
}
template<typename T>
template<typename TT>
optional<T>& optional<T>::operator=(TT&& val)
{
    if (m_is_initialized)
    {
        embedded_type<T>::assign(&m_storage, mpl::forward<TT>(val));
    }
    else
    {
        embedded_type<T>::construct(&m_storage,mpl::forward<TT>(val));

        m_is_initialized = true;
    }

    return *this;
}
template<typename T>
bool optional<T>::isInitialized() const
{
    return m_is_initialized;
}
template<typename T>
optional<T>::operator bool() const
{
    return m_is_initialized;
}
template<typename T>
bool optional<T>::operator!() const
{
    return !m_is_initialized;
}
template<typename T>
typename embedded_type<T>::cref_type optional<T>::get() const
{
    ASSERT(m_is_initialized, "Accessing non initialized data!");

    typedef typename embedded_type<T>::cref_type retType;

    //ok, get it!
    embedded_type<T> *_data = reinterpret_cast<embedded_type<T> *>(const_cast<data_type *>(&m_storage));

    ASSERT(_data, "Accessing null data!");

    return mpl::forward<retType>(_data->get());
}
template<typename T>
typename embedded_type<T>::ref_type optional<T>::get()
{
    ASSERT(m_is_initialized, "Accessing non initialized data!");

    typedef typename embedded_type<T>::ref_type retType;

    //ok, get it!
    embedded_type<T> *_data = reinterpret_cast<embedded_type<T> *>(&m_storage);

    ASSERT(_data, "Accessing null data!");

    return mpl::forward<retType>(_data->get());
}
template<typename T>
typename embedded_type<T>::cref_type optional<T>::get_value_or(typename embedded_type<T>::cref_type default_value) const
{
    if (m_is_initialized)
    {
        typedef typename embedded_type<T>::cref_type retType;

        //ok, get it!
        embedded_type<T> *_data = reinterpret_cast<embedded_type<T> *>(const_cast<data_type *>(&m_storage));

        ASSERT(_data, "Accessing null data!");

        return mpl::forward<retType>(_data->get());
    }

    return default_value;
}
template<typename T>
typename embedded_type<T>::ref_type optional<T>::get_value_or(typename embedded_type<T>::ref_type default_value)
{
    if (m_is_initialized)
    {
        typedef typename embedded_type<T>::ref_type retType;

        //ok, get it!
        embedded_type<T> *_data = reinterpret_cast<embedded_type<T> *>(const_cast<data_type *>(&m_storage));

        ASSERT(_data, "Accessing null data!");

        return mpl::forward<retType>(_data->get());
    }

    return default_value;
}
template<typename T>
typename embedded_type<T>::cpointer_type optional<T>::get_ptr() const
{
    if (m_is_initialized)
    {
        typedef typename embedded_type<T>::cpointer_type retType;

        //ok, get it!
        embedded_type<T> *_data = reinterpret_cast<embedded_type<T> *>(const_cast<data_type *>(&m_storage));

        ASSERT(_data, "Accessing null data!");

        return &(_data->m_data);
    }

    return nullptr;
}
template<typename T>
typename embedded_type<T>::pointer_type optional<T>::get_ptr()
{
    if (m_is_initialized)
    {
        typedef typename embedded_type<T>::cpointer_type retType;

        //ok, get it!
        embedded_type<T> *_data = reinterpret_cast<embedded_type<T> *>(&m_storage);

        ASSERT(_data, "Accessing null data!");

        return &(_data->m_data);
    }

    return nullptr;
}
template<typename T>
T optional<T>::extract()
{
    ASSERT(m_is_initialized, "Accessing non initialized data!");

    // do a local copy and forward it
    typedef typename embedded_type<T>::rref_type retType;
    typedef typename embedded_type<T>::internal_type rawType;

    //ok, get it!
    embedded_type<T> *_data = reinterpret_cast<embedded_type<T> *>(const_cast<data_type *>(&m_storage));

    ASSERT(_data, "Accessing null data!");

    rawType res = mpl::forward<retType>(_data->get());

    embedded_type<T>::destroy(&m_storage);

    m_is_initialized = false;

    return mpl::forward<retType>(res);
}
template<typename T>
typename embedded_type<T>::ref_type optional<T>::operator*() const
{
    ASSERT(m_is_initialized, "Accessing non initialized data!");

    typedef typename embedded_type<T>::ref_type retType;

    //ok, get it!
    embedded_type<T> *_data = reinterpret_cast<embedded_type<T>*>(const_cast<data_type *>(&m_storage));

    ASSERT(_data, "Accessing null data!");

    return mpl::forward<retType>(_data->get());
}
template<typename T>
typename embedded_type<T>::ref_type optional<T>::operator*()
{
    ASSERT(m_is_initialized, "Accessing non initialized data!");

    typedef typename embedded_type<T>::ref_type retType;

    //ok, get it!
    embedded_type<T> *_data = reinterpret_cast<embedded_type<T>*>(&m_storage);

    ASSERT(_data, "Accessing null data!");

    return mpl::forward<retType>(_data->get());
}
template<typename T>
typename embedded_type<T>::cpointer_type optional<T>::operator->() const
{
    ASSERT(m_is_initialized, "Accessing non initialized data!");

    //ok, get it!
    embedded_type<T> *_data = reinterpret_cast<embedded_type<T>*>(const_cast<data_type *>(&m_storage));

    ASSERT(_data, "Accessing null data!");

    return _data->operator->();
}
template<typename T>
typename embedded_type<T>::pointer_type optional<T>::operator->()
{
    ASSERT(m_is_initialized, "Accessing non initialized data!");

    //ok, get it!
    embedded_type<T> *_data = reinterpret_cast<embedded_type<T>*>(const_cast<data_type *>(&m_storage));

    ASSERT(_data, "Accessing null data!");

    return _data->operator->();
}
template<typename T>
void optional<T>::swap(const optional<T>& other)
{
    optional<T>& ncOther = const_cast<optional<T>&>(other);

    if (m_is_initialized && ncOther.m_is_initialized)
    {
        embedded_type<T> *_thisData = reinterpret_cast<embedded_type<T>*>(const_cast<data_type *>(&m_storage));
        embedded_type<T> *_otherData = reinterpret_cast<embedded_type<T>*>(const_cast<data_type *>(&(ncOther.m_storage)));

        embedded_type<T>::swap(&m_storage, _thisData->get(), &(ncOther.m_storage), _otherData->get());
    }
    else if (m_is_initialized)
    {
        embedded_type<T> *_thisData = reinterpret_cast<embedded_type<T>*>(const_cast<data_type *>(&m_storage));

        new(&(ncOther.m_storage)) embedded_type<T>(_thisData->get());
        ncOther.m_is_initialized = true;

        embedded_type<T>::destroy(&m_storage);
        m_is_initialized = false;
    }
    else if (ncOther.m_is_initialized)
    {
        embedded_type<T> *_otherData = reinterpret_cast<embedded_type<T>*>(const_cast<data_type *>(&(ncOther.m_storage)));

        new(&(m_storage)) embedded_type<T>(_otherData->get());
        m_is_initialized = true;

        embedded_type<T>::destroy(&(ncOther.m_storage));
        ncOther.m_is_initialized = false;
    }
}

}
}
