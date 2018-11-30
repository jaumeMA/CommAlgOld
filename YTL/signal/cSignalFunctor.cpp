#include "YTL/signal/cSignalFunctor.h"

namespace yame
{
namespace ytl
{
namespace detail
{

void single_thread_connection_base::connect(bool i_connect)
{
    ASSERT(m_isConnected != i_connect, (i_connect) ? "Connecting already connected signal" : "Disconnecting already disconnected signal");

    m_isConnected = i_connect;
}
bool single_thread_connection_base::isConnected() const
{
    return m_isConnected;
}

multi_thread_connection_base::multi_thread_connection_base()
{
    m_isConnected.store(true);
}
void multi_thread_connection_base::connect(bool i_connect)
{
    bool isCurrentlyConnected = m_isConnected.load();

    do
    {
        ASSERT(isCurrentlyConnected != i_connect, (i_connect) ? "Connecting already connected signal" : "Disconnecting already disconnected signal");

        isCurrentlyConnected = i_connect;
    }
    while(m_isConnected.compare_exchange_weak(isCurrentlyConnected,i_connect));
}
bool multi_thread_connection_base::isConnected() const
{
    return m_isConnected.load();
}

}
}
}
