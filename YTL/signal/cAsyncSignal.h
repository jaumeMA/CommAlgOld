
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

#include "YTL/container/cQueue.h"
#include "YTL/signal/cSignalFunctor.h"
#include "YTL/signal/cAsyncConnection.h"
#include "YTL/mpl/cFunctionTemplateHelper.h"
#include "YTL/mpl/cAccessBase.h"

extern yame::mpl::place_holder _arg;

namespace yame
{
namespace ytl
{
namespace detail
{

template<typename ... Types>
class async_signal_executor
{
public:
    virtual void execute(typename mpl::add_reference<const Types>::type ... args) = 0;
};

}

template<typename,typename=void>
class async_signal;

template<typename ... Types, typename KeyExec>
class async_signal<void(Types...),KeyExec> : private detail::async_signal_executor<Types...>
{
    YTL_PUBLISH_ACCESS(KeyExec,async_signal,detail::async_signal_executor<Types...>);

public:
    async_signal() = default;
    async_signal(async_signal<void(Types...),KeyExec>&& other);
    ~async_signal();

    async_signal(const async_signal<void(Types...),KeyExec>& other) = delete;
    async_signal<void(Types...),KeyExec>& operator=(const async_signal<void(Types...),KeyExec>& other) = delete;

    //non static member functions
    template<typename T, typename baseT, typename ... ExtraTypes, typename ... ExtraArgs>
    async_connection connect(T *object, void(baseT::*hook)(Types...,ExtraTypes ...), typename mpl::make_type_place_holder<Types>::type ... vals, ExtraArgs&& ... extraVals);

    //static member/free functions
    template<typename ... ExtraTypes, typename ... ExtraArgs>
    async_connection connect(void(*hook)(Types...,ExtraTypes ...), typename mpl::make_type_place_holder<Types>::type ... vals, ExtraArgs&& ... extraVals);

    //functors
    template<typename T, typename ... ExtraArgs>
    async_connection connect(const T& hook, typename mpl::make_type_place_holder<Types>::type ... vals, ExtraArgs&& ... extraVals);

private:
    void disconnect();
    void execute(typename mpl::add_reference<const Types>::type ... args) override;

    detail::cSignalFunctor<static_cast<int>(detail::SignalType::MultiThreaded),Types...>* m_pFirst = NULL;
    detail::cSignalFunctor<static_cast<int>(detail::SignalType::MultiThreaded),Types...>* m_pLast = NULL;
};

}
}

#include "YTL/signal/detail/cAsyncSignal.tpp"
