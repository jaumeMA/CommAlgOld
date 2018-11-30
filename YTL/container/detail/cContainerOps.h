#pragma once

#include "YTL/container/cMap.h"

template<typename T, typename R, typename N, template<typename> class A, template<typename,typename> class B >
inline bool operator==(const yame::container::detail::cMapImpl<T,R,N,A,B>& i_lhs, const yame::container::detail::cMapImpl<T,R,N,A,B>& i_rhs);
template<typename T, typename R, typename N, template<typename> class A, template<typename,typename> class B >
inline bool operator!=(const yame::container::detail::cMapImpl<T,R,N,A,B>& i_lhs, const yame::container::detail::cMapImpl<T,R,N,A,B>& i_rhs);
template<typename T, typename R>
inline bool operator==(const yame::container::detail::cMapNode<T,R>& i_lhs, const yame::container::detail::cMapNode<T,R>& i_rhs);
template<typename T, typename R>
inline bool operator!=(const yame::container::detail::cMapNode<T,R>& i_lhs, const yame::container::detail::cMapNode<T,R>& i_rhs);

#include "YTL/container/detail/cContainerOps.tpp"
