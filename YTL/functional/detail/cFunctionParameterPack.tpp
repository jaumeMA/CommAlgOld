
namespace yame
{
namespace ytl
{
namespace detail
{

template<typename ... Types>
function_parameter_pack<Types...>::function_parameter_pack()
: container::parameter_pack<ytl::optional<Types>...>()
{
}

template<typename ... Types>
function_parameter_pack<Types...>::function_parameter_pack(Types ... vals)
: container::parameter_pack<ytl::optional<Types>...>(mpl::forward<Types>(vals)...)
{
}

template<typename ... Types>
function_parameter_pack<Types...>::function_parameter_pack(const function_parameter_pack<Types...>& other)
: container::parameter_pack<ytl::optional<Types>...>(other)
{
}

template<typename ... Types>
function_parameter_pack<Types...>::function_parameter_pack(const container::parameter_pack<Types...>& other)
: container::parameter_pack<ytl::optional<Types>...>(other)
{
}

template<typename ... Types>
template<int ... posTypes>
void function_parameter_pack<Types...>::specialize(typename mpl::nth_type_of<posTypes,Types...>::type ... args)
{
    container::parameter_pack<ytl::optional<Types>...>::template setValues<posTypes...>(mpl::forward<typename mpl::nth_type_of<posTypes,Types...>::type>(args)...);

    return;
}

template<typename ... Types>
template<int ... posTypes>
void function_parameter_pack<Types...>::specialize(mpl::sequence<posTypes...> rankSequence, typename mpl::nth_type_of<posTypes,Types...>::type ... args)
{
    container::parameter_pack<ytl::optional<Types>...>::template setValues<posTypes...>(mpl::forward<typename mpl::nth_type_of<posTypes,Types...>::type>(args)...);

    return;
}

}
}
}
