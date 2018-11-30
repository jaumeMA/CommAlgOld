#pragma once

namespace yame
{
namespace container
{
namespace detail
{

enum ReferenceCategory
{
    NonConstReference = 0,
    ConstReference = 1,
    Value = 2
};

template<typename>
struct deduceCategoryFromType;

template<typename T>
struct deduceCategoryFromType<const T&>
{
    static const ReferenceCategory category = ReferenceCategory::ConstReference;
};

template<typename T>
struct deduceCategoryFromType<T&>
{
    static const ReferenceCategory category = ReferenceCategory::NonConstReference;
};

template<typename T>
struct deduceCategoryFromType
{
    static const ReferenceCategory category = ReferenceCategory::Value;
};

template<typename,ReferenceCategory>
struct getReference;

template<typename TT>
struct getReference<TT,ReferenceCategory::NonConstReference>
{
    typedef typename mpl::add_reference<TT>::type type;
    static const ReferenceCategory value = NonConstReference;
};
template<typename TT>
struct getReference<TT,ReferenceCategory::ConstReference>
{
    typedef typename mpl::add_constness<typename mpl::add_reference<TT>::type>::type type;
    static const ReferenceCategory value = ConstReference;
};
template<typename TT>
struct getReference<TT,ReferenceCategory::Value>
{
    typedef typename mpl::drop_reference<typename mpl::drop_constness<TT>::type>::type type;
    static const ReferenceCategory value = Value;
};

template<typename,ReferenceCategory>
struct getPointer;

template<typename TT>
struct getPointer<TT,ReferenceCategory::NonConstReference>
{
    typedef typename mpl::add_pointer<TT>::type type;
    static const ReferenceCategory value = NonConstReference;
};
template<typename TT>
struct getPointer<TT,ReferenceCategory::ConstReference>
{
    typedef typename mpl::add_constness<typename mpl::add_pointer<TT>::type>::type type;
    static const ReferenceCategory value = ConstReference;
};
template<typename TT>
struct getPointer<TT,ReferenceCategory::Value>
{
    typedef typename mpl::drop_pointer<typename mpl::drop_constness<TT>::type>::type type;
    static const ReferenceCategory value = Value;
};

}
}
}
