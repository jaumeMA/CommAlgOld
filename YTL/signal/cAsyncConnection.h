
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

class async_connection
{
public:
    async_connection() = default;
    async_connection(detail::multi_thread_connection_base& i_conn);
    async_connection(const async_connection& other) = delete;
    async_connection(async_connection&& other);

    bool is_connected() const;
    void connect();
    void disconnect();
    async_connection& operator=(const async_connection& other) = delete;
    async_connection& operator=(detail::multi_thread_connection_base& other);
    async_connection& operator=(async_connection&& other);
    operator bool() const;

private:
    detail::multi_thread_connection_base* m_referredConnection = NULL;
};

}
}
