#pragma once

namespace yame
{
namespace container
{
namespace detail
{

template<template<typename,typename> class NestedBalancer,typename Map,typename Node>
class cRecursiveMultiMapBalancer
{
public:
    typedef NestedBalancer<Map,Node> Balancer;

    inline static Node* balance(Map* fromMap, Node* fromNode);
};

template<template<typename,typename> class NestedBalancer,typename Map,typename Node>
class cMultiMapBalancer
{
public:
    typedef NestedBalancer<Map,Node> Balancer;

    inline static Node* balance(Map* fromMap, Node* fromNode);
};

}

template<template<typename,typename,template<typename>class,template<typename,typename>class> class Map>
struct MultiBalancer;

template<template<typename,typename,template<typename>class,template<typename,typename>class> class Map>
struct RecursiveBalancer;

}
}

#include "YTL/container/detail/cMultiMapBalancer.tpp"
