
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
#include "YTL/signal/cConnection.h"
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
class signal_executor
{
public:
    virtual void execute(typename mpl::add_reference<const Types>::type ... args) = 0;
};

}

template<typename,typename=void>
class signal;

template<typename ... Types, typename KeyExec>
class signal<void(Types...),KeyExec> : private detail::signal_executor<Types...>
{
    YTL_PUBLISH_ACCESS(KeyExec,signal,detail::signal_executor<Types...>)

public:
    signal() = default;
    signal(signal<void(Types...),KeyExec>&& other);
    ~signal();

    signal(const signal<void(Types...),KeyExec>& other) = delete;
    signal<void(Types...),KeyExec>& operator=(const signal<void(Types...),KeyExec>& other) = delete;

    //non static member functions
    template<typename T, typename baseT, typename ... ExtraTypes, typename ... ExtraArgs>
    connection connect(T *object, void (baseT::*hook)(Types...,ExtraTypes ...), typename mpl::make_type_place_holder<Types>::type ... vals, ExtraArgs&& ... extraVals);

    //static member/free functions
    template<typename ... ExtraTypes, typename ... ExtraArgs>
    connection connect(void(*hook)(Types...,ExtraTypes ...), typename mpl::make_type_place_holder<Types>::type ... vals, ExtraArgs&& ... extraVals);

    //functors
    template<typename T, typename ... ExtraArgs>
    connection connect(const T& hook, typename mpl::make_type_place_holder<Types>::type ... vals, ExtraArgs&& ... extraVals);

private:
    void execute(typename mpl::add_reference<const Types>::type ... args) override;

    void disconnect();

    detail::cSignalFunctor<static_cast<int>(detail::SignalType::SingleThreaded),Types...>* m_pFirst = NULL;
    detail::cSignalFunctor<static_cast<int>(detail::SignalType::SingleThreaded),Types...>* m_pLast = NULL;
};

}
}

#include "YTL/signal/detail/cSignal.tpp"
