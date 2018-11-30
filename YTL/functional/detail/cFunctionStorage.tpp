
#include "System/cException.h"

#pragma GCC diagnostic ignored "-Wplacement-new="

namespace yame
{
namespace ytl
{
namespace detail
{

template<int Size, typename Return, typename ... Types>
const unsigned char function_storage<Size,Return(Types...)>::zeroBlock[sizeof(FuncPtr)] = {0};

template<int Size, typename Return, typename ... Types>
function_storage<Size,Return(Types...)>::function_storage()
{
    memset(&m_embeddedStorage, 0, Size);
}
template<int Size, typename Return, typename ... Types>
function_storage<Size,Return(Types...)>::function_storage(const function_storage& other)
{
    memset(&m_embeddedStorage, 0, Size);

    if(other.empty() == false)
    {
        if(ConstFuncPtr nestedFunc = other.getFuncPtr())
        {
            nestedFunc->clone(&m_embeddedStorage, Size);
        }
    }
}
template<int Size, typename Return, typename ... Types>
function_storage<Size,Return(Types...)>::function_storage(function_storage&& other)
{
    memset(&m_embeddedStorage, 0, Size);

    if(other.empty() == false)
    {
        memcpy(&m_embeddedStorage, &other.m_embeddedStorage, Size);

        memset(&other.m_embeddedStorage, 0, Size);
    }
}
template<int Size, typename Return, typename ... Types>
function_storage<Size,Return(Types...)>::function_storage(const FuncPtr& i_ptr)
{
    &m_embeddedStorage << tagged_pointer<function_impl_base<Return,container::parameter_pack<Types...>>>(i_ptr,0x01);
}
template<int Size, typename Return, typename ... Types>
function_storage<Size,Return(Types...)>::~function_storage()
{
    destroy();
}
template<int Size, typename Return, typename ... Types>
function_storage<Size,Return(Types...)>& function_storage<Size,Return(Types...)>::operator=(const function_storage<Size,Return(Types...)>& other)
{
    destroy();

    if(other.empty() == false)
    {
        if(ConstFuncPtr nestedFunc = other.getFuncPtr())
        {
            nestedFunc->clone(&m_embeddedStorage, Size);
        }
    }

    return *this;
}
template<int Size, typename Return, typename ... Types>
function_storage<Size,Return(Types...)>& function_storage<Size,Return(Types...)>::operator=(function_storage<Size,Return(Types...)>&& other)
{
    destroy();

    if(other.empty() == false)
    {
        memcpy(&m_embeddedStorage, &other.m_embeddedStorage, Size);

        memset(&other.m_embeddedStorage, 0, Size);
    }

    return *this;
}
template<int Size, typename Return, typename ... Types>
function_storage<Size,Return(Types...)>& function_storage<Size,Return(Types...)>::operator=(const FuncPtr& i_ptr)
{
    destroy();

    &m_embeddedStorage << tagged_pointer<function_impl_base<Return,container::parameter_pack<Types...>>>(i_ptr,0x01);

    return *this;
}
template<int Size, typename Return, typename ... Types>
template<typename Functor, typename ... Args>
tagged_pointer<function_impl_base<Return,container::parameter_pack<Types...>>> function_storage<Size,Return(Types...)>::construct(Args&& ... i_args)
{
    static const size_t classSize = sizeof(Functor);
    static const bool localInvariance = mpl::is_local_invariant<Functor>::value;

    if(localInvariance && classSize <= Size)
    {
        return new (&m_embeddedStorage) Functor(mpl::forward<Args>(i_args)...);
    }
    else
    {
        tagged_pointer<function_impl_base<Return,container::parameter_pack<Types...>>> functionCall = tagged_pointer<function_impl_base<Return,container::parameter_pack<Types...>>>(new Functor(mpl::forward<Args>(i_args)...), 0x01);

        return &m_embeddedStorage << functionCall;
    }
}
template<int Size, typename Return, typename ... Types>
void function_storage<Size,Return(Types...)>::destroy()
{
    typedef function_impl_base<Return,container::parameter_pack<Types...>> FuncType;

    tagged_pointer<FuncType> taggedPtr;

    &m_embeddedStorage >> taggedPtr;

    taggedPtr.destroy();

    memset(&m_embeddedStorage, 0, Size);
}
template<int Size, typename Return, typename ... Types>
tagged_pointer<function_impl_base<Return,container::parameter_pack<Types...>>> function_storage<Size,Return(Types...)>::clone(constFuncRef i_func)
{
    return i_func.clone(&m_embeddedStorage, Size);
}
template<int Size, typename Return, typename ... Types>
typename function_storage<Size,Return(Types...)>::FuncPtr function_storage<Size,Return(Types...)>::getFuncPtr()
{
    typedef function_impl_base<Return,container::parameter_pack<Types...>> FuncType;

    ASSERT(empty() == false, "Derreferencing null function");

    tagged_pointer<FuncType> taggedPtr;

    &m_embeddedStorage >> taggedPtr;

    return taggedPtr.get_pointer();
}
template<int Size, typename Return, typename ... Types>
typename function_storage<Size,Return(Types...)>::ConstFuncPtr function_storage<Size,Return(Types...)>::getFuncPtr() const
{
    typedef const function_impl_base<Return,container::parameter_pack<Types...>> FuncType;

    ASSERT(empty() == false, "Derreferencing null function");

    tagged_pointer<FuncType> taggedPtr;

    &m_embeddedStorage >> taggedPtr;

    return taggedPtr.get_pointer();
}
template<int Size, typename Return, typename ... Types>
bool function_storage<Size,Return(Types...)>::empty() const
{
    typedef const function_impl_base<Return,container::parameter_pack<Types...>> FuncType;

    tagged_pointer<FuncType> taggedPtr;

    &m_embeddedStorage >> taggedPtr;

    return taggedPtr.empty();
}
template<int Size, typename Return, typename ... Types>
void* function_storage<Size,Return(Types...)>::getArena()
{
    return static_cast<void*>(&m_embeddedStorage);
}
template<int Size, typename Return, typename ... Types>
const void* function_storage<Size,Return(Types...)>::getArena() const
{
    return static_cast<const void*>(&m_embeddedStorage);
}
template<int Size, typename Return, typename ... Types>
size_t function_storage<Size,Return(Types...)>::getArenaSize()
{
    return Size;
}

}
}
}
