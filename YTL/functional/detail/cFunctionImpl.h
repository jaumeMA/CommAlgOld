
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

#include "YTL/container/cParameterPack.h"
#include "YTL/mpl/cParameterPackTemplateHelper.h"
#include "YTL/mpl/cFunctionTemplateHelper.h"
#include "YTL/types/taggedPtr/tagged_pointer.h"
#include "Utils/rtti/rtti.h"

namespace yame
{
namespace ytl
{
namespace detail
{

template<typename Return, typename ... Types>
struct function_impl_base;

template<typename Return, typename ... Types>
struct function_impl_base<Return, container::parameter_pack<Types...>>
{
    typedef function_impl_base<Return, container::parameter_pack<Types...>> thisClass;

    PUBLISH_RTTI_BASE_TYPE(thisClass)

	template<typename,typename,typename>
	struct specialized_impl;

	template<int ... specIndexs, int ... notSpecIndexs, int ... totalIndexs>
	struct specialized_impl<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>,mpl::sequence<totalIndexs...>> : function_impl_base<Return, typename mpl::get_sub_parameter_pack<Types...>::template not_at<specIndexs...>::type>
	{
        typedef specialized_impl<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>,mpl::sequence<totalIndexs...>> thisClass;
        typedef function_impl_base<Return, typename mpl::get_sub_parameter_pack<Types...>::template not_at<specIndexs...>::type> baseClass;

        PUBLISH_RTTI_TYPE_BASES(thisClass,baseClass)

        typedef tagged_pointer<function_impl_base<Return, container::parameter_pack<Types...>>> TaggedFuncPtr;
        typedef tagged_pointer<function_impl_base<Return, typename mpl::get_sub_parameter_pack<Types...>::template not_at<specIndexs...>::type>> TaggedSpecFuncPtr;

        specialized_impl() = default;
        template<typename ... Args>
		specialized_impl(TaggedFuncPtr i_object, Args&& ... i_args);
		specialized_impl(TaggedFuncPtr i_object, const typename mpl::get_sub_parameter_pack<Types...>::template at<specIndexs...>::type& i_specArgs);
		specialized_impl(TaggedFuncPtr i_object, typename mpl::get_sub_parameter_pack<Types...>::template at<specIndexs...>::type&& i_specArgs);
        ~specialized_impl();

		Return operator()(typename mpl::nth_type_of<notSpecIndexs, Types...>::type ... args) const override;
        TaggedSpecFuncPtr clone(void* i_arena, size_t i_arenaSize) const override;
        TaggedSpecFuncPtr _clone(void* i_arena, size_t i_arenaSize) const override;

    private:
		typename mpl::get_sub_parameter_pack<Types...>::template at<specIndexs...>::type m_specArgs;
        TaggedFuncPtr m_object;
	};

	function_impl_base() = default;
	virtual ~function_impl_base() = default;

	template<typename ... Args>
	tagged_pointer<function_impl_base<Return, typename mpl::get_sub_parameter_pack<Types...>::template at_seq<typename mpl::get_pos_of_type<mpl::place_holder, mpl::is_type_constructible>::template at<Args...>::type>::type>> specialize(void* i_arena, size_t i_arenaSize, Args&& ... args) const;
	template<int ... ArgIndexs, typename ... Args>
	tagged_pointer<function_impl_base<Return, typename mpl::get_sub_parameter_pack<Types...>::template not_at<ArgIndexs...>::type>> specializeAt(void* i_arena, size_t i_arenaSize, Args&& ... args) const;

	virtual Return operator()(Types ... args) const = 0;
	virtual tagged_pointer<function_impl_base<Return, container::parameter_pack<Types...>>> clone(void* i_arena, size_t i_arenaSize) const = 0;
	virtual tagged_pointer<function_impl_base<Return, container::parameter_pack<Types...>>> _clone(void* i_arena, size_t i_arenaSize) const = 0;
};

//non static member function case
template<typename ObjectType, typename Return, typename ... Types>
class relative_function_impl : public function_impl_base<Return, container::parameter_pack<Types...>>
{
    typedef relative_function_impl<ObjectType,Return,Types...> thisClass;
    typedef function_impl_base<Return,container::parameter_pack<Types...>> baseClass;

    PUBLISH_RTTI_TYPE_BASES(thisClass,baseClass)

    typedef Return(ObjectType::*FuncPointerType)(Types...);

public:
	relative_function_impl(ObjectType* i_object, FuncPointerType i_funcPointer);

	Return operator()(Types ... args) const override;
	tagged_pointer<function_impl_base<Return, container::parameter_pack<Types...>>> clone(void* i_arena, size_t i_arenaSize) const override;
	tagged_pointer<function_impl_base<Return, container::parameter_pack<Types...>>> _clone(void* i_arena, size_t i_arenaSize) const override;

protected:
	ObjectType* m_object;
    FuncPointerType m_funcPointer;
};

//static member function or free function case
template<typename Return, typename ... Types>
class free_function_impl : public function_impl_base<Return, container::parameter_pack<Types...>>
{
    typedef free_function_impl<Return,Types...> thisClass;
    typedef function_impl_base<Return,container::parameter_pack<Types...>> baseClass;

    PUBLISH_RTTI_TYPE_BASES(thisClass,baseClass)

    typedef Return(*FuncPointerType)(Types...);

public:
	free_function_impl(FuncPointerType i_funcPointer);

	Return operator()(Types ... args) const override;
	tagged_pointer<function_impl_base<Return, container::parameter_pack<Types...>>> clone(void* i_arena, size_t i_arenaSize) const override;
	tagged_pointer<function_impl_base<Return, container::parameter_pack<Types...>>> _clone(void* i_arena, size_t i_arenaSize) const override;
    FuncPointerType getFuncAddr() const;

protected:
    FuncPointerType m_funcPointer;
};

//functor (lambdas) case
template<typename T, typename Return, typename ... Types>
class functor_impl : public function_impl_base<Return, container::parameter_pack<Types...>>
{
    typedef functor_impl<T,Return,Types...> thisClass;
    typedef function_impl_base<Return,container::parameter_pack<Types...>> baseClass;

    PUBLISH_RTTI_TYPE_BASES(thisClass,baseClass)

public:
	functor_impl(const T& i_functor);

	Return operator()(Types ... args) const override;
	tagged_pointer<function_impl_base<Return, container::parameter_pack<Types...>>> clone(void* i_arena, size_t i_arenaSize) const override;
	tagged_pointer<function_impl_base<Return, container::parameter_pack<Types...>>> _clone(void* i_arena, size_t i_arenaSize) const override;
    const T& getCallable() const;

protected:
    T m_functor;
};

}
}

namespace mpl
{

//template<typename Return,typename ... Types, int ... specIndexs, int ... notSpecIndexs, int ... totalIndexs>
//struct is_local_invariant<ytl::detail::function_impl_base<Return, container::parameter_pack<Types...>>::specialized_impl<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>,mpl::sequence<totalIndexs...>>>
//{
//    static const bool value = is_local_invariant<typename mpl::nth_type_of<specIndexs,Types...>::type ...>::value;
//};

}
}

#include "YTL/functional/detail/cFunctionImpl.tpp"
