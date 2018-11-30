
#include "Utils/cMacroHelper.h"
#include "YTL/mpl/cTemplateHelper.h"
#include "Utils/rtti/rtti.h"

namespace yame
{
namespace ytl
{

#ifdef LENT_WITH_LOGIC

template<typename T>
lent_ptr<T>::lent_ptr(T* i_data, tagged_reference_counter i_refCounter)
: m_refCounter(i_refCounter)
, m_data(i_data)
{
    if (m_refCounter)
    {
        m_refCounter->incrementWeakReference();
    }
}
template<typename T>
lent_ptr<T>::lent_ptr()
: m_refCounter(NULL)
, m_data(NULL)
{
}
template<typename T>
lent_ptr<T>::lent_ptr(const lent_ptr& other)
: m_refCounter(NULL)
, m_data(NULL)
{
    m_data = other.m_data;

    if(m_refCounter = other.m_refCounter)
    {
        m_refCounter->incrementWeakReference();
    }
}
template<typename T>
lent_ptr<T>::lent_ptr(lent_ptr&& other)
: m_refCounter(NULL)
, m_data(NULL)
{
    mpl::swapItems(m_data,other.m_data);
    mpl::swapItems(m_refCounter,other.m_refCounter);
}
template<typename T>
template<typename TT>
lent_ptr<T>::lent_ptr(const lent_ptr<TT>& other)
: m_refCounter(NULL)
, m_data(NULL)
{
    static_assert(mpl::is_base_of<T,TT>::value, "You shall provide inherited classes");

    m_data = other.m_data;

    if(m_refCounter = other.m_refCounter)
    {
        m_refCounter->incrementWeakReference();
    }
}
template<typename T>
template<typename TT>
lent_ptr<T>::lent_ptr(lent_ptr<TT>&& other)
: m_refCounter(NULL)
, m_data(NULL)
{
    static_assert(mpl::is_base_of<T,TT>::value, "You shall provide inherited classes");

    if(m_data = dynamic_cast<T*>(other.m_data))
    {
        mpl::swapItems(m_refCounter,other.m_refCounter);
    }

    other.m_data = NULL;
}
template<typename T>
template<typename TT>
lent_ptr<T>::lent_ptr(const unique_ptr<TT>& other)
: m_refCounter(NULL)
, m_data(NULL)
{
    static_assert(mpl::is_same_type<T,TT>::value || mpl::is_base_of<T,TT>::value, "You shall provide inherited classes");

    m_data = other.m_data;

    if(m_refCounter = other.m_refCounter)
    {
        m_refCounter->incrementWeakReference();
    }
}
template<typename T>
lent_ptr<T>::~lent_ptr()
{
    if(m_refCounter)
    {
        m_refCounter->decrementWeakReference();
    }
}
template<typename T>
lent_ptr<T>& lent_ptr<T>::operator=(const mpl::null_ptr_type&)
{
    if (m_refCounter.empty() == false)
    {
        m_refCounter->decrementWeakReference();
    }

    return *this;
}
template<typename T>
lent_ptr<T>& lent_ptr<T>::operator=(const lent_ptr& other)
{
    if(m_refCounter)
    {
        m_refCounter->decrementWeakReference();
    }

    m_data = other.m_data;

    if(m_refCounter = other.m_refCounter)
    {
        m_refCounter->incrementWeakReference();
    }

    return *this;
}
template<typename T>
lent_ptr<T>& lent_ptr<T>::operator=(lent_ptr&& other)
{
    if(m_refCounter)
    {
        m_refCounter->decrementWeakReference();
    }

    m_data = other.m_data;
    other.m_data = NULL;
    m_refCounter = other.m_refCounter;
    other.m_refCounter = NULL;

    return *this;
}
template<typename T>
template<typename TT>
lent_ptr<T>& lent_ptr<T>::operator=(const lent_ptr<TT>& other)
{
    static_assert(mpl::is_base_of<TT,T>::value, "You shall provide inherited classes");

    if(m_refCounter)
    {
        m_refCounter->decrementWeakReference();
    }

    m_data = other.m_data;

    if(m_refCounter = other.m_refCounter)
    {
        m_refCounter->incrementWeakReference();
    }

    return *this;
}
template<typename T>
template<typename TT>
lent_ptr<T>& lent_ptr<T>::operator=(lent_ptr<TT>&& other)
{
    static_assert(mpl::is_base_of<TT,T>::value, "You shall provide inherited classes");

    if(m_refCounter)
    {
        m_refCounter->decrementWeakReference();
    }

    m_data = other.m_data;

    other.m_data = NULL;
    m_refCounter = other.m_refCounter;
    other.m_refCounter = NULL;

    return *this;
}
template<typename T>
template<typename TT>
lent_ptr<T>& lent_ptr<T>::operator=(const unique_ptr<TT>& other)
{
    static_assert(mpl::is_base_of<T,TT>::value, "You shall provide inherited classes");

    if(m_refCounter)
    {
        m_refCounter->decrementWeakReference();
    }

    m_data = other.m_data;
    m_refCounter = other.m_refCounter;

    if (m_refCounter)
    {
        m_refCounter->incrementWeakReference();
    }

    return *this;
}
template<typename T>
bool lent_ptr<T>::operator==(mpl::null_ptr_type) const
{
    return m_data == NULL;
}
template<typename T>
bool lent_ptr<T>::operator==(const lent_ptr<T>& other) const
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
bool lent_ptr<T>::operator!=(mpl::null_ptr_type) const
{
    return m_data != NULL;
}
template<typename T>
bool lent_ptr<T>::operator!=(const lent_ptr<T>& other)
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
T* lent_ptr<T>::operator->()
{
    return m_data;
}
template<typename T>
const T* lent_ptr<T>::operator->() const
{
    return m_data;
}
template<typename T>
T& lent_ptr<T>::operator*()
{
    if(m_data == NULL)
    {
        MAKE_IT_CRASH
    }

    return *m_data;
}
template<typename T>
const T& lent_ptr<T>::operator*() const
{
    if(m_data == NULL)
    {
        MAKE_IT_CRASH
    }

    return *m_data;
}
template<typename T>
lent_ptr<T>::operator bool() const
{
    return m_data != NULL;
}
template<typename T>
void lent_ptr<T>::clear()
{
    m_data = NULL;

    if (m_refCounter)
    {
        m_refCounter->decrementWeakReference();

        m_refCounter = NULL;
    }
}
template<typename T>
T* lent_ptr<T>::getData()
{
    return m_data;
}
template<typename T>
const T* lent_ptr<T>::getData() const
{
    return m_data;
}
template<typename T>
typename lent_ptr<T>::tagged_reference_counter lent_ptr<T>::getReferenceCounter() const
{
    return m_refCounter;
}

template<typename T>
lent_ref<T>::lent_ref(T* i_data, tagged_reference_counter i_refCounter)
: lent_ptr<T>(i_data,i_refCounter)
{
}
template<typename T>
template<typename TT>
lent_ref<T>::lent_ref(const lent_ref<TT>& other)
: lent_ptr<T>(other)
{
}
template<typename T>
template<typename TT>
lent_ref<T>::lent_ref(lent_ref<TT>&& other)
: lent_ptr<T>(mpl::move(other))
{
}
template<typename T>
template<typename TT>
lent_ref<T>::lent_ref(const unique_ref<TT>& other)
: lent_ptr<T>(mpl::move(other))
{
}
template<typename T>
lent_ref<T>::lent_ref(T *pData, enable_ref_from_this& referenceCounter)
: lent_ptr<T>(pData,referenceCounter.getRefCounter())
{
}

template<typename T>
lent_ref<T> promote_to_ref(const lent_ptr<T>& other)
{
    return lent_ref<T>(other.getData(), other.getReferenceCounter());
}
template<typename TT, typename T>
lent_ptr<TT> dynamic_lent_cast(const lent_ptr<T>& other)
{
    static_assert(mpl::is_base_of<T,TT>::value || mpl::is_base_of<TT,T>::value, "You shall provide related classes by inheritance");
    static_assert(std::is_polymorphic<T>::value, "You shall work with polymprphic classes, try providing virtual destructor");

    if(T* pData = other.getData())
    {
        if(TT* innerData = rtti::dynamicCast<TT>(pData))
        {
            return lent_ref<TT>(innerData, other.getReferenceCounter());
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
lent_ptr<TT> static_lent_cast(const lent_ptr<T>& other)
{
    if(T* pData = other.getData())
    {
        TT* innerData = static_cast<TT*>(pData);

        return lent_ref<TT>(innerData, other.getReferenceCounter());
    }
    else
    {
        return null_ptr;
    }
}
template<typename TT, typename T>
lent_ptr<TT> dynamic_lent_cast(const lent_ref<T>& other)
{
    static_assert(mpl::is_base_of<T,TT>::value || mpl::is_base_of<TT,T>::value, "You shall provide related classes by inheritance");
    static_assert(std::is_polymorphic<T>::value, "You shall work with polymprphic classes, try providing virtual destructor");
    ASSERT(other.getData(), "You shall provide a non empty pointer as ref");

    if(TT* innerData = rtti::dynamicCast<TT>(other.getData()))
    {
        return lent_ref<TT>(innerData, other.getReferenceCounter());
    }
    else
    {
        return null_ptr;
    }
}
template<typename TT, typename T>
lent_ref<TT> static_lent_cast(const lent_ref<T>& other)
{
    ASSERT(other.getData(), "You shall provide a non empty pointer as ref");

    const TT* innerData = static_cast<const TT*>(other.getData());

    return lent_ref<TT>(const_cast<T*>(innerData), other.getReferenceCounter());
}
template<typename T>
lent_ptr<T> const_lent_cast(const lent_ptr<const T>& other)
{
    if(const T* pData = other.getData())
    {
        T* innerData = const_cast<T*>(pData);

        return lent_ref<T>(innerData, other.getReferenceCounter());
    }
    else
    {
        return null_ptr;
    }
}
template<typename T>
lent_ref<T> const_lent_cast(const lent_ref<const T>& other)
{
    ASSERT(other.getData(), "You shall provide a non empty ref");
    T* innerData = const_cast<T*>(other.getData());

    return lent_ref<T>(innerData, other.lend());
}
template<typename T>
lent_ref<T> lend(T& other, const typename T::lendable_tag* _foo)
{
    typedef typename mpl::drop_constness<T>::type no_const_type;
    no_const_type& otherNoConst = const_cast<no_const_type&>(other);

    return lent_ref<T>(&otherNoConst,otherNoConst.ref_from_this());
}

#else

template<typename T>
lent_ref<T> promote_to_ref(const lent_ptr<T>& other)
{
    return other;
}
template<typename TT, typename T>
lent_ptr<TT> dynamic_lent_cast(const lent_ptr<T>& other)
{
    static_assert(mpl::is_base_of<T,TT>::value, "You shall provide related classes by inheritance");
    static_assert(std::is_polymorphic<T>::value, "You shall work with polymprphic classes, try providing virtual destructor");

    if(other)
    {
        return rtti::dynamicCast<TT>(other);
    }
    else
    {
        return NULL;
    }
}
template<typename TT, typename T>
lent_ptr<TT> static_lent_cast(const lent_ptr<T>& other)
{
    return rtti::staticCast<TT>(other);
}
template<typename T>
lent_ptr<T> const_lent_cast(const lent_ptr<const T>& other)
{
    if(other)
    {
        return const_cast<T*>(other);
    }
    else
    {
        return NULL;
    }
}
template<typename T>
lent_ref<T> lend(T& other, const typename T::lendable_tag* _foo)
{
    typedef typename mpl::drop_constness<T>::type no_const_type;

    return const_cast<no_const_type*>(&other);
}

#endif

}
}
