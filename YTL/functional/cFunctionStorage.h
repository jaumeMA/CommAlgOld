#pragma once

#include "detail/cFunctionImpl.h"
#include "YTL/types/taggedPtr/tagged_pointer.h"

namespace yame
{
namespace ytl
{
namespace detail
{

template<int, typename ...>
struct function_storage;

template<int Size, typename Return, typename ... Types>
struct function_storage<Size,Return(Types...)>
{
    static_assert((Size%4)==0, "You shall provide a multiple of 4 for storage size");

public:
    typedef function_impl_base<Return,container::parameter_pack<Types...>>* FuncPtr;
    typedef const function_impl_base<Return,container::parameter_pack<Types...>>* ConstFuncPtr;
    typedef const function_impl_base<Return,container::parameter_pack<Types...>>& constFuncRef;

    function_storage();
    function_storage(const function_storage&);
    function_storage(function_storage&&);
    function_storage(const FuncPtr& i_ptr);
    ~function_storage();
    function_storage<Size,Return(Types...)>& operator=(const function_storage<Size,Return(Types...)>& other);
    function_storage<Size,Return(Types...)>& operator=(function_storage<Size,Return(Types...)>&& other);
    function_storage<Size,Return(Types...)>& operator=(const FuncPtr& i_ptr);
    template<typename Functor, typename ... Args>
    inline tagged_pointer<function_impl_base<Return,container::parameter_pack<Types...>>> construct(Args&& ... i_args);
    inline void destroy();
    inline tagged_pointer<function_impl_base<Return,container::parameter_pack<Types...>>> clone(constFuncRef i_func);
    inline FuncPtr getFuncPtr();
    inline ConstFuncPtr getFuncPtr() const;
    inline bool empty() const;
    inline void* getArena();
    inline const void* getArena() const;
    static inline size_t getArenaSize();

private:
    static const unsigned char zeroBlock[sizeof(FuncPtr)];
    static const size_t bitMask = (1 << (sizeof(FuncPtr) << 3)) - 2;
    typedef typename std::aligned_storage<Size>::type arena_type;
    arena_type m_embeddedStorage;
};


}
}
}

#include "YTL/functional/detail/cFunctionStorage.tpp"
