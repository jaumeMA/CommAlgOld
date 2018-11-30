#pragma once

#include "YTL/functional/cComposedCallable.h"

#define DEFINE_HIGHER_ORDER_UNARY_FUNCTION(NAME,OP) \
namespace yame \
{ \
namespace ytl \
{ \
template<typename Return> \
struct NAME##_unary_op\
{ \
    typedef decltype((OP *reinterpret_cast<const Return*>(NULL))) inner_return; \
    NAME##_unary_op() = default; \
    inner_return operator()(Return i_value) const \
    { \
        return OP i_value; \
    } \
}; \
template<typename ...> \
class NAME##_callable_t; \
 \
template<typename Return, typename ... Types> \
class NAME##_callable_t<Return(Types...)> : public yame::ytl::composed_callable<NAME##_unary_op<Return>,Types...> \
{ \
public: \
    NAME##_callable_t(const yame::ytl::function<Return(Types...)>& i_func) \
    : yame::ytl::composed_callable<NAME##_unary_op<Return>,Types...>(NAME##_unary_op<Return>(),i_func) \
    {} \
}; \
template<typename Return, typename ... Types> \
yame::ytl::function<Return(Types...)> operator OP(const yame::ytl::function<Return(Types...)>& i_function) \
{ \
    return yame::ytl::NAME##_callable_t<Return(Types...)>(i_function); \
} \
} \
}

#define DEFINE_HIGHER_ORDER_BINARY_FUNCTION(NAME,OP) \
namespace yame \
{ \
namespace ytl \
{ \
template<typename Return> \
struct NAME##_binary_op\
{ \
    typedef decltype((*reinterpret_cast<const Return*>(NULL) OP *reinterpret_cast<const Return*>(NULL))) inner_return; \
    NAME##_binary_op() = default; \
    inner_return operator()(Return i_lhs, Return i_rhs) const \
    { \
        return i_lhs OP i_rhs; \
    } \
}; \
template<typename...> \
class NAME##_callable_t; \
 \
template<typename Return, typename ... Types> \
class NAME##_callable_t<Return(Types...)> : public yame::ytl::composed_callable<NAME##_binary_op<Return>,Types...> \
{ \
public: \
    NAME##_callable_t(const yame::ytl::function<Return(Types...)>& i_lhs, const yame::ytl::function<Return(Types...)>& i_rhs) \
    : yame::ytl::composed_callable<NAME##_binary_op<Return>,Types...>(NAME##_binary_op<Return>(),i_lhs,i_rhs) \
    {} \
}; \
template<typename Return, typename ... Types> \
yame::ytl::function<Return(Types...)> operator OP(const yame::ytl::function<Return(Types...)>& i_lhs, const yame::ytl::function<Return(Types...)>& i_rhs) \
{ \
    if(i_lhs != null_ptr && i_rhs != null_ptr) \
    { \
        return yame::ytl::NAME##_callable_t<Return(Types...)>(i_lhs, i_rhs); \
    } \
    else if(i_lhs != null_ptr) \
    { \
        return i_lhs; \
    } \
    else \
    { \
        return i_rhs; \
    } \
} \
} \
}


DEFINE_HIGHER_ORDER_UNARY_FUNCTION(not,!)
DEFINE_HIGHER_ORDER_UNARY_FUNCTION(neg,-)
DEFINE_HIGHER_ORDER_BINARY_FUNCTION(and,&&)
DEFINE_HIGHER_ORDER_BINARY_FUNCTION(sum,+)
DEFINE_HIGHER_ORDER_BINARY_FUNCTION(subs,-)
DEFINE_HIGHER_ORDER_BINARY_FUNCTION(prod,*)
DEFINE_HIGHER_ORDER_BINARY_FUNCTION(div,/)

namespace yame
{
namespace ytl
{

template<typename>
class constant_function;

template<typename Return, typename ... Types>
class constant_function<function<Return(Types...)>> : public function<Return(Types...)>
{
public:
    constant_function(const Return& i_constantValue)
    : function<Return(Types...)>([i_constantValue](Types ... i_args) { return i_constantValue; })
    {}
};
template<typename>
class identity_function;

template<typename Return, typename ... Types>
class identity_function<function<Return(Types...)>> : public function<Return(Types...)>
{
public:
    template<size_t Component>
    identity_function(const mpl::numeric_type<Component>&)
    : function<Return(Types...)>([](Types ... i_args){ return Return(mpl::nth_val_of<Component>(mpl::forward<Types>(i_args)...));})
    , m_component(Component)
    {}
    size_t getComponent() const
    {
        return m_component;
    }

private:
    size_t m_component;
};

//shift free operator for currying the function
template<typename Return>
inline yame::ytl::curried_function<Return>& operator<<(yame::ytl::curried_function<Return>& i_lhs, const yame::ytl::function<Return()>& i_rhs);

template<typename Return, typename Type, typename ... Types>
inline yame::ytl::curried_function<Return,Type,Types...>& operator<<(yame::ytl::curried_function<Return,Type,Types...>& i_lhs, const yame::ytl::function<Return(Type,Types...)>& i_rhs);

//add the algebra of functions
template<typename Return, typename ... Types>
inline yame::ytl::function<Return(Types...)> operator*(const Return& i_lhs, const yame::ytl::function<Return(Types...)>& i_rhs);

template<typename Return, typename ... Types>
inline yame::ytl::function<Return(Types...)> operator*(const yame::ytl::function<Return(Types...)>& i_lhs, const Return& i_rhs);

template<typename Return, typename ... Types>
inline yame::ytl::function<Return(Types...)> operator+(const Return& i_lhs, const yame::ytl::function<Return(Types...)>& i_rhs);

template<typename Return, typename ... Types>
inline yame::ytl::function<Return(Types...)> operator+(const yame::ytl::function<Return(Types...)>& i_lhs, const Return& i_rhs);

template<typename Return, typename ... Types>
inline yame::ytl::function<Return(Types...)> operator-(const Return& i_lhs, const yame::ytl::function<Return(Types...)>& i_rhs);

template<typename Return, typename ... Types>
inline yame::ytl::function<Return(Types...)> operator-(const yame::ytl::function<Return(Types...)>& i_lhs, const Return& i_rhs);

template<typename Return, typename ... Types>
inline yame::ytl::function<Return(Types...)> operator/(const Return& i_lhs, const yame::ytl::function<Return(Types...)>& i_rhs);

template<typename Return, typename ... Types>
inline yame::ytl::function<Return(Types...)> operator/(const yame::ytl::function<Return(Types...)>& i_lhs, const Return& i_rhs);

}
}

#include "YTL/functional/detail/cFunctionOps.tpp"
