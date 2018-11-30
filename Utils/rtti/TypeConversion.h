#pragma once

#include "YTL/types/optional/optional.h"
#include "YTL/container/cPair.h"
#include "YTL/mpl/cTemplateHelper.h"

#pragma GCC diagnostic ignored "-Wnon-template-friend"

namespace yame
{
namespace rtti
{
namespace detail
{

template<typename T, typename TT>
struct rtti_conversor_tag
{
    friend constexpr std::ptrdiff_t _relate_static(const rtti_conversor_tag<T,TT>&);
    friend constexpr std::ptrdiff_t _relate_static(const rtti_conversor_tag<TT,T>&);
    friend std::ptrdiff_t _relate_dynamic(const rtti_conversor_tag<T,TT>&);
    friend constexpr bool single_relator_instance(const rtti_conversor_tag<T,TT>&);

    constexpr rtti_conversor_tag() = default;
    rtti_conversor_tag(T* i_refPtr)
    : m_refPtr(i_refPtr)
    , m_relatedPtr(NULL)
    {
    }
    rtti_conversor_tag(T* i_refPtr, const char* i_relatedPtr)
    : m_refPtr(i_refPtr)
    , m_relatedPtr(i_relatedPtr)
    {
    }

    T* m_refPtr = NULL;
    const char* m_relatedPtr = NULL;
};

template<typename T, typename TT>
rtti_conversor_tag<T,TT> relateTag(const rtti_conversor_tag<T,TT>& other)
{
    return rtti_conversor_tag<T,TT>(other.m_refPtr,other.m_refPtr->_rtti_layout().first);
}

template<typename T, typename TT, typename Joint>
struct rtti_static_conversor;

template<typename T, typename TT>
struct rtti_static_conversor<T,TT,void>
{
    static_assert(mpl::is_base_of<TT,T>::value, "You shall provide inherited classes");

    constexpr static const T* s_rhsPtr = mpl::instantiatePointer<T>::value;//reinterpret_cast<T*>(0xDEAD);
    constexpr static const TT* s_lhsPtr = s_rhsPtr;

    friend constexpr std::ptrdiff_t _relate_static(const rtti_conversor_tag<T,TT>&)
    {
        return reinterpret_cast<const char*>(s_lhsPtr) - reinterpret_cast<const char*>(s_rhsPtr);
    }
    friend constexpr std::ptrdiff_t _relate_static(const rtti_conversor_tag<TT,T>&)
    {
        return reinterpret_cast<const char*>(s_rhsPtr) - reinterpret_cast<const char*>(s_lhsPtr);
    }
    friend constexpr bool single_relator_instance(const rtti_conversor_tag<T,TT>&)
    {
        return true;
    }
    static const bool _expansion = single_relator_instance(rtti_conversor_tag<T,TT>{});
};

template<typename T, typename TT, typename Joint>
struct rtti_static_conversor
{
    static_assert(mpl::is_base_of<T,Joint>::value, "You shall provide inherited classes");
    static_assert(mpl::is_base_of<TT,Joint>::value, "You shall provide inherited classes");

    constexpr static const Joint* s_refPtr = mpl::instantiatePointer<Joint>::value;//reinterpret_cast<Joint*>(0xDEAD);
    constexpr static const T* s_lhsPtr = s_refPtr;
    constexpr static const TT* s_rhsPtr = s_refPtr;

    friend constexpr std::ptrdiff_t _relate_static(const rtti_conversor_tag<T,TT>&)
    {
        return (reinterpret_cast<const char*>(s_refPtr) - reinterpret_cast<const char*>(s_lhsPtr)) - (reinterpret_cast<const char*>(s_refPtr) - reinterpret_cast<const char*>(s_rhsPtr));
    }
    friend constexpr bool single_relator_instance(const rtti_conversor_tag<T,TT>&)
    {
        return true;
    }
    static const bool _expansion = single_relator_instance(rtti_conversor_tag<T,TT>{});
};

template<typename T, typename TT, typename Joint>
struct rtti_dynamic_conversor;

template<typename T, typename TT>
struct rtti_dynamic_conversor<T,TT,void>
{
    static_assert(mpl::is_base_of<TT,T>::value, "You shall provide inherited classes");

    friend std::ptrdiff_t _relate_dynamic(const rtti_conversor_tag<T,TT>& i_tag)
    {
        const T* s_rhsPtr = i_tag.m_refPtr;
        const TT* s_lhsPtr = s_rhsPtr;

        return reinterpret_cast<const char*>(s_lhsPtr) - reinterpret_cast<const char*>(s_rhsPtr);
    }
    friend constexpr bool single_relator_instance(const rtti_conversor_tag<T,TT>&)
    {
        return true;
    }
    static const bool _expansion = single_relator_instance(rtti_conversor_tag<T,TT>{});
};

template<typename T, typename TT, typename Joint>
struct rtti_dynamic_conversor
{
    static_assert(mpl::is_base_of<T,Joint>::value, "You shall provide inherited classes");
    static_assert(mpl::is_base_of<TT,Joint>::value, "You shall provide inherited classes");

    friend std::ptrdiff_t _relate_dynamic(const rtti_conversor_tag<T,TT>& i_tag)
    {
        const Joint* _refPtr = reinterpret_cast<const Joint*>(i_tag.m_relatedPtr);
        const TT* _lhsPtr = _refPtr;
        return reinterpret_cast<const char*>(_lhsPtr) - reinterpret_cast<const char*>(i_tag.m_refPtr);
    }
    friend constexpr bool single_relator_instance(const rtti_conversor_tag<T,TT>&)
    {
        return true;
    }
    static const bool _expansion = single_relator_instance(rtti_conversor_tag<T,TT>{});
};

template<typename T, typename TT, typename Joint>
using __expand_template = typename mpl::static_if<mpl::is_virtual_base_of<T,TT>::value || mpl::is_virtual_base_of<TT,T>::value,rtti_dynamic_conversor<T,TT,Joint>,rtti_static_conversor<T,TT,Joint>>::type;
struct __do_not_expand_template { static const bool _expansion = true; };

template<typename Joint, typename T, typename TT, bool = single_relator_instance(rtti_conversor_tag<T,TT>{})>
__do_not_expand_template check(int, const rtti_conversor_tag<T,TT>&);
template<typename Joint, typename T, typename TT, typename ... Types>
__expand_template<T,TT,Joint> check(float, const rtti_conversor_tag<T,TT>&);

template<typename T, typename TT, typename Joint = void>
struct rtti_conversor : public decltype(check<Joint>(0,rtti_conversor_tag<T,TT>{}))
{
};

template<bool,typename,typename>
struct _type_relator;

template<typename T, typename TT>
struct _type_relator<false,T,TT>
{
    inline constexpr static std::ptrdiff_t relate(const rtti_conversor_tag<T,TT>& i_value)
    {
        return _relate_static(i_value);
    }
};

template<typename T, typename TT>
struct _type_relator<true,T,TT>
{
    inline static std::ptrdiff_t relate(const rtti_conversor_tag<T,TT>& i_tag)
    {
        return _relate_dynamic(relateTag(i_tag));
    }
};

template<typename T, typename TT>
struct type_relator : public _type_relator<mpl::is_virtual_base_of<T,TT>::value || mpl::is_virtual_base_of<TT,T>::value,T,TT>
{
    using _type_relator<mpl::is_virtual_base_of<T,TT>::value || mpl::is_virtual_base_of<TT,T>::value,T,TT>::relate;
};

}
}
}
