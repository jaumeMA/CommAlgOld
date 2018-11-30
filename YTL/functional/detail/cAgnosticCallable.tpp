
#include "YTL/functional/cComposedCallable.h"

namespace yame
{
namespace ytl
{

template<typename Return,typename ... Types>
template<typename Callable,typename ... IntermediateTypes>
agnostic_composed_callable<Return(Types...)>::agnostic_composed_callable(const Callable& i_transformReturn, const function<IntermediateTypes(Types...)>& ... i_baseFunctions)
{
    m_composedCallable = composed_callable<function<Return(IntermediateTypes...)>,Types...>(i_transformReturn, i_baseFunctions ...);
    m_intermediateDimension = mpl::get_num_types<IntermediateTypes...>::value;
}
template<typename Return,typename ... Types>
Return agnostic_composed_callable<Return(Types...)>::operator()(Types ... i_args) const
{
    return m_composedCallable(mpl::forward<Types>(i_args) ...);
}
template<typename Return,typename ... Types>
size_t agnostic_composed_callable<Return(Types...)>::getNumIntermediateTypes() const
{
    return m_intermediateDimension;
}
template<typename Return,typename ... Types>
template<typename ... IntermediateTypes>
const detail::function_impl_base<Return,container::parameter_pack<IntermediateTypes...>>* agnostic_composed_callable<Return(Types...)>::getTransform() const
{
    if(const detail::functor_impl<composed_callable<function<Return(IntermediateTypes...)>,Types...>,Return,Types...>* functor = dynamic_cast<const detail::functor_impl<composed_callable<function<Return(IntermediateTypes...)>,Types...>,Return,Types...>*>(m_composedCallable.getFuncPtr()))
    {
        const composed_callable<function<Return(IntermediateTypes...)>,Types...>& nestedCallable = functor->getCallable();

        const function<Return(IntermediateTypes...)>& nestedTransform = nestedCallable.getTransform();

        return nestedTransform.getFuncPtr();
    }
    else
    {
        return NULL;
    }
}
template<typename Return,typename ... Types>
template<int Index,typename ... IntermediateTypes>
const detail::function_impl_base<Return,container::parameter_pack<Types...>>* agnostic_composed_callable<Return(Types...)>::getNestedCallable() const
{
    if(const detail::functor_impl<composed_callable<function<Return(IntermediateTypes...)>,Types...>,Return,Types...>* functor = dynamic_cast<const detail::functor_impl<composed_callable<function<Return(IntermediateTypes...)>,Types...>,Return,Types...>*>(m_composedCallable.getFuncPtr()))
    {
        const composed_callable<function<Return(IntermediateTypes...)>,Types...>& nestedCallable = functor->getCallable();

        return nestedCallable.template getNestedCallable<Index>();
    }
    else
    {
        return NULL;
    }
}

}
}
