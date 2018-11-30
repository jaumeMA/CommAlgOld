
#include "YTL/container/cTreeBalancer.h"

namespace yame
{
namespace container
{
namespace detail
{

template<template<typename,typename> class NestedBalancer,typename Map,typename Node>
Node* cRecursiveMultiMapBalancer<NestedBalancer,Map,Node>::balance(Map* fromMap, Node* fromNode)
{
    typedef typename Node::value_type::Value Value;

    fromNode->m_value.second.setHolder(static_cast<Value*>(fromMap));
    fromNode->m_value.second.setNodeLocation(fromNode);

    return NestedBalancer<Map,Node>::balance(fromMap,fromNode);
}

template<template<typename,typename> class NestedBalancer,typename Map,typename Node>
Node* cMultiMapBalancer<NestedBalancer,Map,Node>::balance(Map* fromMap, Node* fromNode)
{
    typedef typename Node::value_type::Value Value;

    fromNode->m_value.second.setHolder(NULL);
    fromNode->m_value.second.setNodeLocation(fromNode);

    return NestedBalancer<Map,Node>::balance(fromMap,fromNode);
}

}
}
}
