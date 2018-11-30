
/*This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#pragma once

#include "YTL/functional/cFunctionStorage.h"
#include "YTL/mpl/cFunctionTemplateHelper.h"
#include "YTL/functional/detail/cFunctionTemplateHelper.h"

namespace yame
{
namespace ytl
{

const size_t k_embeddedStorageSize = 16;

//wrapper for holding function call before knowing T type
template<typename ...>
class function;

template<typename Return>
class function<Return()> : public detail::function_storage<k_embeddedStorageSize, Return()>
{
public:
    typedef detail::function_impl_base<Return,container::parameter_pack<>> func_ptr_base;

    function() = default;
    function(mpl::null_ptr_type);
    function(const function& other);
    function(function&& other);
    template<typename T>
    requires (mpl::is_valid_functor<T>::value)
    function(T&& functor);
    template<typename T>
    function(T *pRef, Return(T::*call)());
    function(Return(*call)());
    function& operator=(const function& other);
    function& operator=(function&& other);
    function& operator=(detail::function_impl_base<Return,container::parameter_pack<>>* funcBase);
    function& operator=(mpl::null_ptr_type);
    inline Return eval() const;
    operator Return() const;
    Return operator()() const;
    bool operator==(mpl::null_ptr_type) const;
    bool operator!=(mpl::null_ptr_type) const;
    detail::function_impl_base<Return,container::parameter_pack<>>* getFuncPtr();
    const detail::function_impl_base<Return,container::parameter_pack<>>* getFuncPtr() const;
    static function<Return()> clone(const detail::function_impl_base<Return,container::parameter_pack<>>* i_funcPtr);
private:
    function(detail::function_impl_base<Return,container::parameter_pack<>>* funcBase);
};


template<typename Return, typename ... Types>
class function<Return(Types...)>: public detail::function_storage<k_embeddedStorageSize, Return(Types...)>
{
public:
    using detail::function_storage<k_embeddedStorageSize, Return(Types...)>::clone;
    typedef detail::function_impl_base<Return,container::parameter_pack<Types...>> func_ptr_base;

    function() = default;
    function(mpl::null_ptr_type);
    function(const function& other);
    function(function&& other);
    template<typename T>
    requires (mpl::is_valid_functor<T>::value)
    function(T&& functor);
    template<typename T>
    function(T *pRef, Return(T::*call)(Types...));
    function(Return(*call)(Types...));
    function& operator=(const function& other);
    function& operator=(function&& other);
    function& operator=(mpl::null_ptr_type);
    function& operator=(detail::function_impl_base<Return,container::parameter_pack<Types...>>* other);
    template<typename ... Args>
    inline Return eval(Args&& ... args) const;
    template<typename ... Args>
    typename mpl::create_callable<function,Return, typename mpl::get_sub_parameter_pack<Types...>::template at_seq<typename mpl::get_pos_of_type<mpl::place_holder, mpl::is_type_constructible>::template at<Args...>::type>::type>::type operator()(Args&& ... args) const;
    bool operator==(mpl::null_ptr_type) const;
    bool operator!=(mpl::null_ptr_type) const;
    template<int ... ArgIndexs, typename ... Args>
    typename mpl::create_callable<function,Return, typename mpl::get_sub_parameter_pack<Types...>::template not_at<ArgIndexs...>::type>::type specializeAt(Args&& ... args) const;
    detail::function_impl_base<Return,container::parameter_pack<Types...>>* getFuncPtr();
    const detail::function_impl_base<Return,container::parameter_pack<Types...>>* getFuncPtr() const;
    static function<Return(Types...)> clone(const detail::function_impl_base<Return,container::parameter_pack<Types...>>* i_funcPtr);

private:
    function(detail::function_impl_base<Return,container::parameter_pack<Types...>>* funcBase);
};

template<typename Object, typename Return, typename ... Types>
function<Return(Types...)> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...));
template<typename Return, typename ... Types>
function<Return(Types...)> make_function(Return(*i_funcPtr)(Types...));


template<typename Return, typename ... Types>
using curried_function = typename yame::mpl::curry_function<yame::ytl::function<Return(Types...)>>::type;

}
}

#include "detail/cFunction.tpp"
