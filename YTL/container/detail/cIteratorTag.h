#pragma once

namespace yame
{
namespace container
{
namespace detail
{
namespace impl
{

//Iterator tags
struct DirectIteratorTag;
struct ReverseIteratorTag;

template<typename>
struct AgnosticInputIteratorTag;
template<typename>
struct AgnosticOutputIteratorTag;
template<typename>
struct AgnosticForwardIteratorTag;
template<typename>
struct AgnosticBidirectionalIteratorTag;
template<typename>
struct AgnosticRandomAccessIteratorTag;

//direct
typedef AgnosticInputIteratorTag<DirectIteratorTag> InputIteratorTag;
typedef AgnosticOutputIteratorTag<DirectIteratorTag> OutputIteratorTag;
typedef AgnosticForwardIteratorTag<DirectIteratorTag> ForwardIteratorTag;
typedef AgnosticBidirectionalIteratorTag<DirectIteratorTag> BidirectionalIteratorTag;
typedef AgnosticRandomAccessIteratorTag<DirectIteratorTag> RandomAccessIteratorTag;

//reverse
typedef AgnosticBidirectionalIteratorTag<ReverseIteratorTag> ReverseBidirectionalIteratorTag;
typedef AgnosticRandomAccessIteratorTag<ReverseIteratorTag> ReverseRandomAccessIteratorTag;

template<typename Tag>
struct _is_reversed_iterator;

template<template<typename> class AgnosticTag>
struct _is_reversed_iterator<AgnosticTag<DirectIteratorTag>>
{
    static const bool value = false;
};
template<template<typename> class AgnosticTag>
struct _is_reversed_iterator<AgnosticTag<ReverseIteratorTag>>
{
    static const bool value = true;
};

template<typename Tag>
struct is_reversed_iterator
{
    static const bool value = _is_reversed_iterator<Tag>::value;
};

template<typename Tag>
struct _is_reversable_iterator
{
    static const bool value = false;
};

template<typename Tag>
struct _is_reversable_iterator<AgnosticBidirectionalIteratorTag<Tag>>
{
    static const bool value = true;
};
template<typename Tag>
struct _is_reversable_iterator<AgnosticRandomAccessIteratorTag<Tag>>
{
    static const bool value = true;
};

template<typename Tag>
struct is_reversable_iterator
{
    static const bool value = _is_reversable_iterator<Tag>::value;
};

template<typename Tag>
struct _is_inputable_iterator;

template<typename Tag>
struct _is_inputable_iterator
{
    static const bool value = false;
};

template<template<typename> class AgnosticTag, typename DirectionTag>
struct _is_inputable_iterator<AgnosticTag<DirectionTag>>
{
    static const bool value = true;
};
template<typename DirectionTag>
struct _is_inputable_iterator<AgnosticOutputIteratorTag<DirectionTag>>
{
    static const bool value = false;
};

template<typename Tag>
struct is_inputable_iterator
{
    static const bool value = _is_inputable_iterator<Tag>::value;
};

}
}
}
}
