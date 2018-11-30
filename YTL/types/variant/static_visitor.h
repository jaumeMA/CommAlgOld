
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

namespace yame
{
namespace ytl
{
template<typename T, typename ... Args>
class static_visitor
{
public:
    typedef T result_type;

template<typename TT>
    result_type operator()(TT&&, Args ... args)
    {
        static_assert(sizeof(TT)==0, "You haven't provided a call operator for this type in your static_visitor implementation!");
    }

protected:
//we don't want to allow direct instances, but only by inheritance
    static_visitor() {};
    virtual ~static_visitor(){};
};

namespace detail
{
template<typename T>
struct visitor_invoker
{    typedef typename T::result_type result_type;    typedef T Visitor;
public:    visitor_invoker(T& visitor) : m_visitor(visitor) {};
    template<typename TT, typename ... Args>    result_type outer_visit(TT&& t_instance, void* foo, Args&& ... args)    {        return m_visitor(mpl::forward<TT>(t_instance), mpl::forward<Args>(args)...);    }
    template<typename Type, typename TT>    result_type inner_visit(TT&& t_instance)    {        return m_visitor.template operator()<Type,TT>(mpl::forward<TT>(t_instance));    }private:    T& m_visitor;};}
}
}
