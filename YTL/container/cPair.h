
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

#include "YTL/mpl/cTemplateHelper.h"

namespace yame
{
namespace container
{
template<typename T, typename R>
class cPair
{
private:
    template<typename>
    struct isPair;

    template<typename TT,typename RR>
    struct isPair<cPair<TT,RR>>
    {
        static const bool value = true;
    };
    template<typename TT>
    struct isPair
    {
        static const bool value = false;
    };

public:
    typedef T first_type;
    typedef R second_type;
    typedef typename mpl::add_reference<T>::type first_type_reference;
    typedef typename mpl::add_reference<R>::type second_type_reference;

    cPair<T,R>();
    template<typename TFirst>
    cPair<T,R>(TFirst&& i_first, typename mpl::enable_if<isPair<typename mpl::drop_constness<typename mpl::drop_reference<typename mpl::drop_pointer<TFirst>::type>::type>::type>::value == false>::type* foo = NULL);
    template<typename TFirst, typename TSecond, typename ... TSeconds>
    cPair<T,R>(TFirst&& i_first, TSecond&& i_arg, TSeconds&& ... i_args);
    cPair<T,R>(const cPair<T,R>& other);
    cPair<T,R>(cPair<T,R>&& other);
    virtual ~cPair<T,R>();
    cPair<T,R>& operator=(const cPair<T,R>& other);
    cPair<T,R>& operator=(cPair<T,R>&& other);
    bool operator==(const cPair<T,R>& other) const;

private:
    first_type m_first;
    second_type m_second;

public:
    first_type_reference first;
    second_type_reference second;
};

typedef cPair<s32,s32> cRange;


template<typename T, typename R>
inline cPair<T,R> make_pair(T i_first, R i_second);

}

namespace mpl
{

template<typename T, typename R>
struct is_copy_constructible<container::cPair<T,R>>
{
    static const bool value = is_pointer<T>::value == false && is_copy_constructible<T>::value && is_pointer<R>::value == false && is_copy_constructible<R>::value;
};

template<typename>
struct AddType;

template<typename T, typename R>
struct AddType<container::cPair<T,R>>
{
    typedef typename drop_reference<T>::type rawT;
    typedef typename drop_reference<R>::type rawR;
    typedef typename static_if<is_copy_constructible<container::cPair<T,R>>::value, typename add_reference<typename add_constness<container::cPair<rawT,rawR>>::type>::type, typename add_rreference<container::cPair<rawT,rawR>>::type>::type type;
};

template<typename T, typename R>
struct PairType;

template<typename T, typename R>
struct PairType
{
    typedef typename drop_reference<T>::type rawT;
    typedef typename drop_reference<R>::type rawR;
    typedef typename static_if<is_pointer<rawT>::value == false && is_copy_constructible<rawT>::value, typename add_reference<typename add_constness<rawT>::type>::type, typename add_rreference<rawT>::type>::type first_type;
    typedef typename static_if<is_pointer<rawR>::value == false && is_copy_constructible<rawR>::value, typename add_reference<typename add_constness<rawR>::type>::type, typename add_rreference<rawR>::type>::type second_type;
    typedef typename static_if<is_copy_constructible<container::cPair<T,R>>::value, typename add_reference<typename add_constness<container::cPair<rawT,rawR>>::type>::type, typename add_rreference<container::cPair<rawT,rawR>>::type>::type type;
};

}
}

#include "detail/cPair.tpp"
