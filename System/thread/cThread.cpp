#include "cThread.h"

namespace yame
{
namespace system
{

thread::thread()
{
    m_stopped.store(true);
}
void thread::start(const ytl::function<void()>& i_callable)
{
    bool isStopped = m_stopped.load();

    if(isStopped == true)
    {
        if(m_stopped.compare_exchange_weak(isStopped,false))
        {
            m_thread = std::thread(i_callable);
        }
    }
}
void thread::stop()
{
    bool isStopped = m_stopped.load();

    if(isStopped == false)
    {
        if(m_stopped.compare_exchange_weak(isStopped,true))
        {
            m_thread.join();
        }
    }
}
bool thread::is_stopped() const
{
    return m_stopped.load();
}
bool thread::is_stoppable() const
{
    return m_thread.joinable() && m_stopped.load() == false;
}

}
}
