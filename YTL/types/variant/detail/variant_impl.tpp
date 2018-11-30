#include "visitor_invoker.h"

#define CREATE_INNER_VISITOR(_visitor) \
	typedef decltype(_visitor) Visitor; \
		typedef typename Visitor::result_type result_type; \
    \
    typedef result_type(*_funcType)(Visitor&, const variant_impl<Types...>&); \
    \
    Visitor& _invoker = _visitor; \
    \
    static const _funcType _funcTable[] = { &variant_visitor_invoker<Visitor, Types...>::template inner_invoke<Types> ... };

#define CALL_INNER_VISITOR(_variant) \
	(*_funcTable[(_variant).which()])(_invoker,_variant);

namespace yame
{
namespace ytl
{
namespace detail
{

template<typename ... Types>
variant_impl<Types...>::variant_impl()
: m_currentType(_numTypes)
{
}
template<typename ... Types>
void variant_impl<Types...>::construct(const variant_impl<Types...>& other)
{
    m_currentType = other.m_currentType;

    if (other.m_currentType < _numTypes)
    {
        constructor_visitor<Types...> ctr(&m_storage);

        CREATE_INNER_VISITOR(ctr);
        CALL_INNER_VISITOR(other);
    }
}
template<typename ... Types>
void variant_impl<Types...>::construct(variant_impl<Types...>&& other)
{
    m_currentType = other.m_currentType;

    if (other.m_currentType < _numTypes)
    {
        constructor_visitor<Types...> ctr(&m_storage);

        CREATE_INNER_VISITOR(ctr);
        CALL_INNER_VISITOR(other);
    }
}

template<typename ... Types>
template<typename Type, typename TType>
requires (mpl::is_variant<TType>::value == false)
void variant_impl<Types...>::construct(TType&& other)
{
    static const size_t _pos = mpl::nth_pos_of_type<Type,Types...>::value;

    static_assert(_pos >= 0 && _pos < _numTypes, "Not found type!");

    m_currentType = _pos;

    constructor_visitor<Types...>::template construct<_pos>(&m_storage, mpl::forward<TType>(other));
}

template<typename ... Types>
void variant_impl<Types...>::destroy()
{
    if (m_currentType < _numTypes)
    {
        destructor_visitor<Types...> dtr(&m_storage);

        CREATE_INNER_VISITOR(dtr);
        CALL_INNER_VISITOR(*this);

        m_currentType = _numTypes;
    }
}

template<typename ... Types>
variant_impl<Types...>::~variant_impl()
{
    destroy();
}

template<typename ... Types>
variant_impl<Types...>& variant_impl<Types...>::operator=(const variant_impl<Types...>& other)
{
    if (other.m_currentType < _numTypes)
    {
        if (m_currentType == other.m_currentType)
        {
            assigner_visitor<Types...> ctr(&m_storage);

            CREATE_INNER_VISITOR(ctr);
            CALL_INNER_VISITOR(other);
        }
        else
        {
            destroy();

            constructor_visitor<Types...> ctr(&m_storage);

            CREATE_INNER_VISITOR(ctr);
            CALL_INNER_VISITOR(other);

            m_currentType = other.m_currentType;
        }
    }
    else
    {
        destroy();
    }

    return *this;
}

template<typename ... Types>
variant_impl<Types...>& variant_impl<Types...>::operator=(variant_impl<Types...>&& other)
{
    if (other.m_currentType < _numTypes)
    {
        if (m_currentType == other.m_currentType)
        {
            assigner_visitor<Types...> ctr(&m_storage);

            CREATE_INNER_VISITOR(ctr);
            CALL_INNER_VISITOR(other);
        }
        else
        {
            destroy();

            constructor_visitor<Types...> ctr(&m_storage);

            CREATE_INNER_VISITOR(ctr);
            CALL_INNER_VISITOR(other);

            m_currentType = other.m_currentType;
        }
    }
    else
    {
        destroy();
    }

    return *this;
}

template<typename ... Types>
template<typename Type, typename TType>
requires (mpl::is_variant<TType>::value == false)
variant_impl<Types...>& variant_impl<Types...>::assign(TType&& val)
{
    static const size_t _pos = mpl::nth_pos_of_type<Type, Types...>::value;

    static_assert(_pos >= 0 && _pos < _numTypes, "Type out of bounds!");

    if (_pos != m_currentType)
    {
        destroy();

        constructor_visitor<Types...>::template construct<_pos>(&m_storage, mpl::forward<TType>(val));

        m_currentType = _pos;
    }
    else if (m_currentType < _numTypes)
    {
        //just an assignment
        assigner_visitor<Types...>::template assign<_pos>(&m_storage, mpl::forward<TType>(val));
    }

    return *this;
}

template<typename ... Types>
bool variant_impl<Types...>::operator==(const variant_impl<Types...>& other) const
{
    bool res = false;

    if (m_currentType == other.m_currentType)
    {
        if(m_currentType != _numTypes)
        {
            comparison_visitor<Types...,variant_impl<Types...> > comparator(*this);

            CREATE_INNER_VISITOR(comparator);
            res = CALL_INNER_VISITOR(other);
        }
        else
        {
            //empty case, valid case (both were empty)
            res = true;
        }
    }

    return res;
}

template<typename ... Types>
bool variant_impl<Types...>::operator==(variant_impl<Types...>&& other) const
{
    bool res = false;

    if (m_currentType == other.m_currentType)
    {
        comparison_visitor<Types...,variant_impl<Types...> > comparator(*this);

        CREATE_INNER_VISITOR(comparator);
        res = CALL_INNER_VISITOR(other);
    }

    return res;
}

template<typename ... Types>
bool variant_impl<Types...>::operator!=(const variant_impl<Types...>& other) const
{
    bool res = true;

    if (m_currentType == other.m_currentType)
    {
        comparison_visitor<Types...,variant_impl<Types...> > comparator(*this);

        CREATE_INNER_VISITOR(comparator);
        res = !CALL_INNER_VISITOR(other);
    }

    return res;
}

template<typename ... Types>
bool variant_impl<Types...>::operator!=(variant_impl<Types...>&& other) const
{
    bool res = true;

    if (m_currentType == other.m_currentType)
    {
        comparison_visitor<Types...,variant_impl<Types...> > comparator(*this);

        CREATE_INNER_VISITOR(comparator);
        res = !CALL_INNER_VISITOR(other);
    }

    return res;
}

template<typename ... Types>
template<typename Type, typename TType>
requires (mpl::is_variant<TType>::value == false)
bool variant_impl<Types...>::compare(TType&& other) const
{
    bool res = false;

    static const size_t typePos = mpl::nth_pos_of_type<Type, Types...>::value;

    if (m_currentType == typePos)
    {
        typedef typename mpl::nth_type_of<typePos, Types...>::type currType;
        typedef typename embedded_type<currType>::ref_type ref_type;

        val_retriever_visitor<Types...,currType> getter;

        CREATE_INNER_VISITOR(getter);
        ref_type thisVal = CALL_INNER_VISITOR(*this);

        res = (thisVal == mpl::forward<TType>(other));
    }

    return res;
}

template<typename ... Types>
template<typename TType>
typename embedded_type<TType>::ref_type variant_impl<Types...>::get() const
{
    ASSERT(is<TType>(), "Accessing non current type!");

    typedef typename embedded_type<TType>::ref_type retType;

    //ok, get it!
    embedded_type<TType> *_data = reinterpret_cast<embedded_type<TType> *>(const_cast<data_type *>(&m_storage));

    ASSERT(_data, "Accessing null data!");

    return mpl::forward<retType>(_data->get());
}

template<typename ... Types>
template<typename TType>
TType variant_impl<Types...>::extract()
{
    // do a local copy and forward it
    typedef typename embedded_type<TType>::rref_type retType;
    typedef typename embedded_type<TType>::internal_type rawType;

    //ok, get it!
    embedded_type<TType> *_data = reinterpret_cast<embedded_type<TType> *>(const_cast<data_type *>(&m_storage));

    ASSERT(_data, "Accessing null data!");

    rawType res = _data->extract();

    destroy();

    return mpl::forward<retType>(res);
}

template<typename ... Types>
bool variant_impl<Types...>::empty() const
{
    return m_currentType == _numTypes;
}

template<typename ... Types>
template<typename TType>
bool variant_impl<Types...>::is() const
{
    static_assert(mpl::is_among_types<TType, Types...>::value, "Non present type in variant");

    static const size_t isPos = mpl::nth_pos_of_type<TType, Types...>::value;

    return m_currentType == isPos;
}

template<typename ... Types>
uint8_t variant_impl<Types...>::which() const
{
    return m_currentType;
}

template<typename ... Types>
void variant_impl<Types...>::reset()
{
    //just reset value
    if (m_currentType < _numTypes)
    {
        destructor_visitor<Types...> dtr(&m_storage);

        CREATE_INNER_VISITOR(dtr);
        CALL_INNER_VISITOR(*this);

        m_currentType = _numTypes;
    }

    return;
}

template<typename ... Types>
void variant_impl<Types...>::swap(variant_impl<Types...>& other)
{
    if (m_currentType < _numTypes && other.m_currentType < _numTypes)
    {
        //as in copy construction case, just check types are the same or not
        if (m_currentType == other.m_currentType)
        {
            swaper_visitor<Types...,variant_impl<Types...> > letsSwap(other, *this);

            CREATE_INNER_VISITOR(letsSwap);
            CALL_INNER_VISITOR(*this);
        }
        else
        {
            //mmm, we will have to reconstruct types (at least, do it with move semantics in case we may save some allocations)

            //construction of tmp
            variant_impl<Types...> tmp;
            constructor_visitor<Types...> ctr(&tmp.m_storage);
            tmp.m_currentType = m_currentType;
            CREATE_INNER_VISITOR(ctr);
            CALL_INNER_VISITOR(*this);

            //reconstruction of us
            *this = other;

            //reconstruction of other
            other = tmp;
        }
    }
    else if (m_currentType < _numTypes)
    {
        //construct us with other data
        constructor_visitor<Types...> ctr(&(other.m_storage));
        CREATE_INNER_VISITOR(ctr);
        CALL_INNER_VISITOR(*this);
    }
    else if (other.m_currentType < _numTypes)
    {
        //construct other with our data
        constructor_visitor<Types...> ctr(&m_storage);
        CREATE_INNER_VISITOR(ctr);
        CALL_INNER_VISITOR(other);
    }

    return;
}

template<typename ... Types>
template<typename Visitor>
typename Visitor::result_type variant_impl<Types...>::apply_visitor(const Visitor& visitor)
{
    return variant_visitor_invoker<Visitor, Types...>::template invoke<Visitor>(m_currentType, const_cast<Visitor&>(visitor), *this, nullptr);
}

template<typename ... Types>
template<typename Visitor>
typename Visitor::result_type variant_impl<Types...>::apply_visitor(const Visitor& visitor) const
{
    return variant_visitor_invoker<Visitor, Types...>::template invoke<Visitor>(m_currentType, visitor, *this, nullptr);
}
}
}
}
