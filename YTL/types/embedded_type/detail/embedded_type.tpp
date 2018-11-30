
namespace yame
{
namespace ytl
{

template<typename T>
embedded_type<T&>::embedded_type(internal_type& other)
    : m_data(other)
{
}
template<typename T>
embedded_type<T&>::embedded_type(const embedded_type<T&>& other)
    : m_data(other.m_data)
{
}
template<typename T>
typename embedded_type<T&>::ref_type embedded_type<T&>::get()
{
    return m_data;
}
template<typename T>
typename embedded_type<T&>::rref_type embedded_type<T&>::extract()
{
    return m_data;
}
template<typename T>
typename embedded_type<T&>::pointer_type embedded_type<T&>::operator->()
{
    return &m_data;
}
template<typename T>
typename embedded_type<T&>::cpointer_type embedded_type<T&>::operator->() const
{
    return &m_data;
}
template<typename T>
bool embedded_type<T&>::operator==(const embedded_type<T&>& other) const
{
    return m_data == other.m_data;
}
template<typename T>
bool embedded_type<T&>::operator==(embedded_type<T&>&& other) const
{
    //makes no sense to move m_data since by reference collapsing it would remain as lvalue
    return m_data == other.m_data;
}
template<typename T>
template<typename Type>
bool embedded_type<T&>::construct(void* address, Type&& val)
{
    return new(address)embedded_type(mpl::forward<Type>(val)) != NULL;
}
template<typename T>
bool embedded_type<T&>::destroy(void* address)
{
    //in references nothing needs to be done
}
template<typename T>
template<typename Type>
bool embedded_type<T&>::assign(void* address, Type&& val)
{
    //references must be reconstructed every time
    return construct<Type>(address, mpl::forward<Type>(val));
}
template<typename T>
bool embedded_type<T&>::swap(void* addressA, internal_type& valA, void* addressB, internal_type& valB)
{
    internal_type& tmp = valA;

    return construct<internal_type&>(addressA, valB) && construct<internal_type&>(addressB, tmp);
}

template<typename T>
embedded_type<T&&>::embedded_type(T&& other)
    : m_data(mpl::move(other))
{
}
template<typename T>
embedded_type<T&&>::embedded_type(const embedded_type<T&&>& other)
    : m_data(mpl::move(other.m_data))
{
}
template<typename T>
typename embedded_type<T&&>::pointer_type embedded_type<T&&>::operator->()
{
    return &m_data;
}
template<typename T>
typename embedded_type<T&&>::cpointer_type embedded_type<T&&>::operator->() const
{
    return &m_data;
}
template<typename T>
bool embedded_type<T&&>::operator==(const embedded_type<T&&>& other) const
{
    return m_data == mpl::move(other.m_data);
}
template<typename T>
bool embedded_type<T&&>::operator==(embedded_type<T&&>&& other) const
{
    return m_data == mpl::move(other.m_data);
}
template<typename T>
typename embedded_type<T&&>::ref_type embedded_type<T&&>::get()
{
    return mpl::move(m_data);
}
template<typename T>
typename embedded_type<T&&>::rref_type embedded_type<T&&>::extract()
{
    return mpl::move(m_data);
}
template<typename T>
template<typename Type>
bool embedded_type<T&&>::construct(void* address, Type&& val)
{
    return new(address) embedded_type(mpl::move(val)) != NULL;
}
template<typename T>
bool embedded_type<T&&>::destroy(void* address)
{
    //in references nothing needs to be done
    return false;
}
template<typename T>
template<typename Type>
bool embedded_type<T&&>::assign(void* address, Type&& val)
{
    //references must be reconstructed every time
    return construct<Type>(address, mpl::forward<Type>(val));
}
template<typename T>
bool embedded_type<T&&>::swap(void* addressA, internal_type&& valA, void* addressB, internal_type&& valB)
{
    internal_type&& tmp = mpl::move(valA);

    return construct<internal_type&&>(addressA, mpl::move(valB)) && construct<internal_type&&>(addressB, mpl::move(tmp));
}

template<typename T>
embedded_type<T>::embedded_type(const internal_type& other)
    : m_data(other)
{
}
template<typename T>
embedded_type<T>::embedded_type(internal_type& other)
    : m_data(other)
{
}
template<typename T>
embedded_type<T>::embedded_type(internal_type&& other)
    : m_data(mpl::move(other))
{
}
template<typename T>
embedded_type<T>::embedded_type(const embedded_type<T>& other)
    : m_data(other.m_data)
{
}
template<typename T>
template<typename TType>
embedded_type<T>::embedded_type(TType&& val)
    : m_data(mpl::forward<TType>(val))
{
}
template<typename T>
typename embedded_type<T>::pointer_type embedded_type<T>::operator->()
{
    return &m_data;
}
template<typename T>
typename embedded_type<T>::cpointer_type embedded_type<T>::operator->() const
{
    return &m_data;
}
template<typename T>
embedded_type<T>& embedded_type<T>::operator=(const internal_type& other)
{
    m_data = other;

    return *this;
}
template<typename T>
embedded_type<T>& embedded_type<T>::operator=(internal_type&& other)
{
    m_data = mpl::move(other);

    return *this;
}
template<typename T>
embedded_type<T>& embedded_type<T>::operator=(const embedded_type<T>& other)
{
    m_data = other.m_data;

    return *this;
}
template<typename T>
bool embedded_type<T>::operator==(const embedded_type<T>& other) const
{
    return m_data == other.m_data;
}
template<typename T>
bool embedded_type<T>::operator==(embedded_type<T>&& other) const
{
    return m_data == mpl::move(other.m_data);
}
template<typename T>
typename embedded_type<T>::ref_type embedded_type<T>::get()
{
    return mpl::forward<embedded_type<T>::ref_type>(m_data);
}
template<typename T>
typename embedded_type<T>::rref_type embedded_type<T>::extract()
{
    return mpl::forward<embedded_type<T>::rref_type>(m_data);
}
template<typename T>
template<typename Type>
bool embedded_type<T>::construct(void* address, Type&& val)
{
    return new(address)embedded_type(mpl::forward<Type>(val)) != NULL;
}
template<typename T>
bool embedded_type<T>::destroy(void* address)
{
    embedded_type<T>* _data = reinterpret_cast<embedded_type<T>* >(address);

    _data->m_data.~T();

    return true;
}
template<typename T>
template<typename Type>
bool embedded_type<T>::assign(void* address, Type&& val)
{
    embedded_type<T>* _data = reinterpret_cast<embedded_type<T>* >(address);

    _data->m_data = mpl::forward<Type>(val);

    return true;
}
template<typename T>
bool embedded_type<T>::swap(void* addressA, internal_type& valA, void* addressB, internal_type& valB)
{
    internal_type tmp = valA;

    embedded_type<T>* _Adata = reinterpret_cast<embedded_type<T>* >(addressA);
    *_Adata = valB;

    embedded_type<T>* _Bdata = reinterpret_cast<embedded_type<T>* >(addressB);
    *_Bdata = tmp;

    return true;
}

}
}
