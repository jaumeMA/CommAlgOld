#pragma once

#include "YTL/container/detail/cContainerOps.h"

namespace yame
{
namespace container
{
namespace detail
{

}
}
}

template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename> class> class Map, template<typename> class A,template<typename,typename> class B, typename V>
inline bool operator==(const yame::container::detail::cMultiMapValue<T,R,Map,A,B,V>& i_lhs, const yame::container::detail::cMultiMapValue<T,R,Map,A,B,V>& i_rhs);
template<typename T, typename R, template<typename,typename,template<typename>class,template<typename,typename> class> class Map, template<typename> class A,template<typename,typename> class B, typename V>
inline bool operator!=(const yame::container::detail::cMultiMapValue<T,R,Map,A,B,V>& i_lhs, const yame::container::detail::cMultiMapValue<T,R,Map,A,B,V>& i_rhs);

#include "YTL/container/detail/cMultiMapOps.tpp"
