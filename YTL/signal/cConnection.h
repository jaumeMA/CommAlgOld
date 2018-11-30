
/*This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#pragma once

#include "YTL/signal/cSignalFunctor.h"

namespace yame
{
namespace ytl
{

class connection
{
public:
    connection() = default;
    connection(detail::single_thread_connection_base& i_conn);
    connection(const connection& other) = delete;
    connection(connection&& other);

    bool is_connected() const;
    void connect();
    void disconnect();
    connection& operator=(const connection& other) = delete;
    connection& operator=(detail::single_thread_connection_base& other);
    connection& operator=(connection&& other);
    operator bool() const;

private:
    detail::single_thread_connection_base* m_referredConnection = NULL;
};

}
}
