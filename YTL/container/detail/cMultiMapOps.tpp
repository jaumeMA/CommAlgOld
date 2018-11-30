
namespace yame
{
namespace container
{
namespace detail
{

}
}
}

template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
bool operator==(const yame::container::detail::cMultiMapValue<T,R,Map,A,B,V>& i_lhs, const yame::container::detail::cMultiMapValue<T,R,Map,A,B,V>& i_rhs)
{
    typedef typename yame::container::detail::cMultiMapValue<T,R,Map,A,B,V>::MapClass MapClass;

    return i_lhs.getValue() == i_rhs.getValue() && static_cast<const MapClass&>(i_lhs) == static_cast<const MapClass&>(i_rhs);
}
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename>class> class Map, template<typename> class A, template<typename,typename> class B, typename V>
bool operator!=(const yame::container::detail::cMultiMapValue<T,R,Map,A,B,V>& i_lhs, const yame::container::detail::cMultiMapValue<T,R,Map,A,B,V>& i_rhs)
{
    typedef typename yame::container::detail::cMultiMapValue<T,R,Map,A,B,V>::MapClass MapClass;

    return i_lhs.getValue() != i_rhs.getValue() || static_cast<const MapClass&>(i_lhs) == static_cast<const MapClass&>(i_rhs);
}

