
namespace yame
{
namespace container
{
namespace detail
{

template<typename IterableBackEnd,typename Traits>
cForwardIterableImplBackEnd<IterableBackEnd,Traits>::cForwardIterableImplBackEnd(nested_iterable_interface& prevIterable)
{
}
template<typename IterableBackEnd,typename Traits>
template<typename T>
const typename cForwardIterableImplBackEnd<IterableBackEnd,Traits>::nested_iterable_interface* cForwardIterableImplBackEnd<IterableBackEnd,Traits>::_getNextIterable(const T* iterableBackEnd, decltype(&T::m_nestedIterable) foo) const
{
    return &iterableBackEnd->m_nestedIterable;
}
template<typename IterableBackEnd,typename Traits>
const typename cForwardIterableImplBackEnd<IterableBackEnd,Traits>::nested_iterable_interface* cForwardIterableImplBackEnd<IterableBackEnd,Traits>::_getNextIterable(...) const
{
    return NULL;
}
template<typename IterableBackEnd,typename Traits>
template<typename T>
typename cForwardIterableImplBackEnd<IterableBackEnd,Traits>::nested_iterable_interface* cForwardIterableImplBackEnd<IterableBackEnd,Traits>::_getNextIterable(T* iterableBackEnd, decltype(&T::m_nestedIterable) foo)
{
    return &iterableBackEnd->m_nestedIterable;
}
template<typename IterableBackEnd,typename Traits>
typename cForwardIterableImplBackEnd<IterableBackEnd,Traits>::nested_iterable_interface* cForwardIterableImplBackEnd<IterableBackEnd,Traits>::_getNextIterable(...)
{
    return NULL;
}
template<typename IterableBackEnd,typename Traits>
const typename cForwardIterableImplBackEnd<IterableBackEnd,Traits>::nested_iterable_interface* cForwardIterableImplBackEnd<IterableBackEnd,Traits>::getNextIterable() const
{
    return _getNextIterable(static_cast<const IterableBackEnd*>(this));
}
template<typename IterableBackEnd,typename Traits>
typename cForwardIterableImplBackEnd<IterableBackEnd,Traits>::nested_iterable_interface* cForwardIterableImplBackEnd<IterableBackEnd,Traits>::getNextIterable()
{
    return _getNextIterable(static_cast<IterableBackEnd*>(this));
}

template<typename IterableBackEnd,typename Traits>
cBidirectionalIterableImplBackEnd<IterableBackEnd,Traits>::cBidirectionalIterableImplBackEnd(nested_iterable_interface& i_prevIterable)
: m_prevIterable(&i_prevIterable)
{
}
template<typename IterableBackEnd,typename Traits>
template<typename T>
const typename cBidirectionalIterableImplBackEnd<IterableBackEnd,Traits>::nested_iterable_interface* cBidirectionalIterableImplBackEnd<IterableBackEnd,Traits>::_getNextIterable(const T* iterableBackEnd, decltype(&T::m_nestedIterable) foo) const
{
    return &iterableBackEnd->m_nestedIterable;
}
template<typename IterableBackEnd,typename Traits>
const typename cBidirectionalIterableImplBackEnd<IterableBackEnd,Traits>::nested_iterable_interface* cBidirectionalIterableImplBackEnd<IterableBackEnd,Traits>::_getNextIterable(...) const
{
    return NULL;
}
template<typename IterableBackEnd,typename Traits>
template<typename T>
typename cBidirectionalIterableImplBackEnd<IterableBackEnd,Traits>::nested_iterable_interface* cBidirectionalIterableImplBackEnd<IterableBackEnd,Traits>::_getNextIterable(T* iterableBackEnd, decltype(&T::m_nestedIterable) foo)
{
    return &iterableBackEnd->m_nestedIterable;
}
template<typename IterableBackEnd,typename Traits>
typename cBidirectionalIterableImplBackEnd<IterableBackEnd,Traits>::nested_iterable_interface* cBidirectionalIterableImplBackEnd<IterableBackEnd,Traits>::_getNextIterable(...)
{
    return NULL;
}
template<typename IterableBackEnd,typename Traits>
const typename cBidirectionalIterableImplBackEnd<IterableBackEnd,Traits>::nested_iterable_interface* cBidirectionalIterableImplBackEnd<IterableBackEnd,Traits>::getNextIterable() const
{
    return _getNextIterable(static_cast<const IterableBackEnd*>(this));
}
template<typename IterableBackEnd,typename Traits>
typename cBidirectionalIterableImplBackEnd<IterableBackEnd,Traits>::nested_iterable_interface* cBidirectionalIterableImplBackEnd<IterableBackEnd,Traits>::getNextIterable()
{
    return _getNextIterable(static_cast<IterableBackEnd*>(this));
}
template<typename IterableBackEnd,typename Traits>
const typename cBidirectionalIterableImplBackEnd<IterableBackEnd,Traits>::nested_iterable_interface* cBidirectionalIterableImplBackEnd<IterableBackEnd,Traits>::getPrevIterable() const
{
    return m_prevIterable;
}
template<typename IterableBackEnd,typename Traits>
typename cBidirectionalIterableImplBackEnd<IterableBackEnd,Traits>::nested_iterable_interface* cBidirectionalIterableImplBackEnd<IterableBackEnd,Traits>::getPrevIterable()
{
    return m_prevIterable;
}

template<size_t NumIterables, typename Traits>
cIterableImplBackEnd<NumIterables,Traits,0>::cIterableImplBackEnd(ytl::lent_ref<iterable_private_interface> i_iterable)
: backend_base()
, m_privateIterable(i_iterable)
{
}
template<size_t NumIterables, typename Traits>
cIterableImplBackEnd<NumIterables,Traits,0>::cIterableImplBackEnd(nested_iterable_interface& i_prevIterable, ytl::lent_ref<iterable_private_interface> i_iterable)
: backend_base(i_prevIterable)
, m_privateIterable(i_iterable)
{
}
template<size_t NumIterables, typename Traits>
const typename Traits::iterable_private_interface& cIterableImplBackEnd<NumIterables,Traits,0>::getPrivateIterable() const
{
    return *m_privateIterable;
}
template<size_t NumIterables, typename Traits>
typename Traits::iterable_private_interface& cIterableImplBackEnd<NumIterables,Traits,0>::getPrivateIterable()
{
    return *m_privateIterable;
}
template<size_t NumIterables, typename Traits>
template<int IterablePosition>
const typename cIterableImplBackEnd<NumIterables,Traits,0>::nested_iterable_interface& cIterableImplBackEnd<NumIterables,Traits,0>::getNthIterable() const
{
    static_assert(IterablePosition==0,"Index out of bounds");

    return *this;
}
template<size_t NumIterables, typename Traits>
template<int IterablePosition>
typename cIterableImplBackEnd<NumIterables,Traits,0>::nested_iterable_interface& cIterableImplBackEnd<NumIterables,Traits,0>::getNthIterable()
{
    static_assert(IterablePosition==0,"Index out of bounds");

    return *this;
}

template<size_t NumIterables, typename Traits, size_t IterablePos>
template<typename ... IterableTypes>
cIterableImplBackEnd<NumIterables,Traits,IterablePos>::cIterableImplBackEnd(ytl::lent_ref<iterable_private_interface> i_privateIterable, IterableTypes&& ... i_privateIterables)
: backend_base()
, m_privateIterable(i_privateIterable)
, m_nestedIterable(*this,mpl::forward<IterableTypes>(i_privateIterables)...)
{
}
template<size_t NumIterables, typename Traits, size_t IterablePos>
template<typename ... IterableTypes>
cIterableImplBackEnd<NumIterables,Traits,IterablePos>::cIterableImplBackEnd(nested_iterable_interface& i_prevIterable, ytl::lent_ref<iterable_private_interface> i_privateIterable, IterableTypes&& ... i_privateIterables)
: backend_base(i_prevIterable)
, m_privateIterable(i_privateIterable)
, m_nestedIterable(*this,mpl::forward<IterableTypes>(i_privateIterables)...)
{
}
template<size_t NumIterables, typename Traits, size_t IterablePos>
const typename cIterableImplBackEnd<NumIterables,Traits,IterablePos>::iterable_private_interface& cIterableImplBackEnd<NumIterables,Traits,IterablePos>::getPrivateIterable() const
{
    return *m_privateIterable;
}
template<size_t NumIterables, typename Traits, size_t IterablePos>
typename cIterableImplBackEnd<NumIterables,Traits,IterablePos>::iterable_private_interface& cIterableImplBackEnd<NumIterables,Traits,IterablePos>::getPrivateIterable()
{
    return *m_privateIterable;
}
template<size_t NumIterables, typename Traits, size_t IterablePos>
template<int IterablePosition>
const typename cIterableImplBackEnd<NumIterables,Traits,IterablePos>::nested_iterable_interface& cIterableImplBackEnd<NumIterables,Traits,IterablePos>::getNthIterable() const
{
    if(IterablePosition == 0)
    {
        return *this;
    }
    else
    {
        return m_nestedIterable.template getNthIterable<IterablePosition-1>();
    }
}
template<size_t NumIterables, typename Traits, size_t IterablePos>
template<int IterablePosition>
typename cIterableImplBackEnd<NumIterables,Traits,IterablePos>::nested_iterable_interface& cIterableImplBackEnd<NumIterables,Traits,IterablePos>::getNthIterable()
{
    if(IterablePosition == 0)
    {
        return *this;
    }
    else
    {
        return m_nestedIterable.template getNthIterable<IterablePosition-1>();
    }
}

}
}
}
