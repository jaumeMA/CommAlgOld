#pragma once

#include "YTL/container/cParameterPack.h"
#include "YTL/mpl/cTemplateHelper.h"

namespace yame
{
namespace mpl
{

template<int pos, typename ... Types>
struct nth_type_of<pos,container::parameter_pack<Types...>>
{
    typedef typename nth_type_of<pos,Types...>::type type;
};

template<typename>
struct get_parameter_pack_num_types;

template<typename ... Types>
struct get_parameter_pack_num_types<container::parameter_pack<Types...>>
{
    static const int value = get_num_types<Types...>::value;
};

template<typename ... Types>
struct get_sub_parameter_pack
{
static const int numTypes = get_num_types<Types...>::value;

template<typename dummy, int ... Indexs>
struct _at;

template<typename dummy>
struct _at<dummy>
{
    template<typename ... FoundTypes>
    struct is
    {
        typedef container::parameter_pack<FoundTypes...> type;
    };
};

template<typename dummy, int Index, int ... Indexs>
struct _at<dummy, Index, Indexs...>
{
    template<typename ... FoundTypes>
    struct is
    {
        typedef typename _at<dummy, Indexs...>::template is<FoundTypes..., typename nth_type_of<Index, Types...>::type>::type type;
    };
};

template<int ... Indexs>
struct at
{
    typedef typename _at<void, Indexs...>::template is<>::type type;
};

template<typename Seq>
struct at_seq;

template<int ... Indexs>
struct at_seq<sequence<Indexs...>>
{
    typedef typename _at<void, Indexs...>::template is<>::type type;
};

template<int ... Indexs>
struct not_at
{
    typedef typename create_range_rank_compl<0,numTypes,Indexs...>::type ComplIndexs;

    typedef typename at_seq<ComplIndexs>::type type;
};

template<typename Seq>
struct not_at_seq;

template<int ... Indexs>
struct not_at_seq<sequence<Indexs...>>
{
    typedef typename create_range_rank_compl<0,numTypes,Indexs...>::type ComplIndexs;

    typedef typename at_seq<ComplIndexs>::type type;
};
};

template<typename Type, template<typename,typename> class Cond>
struct remove_type_if
{
    template<bool, typename ...>
    struct _from;

    template<typename TType, typename ... TTypes>
    struct _from<true, TType, TTypes...>
    {
        typedef typename nth_type_of<0, TTypes...>::type nextType;

        template<typename ... FoundTypes>
        struct is
        {
            typedef typename _from<Cond<Type, nextType>::value, TTypes...>::template is<FoundTypes...>::type type;

            static constexpr type construct(FoundTypes ... prevArgs, TType arg, TTypes ... args)
            {
                return _from<Cond<Type, nextType>::value, TTypes...>::template is<FoundTypes...>::construct(forward<FoundTypes>(prevArgs)..., forward<TTypes>(args)...);
            }
        };
    };

    template<typename TType, typename ... TTypes>
    struct _from<false, TType, TTypes...>
    {
        typedef typename nth_type_of<0, TTypes...>::type nextType;

        template<typename ... FoundTypes>
        struct is
        {
            typedef typename _from<Cond<Type, nextType>::value, TTypes...>::template is<FoundTypes...,TType>::type type;

            static constexpr type construct(FoundTypes ... prevArgs, TType arg, TTypes ... args)
            {
                return _from<Cond<Type, nextType>::value, TTypes...>::template is<FoundTypes..., TType>::construct(forward<FoundTypes>(prevArgs)..., forward<TType>(arg), forward<TTypes>(args)...);
            }
        };
    };

    template<typename TType>
    struct _from<true, TType>
    {
        template<typename ... FoundTypes>
        struct is
        {
            typedef container::parameter_pack<FoundTypes...> type;

            static constexpr type construct(FoundTypes ... args, TType arg)
            {
                return type(forward<FoundTypes>(args)...);
            }
        };
    };

    template<typename TType>
    struct _from<false, TType>
    {
        template<typename ... FoundTypes>
        struct is
        {
            typedef container::parameter_pack<FoundTypes..., TType> type;

            static constexpr type construct(FoundTypes ... args, TType arg)
            {
                return type(forward<FoundTypes>(args)..., forward<TType>(arg));
            }
        };
    };

    template<typename TType, typename ... TTypes>
    struct from
    {
        typedef typename _from<Cond<Type, TType>::value, TType, TTypes...>::template is<>::type type;

        static constexpr type construct(TType arg, TTypes ... args)
        {
            return _from<Cond<Type, TType>::value, TType, TTypes...>::template is<>::construct(forward<TType>(arg), forward<TTypes>(args)...);
        }
    };
};

template<typename Type, template<typename,typename> class Cond>
struct get_pos_of_type
{
    template<int,bool,typename ...>
    struct _at;

    template<int currPos, typename TType, typename ... TTypes>
    struct _at<currPos, false, TType, TTypes...>
    {
        typedef typename nth_type_of<0, TTypes...>::type nextType;

        template<int ... FoundPos>
        struct is
        {
            typedef typename _at<currPos+1, Cond<Type, nextType>::value, TTypes...>::template is<FoundPos...>::type type;
        };
    };

    template<int currPos, typename TType, typename ... TTypes>
    struct _at<currPos, true, TType, TTypes...>
    {
        typedef typename nth_type_of<0, TTypes...>::type nextType;

        template<int ... FoundPos>
        struct is
        {
            typedef typename _at<currPos+1, Cond<Type, nextType>::value, TTypes...>::template is<FoundPos..., currPos>::type type;
        };
    };

    template<int currPos, typename TType>
    struct _at<currPos, false, TType>
    {
        template<int ... FoundPos>
        struct is
        {
            typedef mpl::sequence<FoundPos...> type;
        };
    };

    template<int currPos, typename TType>
    struct _at<currPos, true, TType>
    {
        template<int ... FoundPos>
        struct is
        {
            typedef mpl::sequence<FoundPos...,currPos> type;
        };
    };

    template<typename TType, typename ... TTypes>
    struct at
    {
        typedef typename _at<0, Cond<Type, TType>::value, TType, TTypes...>::template is<>::type type;
    };
};

template<typename Type, template<typename,typename> class Cond>
struct get_pos_of_not_type
{
    template<int,bool,typename ...>
    struct _at;

    template<int currPos, typename TType, typename ... TTypes>
    struct _at<currPos, false, TType, TTypes...>
    {
        typedef typename nth_type_of<0, TTypes...>::type nextType;

        template<int ... FoundPos>
        struct is
        {
            typedef typename _at<currPos+1, Cond<Type, nextType>::value, TTypes...>::template is<FoundPos..., currPos>::type type;
        };
    };

    template<int currPos, typename TType, typename ... TTypes>
    struct _at<currPos, true, TType, TTypes...>
    {
        typedef typename nth_type_of<0, TTypes...>::type nextType;

        template<int ... FoundPos>
        struct is
        {
            typedef typename _at<currPos+1, Cond<Type, nextType>::value, TTypes...>::template is<FoundPos...>::type type;
        };
    };

    template<int currPos, typename TType>
    struct _at<currPos, false, TType>
    {
        template<int ... FoundPos>
        struct is
        {
            typedef mpl::sequence<FoundPos...,currPos> type;
        };
    };

    template<int currPos, typename TType>
    struct _at<currPos, true, TType>
    {
        template<int ... FoundPos>
        struct is
        {
            typedef mpl::sequence<FoundPos...> type;
        };
    };

    template<typename TType, typename ... TTypes>
    struct at
    {
        typedef typename _at<0, Cond<Type, TType>::value, TType, TTypes...>::template is<>::type type;
    };
};

template<typename>
struct arg_sequence;

template<int ... typeIndexs>
struct arg_sequence<sequence<typeIndexs...>>
{
    template<typename ... Types>
    static constexpr container::parameter_pack<typename nth_type_of<typeIndexs, Types...>::type ...> construct_parameter_pack(Types&& ... i_args)
    {
        return container::parameter_pack<typename nth_type_of<typeIndexs, Types...>::type ...>(forward<typename nth_type_of<typeIndexs, Types...>::type>(nth_val_of<typeIndexs>(i_args...))...);
    }
};

}
}
