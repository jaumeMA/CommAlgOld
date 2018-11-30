
#include "YTL/signal/cConnection.h"

namespace yame
{
namespace ytl
{

connection::connection(detail::single_thread_connection_base& i_conn)
: m_referredConnection(&i_conn)
{}
connection::connection(connection&& other)
: m_referredConnection(other.m_referredConnection)
{
    other.m_referredConnection = NULL;
}
bool connection::is_connected() const
{
    return m_referredConnection != NULL && m_referredConnection->isConnected();
}
void connection::connect()
{
    ASSERT(m_referredConnection, "Trying to connect an unwired connection");

    m_referredConnection->connect(true);
}
void connection::disconnect()
{
    ASSERT(m_referredConnection, "Trying to disconnect an unwired connection");

    m_referredConnection->connect(false);
}
connection& connection::operator=(detail::single_thread_connection_base& other)
{
    ASSERT(m_referredConnection == NULL, "Trying to wire an already wired connection, you must disconnect it first fro signal");

    m_referredConnection = &other;

    return *this;
}
connection& connection::operator=(connection&& other)
{
    m_referredConnection = other.m_referredConnection;

    other.m_referredConnection = NULL;

    return *this;
}
connection::operator bool() const
{
    return m_referredConnection != NULL;
}

}
}
