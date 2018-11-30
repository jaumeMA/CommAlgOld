
namespace yame
{
    namespace io
    {
        template<typename obsInterface>
        void cObservable<obsInterface>::registerObserver(cObserver<obsInterface>* observer)
        {
            if(observer)
            {
                m_observers.push_back(observer);
            }

            return;
        }
        template<typename obsInterface>
        void cObservable<obsInterface>::unRegisterObserver(cObserver<obsInterface>* observer)
        {
            typename container::cArray<cObserver<obsInterface> *>::iterator itObserver = container::find(m_observers.begin(),m_observers.end(),observer);

            if(itObserver != m_observers.end())
            {
                m_observers.erase(itObserver);
            }

            return;
        }
        template<typename obsInterface>
        size_t cObservable<obsInterface>::getNumObservers() const
        {
            return m_observers.getSize();
        }
        template<typename obsInterface>
        bool cObservable<obsInterface>::empty() const
        {
            return m_observers.empty();
        }
        template<typename obsInterface>
        void cObservable<obsInterface>::clear()
        {
            m_observers.clear();

            return;
        }
        template<typename obsInterface>
        template<typename Return, typename ... FuncTypes, typename ... Args>
        void cObservable<obsInterface>::notify(Return(obsInterface::*_funcPtr)(FuncTypes...), Args&& ... args)
        {
            //just notify
            //static_assert(mpl::can_construct<FuncTypes ...>::template with<Args&& ...>::value, "Passed value cannot construct types from call");

            if(_funcPtr != NULL)
            {
                typename container::cArray<cObserver<obsInterface> *>::iterator itObserver = m_observers.begin();

                for(;itObserver!=m_observers.end();itObserver++)
                {
                    cObserver<obsInterface> *observer = *itObserver;

                    if(observer)
                    {
                        (observer->*_funcPtr)(mpl::forward<Args>(args)...);
                    }
                }
            }

        }
        template<typename obsInterface, typename ... obsInterfaces>
        cObservable<obsInterface,obsInterfaces...>::operator cObservable<obsInterface>&()
        {
            return *this;
        }
        template<typename obsInterface, typename ... obsInterfaces>
        size_t cObservable<obsInterface,obsInterfaces...>::getNumObservers() const
        {
            return cObservable<obsInterface>::getNumObservers() + cObservable<obsInterfaces...>::getNumObservers();
        }
        template<typename obsInterface, typename ... obsInterfaces>
        bool cObservable<obsInterface,obsInterfaces...>::empty() const
        {
            return cObservable<obsInterface>::empty() && cObservable<obsInterfaces...>::empty();
        }
        template<typename obsInterface, typename ... obsInterfaces>
        void cObservable<obsInterface,obsInterfaces...>::clear()
        {
            cObservable<obsInterface>::clear();

            cObservable<obsInterfaces...>::clear();

            return;
        }

        template<typename obsInterface>
        cObserver<obsInterface>::cObserver()
        : obsInterface()
        {
        }
        template<typename obsInterface>
        cObserver<obsInterface>::~cObserver()
        {
        }
    }
}
