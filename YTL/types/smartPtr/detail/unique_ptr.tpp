
#include "Utils/cMacroHelper.h"

namespace yame
{
namespace ytl
{

template<typename T>
unique_ptr<T>::unique_ptr()
: m_refCounter(NULL)
, m_data(NULL)
, m_deleter(NULL)
{
}
template<typename T>
unique_ptr<T>::unique_ptr(const mpl::null_ptr_type&)
: m_refCounter(NULL)
, m_data(NULL)
, m_deleter(NULL)
{
}
template<typename T>
unique_ptr<T>::unique_ptr(T* i_data, ISmartPtrDeleter* i_refDeleter)
: m_refCounter(NULL)
, m_data(i_data)
, m_deleter(i_refDeleter)
{
    if(m_refCounter = make_tagged_pointer<unique_reference_counter>(0))
    {
        m_refCounter->incrementStrongReference();
    }
}
template<typename T>
unique_ptr<T>::unique_ptr(unique_ptr&& other)
: m_refCounter(NULL)
, m_data(NULL)
, m_deleter(other.m_deleter)
{
    std::swap(m_data,other.m_data);
    std::swap(m_refCounter,other.m_refCounter);
}
template<typename T>
template<typename TT>
unique_ptr<T>::unique_ptr(unique_ptr<TT>&& other)
: m_refCounter(NULL)
, m_data(NULL)
, m_deleter(other.m_deleter)
{
    static_assert(std::is_base_of<T,TT>::value, "You shall provide inherited classes");

    if(m_data = other.m_data)
    {
        std::swap(m_refCounter,other.m_refCounter);
    }

    other.m_data = NULL;
}
template<typename T>
unique_ptr<T>::~unique_ptr()
{
    if(m_refCounter)
    {
        m_refCounter->decrementStrongReference();

        clearIfCounterVoid();
    }
}
template<typename T>
unique_ptr<T>& unique_ptr<T>::operator=(const mpl::null_ptr_type&)
{
    if (m_refCounter)
    {
        m_refCounter->decrementStrongReference();

        clearIfCounterVoid();
    }

    return *this;
}
template<typename T>
unique_ptr<T>& unique_ptr<T>::operator=(unique_ptr&& other)
{
    if(m_refCounter)
    {
        m_refCounter->decrementStrongReference();

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
unique_ptr<T>& unique_ptr<T>::operator=(unique_ptr<TT>&& other)
{
    static_assert(std::is_base_of<T,TT>::value, "You shall provide inherited classes");

    if(m_refCounter)
    {
        m_refCounter->decrementStrongReference();

        clearIfCounterVoid();
    }

    if(m_data = other.m_data)
    {
        m_deleter = other.m_deleter;
    }

    other.m_data = NULL;
    m_refCounter = other.m_refCounter;
    other.m_refCounter = NULL;

    return *this;
}
template<typename T>
bool unique_ptr<T>::operator==(const mpl::null_ptr_type&) const
{
    return m_data == NULL;
}
template<typename T>
bool unique_ptr<T>::operator==(const unique_ptr<T>& other) const
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
bool unique_ptr<T>::operator!=(const mpl::null_ptr_type&) const
{
    return m_data != NULL;
}
template<typename T>
bool unique_ptr<T>::operator!=(const unique_ptr<T>& other)
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
T* unique_ptr<T>::operator->()
{
    return m_data;
}
template<typename T>
const T* unique_ptr<T>::operator->() const
{
    return m_data;
}
template<typename T>
T& unique_ptr<T>::operator*()
{
    if(m_data == NULL)
    {
        MAKE_IT_CRASH
    }

    return *m_data;
}
template<typename T>
const T& unique_ptr<T>::operator*() const
{
    if(m_data == NULL)
    {
        MAKE_IT_CRASH
    }

    return *m_data;
}
template<typename T>
unique_ptr<T>::operator bool() const
{
    return m_data != NULL;
}
template<typename T>
void unique_ptr<T>::clear()
{
    if (m_refCounter)
    {
        m_refCounter->decrementStrongReference();

        clearIfCounterVoid();
    }
}
template<typename T>
const T* unique_ptr<T>::getData() const
{
    return m_data;
}
template<typename T>
T* unique_ptr<T>::extractData()
{
    T* res = m_data;

    m_data = NULL;

    return res;
}
template<typename T>
T* unique_ptr<T>::getData()
{
    return m_data;
}
template<typename T>
ISmartPtrDeleter* unique_ptr<T>::getDeleter()
{
    return m_deleter;
}
template<typename T>
typename unique_ptr<T>::tagged_reference_counter unique_ptr<T>::extract_reference_counter()
{
    tagged_reference_counter res;

    if(res = m_refCounter.extract())
    {
        res->decrementStrongReference();
    }

    return res;
}
template<typename T>
unique_ptr<T>::unique_ptr(T* i_data, tagged_reference_counter i_refCounter, ISmartPtrDeleter* i_deleter)
: m_data(i_data)
, m_refCounter(i_refCounter)
, m_deleter(i_deleter)
{
    m_refCounter->incrementStrongReference();
}
template<typename T>
void unique_ptr<T>::clearIfCounterVoid()
{
    if(m_refCounter->hasStrongReferences() == false)
    {
        ASSERT(m_refCounter->hasWeakReferences() == false, "Still lent references alive while destroying unique reference");

        const bool isTagged = m_refCounter.is_tagged();
        unique_reference_counter* refCounter = m_refCounter.extract_pointer();

        if (isTagged == false)
        {
            delete refCounter;
        }
        else
        {
            refCounter->~unique_reference_counter();
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
        m_refCounter = NULL;
    }
}

template<typename T>
unique_ref<T>::unique_ref(T *pData, ISmartPtrDeleter* deleter)
: unique_ptr<T>(pData,deleter)
{
}
template<typename T>
unique_ref<T>::unique_ref(unique_ref<T>&& other)
: unique_ptr<T>(mpl::move(other))
{
}
template<typename T>
template<typename TT>
unique_ref<T>::unique_ref(TT *pData, ISmartPtrDeleter* deleter)
: unique_ptr<T>(pData,deleter)
{
}
template<typename T>
template<typename TT>
unique_ref<T>::unique_ref(unique_ref<TT>&& other)
: unique_ptr<T>(mpl::move(other))
{
}
template<typename T>
unique_ref<T>& unique_ref<T>::operator=(unique_ref&& other)
{
    unique_ptr<T>::operator=(mpl::move(other));

    return *this;
}
template<typename T>
unique_ref<T>::unique_ref(T *pData, tagged_reference_counter referenceCounter, ISmartPtrDeleter* i_deleter)
: unique_ptr<T>(pData,referenceCounter,i_deleter)
{
}

template<typename T, typename ... Args>
unique_ptr<T> make_unique_ptr(Args&& ... i_args)
{
	char* allocatedMemory = reinterpret_cast<char*>(malloc(sizeof(T) + sizeof(unique_reference_counter)));

 	T* allocatedObject = new (allocatedMemory) T(mpl::forward<Args>(i_args) ...);

	unique_reference_counter* refCounter = new (allocatedMemory + sizeof(T)) unique_reference_counter();

	return unique_ptr<T>(allocatedObject,tagged_pointer<unique_reference_counter>(refCounter,0x01));
}
template<typename T, typename ... Args>
unique_ref<T> make_unique_ref(Args&& ... i_args)
{
	char* allocatedMemory = reinterpret_cast<char*>(malloc(sizeof(T) + sizeof(unique_reference_counter)));

 	T* allocatedObject = new (allocatedMemory) T(mpl::forward<Args>(i_args) ...);

	unique_reference_counter* refCounter = new (allocatedMemory + sizeof(T)) unique_reference_counter();

	return unique_ref<T>(allocatedObject,tagged_pointer<unique_reference_counter>(refCounter,0x01));
}
template<typename T>
lent_ptr<T> lend(unique_ptr<T>& i_uniqueRef)
{
	return lent_ptr<T>(i_uniqueRef.m_data,i_uniqueRef.m_refCounter);
}
template<typename T>
unique_ref<T> promote_to_ref(unique_ptr<T>&& other)
{
    tagged_pointer<unique_reference_counter> referenceCounter = other.extract_reference_counter();
    ISmartPtrDeleter* deleter = other.getDeleter();
    T* pData = other.extractData();

    return unique_ref<T>(pData,referenceCounter,deleter);
}
template<typename TT, typename T>
unique_ptr<TT> dynamic_unique_cast(unique_ptr<T>&& other)
{
    static_assert(std::is_polymorphic<T>::value, "You shall work with polymprphic classes, try providing virtual destructor");

    if(T* pData = other.extractData())
    {
        if(TT* innerData = rtti::dynamicCast<TT>(pData))
        {
            tagged_pointer<unique_reference_counter> referenceCounter = other.extract_reference_counter();
            ISmartPtrDeleter* deleter = other.getDeleter();

            return unique_ref<TT>(innerData, referenceCounter, deleter);
        }
        else
        {
            return null_ptr;
        }
    }
    else
    {
        return null_ptr;
    }
}
template<typename TT, typename T>
unique_ptr<TT> static_unique_cast(unique_ptr<T>&& other)
{
    static_assert(mpl::is_base_of<T,TT>::value || mpl::is_base_of<TT,T>::value, "You shall provide related classes by inheritance");
    static_assert(std::is_polymorphic<T>::value, "You shall work with polymprphic classes, try providing virtual destructor");

    if(T* pData = other.extractData())
    {
        TT* innerData = rtti::staticCast<TT>(pData);
        tagged_pointer<unique_reference_counter> referenceCounter = other.extract_reference_counter();
        ISmartPtrDeleter* deleter = other.getDeleter();

        return unique_ref<TT>(innerData, referenceCounter, deleter);
    }
    else
    {
        return null_ptr;
    }
}
template<typename TT, typename T>
unique_ptr<TT> dynamic_unique_cast(unique_ref<T>&& other)
{
    static_assert(std::is_polymorphic<T>::value, "You shall work with polymprphic classes, try providing virtual destructor");

    if(TT* innerData = rtti::dynamicCast<TT>(other.extractData()))
    {
        tagged_pointer<unique_reference_counter> referenceCounter = other.extract_reference_counter();
        ISmartPtrDeleter* deleter = other.getDeleter();

        return unique_ref<TT>(innerData, referenceCounter, deleter);
    }
    else
    {
        return null_ptr;
    }
}
template<typename TT, typename T>
unique_ref<TT> static_unique_cast(unique_ref<T>&& other)
{
    tagged_pointer<unique_reference_counter> referenceCounter = other.extract_reference_counter();
    ISmartPtrDeleter* deleter = other.getDeleter();
    TT* innerData = rtti::staticCast<TT>(other.extractData());

    return unique_ref<TT>(innerData, referenceCounter, deleter);
}
template<typename T>
unique_ptr<T> const_unique_cast(unique_ptr<const T>&& other)
{
    if(const T* pData = other.extractData())
    {
        tagged_pointer<unique_reference_counter> referenceCounter = other.extract_reference_counter();
        ISmartPtrDeleter* deleter = other.getDeleter();
        T* innerData = const_cast<T*>(pData);

        return unique_ptr<T>(innerData, referenceCounter, deleter);
    }
    else
    {
        return null_ptr;
    }
}
template<typename T>
unique_ref<T> const_unique_cast(unique_ref<const T>&& other)
{
    const T* pData = other.extractData();
    ASSERT(pData, "You shall provide a non empty ref");
    tagged_pointer<unique_reference_counter> referenceCounter = other.extract_reference_counter();
    ISmartPtrDeleter* deleter = other.getDeleter();
    T* innerData = const_cast<T*>(pData);

    other.clear();

    return unique_ref<T>(innerData, referenceCounter, deleter);
}

}
}
