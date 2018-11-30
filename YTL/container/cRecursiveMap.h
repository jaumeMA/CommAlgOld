#pragma once

#include "YTL/container/cMap.h"
#include "YTL/container/cMultiMapBalancer.h"

namespace yame
{
namespace container
{

template<>
struct MultiBalancer<cMap>
{
template<typename Map,typename Node>
using Balancer = detail::cMultiMapBalancer<detail::EmptyBalancer,Map,Node>;
};

template<>
struct RecursiveBalancer<cMap>
{
template<typename Map,typename Node>
using Balancer = detail::cRecursiveMultiMapBalancer<detail::EmptyBalancer,Map,Node>;
};

}
}
