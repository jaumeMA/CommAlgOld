
namespace yame
{
namespace container
{
namespace detail
{

template<template <typename> class Iterable, typename  Traits>
typename cIterable<Iterable,Traits>::iterator_type cIterable<Iterable,Traits>::erase(iterator_type i_it)
{
    node_pointer_type currNode = i_it.getCurrentNode();
    pair_node* currPair = reinterpret_cast<pair_node*>(const_cast<typename mpl::drop_constness<node_pointer_type>::type>(currNode));
    const nested_iterable_interface* firstBackend = static_cast<const cIterableImplBackEnd<Traits::numIterables,Traits,0>*>(this);
    inner_iterable_private_interface& firstPrivateIterable = const_cast<inner_iterable_private_interface&>(firstBackend->getPrivateIterable());
    //by construction we know that this object is in the end a non const iterable, the thing is that we have to help the compiler to find the pointer to the proper interface by some pointer arithmetic
    inner_iterable& firstIterable = *static_cast<inner_iterable *>((void *)((u8*)&firstPrivateIterable - sizeof(inner_iterable)));

    iterator_type it(*this);

    if(currPair->innerNode != inner_iterator_type::m_pHead)
    {
        inner_node_pointer_type nextNode = firstPrivateIterable.getNextElem(currPair->innerNode);
        firstIterable.eraseNode(currPair->innerNode);

        it.setCurrentNode(this->createPair(firstBackend,nextNode));
    }

    return it;
}
template<template <typename> class Iterable, typename  Traits>
typename cIterable<Iterable,Traits>::iterator_type cIterable<Iterable,Traits>::add(add_type i_value)
{
    const nested_iterable_interface* firstBackend = static_cast<const cIterableImplBackEnd<Traits::numIterables,Traits,0>*>(this);
    inner_iterable_private_interface& firstPrivateIterable = const_cast<inner_iterable_private_interface&>(firstBackend->getPrivateIterable());
    //by construction we know that this object is in the end a non const iterable, the thing is that we have to help the compiler to find the pointer to the proper interface by some pointer arithmetic
    inner_iterable& firstIterable = *static_cast<inner_iterable *>((void *)((u8*)&firstPrivateIterable - sizeof(inner_iterable)));

    inner_node_pointer_type newNode = firstIterable.addNode(mpl::forward<add_type>(i_value));

    iterator_type it(*this);

    it.setCurrentNode(this->createPair(firstBackend,newNode));

    return it;
}

}
}
}
