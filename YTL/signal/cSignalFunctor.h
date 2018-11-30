
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

#include "YTL/functional/cFunction.h"
#include <atomic>

namespace yame
{
namespace ytl
{
namespace detail
{

enum class SignalType
{
    SingleThreaded,
    MultiThreaded
};

class single_thread_connection_base
{
public:
    single_thread_connection_base() = default;
    virtual ~single_thread_connection_base() = default;

    void connect(bool i_connect);
    bool isConnected() const;

private:
    bool m_isConnected = true;
};

class multi_thread_connection_base
{
public:
    multi_thread_connection_base();
    virtual ~multi_thread_connection_base() = default;

    void connect(bool i_connect);
    bool isConnected() const;

private:
    std::atomic<bool> m_isConnected;
};

template<int AccessType, typename ... Types>
class cSignalFunctor : public mpl::static_if<AccessType==static_cast<int>(SignalType::SingleThreaded),single_thread_connection_base,multi_thread_connection_base>::type
{
    typedef typename mpl::static_if<AccessType==static_cast<int>(SignalType::SingleThreaded),single_thread_connection_base,multi_thread_connection_base>::type connection_base;

public:
    cSignalFunctor(const function<void(Types...)>& call);
    cSignalFunctor(const cSignalFunctor<AccessType,Types...>& other) = delete;
    cSignalFunctor(cSignalFunctor<AccessType,Types...>&& other);
    ~cSignalFunctor();

    cSignalFunctor<AccessType,Types...>* setNextNode(cSignalFunctor<AccessType,Types...>* pNext);
    cSignalFunctor<AccessType,Types...>* getNextNode() const;
    void operator()(typename mpl::add_reference<const Types>::type ... args);
    bool operator==(const cSignalFunctor<AccessType,Types...>& other) const;

private:
    function<void(Types...)>  m_call;
    cSignalFunctor<AccessType,Types...>* m_pNext;
};

}
}
}

#include "YTL/signal/detail/cSignalFunctor.tpp"
