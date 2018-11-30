
#include "YTL/signal/cAsyncConnection.h"

namespace yame
{
namespace ytl
{

async_connection::async_connection(detail::multi_thread_connection_base& i_conn)
: m_referredConnection(&i_conn)
{}
async_connection::async_connection(async_connection&& other)
: m_referredConnection(other.m_referredConnection)
{
    other.m_referredConnection = NULL;
}
bool async_connection::is_connected() const
{
    return m_referredConnection != NULL && m_referredConnection->isConnected();
}
void async_connection::connect()
{
    ASSERT(m_referredConnection, "Trying to connect an unwired async_connection");

    m_referredConnection->connect(true);
}
void async_connection::disconnect()
{
    ASSERT(m_referredConnection, "Trying to disconnect an unwired async_connection");

    m_referredConnection->connect(false);
}
async_connection& async_connection::operator=(detail::multi_thread_connection_base& other)
{
    ASSERT(m_referredConnection == NULL, "Trying to wire an already wired async_connection, you must disconnect it first fro signal");

    m_referredConnection = &other;

    return *this;
}
async_connection& async_connection::operator=(async_connection&& other)
{
    m_referredConnection = other.m_referredConnection;

    other.m_referredConnection = NULL;

    return *this;
}
async_connection::operator bool() const
{
    return m_referredConnection != NULL;
}

}
}

