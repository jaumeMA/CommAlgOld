
namespace yame
{
namespace container
{
namespace detail
{

}
}
}

template<typename T, typename R, typename N, template<typename> class A, template<typename,typename> class B >
bool operator==(const yame::container::detail::cMapImpl<T,R,N,A,B>& i_lhs, const yame::container::detail::cMapImpl<T,R,N,A,B>& i_rhs)
{
    return false;
}
template<typename T, typename R, typename N, template<typename> class A, template<typename,typename> class B >
bool operator!=(const yame::container::detail::cMapImpl<T,R,N,A,B>& i_lhs, const yame::container::detail::cMapImpl<T,R,N,A,B>& i_rhs)
{
    return false;
}
template<typename T, typename R>
bool operator==(const yame::container::detail::cMapNode<T,R>& i_lhs, const yame::container::detail::cMapNode<T,R>& i_rhs)
{
    return i_lhs.first == i_rhs.first && i_lhs.second == i_rhs.second;
}
template<typename T, typename R>
bool operator!=(const yame::container::detail::cMapNode<T,R>& i_lhs, const yame::container::detail::cMapNode<T,R>& i_rhs)
{
    return i_lhs.first != i_rhs.first || i_lhs.second != i_rhs.second;
}
