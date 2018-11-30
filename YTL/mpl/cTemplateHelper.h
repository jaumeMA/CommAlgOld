
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

// This file basically provides a set of helpers for manipulating variadic template parameter packs such as sum, rest, prod of variadic template packs or retrieve the n-th template parameter, remove the n-th variadic template parameter, and things like this. By now we force that helpers to work with homogeneous variadic template packs

#pragma once

#include <stdlib.h>
#include <type_traits>
#include <cstring>
#include "Utils/engineTypes.h"
#include <limits>
#include <utility>

#define SCOPE_SYMBOL ::

#define YTL_HAS_DEFINED_SYMBOL_DECL(class_scope, symbol) \
template<typename class_scope> \
struct has_defined_##symbol \
{ \
    template<typename T> \
    constexpr static bool is_there(typename T::symbol* _foo) { return true; } \
    template <typename> \
    constexpr static bool is_there(...) { return false; } \
    \
    static const bool value = is_there<class_scope>(NULL); \
};

#define YTL_HAS_DEFINED_SYMBOL(scope, symbol) \
has_defined_##symbol<scope>::value


namespace yame
{
namespace mpl
{

struct none_t
{
};

struct null_ptr_type
{
};

//stupid helper for gcc
template<typename T>
struct friend_maker
{
    typedef T type;
};

template<typename T>
struct drop_reference
{
    typedef T type;
};

template<typename T>
struct drop_reference<T&>
{
    typedef T type;
};

template<typename T>
struct drop_reference<T&&>
{
    typedef T type;
};

template<typename T>
struct drop_rreference
{
    typedef T type;
};

template<typename T>
struct drop_rreference<T&>
{
    typedef T& type;
};

template<typename T>
struct drop_rreference<T&&>
{
    typedef T type;
};

template<typename T>
struct add_reference
{
    typedef T& type;
};

template<typename T>
struct add_reference<T&>
{
    typedef T& type;
};

template<typename T>
struct add_reference<T&&>
{
    typedef T& type;
};

template<typename T>
struct add_rreference
{
    typedef T&& type;
};

template<typename T>
struct add_rreference<T&>
{
    typedef T& type;
};

template<typename T>
struct add_rreference<T&&>
{
    typedef T&& type;
};

template<typename T>
struct drop_constness
{
    typedef T type;
};

template<typename T>
struct drop_constness<const T>
{
    typedef T type;
};

template<typename T>
struct drop_constness<const T&>
{
    typedef T& type;
};

template<typename T>
struct drop_constness<const T&&>
{
    typedef T&& type;
};

template<typename T>
struct drop_constness<const T*>
{
    typedef T* type;
};

template<typename T>
struct add_constness
{
    typedef const T type;
};

template<typename T>
struct add_constness<T&>
{
    typedef const T& type;
};

template<typename T>
struct add_constness<T&&>
{
    typedef const T&& type;
};

template<typename T>
struct add_constness<T*>
{
    typedef const T* type;
};

template<typename T>
struct drop_pointer
{
    typedef T type;
};

template<typename T>
struct drop_pointer<T*>
{
    typedef T type;
};

template<typename T>
struct drop_pointer<const T*>
{
    typedef const T type;
};

template<typename T>
struct add_pointer
{
    typedef T* type;
};

//template<typename T>
//struct add_pointer<T*>
//{
//    typedef T* type;
//};
//
//template<typename T>
//struct add_pointer<const T*>
//{
//    typedef const T* type;
//};

//since we use a nested type (type) as a type value and the template parameter is at the left of this type (w.r.t scope keyword)
//this method cannot perform type deduction, which is the desired behaviour
template<typename T>
constexpr T&& forward(typename drop_reference<T>::type& arg)
{
    return static_cast<T&&>(arg);
};

template<typename T>
constexpr T&& forward(typename drop_reference<T>::type&& arg)
{
    return static_cast<T&&>(arg);
};

//even although libc uses c-like cast, i use reinterpret cast, cleaner and fit better in c++ code
template<typename T>
constexpr typename drop_reference<T>::type&& move(T&& arg)
{
    return reinterpret_cast<typename drop_reference<T>::type&&>(arg);
};

template<typename A, typename B>
struct is_same_type;

template<typename A, typename B>
struct is_same_type
{
    static const bool value = false;
};

template<typename A>
struct is_same_type<A,A>
{
    static const bool value = true;
};

template<typename A, typename ... B>
struct are_same_type;

template<typename A>
struct are_same_type<A>
{
    static const bool value = true;
};

template<typename A, typename B, typename ... BBs>
struct are_same_type<A,B,BBs...>
{
    static const bool value = mpl::is_same_type<A,B>::value && are_same_type<A,BBs...>::value;
};

template<typename T>
struct is_pointer;

template<typename T>
struct is_pointer<T *>
{
    static const bool value = true;
};

template<>
struct is_pointer<null_ptr_type>
{
    static const bool value = true;
};

template<typename T>
struct is_pointer
{
    static const bool value = false;
};

template<typename T>
struct is_const_pointer;

template<typename T>
struct is_const_pointer<const T *>
{
    static const bool value = true;
};

template<typename T>
struct is_const_pointer
{
    static const bool value = false;
};

template<typename T>
struct is_lreference;

template<typename T>
struct is_lreference<T&>
{
    static const bool value = true;
};

template<typename T>
struct is_lreference
{
    static const bool value = false;
};

template<typename T>
struct is_const;

template<typename T>
struct is_const<const T>
{
    static const bool value = true;
};

template<typename T>
struct is_const
{
    static const bool value = false;
};

template<typename T>
struct is_const_lreference
{
    static const bool value = is_lreference<T>::value && is_const<T>::value;
};

template<typename T>
struct is_rreference;

template<typename T>
struct is_rreference<T&&>
{
    static const bool value = true;
};

template<typename T>
struct is_rreference
{
    static const bool value = false;
};

template<typename T>
struct is_reference
{
    static const bool value = is_lreference<T>::value | is_rreference<T>::value;
};

template<int Value>
struct is_power_of_two;

template<>
struct is_power_of_two<1>
{
    static const bool value = true;
};

template<int Value>
struct is_power_of_two
{
    static const bool value = (Value%2) == 0 && is_power_of_two<Value/2>::value;
};

template<typename>
struct is_integral
{
    static const bool value = false;
};

template<>
struct is_integral<bool>
{
    static const bool value = true;
};

template<>
struct is_integral<char>
{
    static const bool value = true;
};

template<>
struct is_integral<unsigned char>
{
    static const bool value = true;
};

template<>
struct is_integral<short>
{
    static const bool value = true;
};

template<>
struct is_integral<unsigned short>
{
    static const bool value = true;
};

template<>
struct is_integral<int>
{
    static const bool value = true;
};

template<>
struct is_integral<unsigned int>
{
    static const bool value = true;
};

template<>
struct is_integral<long>
{
    static const bool value = true;
};

template<>
struct is_integral<unsigned long>
{
    static const bool value = true;
};

template<>
struct is_integral<long long>
{
    static const bool value = true;
};

template<>
struct is_integral<unsigned long long>
{
    static const bool value = true;
};

template<typename>
struct is_numeric
{
    static const bool value = false;
};

template<>
struct is_numeric<bool>
{
    static const bool value = true;
};

template<>
struct is_numeric<char>
{
    static const bool value = true;
};

template<>
struct is_numeric<unsigned char>
{
    static const bool value = true;
};

template<>
struct is_numeric<short>
{
    static const bool value = true;
};

template<>
struct is_numeric<unsigned short>
{
    static const bool value = true;
};

template<>
struct is_numeric<int>
{
    static const bool value = true;
};

template<>
struct is_numeric<unsigned int>
{
    static const bool value = true;
};

template<>
struct is_numeric<long>
{
    static const bool value = true;
};

template<>
struct is_numeric<unsigned long>
{
    static const bool value = true;
};

template<>
struct is_numeric<long long>
{
    static const bool value = true;
};

template<>
struct is_numeric<unsigned long long>
{
    static const bool value = true;
};

template<>
struct is_numeric<float>
{
    static const bool value = true;
};

template<>
struct is_numeric<double>
{
    static const bool value = true;
};

template<>
struct is_numeric<long double>
{
    static const bool value = true;
};

template<typename T>
struct wrapper
{
    typedef T type;
};

struct place_holder
{
    //dummy class for passing uninitialized arguments
};

static const place_holder place_arg;

template<typename T>
struct make_type_place_holder
{
    typedef place_holder type;
};

template<typename T>
struct is_place_holder
{
    static const bool value = is_same_type<T,place_holder>::value;
};

struct void_type
{
    //empty class for representing void type.
};

template<size_t Value, typename Numeric = size_t>
struct numeric_type
{
    static const Numeric value = Numeric(Value);
};

template<bool cond, typename Type=void>
struct enable_if;

template<typename Type>
struct enable_if<true,Type>
{
    typedef Type type;
};

template<typename Type>
struct enable_if<false,Type>
{
};

template<bool,typename ...>
struct static_if;

template<typename T,typename ... TT>
struct static_if<true,T,TT...>
{
    typedef T type;
};

template<typename T,typename TT>
struct static_if<false,T,TT>
{
    typedef TT type;
};

template<typename T>
struct static_if<false,T>
{
    //no type at all, enable_if like
};

template<typename ...>
struct get_num_types;

template<typename T, typename ... Types>
struct get_num_types<T,Types...>
{
    static const int value = 1 + get_num_types<Types...>::value;
};
template<>
struct get_num_types<>
{
    static const int value = 0;
};

template<template<typename> class,typename ...>
struct get_num_of_types_of;

template<template<typename> class predicate, typename Type, typename ... Types>
struct get_num_of_types_of<predicate,Type,Types...>
{
    static const int value = predicate<Type>::value + get_num_of_types_of<predicate,Types...>::value;
};
template<template<typename> class predicate>
struct get_num_of_types_of<predicate>
{
    static const int value = 0;
};

template<typename ...>
struct homogeneous_types;

template<typename Type, typename ... Types>
struct homogeneous_types<Type,Types...>
{
    static const bool value = is_same_type<Type,typename homogeneous_types<Types...>::type>::value && homogeneous_types<Types...>::value;
    typedef Type type;
};

template<typename Type>
struct homogeneous_types<Type>
{
    static const bool value = true;
    typedef Type type;
};

template<int, typename ...>
struct nth_type_of;

template<int Pos, typename Type, typename ... Types>
struct nth_type_of<Pos,Type,Types...>
{
    typedef typename nth_type_of<Pos-1,Types...>::type type;
};

template<typename Type, typename ... Types>
struct nth_type_of<0,Type,Types...>
{
    typedef Type type;
};

template<int Pos>
struct nth_type_of<Pos>
{
    typedef void_type type;
};

template<typename T>
struct transform_type
{
template<typename TT>
struct to
{
    typedef TT type;
};
};

template<int Index>
struct transform_index_type
{
template<typename TT>
struct to
{
    typedef TT type;
};
};

template<typename ... Bases>
class composed_class : public Bases ...
{};

template<int pos, typename ... Args>
struct _nth_val_of;

template<int pos, typename Arg, typename ... Args>
struct _nth_val_of<pos,Arg,Args...>
{
    static constexpr typename nth_type_of<pos,Arg,Args...>::type is(Arg arg, Args...args)
    {
        return _nth_val_of<pos-1,Args...>::is(mpl::forward<Args>(args)...);
    }
};

template<typename Arg, typename ... Args>
struct _nth_val_of<0,Arg,Args...>
{
    static constexpr Arg is(Arg arg, Args...args)
    {
        return mpl::forward<Arg>(arg);
    }
};

template<>
struct _nth_val_of<0>
{
    static void is()
    {
        return;
    }
};

template<int pos, typename ... Args>
constexpr typename nth_type_of<pos,Args&&...>::type nth_val_of(Args&& ... args)
{
    static_assert(pos >= 0 && pos < get_num_types<Args...>::value, "Index out of bounds");

    //do it 0 based
    return mpl::forward<typename nth_type_of<pos,Args&&...>::type>(_nth_val_of<pos,Args&&...>::is(mpl::forward<Args&&>(args)...));
}

// T base, TT derived
template<typename T, typename TT>
struct is_base_of;

template<typename T>
struct is_base_of<T,T>
{
    static const bool value = true;
};

template<typename T, typename TT>
struct is_base_of
{
    typedef char (&yes)[2];
    typedef char (&no)[1];
    typedef typename mpl::drop_reference<TT>::type rawTT;
    typedef typename mpl::drop_reference<T>::type rawT;

    struct Tester
    {
        operator rawT* () const;
        operator rawTT* ();
    };

    template<typename TTT>
    static yes Test(rawTT*,TTT);
    static no Test(rawT*,int);

    static const bool value = sizeof(Test(Tester(),0)) == sizeof(yes);
};

template<typename T, typename TT>
struct is_virtual_base_of;

template<typename T>
struct is_virtual_base_of<T,T>
{
    static const bool value = false;
};

template<typename T>
struct _is_virtual_base_of
{
    template<typename TT, typename = decltype(static_cast<const TT*>(std::declval<const T*>()))>
    constexpr static bool check(int)
    {
        return false;
    }
    template<typename TT, typename = void>
    constexpr static bool check(float,...)
    {
        return true;
    }
};

template<typename T, typename TT>
struct is_virtual_base_of
{
    template<bool val = _is_virtual_base_of<T>::template check<TT>(0)>
    constexpr static bool detectVirtuality()
    {
        return val;
    }

    static const bool value = is_base_of<T,TT>::value && detectVirtuality<>();
};

template<typename T>
struct instantiatePointer
{
private:
    static T __arr[0];

public:
    constexpr static const T* value = __arr;
};

template<typename T>
struct is_trivially_copiable
{
    //we have to do it manually since its intrinsic and there is no such trait in current gcc
    static const bool value = true;
};

template<typename ... Args>
struct is_constructible;

template<typename Type>
struct is_constructible<Type*>
{
    static const bool value = true;
};

template<typename Type, typename Arg>
struct is_constructible<Type*,Arg*>
{
    static const bool value = is_base_of<Arg,Type>::value;
};


template<typename Type, typename ... Args>
struct is_constructible<Type&,Args...>
{
    static_assert(get_num_types<Args...>::value == 1, "Cannot construct reference from more than one type!");

    typedef typename nth_type_of<0,Args...>::type firstArg;
    typedef typename drop_reference<firstArg>::type ArgType;
    typedef typename drop_reference<ArgType>::type ArgRawType;
    typedef typename drop_constness<Type>::type RawType;

    static const bool type_constrains = is_reference<firstArg>::value;
    static const bool inheritance_constrain = is_same_type<ArgRawType, RawType>::value || is_base_of<ArgRawType,RawType>::value;
    static const bool constness_constrain = is_const<Type>::value || is_const<ArgType>::value == false;

    static const bool value = type_constrains && inheritance_constrain && constness_constrain;
};

template<typename Type, typename ... Args>
struct is_constructible<Type&&,Args...>
{
    static_assert(get_num_types<Args...>::value == 1, "Cannot construct reference from more than one type!");

    typedef typename nth_type_of<0,Args...>::type firstArg;
    typedef typename drop_reference<firstArg>::type ArgType;
    typedef typename drop_constness<ArgType>::type RawArg;
    typedef typename drop_constness<Type>::type RawType;

    static const bool type_constrains = is_rreference<firstArg>::value || is_const_lreference<firstArg>::value;
    static const bool inheritance_constrain = is_same_type<RawArg, RawType>::value || is_base_of<RawArg,RawType>::value;
    static const bool constness_constrain = is_const<Type>::value == false || is_const<ArgType>::value;

    static const bool value = type_constrains && inheritance_constrain && constness_constrain;
};

template<typename Type, typename ... Args>
struct is_constructible<Type,Args...>
{
    template<typename T>
    static constexpr bool Test(decltype(T(std::declval<Args>() ...))* foo) { return true; }
    template<typename>
    static constexpr bool Test(...) { return false; }

    static const bool value = Test<Type>(0);
};

template<typename T>
struct is_default_constructible : is_constructible<T>
{
    using is_constructible<T>::value;
};
template<typename T>
struct is_copy_constructible;

template<typename T>
struct is_copy_constructible : is_constructible<T,const T&>
{
    using is_constructible<T,const T&>::value;
};
template<typename T>
struct is_copy_constructible<T*>
{
    static const bool value = true;
};

template<typename T>
struct is_move_constructible;

template<typename T>
struct is_move_constructible : is_constructible<T,T&&>
{
    using is_constructible<T,T&&>::value;
};
template<typename T>
struct is_move_constructible<T*>
{
    static const bool value = true;
};

template<typename typeA, typename typeB>
struct is_type_constructible : is_constructible<typeA, typeB>
{
    using is_constructible<typeA, typeB>::value;
};

template<bool isDefaultConstructible>
struct initialize_memory;

template<>
struct initialize_memory<true>
{
    template<typename T>
    static constexpr void init(void* address)
    {
        if(address)
        {
            new (address) T();
        }

        return;
    }
};

template<>
struct initialize_memory<false>
{
    template<typename T>
    static constexpr void init(void* address)
    {
        if(address)
        {
            //old style initialization
            memset(address,0,sizeof(T));
        }

        return;
    }
};

template<typename T, typename ... Args>
void construct(T* address)
{
    initialize_memory<is_default_constructible<T>::value>::template init<T>((void*)address);
};

template<typename T>
struct total_dim
{
    static const size_t value = 0;
};

template<typename T>
struct total_rank
{
    static const size_t value = 1;
};

template<typename T>
struct type_size
{
    static const size_t value = sizeof(T);
};

template<typename T>
struct thisType
{
    template<typename dummy, typename ... TT>
    struct _is_of_type;

    template<typename dummy>
    struct _is_of_type<dummy>
    {
        static const bool value = false;
    };

    template<typename dummy, typename TT, typename ... TTT>
    struct _is_of_type<dummy,TT,TTT...>
    {
        static const bool value = thisType<T>::template _is_of_type<dummy,TTT...>::value;
    };

    template<typename dummy,typename ... TT>
    struct _is_of_type<dummy,T,TT...>
    {
        static const bool value = true;
    };

    template<typename ... TT>
    struct is_of_types
    {
        static const bool value = _is_of_type<void,TT...>::value;
    };
};

template<int pos, int ... ranks>
struct nth_rank_of;

template<int pos, int rank, int ... ranks>
struct nth_rank_of<pos,rank,ranks...>
{
    static const int value = nth_rank_of<pos-1,ranks...>::_rank;
    static const int _rank = rank;
};

template<int rank, int ... ranks>
struct nth_rank_of<0,rank,ranks...>
{
    static const int _rank = rank;
    static const int value = _rank;
};

template<int pos>
struct nth_rank_of<pos>
{
    static const int value = -1;
};

template<int ... ranks>
struct get_num_ranks;

template<int rank, int ... ranks>
struct get_num_ranks<rank,ranks...>
{
    static const int value = 1+get_num_ranks<ranks...>::value;
};
template<>
struct get_num_ranks<>
{
    static const int value = 0;
};

template<int ...ranks>
struct check_monotonic_range;

template<int rank, int ...ranks>
struct check_monotonic_range<rank,ranks...>
{
    static const bool cond = rank < check_monotonic_range<ranks...>::_rank && check_monotonic_range<ranks...>::cond;
    static const int _rank = rank;
};

template<>
struct check_monotonic_range<>
{
    static const bool cond = true;
    static const int _rank = std::numeric_limits<int>::max();
};

template<int rankA, int rankB>
struct is_same_rank
{
    static const bool value = rankA == rankB;
};

template<int rankToAdd, int ... ranks>
struct addSortedRank;

template<int ... ranks>
struct sequence
{
    template<template<int ...> class M>
    struct derived_type
    {
        typedef M<ranks...> derivedType;
    };

    template<typename dummy,int ... otherRanks>
    struct _merge;

    template<typename dummy,int otherRank, int ... otherRanks>
    struct _merge<dummy,otherRank,otherRanks...>
    {
        typedef typename addSortedRank<otherRank,ranks...>::type::template _merge<void,otherRanks...>::type type;
    };

    template<typename dummy>
    struct _merge<dummy>
    {
        typedef sequence<ranks...> type;
    };

    template<int ... otherRanks>
    struct merge
    {
        typedef typename _merge<void,otherRanks...>::type type;
    };

    template<int currIndex, int currNum, int ... sub_ranks>
    struct _get_relative_num;

    template<int currIndex, int currNum, int sub_rank, int ... sub_ranks>
    struct _get_relative_num<currIndex, currNum, sub_rank, sub_ranks...>
    {
        static const int value = _get_relative_num<currIndex+1, currNum, sub_ranks...>::value;
    };

    template<int currIndex, int currNum, int ... sub_ranks>
    struct _get_relative_num<currIndex, currNum, currNum, sub_ranks...>
    {
        static const int value = currIndex;
    };

    template<int num>
    struct get_relative_num
    {
        static const int value = _get_relative_num<0,num,ranks...>::value;
    };

    template<int ... nums>
    struct get_relative_rank
    {
        typedef sequence<get_relative_num<nums>::value ...> type;
    };

    template<int ... nums>
    struct merge_relative
    {
        typedef typename merge<get_relative_num<nums>::value ...>::type type;
    };

    template<typename dummy, int ... otherRanks>
    struct _add;

    template<typename dummy, int otherRank, int ... otherRanks>
    struct _add<dummy,otherRank,otherRanks...>
    {
        template<int ... mergedRanks>
        struct is
        {
            typedef typename _add<dummy,otherRanks...>::template is<mergedRanks..., otherRank>::type type;
        };
    };

    template<typename dummy>
    struct _add<dummy>
    {
        template<int ... mergedRanks>
        struct is
        {
            typedef sequence<mergedRanks...> type;
        };
    };

    template<typename>
    struct add;

    template<int ... otherRanks>
    struct add<sequence<otherRanks...>>
    {
        typedef typename _add<void,otherRanks...>::template is<ranks...>::type type;
    };
};

template<int ...>
struct sum_ranks;

template<>
struct sum_ranks<>
{
    static const int value = 0;
};

template<int ... ranks>
struct sum_ranks
{
    static const int value = (ranks + ...);
};

template<int ... ranks>
struct acc_sequence;

template<>
struct acc_sequence<>
{
    template<int carry, int ... otherRanks>
    struct _partial
    {
        typedef sequence<otherRanks...> type;
    };
};

template<int rank, int ... ranks>
struct acc_sequence<rank,ranks...>
{
    template<int carry, int ... otherRanks>
    struct _partial
    {
        typedef typename acc_sequence<ranks...>::template _partial<rank+carry,otherRanks...,rank+carry>::type type;
    };

    typedef typename _partial<0>::type type;
};


template<int N>
static constexpr bool are_ranks_equal_to()
{
    return false;
}

template<int N, typename rank, typename ... ranks>
static constexpr bool are_ranks_equal_to(rank i_value, ranks ... i_values)
{
    return i_value == N && (get_num_types<ranks...>::value == 0 || are_ranks_equal_to<N,ranks...>(i_values...));
}

template<int ... ranks>
struct sort_ranks
{
    typedef typename sequence<>::template merge<ranks...>::type type;
};

template<int val1, int val2, bool cond>
struct _get_max;

template<int val1, int val2>
struct _get_max<val1,val2,true>
{
    static const int value = val1;
};

template<int val1, int val2>
struct _get_max<val1,val2,false>
{
    static const int value = val2;
};

template<int val1, int val2>
struct get_max
{
    static const int value = _get_max<val1,val2,(val1 > val2)>::value;
};

template<int ... vals>
struct _max_of;

template<>
struct _max_of<>
{
    template<int currMax, int _indCurrMax, bool bigger>
    struct is;

    template<int currMax, int _indCurrMax>
    struct is<currMax, _indCurrMax, false>
    {
        static const int indCurrMax = _indCurrMax;
    };

    template<int currMax, int _indCurrMax>
    struct is<currMax, _indCurrMax, true>
    {
        static const int indCurrMax = 0;
    };
};

template<int val, int ... vals>
struct _max_of<val, vals...>
{
    template<int currMax, int _indCurrMax, bool bigger>
    struct is;

    template<int currMax, int _indCurrMax>
    struct is<currMax,_indCurrMax,false>
    {
        static const int indCurrMax = _max_of<vals...>::template is<get_max<currMax, val>::value, _indCurrMax, (val > currMax)>::indCurrMax;
    };

    template<int currMax, int _indCurrMax>
    struct is<currMax, _indCurrMax, true>
    {
        static const int indCurrMax = _max_of<vals...>::template is<get_max<currMax, val>::value, 1+sizeof...(vals), (val > currMax)>::indCurrMax;
    };
};

template<int val, int ... vals>
struct max_of
{
    static const int index = sizeof...(vals) - _max_of<vals...>::template is<val, sizeof...(vals), false>::indCurrMax;
};

//////////////////////////////////////////////////////////////////////////

template<typename ... Types>
struct max_size_of;

template<typename Type, typename ... Types>
struct max_size_of<Type, Types...>
{
    typedef typename nth_type_of<max_of<sizeof(Type), sizeof(Types)...>::index, Type, Types...>::type type;
    static const size_t value = sizeof(type);
};

template<typename Type>
struct max_size_of<Type>
{
    static const int value = sizeof(Type);
    typedef Type type;
};

//////////////////////////////////////////////////////////////////////////

template<template <class, class> class predicate, bool cond, int _pos, typename ... Types>
struct _nth_pos_of_predicate;

template<template <class, class> class predicate, int _pos, typename TType, typename Type, typename ... Types>
struct _nth_pos_of_predicate<predicate, false, _pos, TType, Type, Types...>
{
    static const size_t value = _nth_pos_of_predicate<predicate, predicate<Type,TType>::value, _pos + 1, TType, Types...>::value;
};

template<template <class, class> class predicate, int _pos, typename Type, typename ... Types>
struct _nth_pos_of_predicate < predicate, true, _pos, Type, Types... >
{
    static const size_t value = _pos;
};

template<template <class, class> class predicate, int _pos, typename Type>
struct _nth_pos_of_predicate < predicate, false, _pos, Type>
{
    static const size_t value = UINT_MAX;
};

template<template <class,class> class predicate, typename Type, typename ... Types>
struct nth_pos_of_predicate;

template<template <class, class> class predicate, typename TType, typename Type, typename ... Types>
struct nth_pos_of_predicate<predicate, TType, Type, Types...>
{
    static const size_t value = _nth_pos_of_predicate<predicate, predicate<Type, TType>::value, 0, TType, Types...>::value;
};

template<template <class, class> class predicate, typename Type>
struct nth_pos_of_predicate < predicate, Type>
{
    static const size_t value = UINT_MAX;
};

//////////////////////////////////////////////////////////////////////////

template<typename Type, typename ... Types>
struct nth_pos_of_type
{
    static const size_t value = nth_pos_of_predicate<is_same_type,Type,Types...>::value;
};

//////////////////////////////////////////////////////////////////////////
//less strict than previous one

template<typename Type, typename ... Types>
struct nth_pos_of_constructible_type
{
    static const size_t value = nth_pos_of_predicate<is_type_constructible, Type, Types...>::value;
};

//////////////////////////////////////////////////////////////////////////

template<typename Type, typename ... Types>
struct is_among_types
{
    static const bool value = (nth_pos_of_predicate<is_same_type, Type, Types...>::value != UINT_MAX);
};

//////////////////////////////////////////////////////////////////////////
//less strict than previous one

template<typename Type, typename ... Types>
struct is_among_constructible_types
{
    static const bool value = (nth_pos_of_predicate<is_type_constructible, Type, Types...>::value != UINT_MAX);
};

//////////////////////////////////////////////////////////////////////////

template<typename ... Types>
struct has_duplicate_types;

template<typename Type, typename ... Types>
struct has_duplicate_types < Type, Types... >
{
    static const bool value = is_among_types<Type,Types...>::value || has_duplicate_types<Types...>::value;
};

template<>
struct has_duplicate_types < >
{
    static const bool value = false;
};

template<int, int, int ...>
struct _create_range_rank;

template<int endRank, int currRank, int ... ranks>
struct _create_range_rank
{
    typedef typename _create_range_rank<endRank,currRank+1,ranks...,currRank>::type type;
};

template<int endRank, int ... ranks>
struct _create_range_rank<endRank,endRank,ranks...>
{
    typedef sequence<ranks...> type;
};

template<int initRank, int endRank>
struct create_range_rank
{
    typedef typename _create_range_rank<endRank,initRank>::type type;
};

template<int rankToAdd, int currPos, int pos, int ... ranks>
struct _addRank;

template<int rankToAdd, int currPos, int pos, int rank, int ... ranks>
struct _addRank<rankToAdd,currPos,pos,rank,ranks...>
{
    template<int ... leftRanks>
    struct left_ranks
    {
        typedef typename _addRank<rankToAdd,currPos+1,pos,ranks...>::template left_ranks<leftRanks...,rank>::type type;
    };
};

template<int rankToAdd, int pos, int ... ranks>
struct _addRank<rankToAdd,pos,pos,ranks...>
{
    template<int ... leftRanks>
    struct left_ranks
    {
        typedef sequence<leftRanks...,rankToAdd,ranks...> type;
    };
};

template<int rankToAdd, int pos, int ... ranks>
struct addRank
{
    typedef typename _addRank<rankToAdd,0,pos,ranks...>::type type;
};

template<int rankToAdd, int ... ranks>
struct _addSortedRank;

template<int rankToAdd, int rank, int ... ranks>
struct _addSortedRank<rankToAdd,rank,ranks...>
{
    static const int _rank = rank;

    template<bool cond,int ... leftRanks>
    struct _left_ranks;

    template<int ... leftRanks>
    struct _left_ranks<true,leftRanks...>
    {
        typedef sequence<leftRanks...,rankToAdd,rank,ranks...> type;
    };

    template<int ... leftRanks>
    struct _left_ranks<false,leftRanks...>
    {
        typedef typename _addSortedRank<rankToAdd,ranks...>::template _left_ranks<(rankToAdd<_addSortedRank<rankToAdd,ranks...>::_rank),leftRanks...,rank>::type type;
    };

    struct left_ranks
    {
        typedef typename _left_ranks<(rankToAdd<rank)>::type type;
    };
};

template<int rankToAdd>
struct _addSortedRank<rankToAdd>
{
    static const int _rank = -1;

    template<bool cond, int ... leftRanks>
    struct _left_ranks
    {
        typedef sequence<leftRanks...,rankToAdd> type;
    };

    struct left_ranks
    {
        typedef typename _left_ranks<true>::type type;
    };
};

template<int rankToAdd, int ... ranks>
struct addSortedRank
{
    typedef typename _addSortedRank<rankToAdd,ranks...>::left_ranks::type type;
};

template<int num, int ... ranks>
struct is_num_in_ranks;

template<int num>
struct is_num_in_ranks<num>
{
    static const bool value = false;
};

template<int num, int rank, int ... ranks>
struct is_num_in_ranks<num,rank,ranks...>
{
    static const bool value = num==rank || is_num_in_ranks<num,ranks...>::value;
};

template<int endRank, int currRank, int ... ranks>
struct _create_range_rank_compl;

template<int endRank, int currRank, int ... ranks>
struct _create_range_rank_compl
{
    template<bool isInRank, int ... addedRanks>
    struct addRank;

    template<int ... addedRanks>
    struct addRank<true,addedRanks...>
    {
        typedef typename _create_range_rank_compl<endRank,currRank+1,ranks...>:: template addRank<!is_num_in_ranks<currRank+1,ranks...>::value,addedRanks...,currRank>::type type;
    };

    template<int ... addedRanks>
    struct addRank<false,addedRanks...>
    {
        typedef typename _create_range_rank_compl<endRank,currRank+1,ranks...>:: template addRank<!is_num_in_ranks<currRank+1,ranks...>::value,addedRanks...>::type type;
    };
};

template<int endRank, int ... ranks>
struct _create_range_rank_compl<endRank,endRank,ranks...>
{
    template<bool isInRank, int ... addedRanks>
    struct addRank
    {
        typedef sequence<addedRanks...> type;
    };
};

//impotant note: endrank is not considered under construction
template<int initRank, int endRank, int ... ranks>
struct create_range_rank_compl
{
    typedef typename _create_range_rank_compl<endRank,initRank,ranks...>::template addRank<!is_num_in_ranks<initRank,ranks...>::value>::type type;
};

template<typename T>
void swapItems(T& a, T& b)
{
    T c(a);
    a=move(b);
    b=move(c);

    return;
}

template<typename T, template<typename,int...> class, int ... ranks>
struct get_sub_tuple;

template<typename T, template<typename,int...> class M, int rank, int ... ranks>
struct get_sub_tuple<T,M,rank,ranks...>
{
    typedef M<T,rank,ranks...> type;
};
//for nested tuples
template<typename T, template<typename,int...> class M, int ... ranks>
struct get_sub_tuple<M<T,ranks...>,M>
{
    typedef M<T,ranks...> type;
};
template<typename T, template<typename,int...> class M>
struct get_sub_tuple<T,M>
{
    typedef T type;
};

//SFINAE for computation through recursion over T of the depth of this type
template<typename T>
constexpr u8 getDepthTupleType(T)
{
    return 0;
}

template<typename T>
constexpr u8 getDepthTupleType(typename T::primitive_type)
{
    return  1 + getDepthTupleType<typename T::base_type>(typename T::primitive_type());
}

//helper for retrieving primitive type of this tuple
template<typename,bool>
struct get_primitive_type;

template<typename T>
struct get_primitive_type<T,true>
{
    typedef typename T::primitive_type type;
};

template<typename T>
struct get_primitive_type<T,false>
{
    typedef T type;
};

template<typename T>
struct has_base_type
{
    typedef char yes[1];
    typedef char no[2];

    template <typename C>
    static yes& test(typename C::base_type*);

    template <typename>
    static no& test(...);

    static const bool value = sizeof(test<T>(NULL)) == sizeof(yes);
};

//the following helper is for retrieving the n-th base_type of this tuple
template <typename, int, int>
struct get_base_type;

template <typename T, int val, int n=0>
struct get_base_type
{
    typedef typename get_base_type<typename T::base_type,val,n+1>::type type;
};

template <typename T, int val>
struct get_base_type<T,val,val>
{
    typedef T type;
};


template <typename, int, int, template<typename,int...> class>
struct append_dim;

template <typename T, int n, int val, template<typename,int...> class M>
struct append_dim
{
    template<int ... summRanks>
    struct is;

    template<int ... summRanks>
    struct is
    {
        typedef typename append_dim<T,n-1,val,M>::template is<summRanks...,val>::type type;
    };
};

template <typename T, int val, template<typename,int...> class M>
struct append_dim<T,0,val,M>
{
    template<int ... summRanks>
    struct is;

    template<int summRank, int ... summRanks>
    struct is<summRank,summRanks...>
    {
        typedef M<T,summRank,summRanks...>  type;
    };
};

//template <typename, template<typename,int...> class, int ... ranks>
//struct sum_rank;
//
//template <typename T, template<typename,int...> class M, int rank, int ... ranks>
//struct sum_rank<T,M,rank,ranks...>
//{
//    template<class dummy=void, int ... ranksOther>
//    struct with;
//
//    template<class dummy, int rankOther, int ... ranksOther>
//    struct with<dummy,rankOther,ranksOther...>
//    {
//        template<class dummyIs=void,int ... summRanks>
//        struct is;
//
//        template<class dummyIs,int ... summRanks>
//        struct is
//        {
//            typedef typename sum_rank<T,M,ranks...>::template with<dummy,ranksOther...>::template is<dummyIs,summRanks...,rank+rankOther>::type type;
//        };
//    };
//
//    template<class dummy>
//    struct with<dummy>
//    {
//        template<class dummyIs=void, int ... summRanks>
//        struct is;
//
//        template<class dummyIs, int ... summRanks>
//        struct is
//        {
//            typedef typename sum_rank<T,M,ranks...>::template with<dummy>::template is<dummyIs,summRanks...,rank>::type type;
//        };
//    };
//};
//
//template <typename T, template<typename,int...> class M>
//struct sum_rank<T,M>
//{
//    template<class dummy=void,int ... ranksOther>
//    struct with;
//
//    template<class dummy,int rankOther, int ... ranksOther>
//    struct with<dummy,rankOther,ranksOther...>
//    {
//        template<class dummyIs, int ... summRanks>
//        struct is;
//
//        template<class dummyIs, int ... summRanks>
//        struct is
//        {
//            typedef typename sum_rank<T,M>::template with<dummy,ranksOther...>::template is<dummyIs,summRanks...,rankOther>::type type;
//        };
//    };
//
//    template<class dummy>
//    struct with<dummy>
//    {
//        template<class dummyIs=void, int ... summRanks>
//        struct is;
//
//        template<class dummyIs, int summRank, int ... summRanks>
//        struct is<dummyIs,summRank,summRanks...>
//        {
//            typedef M<T,summRank,summRanks...>  type;
//        };
//
//        template<class dummyIs>
//        struct is<dummyIs>
//        {
//            typedef M<T,1>  type;
//        };
//    };
//};
//
//template <typename, template<typename,int...> class, int ... ranks>
//struct subs_rank;
//
//template <typename T, template<typename,int...> class M, int rank, int ... ranks>
//struct subs_rank<T,M,rank,ranks...>
//{
//    template<class dummy=void, int ... ranksOther>
//    struct with;
//
//    template<class dummy, int rankOther, int ... ranksOther>
//    struct with<dummy,rankOther,ranksOther...>
//    {
//        template<class dummyIs=void,int ... subsRanks>
//        struct is;
//
//        template<class dummyIs,int ... subsRanks>
//        struct is
//        {
//            typedef typename subs_rank<T,M,ranks...>::template with<dummy,ranksOther...>::template is<dummyIs,subsRanks...,rank-rankOther>::type type;
//        };
//    };
//
//    template<class dummy>
//    struct with<dummy>
//    {
//        template<class dummyIs=void, int ... subsRanks>
//        struct is;
//
//        template<class dummyIs, int ... subsRanks>
//        struct is
//        {
//            typedef typename subs_rank<T,M,ranks...>::template with<dummy>::template is<dummyIs,subsRanks...,rank>::type type;
//        };
//    };
//};
//
//template <typename T, template<typename,int...> class M>
//struct subs_rank<T,M>
//{
//    template<class dummy=void,int ... ranksOther>
//    struct with;
//
//    template<class dummy,int rankOther, int ... ranksOther>
//    struct with<dummy,rankOther,ranksOther...>
//    {
//        template<class dummyIs, int ... subsRanks>
//        struct is;
//
//        template<class dummyIs, int ... subsRanks>
//        struct is
//        {
//            typedef typename subs_rank<T,M>::template with<dummy,ranksOther...>::template is<dummyIs,subsRanks...>::type type;
//        };
//    };
//
//    template<class dummy>
//    struct with<dummy>
//    {
//        template<class dummyIs=void, int ... subsRanks>
//        struct is;
//
//        template<class dummyIs, int subsRank, int ... subsRanks>
//        struct is<dummyIs,subsRank,subsRanks...>
//        {
//            typedef M<T,subsRank,subsRanks...>  type;
//        };
//
//        template<class dummyIs>
//        struct is<dummyIs>
//        {
//            typedef M<T,1>  type;
//        };
//    };
//};
//
//template <typename, template<typename,int...> class, int pos, int iter, int ... ranks>
//struct remove_dim;
//
//template <typename T, template<typename,int...> class M, int pos, int iter, int rank, int ... ranks>
//struct remove_dim<T,M,pos,iter,rank,ranks...>
//{
//    template<int ... summRanks>
//    struct is;
//
//    template<int ... summRanks>
//    struct is
//    {
//        typedef typename sum_rank<T,M,pos,iter+1,ranks...>::template  is<summRanks...,rank>::type type;
//    };
//};
//
//template <typename T, template<typename,int...> class M, int pos, int rank, int ... ranks>
//struct remove_dim<T,M,pos,pos,rank,ranks...>
//{
//    template<int ... summRanks>
//    struct is;
//
//    template<int ... summRanks>
//    struct is
//    {
//        typedef typename sum_rank<T,M,pos,pos+1,ranks...>::template  is<summRanks...>::type type;
//    };
//};
//
//template <typename T, template<typename,int...> class M, int pos, int iter>
//struct remove_dim<T,M,pos,iter>
//{
//    template<int ... summRanks>
//    struct is;
//
//    template<int summRank, int ... summRanks>
//    struct is<summRank,summRanks...>
//    {
//        typedef M<T,summRank,summRanks...>  type;
//    };
//};

template<int A, int B, bool>
struct is_max;

template<int A, int B>
struct is_max<A,B,true>
{
    static const int value = A;
};

template<int A, int B>
struct is_max<A,B,false>
{
    static const int value = B;
};

template <typename, template<typename,int...> class, int ... ranks>
struct get_max_of;

template <typename T, template<typename,int...> class M, int rank, int ... ranks>
struct get_max_of<T,M,rank,ranks...>
{
    template<class dummy=void, int ... ranksOther>
    struct and_of;

    template<class dummy, int rankOther, int ... ranksOther>
    struct and_of<dummy,rankOther,ranksOther...>
    {
        template<int ... summRanks>
        struct is;

        template<int ... summRanks>
        struct is
        {
            typedef typename get_max_of<T,M,ranks...>::template and_of<dummy,ranksOther...>::template is<summRanks...,is_max< rank,rankOther,(rank>rankOther) >::value >::type type;
        };
    };

    template<class dummy>
    struct and_of<dummy>
    {
        template<int ... summRanks>
        struct is;

        template<int ... summRanks>
        struct is
        {
            typedef typename get_max_of<T,M,ranks...>::template and_of<dummy>::template is<summRanks...,rank>::type type;
        };
    };
};

template <typename T, template<typename,int...> class M>
struct get_max_of<T,M>
{
    template<class dummy=void,int ... ranksOther>
    struct and_of;

    template<class dummy,int rankOther, int ... ranksOther>
    struct and_of<dummy,rankOther,ranksOther...>
    {
        template<int ... summRanks>
        struct is;

        template<int ... summRanks>
        struct is
        {
            typedef typename get_max_of<T,M>::template and_of<dummy,ranksOther...>::template is<summRanks...,rankOther>::type type;
        };
    };

    template<class dummy>
    struct and_of<dummy>
    {
        template<int ... summRanks>
        struct is;

        template<int summRank, int ... summRanks>
        struct is<summRank,summRanks...>
        {
            typedef M<T,summRank,summRanks...>  type;
        };
    };
};

template<int A, int B, bool>
struct is_min;

template<int A, int B>
struct is_min<A,B,true>
{
    static const int value = A;
};

template<int A, int B>
struct is_min<A,B,false>
{
    static const int value = B;
};

template <typename, template<typename,int...> class, int ... ranks>
struct get_min_of;

template <typename T, template<typename,int...> class M, int rank, int ... ranks>
struct get_min_of<T,M,rank,ranks...>
{
    template<class dummy=void, int ... ranksOther>
    struct and_of;

    template<class dummy, int rankOther, int ... ranksOther>
    struct and_of<dummy,rankOther,ranksOther...>
    {
        template<int ... summRanks>
        struct is;

        template<int ... summRanks>
        struct is
        {
            typedef typename get_min_of<T,M,ranks...>::template and_of<dummy,ranksOther...>::template is<summRanks...,is_min< rank,rankOther,(rank>rankOther) >::value >::type type;
        };
    };

    template<class dummy>
    struct and_of<dummy>
    {
        template<int ... summRanks>
        struct is;

        template<int ... summRanks>
        struct is
        {
            typedef typename get_min_of<T,M,ranks...>::template and_of<dummy>::template is<summRanks...,rank>::type type;
        };
    };
};

template <typename T, template<typename,int...> class M>
struct get_min_of<T,M>
{
    template<class dummy=void,int ... ranksOther>
    struct and_of;

    template<class dummy,int rankOther, int ... ranksOther>
    struct and_of<dummy,rankOther,ranksOther...>
    {
        template<int ... summRanks>
        struct is;

        template<int ... summRanks>
        struct is
        {
            typedef typename get_min_of<T,M>::template and_of<dummy,ranksOther...>::template is<summRanks...,rankOther>::type type;
        };
    };

    template<class dummy>
    struct and_of<dummy>
    {
        template<int ... summRanks>
        struct is;

        template<int summRank, int ... summRanks>
        struct is<summRank,summRanks...>
        {
            typedef M<T,summRank,summRanks...>  type;
        };
    };
};


template<int ... nums>
struct Prod;
template<int num, int ... nums>
struct Prod<num,nums...>
{
    static const u16 value = num*Prod<nums...>::value;
};

template<>
struct Prod<>
{
    static const u16 value = 1;
};

template<int ... nums>
struct Prod_Max;

template<int num, int ... nums>
struct Prod_Max<num,nums...>
{
    template<class dummy=void,int ... otherNums>
    struct with;

    template<class dummy, int otherNum, int ... otherNums>
    struct with<dummy,otherNum,otherNums...>
    {
        static const u16 value = is_min< num,otherNum,(num>otherNum) >::value * Prod_Max<nums...>::template with<dummy,otherNums...>::value;
    };

    template<class dummy>
    struct with<dummy>
    {
        static const u16 value = num*Prod_Max<nums...>::template with<dummy>::value;
    };
};

template<>
struct Prod_Max<>
{
    template<class dummy=void,int ... otherNums>
    struct with;

    template<class dummy, int otherNum, int ... otherNums>
    struct with<dummy,otherNum,otherNums...>
    {
        static const u16 value = otherNum*Prod_Max<>::template with<dummy,otherNums...>::value;
    };

    template<class dummy>
    struct with<dummy>
    {
        static const u16 value = 1;
    };
};

template <int N>
struct Factorial
{
    static const u16 value = N*Factorial<N-1>::value;
};

template <>
struct Factorial<1>
{
    static const u16 value = 1;
};

template <>
struct Factorial<0>
{
    static const u16 value = 1;
};

template<int N, int M>
struct NoverM
{
    static const u16 value = Factorial<N>::value/Factorial<M>::value;
};

template<typename TT>
struct promoted_type
{
    //for unqualified types return int by default
    typedef TT type;
};

template<>
struct promoted_type<f32>
{
    typedef double type;
};

template<>
struct promoted_type<u32>
{
    typedef int type;
};

template<>
struct promoted_type<u16>
{
    typedef int type;
};

template<>
struct promoted_type<u8>
{
    typedef int type;
};

template<typename T, template<typename,int...> class M, int ... ranks>
struct inherit_tupla;

template<typename T, template<typename,int...> class M, int rank, int ... ranks>
struct inherit_tupla<T,M,rank,ranks...>
{
    template<int ... isRanks>
    struct is;

    template<int ... isRanks>
    struct is
    {
        typedef typename inherit_tupla<T,M,ranks...>::template is<isRanks...,rank>::type type;
    };
};

//as soon as we find a 0 we truncate the ranks
template<typename T, template<typename,int...> class M, int ... ranks>
struct inherit_tupla<T,M,0,ranks...>
{
    template<int ... isRanks>
    struct is;

    template<int ... isRanks>
    struct is
    {
        typedef M<T> type;
    };
};

//specialization for when we finish iterating over the variadic parameters
template<typename T, template<typename,int...> class M>
struct inherit_tupla<T,M>
{
    template<int ... isRanks>
    struct is;

    template<int isRank, int ... isRanks>
    struct is<isRank,isRanks...>
    {
        typedef M<T,isRank,isRanks...> type;
    };
};

template<template<typename,int...> class TT, typename T, int ... ranks>
struct make_nested_type;

template<template<typename,int...> class TT, typename T, int rank, int ... ranks>
struct make_nested_type<TT,T,rank,ranks...>
{
    typedef typename make_nested_type<TT,TT<T,rank>,ranks...>::type type;
};

template<template<typename,int...> class TT, typename T>
struct make_nested_type<TT,T>
{
    typedef T type;
};

//this helper is for creating signatures from template parameters
template<typename T, int count, typename resT=void>
struct create_function_signature;

template<typename T, int count, typename resT>
struct create_function_signature
{
    template<typename ... TT>
    struct is
    {
        typedef typename create_function_signature<T,count-1,resT>::template is<T,TT...>::type type;
    };
};

template<typename T, typename resT>
struct create_function_signature<T,0,resT>
{
    template<typename ... TT>
    struct is
    {
        typedef resT type(TT...);
    };
};


//this helper is for performing the product of types (i.e. tuples)
template <typename, template<typename,int...> class, int ... ranks>
struct prod_type;

template <typename T, template<typename,int...> class M, int rank, int ... ranks>
struct prod_type<T,M,rank,ranks...>
{
    template<class dummy=void, int ... ranksOther>
    struct with;

    template<class dummy, int rankOther, int ... ranksOther>
    struct with<dummy,rankOther,ranksOther...>
    {
        template<class dummyIs=void,int ... summRanks>
        struct is;

        template<class dummyIs,int ... summRanks>
        struct is
        {
            typedef typename prod_type<T,M,ranks...>::template with<dummy,ranksOther...>::template is<dummyIs,summRanks...,rank+rankOther>::type type;
        };
    };

    template<class dummy>
    struct with<dummy>
    {
        template<class dummyIs=void, int ... summRanks>
        struct is;

        template<class dummyIs, int ... summRanks>
        struct is
        {
            typedef typename prod_type<T,M,ranks...>::template with<dummy>::template is<dummyIs,summRanks...,rank>::type type;
        };
    };
};

template <typename T, template<typename,int...> class M>
struct prod_type<T,M>
{
    template<class dummy=void,int ... ranksOther>
    struct with;

    template<class dummy,int rankOther, int ... ranksOther>
    struct with<dummy,rankOther,ranksOther...>
    {
        template<class dummyIs, int ... summRanks>
        struct is;

        template<class dummyIs, int ... summRanks>
        struct is
        {
            typedef typename prod_type<T,M>::template with<dummy,ranksOther...>::template is<dummyIs,summRanks...,rankOther>::type type;
        };
    };

    template<class dummy>
    struct with<dummy>
    {
        template<class dummyIs=void, int ... summRanks>
        struct is;

        template<class dummyIs, int summRank, int ... summRanks>
        struct is<dummyIs,summRank,summRanks...>
        {
            typedef M<T,summRank,summRanks...>  type;
        };

        template<class dummyIs>
        struct is<dummyIs>
        {
            typedef M<T,1>  type;
        };
    };
};

//general case
template<typename T>
inline constexpr T max(T a, T b)
{
    return (a < b) ? b : a;
}
template<typename T>
inline constexpr T min(T a, T b)
{
    return (a < b) ? a : b;
}

}
}

extern yame::mpl::place_holder _arg;
extern yame::mpl::null_ptr_type null_ptr;
extern yame::mpl::none_t none;
