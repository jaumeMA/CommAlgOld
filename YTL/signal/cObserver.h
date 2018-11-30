
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

#include "container/cArray.h"

namespace yame
{
//by now we set this functionality in io namespace (maybe its ok maybe not)
namespace io
{
template<typename obsInterface>
class cObserver;

//this class is the one who must be inherited from if you want your class to notify
template<typename ... obsInterfaces>
class cObservable;

template<typename obsInterface>
class cObservable<obsInterface>
{
public:
    void registerObserver(cObserver<obsInterface>* observer);
    void unRegisterObserver(cObserver<obsInterface>* observer);
    virtual size_t getNumObservers() const;
    virtual bool empty() const;
    virtual void clear();
    template<typename Return, typename ... FuncTypes, typename ... Args>
    void notify(Return(obsInterface::*_funcPtr)(FuncTypes...), Args&& ... args);

private:

    container::cArray<cObserver<obsInterface> *>  m_observers;
};

template<typename obsInterface, typename ... obsInterfaces>
class cObservable<obsInterface,obsInterfaces...> : public cObservable<obsInterface>, public cObservable<obsInterfaces...>
{
public:
    operator cObservable<obsInterface>&();
    size_t getNumObservers() const;
    bool empty() const;
    void clear();
};


//this class is the one who must be inherited from if you want your class to be notified
template<typename obsInterface>
class cObserver : public obsInterface
{
public:
    cObserver();
    virtual ~cObserver();
};
}
}

#include "detail/cObserver.tpp"
