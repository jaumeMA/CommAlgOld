
namespace yame
{
namespace ytl
{
namespace detail
{

template<int AccessType, typename ... Types>
cSignalFunctor<AccessType,Types...>::cSignalFunctor(const function<void(Types...)>& call)
: connection_base()
, m_call(call)
, m_pNext(NULL)
{
}
template<int AccessType, typename ... Types>
cSignalFunctor<AccessType,Types...>::cSignalFunctor(cSignalFunctor<AccessType,Types...>&& other)
: connection_base(mpl::move(other))
, m_call(mpl::move(other.m_call))
, m_pNext(other.m_pNext)
{
}
template<int AccessType, typename ... Types>
cSignalFunctor<AccessType,Types...>::~cSignalFunctor()
{
    if(m_pNext)
    {
        delete m_pNext;
    }
}
template<int AccessType, typename ... Types>
cSignalFunctor<AccessType,Types...>* cSignalFunctor<AccessType,Types...>::setNextNode(cSignalFunctor<AccessType,Types...>* pNext)
{
    ASSERT(m_pNext==NULL, "Setting next node in a already set node");

    m_pNext = pNext;

    return m_pNext;
}
template<int AccessType, typename ... Types>
cSignalFunctor<AccessType,Types...>* cSignalFunctor<AccessType,Types...>::getNextNode() const
{
    return m_pNext;
}
template<int AccessType, typename ... Types>
void cSignalFunctor<AccessType,Types...>::operator()(typename mpl::add_reference<const Types>::type ... args)
{
    ASSERT(this->isConnected(), "Calling disconnected slot");

    m_call.eval(args...);
}
template<int AccessType, typename ... Types>
bool cSignalFunctor<AccessType,Types...>::operator==(const cSignalFunctor<AccessType,Types...>& other) const
{
    return m_call==other.m_call;
}

}
}
}
