
#include "System/cException.h"

namespace yame
{
namespace ytl
{
namespace detail
{

template<typename Return, typename ... Types>
template<int ... specIndexs, int ... notSpecIndexs, int ... totalIndexs>
template<typename ... Args>
function_impl_base<Return, container::parameter_pack<Types...>>::specialized_impl<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>,mpl::sequence<totalIndexs...>>::specialized_impl(TaggedFuncPtr i_object, Args&& ... i_args)
: m_object(i_object)
, m_specArgs(mpl::forward<typename mpl::nth_type_of<specIndexs,Args...>::type>(mpl::nth_val_of<specIndexs>(mpl::forward<Args>(i_args) ...)) ...)
{}
template<typename Return, typename ... Types>
template<int ... specIndexs, int ... notSpecIndexs, int ... totalIndexs>
function_impl_base<Return, container::parameter_pack<Types...>>::specialized_impl<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>,mpl::sequence<totalIndexs...>>::specialized_impl(TaggedFuncPtr i_object, const typename mpl::get_sub_parameter_pack<Types...>::template at<specIndexs...>::type& i_specArgs)
: m_object(i_object)
, m_specArgs(i_specArgs)
{}
template<typename Return, typename ... Types>
template<int ... specIndexs, int ... notSpecIndexs, int ... totalIndexs>
function_impl_base<Return, container::parameter_pack<Types...>>::specialized_impl<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>,mpl::sequence<totalIndexs...>>::specialized_impl(TaggedFuncPtr i_object, typename mpl::get_sub_parameter_pack<Types...>::template at<specIndexs...>::type&& i_specArgs)
: m_object(i_object)
, m_specArgs(mpl::move(i_specArgs))
{}
template<typename Return, typename ... Types>
template<int ... specIndexs, int ... notSpecIndexs, int ... totalIndexs>
function_impl_base<Return, container::parameter_pack<Types...>>::specialized_impl<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>,mpl::sequence<totalIndexs...>>::~specialized_impl()
{
    m_object.destroy();
}
template<typename Return, typename ... Types>
template<int ... specIndexs, int ... notSpecIndexs, int ... totalIndexs>
Return function_impl_base<Return, container::parameter_pack<Types...>>::specialized_impl<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>,mpl::sequence<totalIndexs...>>::operator()(typename mpl::nth_type_of<notSpecIndexs, Types...>::type ... args) const
{
    typedef typename mpl::sequence<notSpecIndexs...> compl_ranks;
    typedef typename mpl::sequence<specIndexs...>::template add<compl_ranks>::type merged_rank;

    return m_object->operator()(mpl::nth_val_of<merged_rank::template get_relative_num<totalIndexs>::value>(m_specArgs.template getValue<mpl::sequence<specIndexs...>::template get_relative_num<specIndexs>::value>() ..., mpl::forward<typename mpl::nth_type_of<notSpecIndexs, Types...>::type>(args) ...)...);
}
template<typename Return, typename ... Types>
template<int ... specIndexs, int ... notSpecIndexs, int ... totalIndexs>
tagged_pointer<function_impl_base<Return, typename mpl::get_sub_parameter_pack<Types...>::template not_at<specIndexs...>::type>> function_impl_base<Return, container::parameter_pack<Types...>>::specialized_impl<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>,mpl::sequence<totalIndexs...>>::_clone(void* i_arena, size_t i_arenaSize) const
{
    typedef specialized_impl<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>,mpl::sequence<totalIndexs...>> retType;
    static const size_t classSize = sizeof(retType);
    static const bool localInvariance = mpl::is_local_invariant<typename mpl::nth_type_of<specIndexs,Types...>::type ...>::value;

    ASSERT(i_arena != NULL || i_arenaSize == 0, "Unconsitent arena provided");

    if(localInvariance && classSize <= i_arenaSize)
    {
        return new (i_arena) retType(m_object->_clone((u8*) i_arena + classSize, i_arenaSize - classSize), m_specArgs);
    }
    else
    {
        return tagged_pointer<retType>(new retType(m_object->_clone(i_arena, 0), m_specArgs), 0x01);
    }
}
template<typename Return, typename ... Types>
template<int ... specIndexs, int ... notSpecIndexs, int ... totalIndexs>
tagged_pointer<function_impl_base<Return, typename mpl::get_sub_parameter_pack<Types...>::template not_at<specIndexs...>::type>> function_impl_base<Return, container::parameter_pack<Types...>>::specialized_impl<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>,mpl::sequence<totalIndexs...>>::clone(void* i_arena, size_t i_arenaSize) const
{
    typedef specialized_impl<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>,mpl::sequence<totalIndexs...>> retType;
    static const size_t classSize = sizeof(retType);
    static const bool localInvariance = mpl::is_local_invariant<typename mpl::nth_type_of<specIndexs,Types...>::type ...>::value;

    ASSERT(i_arena != NULL || i_arenaSize == 0, "Unconsitent arena provided");

    if(localInvariance && classSize <= i_arenaSize)
    {
        new (i_arena) retType(m_object->_clone((u8*) i_arena + classSize, i_arenaSize - classSize), m_specArgs);
    }
    else
    {
        return i_arena << tagged_pointer<retType>(new retType(m_object->_clone(i_arena, 0), m_specArgs),0x01);
    }
}


template<typename Return, typename ... Types>
template<typename ... Args>
tagged_pointer<function_impl_base<Return, typename mpl::get_sub_parameter_pack<Types...>::template at_seq<typename mpl::get_pos_of_type<mpl::place_holder, mpl::is_type_constructible>::template at<Args...>::type>::type>> function_impl_base<Return, container::parameter_pack<Types...>>::specialize(void* i_arena, size_t i_arenaSize, Args&& ... args) const
{
    static const int numRanks = mpl::get_num_types<Types...>::value;
    typedef specialized_impl<typename mpl::get_pos_of_not_type<mpl::place_holder, mpl::is_type_constructible>::template at<Args&&...>::type,typename mpl::get_pos_of_type<mpl::place_holder, mpl::is_type_constructible>::template at<Args&&...>::type, typename mpl::create_range_rank<0,numRanks>::type> retType;
    static const size_t classSize = sizeof(retType);
    static const bool localInvariance = mpl::is_local_invariant<retType>::value;

    ASSERT(i_arena != NULL || i_arenaSize == 0, "Unconsitent arena provided");

    if(localInvariance && classSize <= i_arenaSize)
    {
        new (i_arena) retType(_clone((u8*) i_arena + classSize, i_arenaSize - classSize), mpl::forward<Args>(args)...);
    }
    else
    {
        return i_arena << tagged_pointer<retType>(new retType(_clone(i_arena, 0), mpl::forward<Args>(args)...),0x01);
    }
}
template<typename Return, typename ... Types>
template<int ... ArgIndexs, typename ... Args>
tagged_pointer<function_impl_base<Return, typename mpl::get_sub_parameter_pack<Types...>::template not_at<ArgIndexs...>::type>> function_impl_base<Return, container::parameter_pack<Types...>>::specializeAt(void* i_arena, size_t i_arenaSize, Args&& ... args) const
{
    static const int numRanks = mpl::get_num_types<Types...>::value;
    typedef specialized_impl<mpl::sequence<ArgIndexs...>,typename mpl::create_range_rank_compl<0,numRanks,ArgIndexs...>::type,typename mpl::create_range_rank<0,numRanks>::type> retType;
    static const size_t classSize = sizeof(retType);
    static const bool localInvariance = mpl::is_local_invariant<retType>::value;

    ASSERT(i_arena != NULL || i_arenaSize == 0, "Unconsitent arena provided");

    if(localInvariance && classSize <= i_arenaSize)
    {
        new (i_arena) retType(_clone((u8*) i_arena + classSize, i_arenaSize - classSize), mpl::forward<Args>(args)...);
    }
    else
    {
        return i_arena << tagged_pointer<retType>(new retType(_clone(i_arena, 0), mpl::forward<Args>(args)...),0x01);
    }
}


template<typename ObjectType, typename Return, typename ... Types>
relative_function_impl<ObjectType,Return,Types...>::relative_function_impl(ObjectType* i_object, FuncPointerType i_funcPointer)
: m_object(i_object)
, m_funcPointer(i_funcPointer)
{}
template<typename ObjectType, typename Return, typename ... Types>
Return relative_function_impl<ObjectType,Return,Types...>::operator()(Types ... args) const
{
    return (m_object->*m_funcPointer)(mpl::forward<Types>(args)...);
}
template<typename ObjectType, typename Return, typename ... Types>
tagged_pointer<function_impl_base<Return, container::parameter_pack<Types...>>> relative_function_impl<ObjectType,Return,Types...>::clone(void* i_arena, size_t i_arenaSize) const
{
    static const size_t classSize = sizeof(relative_function_impl<ObjectType,Return,Types...>);

    ASSERT(i_arena != NULL || i_arenaSize == 0, "Unconsitent arena provided");

    if(classSize <= i_arenaSize)
    {
        new (i_arena) relative_function_impl<ObjectType,Return,Types...>(m_object, m_funcPointer);
    }
    else
    {
        typedef relative_function_impl<ObjectType,Return,Types...> retType;

        return i_arena << tagged_pointer<retType>(new retType(m_object, m_funcPointer),0x01);
    }
}
template<typename ObjectType, typename Return, typename ... Types>
tagged_pointer<function_impl_base<Return, container::parameter_pack<Types...>>> relative_function_impl<ObjectType,Return,Types...>::_clone(void* i_arena, size_t i_arenaSize) const
{
    static const size_t classSize = sizeof(relative_function_impl<ObjectType,Return,Types...>);

    ASSERT(i_arena != NULL || i_arenaSize == 0, "Unconsitent arena provided");

    if(classSize <= i_arenaSize)
    {
        return new (i_arena) relative_function_impl<ObjectType,Return,Types...>(m_object, m_funcPointer);
    }
    else
    {
        typedef relative_function_impl<ObjectType,Return,Types...> retType;

        return tagged_pointer<retType>(new retType(m_object, m_funcPointer), 0x01);
    }
}


template<typename Return, typename ... Types>
free_function_impl<Return,Types...>::free_function_impl(FuncPointerType i_funcPointer)
: m_funcPointer(i_funcPointer)
{}
template<typename Return, typename ... Types>
Return free_function_impl<Return,Types...>::operator()(Types ... args) const
{
    return (*m_funcPointer)(mpl::forward<Types>(args)...);
}
template<typename Return, typename ... Types>
tagged_pointer<function_impl_base<Return, container::parameter_pack<Types...>>> free_function_impl<Return,Types...>::clone(void* i_arena, size_t i_arenaSize) const
{
    static const size_t classSize = sizeof(free_function_impl<Return,Types...>);

    ASSERT(i_arena != NULL || i_arenaSize == 0, "Unconsitent arena provided");

    if(classSize <= i_arenaSize)
    {
        new (i_arena) free_function_impl<Return,Types...>(m_funcPointer);
    }
    else
    {
        typedef free_function_impl<Return,Types...> retType;

        return i_arena << tagged_pointer<retType>(new retType(m_funcPointer),0x01);
    }
}
template<typename Return, typename ... Types>
tagged_pointer<function_impl_base<Return, container::parameter_pack<Types...>>> free_function_impl<Return,Types...>::_clone(void* i_arena, size_t i_arenaSize) const
{
    static const size_t classSize = sizeof(free_function_impl<Return,Types...>);

    ASSERT(i_arena != NULL || i_arenaSize == 0, "Unconsitent arena provided");

    if(classSize <= i_arenaSize)
    {
        return new (i_arena) free_function_impl<Return,Types...>(m_funcPointer);
    }
    else
    {
        typedef free_function_impl<Return,Types...> retType;

        return tagged_pointer<retType>(new retType(m_funcPointer), 0x01);
    }
}
template<typename Return, typename ... Types>
typename free_function_impl<Return,Types...>::FuncPointerType free_function_impl<Return,Types...>::getFuncAddr() const
{
    return m_funcPointer;
}


template<typename T, typename Return, typename ... Types>
functor_impl<T,Return,Types...>::functor_impl(const T& i_functor)
: m_functor(i_functor)
{
}
template<typename T, typename Return, typename ... Types>
Return functor_impl<T,Return,Types...>::operator()(Types ... args) const
{
    return (m_functor)(mpl::forward<Types>(args)...);
}
template<typename T, typename Return, typename ... Types>
tagged_pointer<function_impl_base<Return, container::parameter_pack<Types...>>> functor_impl<T,Return,Types...>::clone(void* i_arena, size_t i_arenaSize) const
{
    static const size_t classSize = sizeof(functor_impl<T,Return,Types...>);
    static const bool localInvariance = mpl::is_local_invariant<T>::value;

    ASSERT(i_arena != NULL || i_arenaSize == 0, "Unconsitent arena provided");

    if(localInvariance && classSize <= i_arenaSize)
    {
        new (i_arena) functor_impl<T,Return,Types...>(m_functor);
    }
    else
    {
        typedef functor_impl<T,Return,Types...> retType;

        return i_arena << tagged_pointer<retType>(new retType(m_functor),0x01);
    }
}
template<typename T, typename Return, typename ... Types>
tagged_pointer<function_impl_base<Return, container::parameter_pack<Types...>>> functor_impl<T,Return,Types...>::_clone(void* i_arena, size_t i_arenaSize) const
{
    static const size_t classSize = sizeof(functor_impl<T,Return,Types...>);
    static const bool localInvariance = mpl::is_local_invariant<T>::value;

    ASSERT(i_arena != NULL || i_arenaSize == 0, "Unconsitent arena provided");

    if(localInvariance && classSize <= i_arenaSize)
    {
        return new (i_arena) functor_impl<T,Return,Types...>(m_functor);
    }
    else
    {
        typedef functor_impl<T,Return,Types...> retType;

        return tagged_pointer<retType>(new retType(m_functor), 0x01);
    }
}
template<typename T, typename Return, typename ... Types>
const T& functor_impl<T,Return, Types...>::getCallable() const
{
    return m_functor;
}

}
}
}
