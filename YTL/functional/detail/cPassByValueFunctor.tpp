
namespace yame
{
namespace ytl
{

template<typename Return,typename ... Types>
pass_by_value_functor<Return(Types...)>::pass_by_value_functor(const function<Return(Types...)>& i_function)
: m_nestedFunction(i_function)
{
}
template<typename Return,typename ... Types>
Return pass_by_value_functor<Return(Types...)>::operator()(typename mpl::drop_constness<typename mpl::drop_reference<Types>::type>::type ... i_args) const
{
    return m_nestedFunction.eval(i_args ...);
}
template<typename Return,typename ... Types>
const function<Return(Types...)>& pass_by_value_functor<Return(Types...)>::getForwardedFunction() const
{
    return m_nestedFunction;
}

}
}
