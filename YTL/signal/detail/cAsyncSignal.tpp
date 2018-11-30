
namespace yame
{
namespace ytl
{

template<typename ... Types,typename KeyExec>
async_signal<void(Types...),KeyExec>::async_signal(async_signal<void(Types...),KeyExec>&& other)
{
    m_pFirst = other.m_pFirst;
    m_pLast = other.m_pLast;

    other.m_pFirst = NULL;
    other.m_pLast = NULL;
}
template<typename ... Types,typename KeyExec>
async_signal<void(Types...),KeyExec>::~async_signal()
{
    disconnect();

    return;
}
template<typename ... Types,typename KeyExec>
template<typename T, typename baseT, typename ... ExtraTypes, typename ... ExtrArgs>
async_connection async_signal<void(Types...),KeyExec>::connect(T *object, void (baseT::*hook)(Types...,ExtraTypes ...), typename mpl::make_type_place_holder<Types>::type ... vals, ExtrArgs&& ... extraVals)
{
    static_assert(mpl::is_base_of<baseT,T>::value,"There is a mismatch in method and object types!");

    function<void(Types...,ExtraTypes...)> funcNotSpec(object,hook);

    m_pLast = (m_pLast) ? m_pLast->setNextNode(new detail::cSignalFunctor<static_cast<int>(detail::SignalType::MultiThreaded),Types...>(funcNotSpec(vals..., mpl::forward<ExtrArgs>(extraVals)...)))
                        : new detail::cSignalFunctor<static_cast<int>(detail::SignalType::MultiThreaded),Types...>(funcNotSpec(vals..., mpl::forward<ExtrArgs>(extraVals)...));

    if(m_pFirst == NULL)
    {
        m_pFirst = m_pLast;
    }

    return *m_pLast;
}
template<typename ... Types,typename KeyExec>
template<typename ... ExtraTypes, typename ... ExtraArgs>
async_connection async_signal<void(Types...),KeyExec>::connect(void (*hook)(Types...,ExtraTypes ...), typename mpl::make_type_place_holder<Types>::type ... vals, ExtraArgs&& ... extraVals)
{
    function<void(Types...,ExtraTypes...)> funcNotSpec(hook);

    m_pLast = (m_pLast) ? m_pLast->setNextNode(new detail::cSignalFunctor<static_cast<int>(detail::SignalType::MultiThreaded),Types...>(funcNotSpec(vals..., mpl::forward<ExtraArgs>(extraVals)...)))
                        : new detail::cSignalFunctor<static_cast<int>(detail::SignalType::MultiThreaded),Types...>(funcNotSpec(vals..., mpl::forward<ExtraArgs>(extraVals)...));

    if(m_pFirst == NULL)
    {
        m_pFirst = m_pLast;
    }

    return *m_pLast;
}
template<typename ... Types,typename KeyExec>
template<typename T, typename ... ExtraArgs>
async_connection async_signal<void(Types...),KeyExec>::connect(const T& hook, typename mpl::make_type_place_holder<Types>::type ... vals, ExtraArgs&& ... extraVals)
{
    function<void(Types...,ExtraArgs...)> funcNotSpec(hook);

    m_pLast = (m_pLast) ? m_pLast->setNextNode(new detail::cSignalFunctor<static_cast<int>(detail::SignalType::MultiThreaded),Types...>(funcNotSpec(vals..., mpl::forward<ExtraArgs>(extraVals)...)))
                        : new detail::cSignalFunctor<static_cast<int>(detail::SignalType::MultiThreaded),Types...>(funcNotSpec(vals..., mpl::forward<ExtraArgs>(extraVals)...));

    if(m_pFirst == NULL)
    {
        m_pFirst = m_pLast;
    }

    return *m_pLast;
}
template<typename ... Types,typename KeyExec>
void async_signal<void(Types...),KeyExec>::disconnect()
{
    if(m_pFirst)
    {
        delete m_pFirst;
    }

    return;
}
template<typename ... Types,typename KeyExec>
void async_signal<void(Types...),KeyExec>::execute(typename mpl::add_reference<const Types>::type ... args)
{
    detail::cSignalFunctor<static_cast<int>(detail::SignalType::MultiThreaded),Types...>* currSlot = m_pFirst;

    while(currSlot)
    {
        if(currSlot->isConnected())
        {
            (*currSlot)(args...);
        }

        currSlot = currSlot->getNextNode();
    }

    return;
}

}
}

