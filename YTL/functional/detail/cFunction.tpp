#include "YTL/mpl/cFunctionTemplateHelper.h"
#include "System/cException.h"
#include "YTL/functional/cAgnosticCallable.h"
#include "YTL/functional/cPassByValueFunctor.h"

namespace yame
{
namespace ytl
{

template<typename Return, typename ... Types>
function<Return(Types...)>::function(mpl::null_ptr_type)
: detail::function_storage<k_embeddedStorageSize,Return(Types...)>()
{
}
template<typename Return, typename ... Types>
function<Return(Types...)>::function(const function& other)
: detail::function_storage<k_embeddedStorageSize, Return(Types...)>(other)
{
}
template<typename Return, typename ... Types>
function<Return(Types...)>::function(function&& other)
: detail::function_storage<k_embeddedStorageSize, Return(Types...)>(mpl::move(other))
{
}
template<typename Return, typename ... Types>
function<Return(Types...)>::function(detail::function_impl_base<Return,container::parameter_pack<Types...>>* funcBase)
: detail::function_storage<k_embeddedStorageSize, Return(Types...)>(funcBase)
{
}
template<typename Return, typename ... Types>
template<typename T>
requires (mpl::is_valid_functor<T>::value)
function<Return(Types...)>::function(T&& functor)
{
    typedef detail::functor_impl<typename mpl::drop_constness<typename mpl::drop_reference<T>::type>::type,Return,Types...> Functor;

    detail::function_storage<k_embeddedStorageSize, Return(Types...)>::template construct<Functor>(mpl::forward<T>(functor));
}
template<typename Return, typename ... Types>
function<Return(Types...)>::function(Return(*call)(Types...))
{
    typedef detail::free_function_impl<Return,Types...> Functor;

    detail::function_storage<k_embeddedStorageSize, Return(Types...)>::template construct<Functor>(call);
}
template<typename Return, typename ... Types>
template<typename T>
function<Return(Types...)>::function(T *pRef, Return(T::*call)(Types...))
{
    typedef detail::relative_function_impl<T,Return,Types...> Functor;

    detail::function_storage<k_embeddedStorageSize, Return(Types...)>::template construct<Functor>(pRef, call);
}
template<typename Return, typename ... Types>
function<Return(Types...)>& function<Return(Types...)>::operator=(const function& other)
{
    detail::function_storage<k_embeddedStorageSize, Return(Types...)>::operator=(other);

    return *this;
}
template<typename Return, typename ... Types>
function<Return(Types...)>& function<Return(Types...)>::operator=(function&& other)
{
    detail::function_storage<k_embeddedStorageSize, Return(Types...)>::operator=(mpl::move(other));

    return *this;
}
template<typename Return, typename ... Types>
function<Return(Types...)>& function<Return(Types...)>::operator=(detail::function_impl_base<Return,container::parameter_pack<Types...>>* funcBase)
{
    detail::function_storage<k_embeddedStorageSize, Return(Types...)>::operator=(funcBase);

    return *this;
}
template<typename Return, typename ... Types>
function<Return(Types...)>& function<Return(Types...)>::operator=(mpl::null_ptr_type)
{
    detail::function_storage<k_embeddedStorageSize, Return(Types...)>::destroy();

    return *this;
}
template<typename Return, typename ... Types>
template<typename ... Args>
Return function<Return(Types...)>::eval(Args&& ... args) const
{
    typename detail::function_storage<k_embeddedStorageSize, Return(Types...)>::ConstFuncPtr _funcPtr = detail::function_storage<k_embeddedStorageSize, Return(Types...)>::getFuncPtr();

    return _funcPtr->operator()(mpl::forward<Args>(args)...);
}
template<typename Return, typename ... Types>
template<typename ... Args>
typename mpl::create_callable<function,Return, typename mpl::get_sub_parameter_pack<Types...>::template at_seq<typename mpl::get_pos_of_type<mpl::place_holder, mpl::is_type_constructible>::template at<Args...>::type>::type>::type function<Return(Types...)>::operator()(Args&& ... i_args) const
{
    typename mpl::create_callable<function,Return, typename mpl::get_sub_parameter_pack<Types...>::template at_seq<typename mpl::get_pos_of_type<mpl::place_holder, mpl::is_type_constructible>::template at<Args...>::type>::type>::type specFunc;

    typename detail::function_storage<k_embeddedStorageSize, Return(Types...)>::ConstFuncPtr _funcPtr = detail::function_storage<k_embeddedStorageSize, Return(Types...)>::getFuncPtr();

    auto _specFuncPtr = _funcPtr->specialize(specFunc.getArena(), this->getArenaSize(), mpl::forward<Args>(i_args)...);

    return specFunc;
}
template<typename Return, typename ... Types>
template<int ... ArgIndexs, typename ... Args>
typename mpl::create_callable<function,Return, typename mpl::get_sub_parameter_pack<Types...>::template not_at<ArgIndexs...>::type>::type function<Return(Types...)>::specializeAt(Args&& ... args) const
{
    typedef typename mpl::create_callable<function,Return, typename mpl::get_sub_parameter_pack<Types...>::template not_at<ArgIndexs...>::type>::type retType;

    retType specFunc;

    typename detail::function_storage<k_embeddedStorageSize, Return(Types...)>::ConstFuncPtr _funcPtr = detail::function_storage<k_embeddedStorageSize, Return(Types...)>::getFuncPtr();

    auto _specFuncPtr = _funcPtr->template specializeAt<ArgIndexs...>(specFunc.getArena(), this->getArenaSize(), mpl::forward<Args>(args)...);

//    if(_specFuncPtr.empty() == false)
//    {
//        specFunc = _specFuncPtr.get_pointer();
//    }

    //in rvo we trust
    return specFunc;
}
template<typename Return, typename ... Types>
bool function<Return(Types...)>::operator==(mpl::null_ptr_type) const
{
    return this->empty();
}
template<typename Return, typename ... Types>
bool function<Return(Types...)>::operator!=(mpl::null_ptr_type) const
{
    return this->empty() == false;
}
template<typename Return, typename ... Types>
detail::function_impl_base<Return,container::parameter_pack<Types...>>* function<Return(Types...)>::getFuncPtr()
{
    return detail::function_storage<k_embeddedStorageSize, Return(Types...)>::getFuncPtr();
}
template<typename Return, typename ... Types>
const detail::function_impl_base<Return,container::parameter_pack<Types...>>* function<Return(Types...)>::getFuncPtr() const
{
    return detail::function_storage<k_embeddedStorageSize, Return(Types...)>::getFuncPtr();
}
template<typename Return, typename ... Types>
function<Return(Types...)> function<Return(Types...)>::clone(const detail::function_impl_base<Return,container::parameter_pack<Types...>>* i_funcPtr)
{
    function<Return(Types...)> res;

    if(i_funcPtr != NULL)
    {
        res.clone(*i_funcPtr);
    }

    return res;
}

//0 args version
template<typename Return>
function<Return()>::function(mpl::null_ptr_type)
: detail::function_storage<k_embeddedStorageSize, Return()>()
{
}
template<typename Return>
function<Return()>::function(const function& other)
: detail::function_storage<k_embeddedStorageSize, Return()>(other)
{
}
template<typename Return>
function<Return()>::function(function&& other)
: detail::function_storage<k_embeddedStorageSize, Return()>(mpl::move(other))
{
}
template<typename Return>
function<Return()>::function(detail::function_impl_base<Return,container::parameter_pack<>>* funcBase)
: detail::function_storage<k_embeddedStorageSize, Return()>(funcBase)
{
}
template<typename Return>
template<typename T>
requires (mpl::is_valid_functor<T>::value)
function<Return()>::function(T&& functor)
{
    typedef detail::functor_impl<typename mpl::drop_reference<T>::type,Return> Functor;

    detail::function_storage<k_embeddedStorageSize, Return()>::template construct<Functor>(mpl::forward<T>(functor));
}
template<typename Return>
function<Return()>::function(Return(*call)())
{
    typedef detail::free_function_impl<Return> Functor;

    detail::function_storage<k_embeddedStorageSize, Return()>::template construct<Functor>(call);
}
template<typename Return>
template<typename T>
function<Return()>::function(T *pRef, Return(T::*call)())
{
    typedef detail::relative_function_impl<T,Return> Functor;

    detail::function_storage<k_embeddedStorageSize, Return()>::template construct<Functor>(pRef, call);
}
template<typename Return>
function<Return()>& function<Return()>::operator=(const function& other)
{
    detail::function_storage<k_embeddedStorageSize, Return()>::operator=(other);

    return *this;
}
template<typename Return>
function<Return()>& function<Return()>::operator=(function&& other)
{
    detail::function_storage<k_embeddedStorageSize, Return()>::operator=(mpl::move(other));

    return *this;
}
template<typename Return>
function<Return()>& function<Return()>::operator=(detail::function_impl_base<Return,container::parameter_pack<>>* funcBase)
{
    detail::function_storage<k_embeddedStorageSize, Return()>::operator=(funcBase);

    return *this;
}
template<typename Return>
function<Return()>& function<Return()>::operator=(mpl::null_ptr_type)
{
    detail::function_storage<k_embeddedStorageSize, Return()>::destroy();

    return *this;
}
template<typename Return>
Return function<Return()>::eval() const
{
    typename detail::function_storage<k_embeddedStorageSize, Return()>::ConstFuncPtr _funcPtr = detail::function_storage<k_embeddedStorageSize, Return()>::getFuncPtr();

    return _funcPtr->operator()();
}
template<typename Return>
function<Return()>::operator Return() const
{
    typename detail::function_storage<k_embeddedStorageSize, Return()>::ConstFuncPtr _funcPtr = detail::function_storage<k_embeddedStorageSize, Return()>::getFuncPtr();

    return _funcPtr->operator()();
}
template<typename Return>
Return function<Return()>::operator()() const
{
    typename detail::function_storage<k_embeddedStorageSize, Return()>::ConstFuncPtr _funcPtr = detail::function_storage<k_embeddedStorageSize, Return()>::getFuncPtr();

    return _funcPtr->operator()();
}
//template<typename Return>
//bool function<Return()>::operator==(const Return& value) const
//{
//    typename detail::function_storage<k_embeddedStorageSize, Return()>::FuncPtr _funcPtr = detail::function_storage<k_embeddedStorageSize, Return()>::getFuncPtr();
//
//    return _funcPtr->operator()() == value;
//}
//template<typename Return>
//bool function<Return()>::operator!=(const Return& value) const
//{
//    typename detail::function_storage<k_embeddedStorageSize, Return()>::FuncPtr _funcPtr = detail::function_storage<k_embeddedStorageSize, Return()>::getFuncPtr();
//
//    return _funcPtr->operator()() != value;
//}
template<typename Return>
bool function<Return()>::operator==(mpl::null_ptr_type) const
{
    return this->empty();
}
template<typename Return>
bool function<Return()>::operator!=(mpl::null_ptr_type) const
{
    return this->empty() == false;
}

template<typename Object, typename Return, typename ... Types>
function<Return(Types...)> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...))
{
    return function<Return(Types...)>(i_object, i_funcPtr);
}
template<typename Return, typename ... Types>
function<Return(Types...)> make_function(Return(*i_funcPtr)(Types...))
{
    return function<Return(Types...)>(i_funcPtr);
}
template<typename Return>
detail::function_impl_base<Return,container::parameter_pack<>>* function<Return()>::getFuncPtr()
{
    return detail::function_storage<k_embeddedStorageSize, Return()>::getFuncPtr();
}
template<typename Return>
const detail::function_impl_base<Return,container::parameter_pack<>>* function<Return()>::getFuncPtr() const
{
    return detail::function_storage<k_embeddedStorageSize, Return()>::getFuncPtr();
}
template<typename Return>
function<Return()> function<Return()>::clone(const detail::function_impl_base<Return,container::parameter_pack<>>* i_funcPtr)
{
    function<Return()> res;

    if(i_funcPtr != NULL)
    {
        res.clone(*i_funcPtr);
    }

    return res;
}

}
}
