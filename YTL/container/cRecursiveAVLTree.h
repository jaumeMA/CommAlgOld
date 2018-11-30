#pragma once

#include "YTL/container/cAVLtree.h"
#include "YTL/container/cMultiMapBalancer.h"

namespace yame
{
namespace container
{

template<>
struct MultiBalancer<cAVLtree>
{
template<typename Map,typename Node>
using Balancer = detail::cMultiMapBalancer<detail::cAVLBalancer,Map,Node>;
};

template<>
struct RecursiveBalancer<cAVLtree>
{
template<typename Map,typename Node>
using Balancer = detail::cRecursiveMultiMapBalancer<detail::cAVLBalancer,Map,Node>;
};

}
}
