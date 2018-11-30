#pragma once

#include "YTL/variant/static_visitor.h"

namespace yame
{
namespace ytl
{
namespace detail
{

template<typename ... Types>
struct constructor_visitor : public static_visitor<void>
{
    typedef constructor_visitor t_visitor;

    template<typename Type>
    struct _constructor;

    template<typename Type>
    struct _constructor
    {
        template<typename TType>
        static void construct(data_type* address, TType&& val);
    };

    constructor_visitor(void *address = NULL);

    template<size_t _pos, typename Type>
    static void construct(data_type* address, Type&& val);

    template<typename Type, typename T>
    void operator()(T&& visit);

private:
    void* m_address;
};

template<typename ... Types>
struct destructor_visitor : public static_visitor<void>
{
    typedef destructor_visitor t_visitor;

    destructor_visitor(void *address = NULL);

    template<typename Type, typename T>
    void operator()(T&&);

    template<size_t _pos>
    void destroy();

private:
    void* m_address;
};

template<typename ... Types>
struct assigner_visitor : public static_visitor<void>
{
    typedef assigner_visitor t_visitor;

    template<typename Type>
    struct _assigner
    {
        template<typename TType>
        static void assign(data_type* address, TType&& val);
    };

    assigner_visitor(void *address = nullptr);

    template<size_t _pos, typename Type>
    static void assign(data_type* address, Type&& val);

    template<typename Type, typename T>
    void operator()(T&& visit);

private:
    void* m_address;
};

template<typename ... Types>
template<typename retType>
struct val_retriever_visitor : public static_visitor <typename embedded_type<retType>::ref_type>
{
    typedef val_retriever_visitor t_visitor;
    typedef typename mpl::drop_constness<typename embedded_type<retType>::internal_type>::type rawType;

    template<typename Type>
    typename embedded_type<retType>::ref_type operator()(typename embedded_type<retType>::ref_type val);

    //for the rest of unsused types
    template<typename Type, typename T>
    typename embedded_type<retType>::ref_type operator()(T&& val, rawType* _foo = 0);
};

template<typename ... Types, typename Variant>
struct swaper_vsitor : public static_visitor<void>
{
    typedef swaper_vsitor t_visitor;

    swaper_vsitor(Variant& _thisVariant, Variant& _otherVariant);

    template<typename Type, typename T>
    void operator()(T&& otherVal);

private:
    Variant& m_thisVariant;
    Variant& m_otherVariant;
};

template<typename ... Types, typename Variant>
struct comparison_visitor : public static_visitor<bool>
{
    typedef comparison_visitor t_visitor;

    comparison_visitor(const Variant& _variant);

    template<typename Type, typename T>
    bool operator()(T&& otherVal);

private:
    const Variant& m_variant;
};

}
}
}
