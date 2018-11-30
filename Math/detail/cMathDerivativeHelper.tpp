
#include "YTL/functional/cPassByValueFunctor.h"
#include "YTL/functional/cFunctionOps.h"

namespace yame
{
namespace math
{
namespace detail
{

template<int Component, ring_type Im, vector_space_type Dom,  int ... Indexs>
requires (is_metric_space<Im>::value && is_metric_space<typename Dom::particle>::value)
vector_function<Im,Dom> derivative_helper_caller<0>::derivative(const mpl::sequence<Indexs...>&, const vector_function<Im,Dom>& i_function)
{
    typedef typename Dom::particle underlying_type;

    return [i_function](typename mpl::transform_index_type<Indexs>::template to<underlying_type>::type ... i_args) -> Im
    {
        static const size_t k_maxSteps = 1000;
        static const float k_radius = 10.f;
        static const float stabTrigger = underlying_type::getEpsilon();

        const underlying_type targetPoint = mpl::nth_val_of<Component>(i_args...);

        //take vicinities around zero, which is the neutral element at the group level
        typename underlying_type::vicinity pathToTargetDownwards = Im::group::neutral_element().get_vicinity();
        typename underlying_type::vicinity pathToTargetUpwards = Im::group::neutral_element().get_vicinity();

        Im prevValue = i_function.eval(mpl::forward<typename mpl::transform_index_type<Indexs>::template to<underlying_type>::type>(i_args) ...);

        pathToTargetDownwards.begin(k_radius * stabTrigger);
        pathToTargetUpwards.begin(-k_radius * stabTrigger);
        for(size_t stepIndex=0;stepIndex<k_maxSteps && !pathToTargetDownwards.end() && !pathToTargetUpwards.end();++stepIndex,++pathToTargetUpwards,++pathToTargetDownwards)
        {
            const underlying_type currLeftPoint = *pathToTargetUpwards;
            const underlying_type currRightPoint = *pathToTargetDownwards;

            Im currValue = (i_function.eval(add_delta_at<Component,Indexs>(i_args, currRightPoint) ...) - i_function.eval(add_delta_at<Component,Indexs>(i_args, currLeftPoint) ...)) / (currRightPoint - currLeftPoint);

            if(prevValue.distance(currValue) < stabTrigger)
            {
                return currValue;
            }
            else
            {
                prevValue = currValue;
            }
        }

        return *reinterpret_cast<Im *>(0xDEAD);
    };
}

template<int Case>
template<int Component,ring_type Im, vector_space_type Dom,  int ... Indexs>
vector_function<Im,Dom> derivative_helper_caller<Case>::derivative(const mpl::sequence<Indexs...>& i_sequence, const vector_function<Im,Dom>& i_function)
{
    vector_function<Im,Dom> derivedFunc = derivative_register<Case>::type::template get_custom_derivative<Component>(i_function);

    if(derivedFunc != null_ptr)
    {
        return derivedFunc;
    }
    else
    {
        return derivative_helper_caller<Case-1>::template derivative<Component>(i_sequence,i_function);
    }
}

template<int Component,ring_type Im, vector_space_type Dom,  int currCase>
vector_function<Im,Dom> derivative_helper::derivative(const vector_function<Im,Dom>& i_function)
{
    typedef typename mpl::create_range_rank<0,Dom::dimension()>::type rangeSeq;

    return derivative_helper_caller<currCase>::template derivative<Component>(rangeSeq(),i_function);
}

template<ring_type Im, vector_space_type Dom,  int ... Indexs>
vector_function<Im,Dom> derivative_helper::derivative(const mpl::sequence<Indexs...>& i_seq, const vector<Im,Dom::dimension()>& i_direction, const vector_function<Im,Dom>& i_function)
{
    static const int Dimension = Dom::dimension();
    static_assert(mpl::get_num_ranks<Indexs...>::value == Dimension, "Mismatch between dimension and derivative indexs");

    auto derivativeTransform = [i_direction](typename mpl::transform_index_type<Indexs>::template to<Im>::type ... i_args) -> Im
    {
        return i_direction * vector<Im,Dimension>(mpl::forward<Im>(i_args) ...);
    };

    return yame::ytl::composed_callable<decltype(derivativeTransform),const typename mpl::transform_index_type<Indexs>::template to<Im>::type& ...>(derivativeTransform, derivative<Indexs>(i_function) ...);
}

template<int Component,ring_type Im, vector_space_type Dom, int DDimension,int ... DerivativeIndexs,typename Return,typename ... Types>
vector_function<Im,Dom> composite_derivative_helper::derivative(const mpl::sequence<DerivativeIndexs...>&, const ytl::agnostic_composed_callable<Return(Types...)>& i_callable)
{
    static const int Dimension = Dom::dimension();
    static_assert(DDimension == mpl::get_num_ranks<DerivativeIndexs...>::value, "Mismatch between indexs and dimension");
    static_assert(Dimension == mpl::get_num_types<Types...>::value, "Mismatch between dimension and types");

    typedef typename Dom::particle underlying_type;
    typedef typename mpl::homogeneous_callable<ytl::pass_by_value_functor,Im,underlying_type,DDimension>::type homogeneous_pass_by_value_functor;
    typedef ytl::detail::functor_impl<homogeneous_pass_by_value_functor,Im,typename mpl::transform_index_type<DerivativeIndexs>::template to<underlying_type>::type ...> functor_by_value;

    if(const functor_by_value* functor = rtti::dynamicCast<const functor_by_value>(i_callable.template getTransform<typename mpl::transform_index_type<DerivativeIndexs>::template to<Im>::type ...>()))
    {
        typedef typename vector_subspace<Dom,DDimension>::type DDom;
        const homogeneous_pass_by_value_functor& passByValueFunctor = functor->getCallable();

        return yame::sum(container::cTupla<typename vector_function<Im,Dom>::func_base,DDimension>{ derivative_helper::derivative<DerivativeIndexs>(vector_function<Im,DDom>::clone(passByValueFunctor.getForwardedFunction().getFuncPtr()))(static_cast<const typename vector_function<Im,Dom>::func_base&>(vector_function<Im,Dom>::clone(i_callable.template getNestedCallable<DerivativeIndexs,typename mpl::transform_index_type<DerivativeIndexs>::template to<Im>::type ...>())) ...) * derivative_helper::derivative<Component>(vector_function<Im,Dom>::clone(i_callable.template getNestedCallable<DerivativeIndexs,typename mpl::transform_index_type<DerivativeIndexs>::template to<Im>::type ...>())) ... });
    }
    else
    {
        return null_ptr;
    }
}
template<int Component,ring_type Im, vector_space_type Dom, int DDimension,typename Return,typename ... Types>
vector_function<Im,Dom> composite_derivative_helper::derivative(const ytl::agnostic_composed_callable<Return(Types...)>& i_callable)
{
    typedef typename mpl::create_range_rank<0,DDimension>::type derivative_index;

    return derivative<Component,Im,Dom,DDimension>(derivative_index(),i_callable);
}
template<int Component,ring_type Im, vector_space_type Dom, int ... MaxComponent,typename Return,typename ... Types>
vector_function<Im,Dom> composite_derivative_helper::derivative(const mpl::sequence<MaxComponent...>&, const ytl::agnostic_composed_callable<Return(Types...)>& i_callable)
{
    typedef vector_function<Im,Dom>(*dimensioned_func)(const ytl::agnostic_composed_callable<Return(Types...)>&);
    static const dimensioned_func _funcs[mpl::get_num_ranks<MaxComponent...>::value] = {&derivative<Component,Im,Dom,MaxComponent,Return,Types...> ...};

    return _funcs[i_callable.getNumIntermediateTypes()-1](i_callable);
}
template<int Component,ring_type Im, vector_space_type Dom>
vector_function<Im,Dom> composite_derivative_helper::get_custom_derivative(const vector_function<Im,Dom>& i_function)
{
    static const int Dimension = Dom::dimension();
    typedef typename Dom::particle underlying_type;
    typedef typename mpl::homogeneous_callable<ytl::agnostic_composed_callable,Im,underlying_type,Dimension>::type agnostic_composed_homogeneous_callable;

    if(const detail::functor_impl<agnostic_composed_homogeneous_callable,Im,Dom>* functor = rtti::dynamicCast<const detail::functor_impl<agnostic_composed_homogeneous_callable,Im,Dom>>(i_function.template getFuncPtr()))
    {
        static const size_t k_maxNumComponents = 5;
        const agnostic_composed_homogeneous_callable& agnosticCallable = functor->getCallable();

        typedef mpl::create_range_rank<1,k_maxNumComponents>::type max_rank;

        return derivative<Component,Im,Dom>(max_rank(),agnosticCallable);
    }
    else
	{
		return null_ptr;
	}
}

template<int Component, ring_type Im, vector_space_type Dom>
vector_function<Im,Dom> identity_derivative_helper::get_custom_derivative(const vector_function<Im,Dom>& i_function)
{
    static const int Dimension = Dom::dimension();
    typedef typename Dom::particle underlying_type;

    if(const detail::functor_impl<projection<Im,Dom>,Im,Dom>* functor = rtti::dynamicCast<const detail::functor_impl<projection<Im,Dom>,Im,Dom>>(i_function.template getFuncPtr()))
    {
        const projection<Im,Dom>& identityCallable = functor->getCallable();

        return (identityCallable.getComponent() == Component) ? constant_function<Im,Dom>(Im::neutral_element()) : constant_function<Im,Dom>(Im::group::neutral_element());
    }
    else
	{
		return null_ptr;
	}
}

template<int Component, ring_type Im, vector_space_type Dom>
vector_function<Im,Dom> constant_derivative_helper::get_custom_derivative(const vector_function<Im,Dom>& i_function)
{
    static const int Dimension = Dom::dimension();
    typedef typename Dom::particle underlying_type;

    if(const detail::functor_impl<constant_function<Im,Dom>,Im,Dom>* functor = rtti::dynamicCast<const detail::functor_impl<constant_function<Im,Dom>,Im,Dom>>(i_function.template getFuncPtr()))
    {
        return constant_function<Im,Dom>(Im::group::neutral_element());
    }
    else
	{
		return null_ptr;
	}
}

template<int Component, ring_type Im, vector_space_type Dom>
vector_function<Im,Dom> neg_derivative_helper::get_custom_derivative(const vector_function<Im,Dom>& i_function)
{
    static const int Dimension = Dom::dimension();
    typedef typename Dom::particle underlying_type;
    typedef typename mpl::homogeneous_callable<ytl::neg_callable_t,Im,underlying_type,Dimension>::type neg_homogeneous_callable;

    if(const detail::functor_impl<neg_homogeneous_callable,Im,Dom>* functor = rtti::dynamicCast<const detail::functor_impl<neg_homogeneous_callable,Im,Dom>>(i_function.template getFuncPtr()))
    {
        const neg_homogeneous_callable& negCallable = functor->getCallable();

        return -derivative_helper::derivative<Component>(vector_function<Im,Dom>::clone(negCallable.template getNestedCallable<0>()));
    }
    else
	{
		return null_ptr;
	}
}

template<int Component, ring_type Im, vector_space_type Dom>
vector_function<Im,Dom> sum_derivative_helper::get_custom_derivative(const vector_function<Im,Dom>& i_function)
{
    static const int Dimension = Dom::dimension();
    typedef typename Dom::particle underlying_type;
    typedef typename mpl::homogeneous_callable<ytl::sum_callable_t,Im,underlying_type,Dimension>::type sum_homogeneous_callable;

    if(const detail::functor_impl<sum_homogeneous_callable,Im,Dom>* functor = rtti::dynamicCast<const detail::functor_impl<sum_homogeneous_callable,Im,Dom>>(i_function.template getFuncPtr()))
    {
        const sum_homogeneous_callable& sumCallable = functor->getCallable();

        return derivative_helper::derivative<Component>(vector_function<Im,Dom>::clone(sumCallable.template getNestedCallable<0>())) + derivative_helper::derivative<Component>(vector_function<Im,Dom>::clone(sumCallable.template getNestedCallable<1>()));
    }
    else
	{
		return null_ptr;
	}
}

template<int Component, ring_type Im, vector_space_type Dom>
vector_function<Im,Dom> subs_derivative_helper::get_custom_derivative(const vector_function<Im,Dom>& i_function)
{
    static const int Dimension = Dom::dimension();
    typedef typename Dom::particle underlying_type;
    typedef typename mpl::homogeneous_callable<ytl::subs_callable_t,Im,underlying_type,Dimension>::type subs_homogeneous_callable;

    if(const functor_impl<subs_homogeneous_callable,Im,Dom>* functor = rtti::dynamicCast<const functor_impl<subs_homogeneous_callable,Im,Dom>>(i_function.template getFuncPtr()))
    {
        const subs_homogeneous_callable& subsCallable = functor->getCallable();

        return derivative_helper::derivative<Component>(vector_function<Im,Dom>::clone(subsCallable.template getNestedCallable<0>())) - derivative_helper::derivative<Component>(vector_function<Im,Dom>::clone(subsCallable.template getNestedCallable<1>()));
    }
    else
	{
		return null_ptr;
	}
}

template<int Component, ring_type Im, vector_space_type Dom>
vector_function<Im,Dom> prod_derivative_helper::get_custom_derivative(const vector_function<Im,Dom>& i_function)
{
    static const int Dimension = Dom::dimension();
    typedef typename Dom::particle underlying_type;
    typedef typename mpl::homogeneous_callable<ytl::prod_callable_t,Im,underlying_type,Dimension>::type prod_homogeneous_callable;

    if(const detail::functor_impl<prod_homogeneous_callable,Im,Dom>* functor = rtti::dynamicCast<const detail::functor_impl<prod_homogeneous_callable,Im,Dom>>(i_function.template getFuncPtr()))
    {
        const prod_homogeneous_callable& prodCallable = functor->getCallable();

        return derivative_helper::derivative<Component>(vector_function<Im,Dom>::clone(prodCallable.template getNestedCallable<0>())) * vector_function<Im,Dom>::clone(prodCallable.template getNestedCallable<1>()) + vector_function<Im,Dom>::clone(prodCallable.template getNestedCallable<0>()) * derivative_helper::derivative<Component>(vector_function<Im,Dom>::clone(prodCallable.template getNestedCallable<1>()));
    }
    else
	{
		return null_ptr;
	}
}

template<int Component, ring_type Im, vector_space_type Dom>
vector_function<Im,Dom> div_derivative_helper::get_custom_derivative(const vector_function<Im,Dom>& i_function)
{
    static const int Dimension = Dom::dimension();
    typedef typename Dom::particle underlying_type;
    typedef typename mpl::homogeneous_callable<ytl::div_callable_t,Im,underlying_type,Dimension>::type div_homogeneous_callable;

    if(const detail::functor_impl<div_homogeneous_callable,Im,Dom>* functor = rtti::dynamicCast<const detail::functor_impl<div_homogeneous_callable,Im,Dom>>(i_function.template getFuncPtr()))
    {
        const div_homogeneous_callable& divCallable = functor->getCallable();

        return (derivative_helper::derivative<Component>(vector_function<Im,Dom>::clone(divCallable.template getNestedCallable<0>())) * vector_function<Im,Dom>::clone(divCallable.template getNestedCallable<1>()) - vector_function<Im,Dom>::clone(divCallable.template getNestedCallable<0>()) * derivative_helper::derivative<Component>(vector_function<Im,Dom>::clone(divCallable.template getNestedCallable<1>()))) / (vector_function<Im,Dom>::clone(divCallable.template getNestedCallable<1>()) * vector_function<Im,Dom>::clone(divCallable.template getNestedCallable<1>()));
    }
    else
	{
		return null_ptr;
	}
}

template<int Component>
vector_function<Real,R1> sin_derivative_helper::get_custom_derivative(const vector_function<Real,R1>& i_function)
{
    static Real(*const addr)(const Real&) = &sin;

    if(const detail::free_function_impl<Real,R1>* func = rtti::dynamicCast<const detail::free_function_impl<Real,R1>>(i_function.getFuncPtr()))
    {
        if(func->getFuncAddr() == addr)
        {
            return cosr;
        }
    }

    return null_ptr;
}
template<int Component>
vector_function<Complex,C1> sin_derivative_helper::get_custom_derivative(const vector_function<Complex,C1>& i_function)
{
    static Complex(*const addr)(const Complex&) = &sin;

    if(const detail::free_function_impl<Complex,C1>* func = rtti::dynamicCast<const detail::free_function_impl<Complex,C1>>(i_function.getFuncPtr()))
    {
        if(func->getFuncAddr() == addr)
        {
            return cosc;
        }
    }

    return null_ptr;
}

template<int Component>
vector_function<Real,R1> cos_derivative_helper::get_custom_derivative(const vector_function<Real,R1>& i_function)
{
    static Real(*const addr)(const Real&) = &cos;

    if(const detail::free_function_impl<Real,R1>* func = rtti::dynamicCast<const detail::free_function_impl<Real,R1>>(i_function.getFuncPtr()))
    {
        if(func->getFuncAddr() == addr)
        {
            return -sinr;
        }
    }

    return null_ptr;
}
template<int Component>
vector_function<Complex,C1> cos_derivative_helper::get_custom_derivative(const vector_function<Complex,C1>& i_function)
{
    static Complex(*const addr)(const Complex&) = &cos;

    if(const detail::free_function_impl<Complex,C1>* func = rtti::dynamicCast<const detail::free_function_impl<Complex,C1>>(i_function.getFuncPtr()))
    {
        if(func->getFuncAddr() == addr)
        {
            return -sinc;
        }
    }

    return null_ptr;
}

template<int Component>
vector_function<Real,R1> tan_derivative_helper::get_custom_derivative(const vector_function<Real,R1>& i_function)
{
    static Real(*const addr)(const Real&) = &tan;

    if(const detail::free_function_impl<Real,R1>* func = rtti::dynamicCast<const detail::free_function_impl<Real,R1>>(i_function.getFuncPtr()))
    {
        if(func->getFuncAddr() == addr)
        {
            return constant_function<Real,R1>(Real::ring::neutral_element()) + tanr * tanr;
        }
    }

    return null_ptr;
}
template<int Component>
vector_function<Complex,C1> tan_derivative_helper::get_custom_derivative(const vector_function<Complex,C1>& i_function)
{
    static Complex(*const addr)(const Complex&) = &tan;

    if(const detail::free_function_impl<Complex,C1>* func = rtti::dynamicCast<const detail::free_function_impl<Complex,C1>>(i_function.getFuncPtr()))
    {
        if(func->getFuncAddr() == addr)
        {
            return constant_function<Complex,C1>(Complex::ring::neutral_element()) + tanc * tanc;
        }
    }

    return null_ptr;
}

template<int Component>
vector_function<Real,R1> exp_derivative_helper::get_custom_derivative(const vector_function<Real,R1>& i_function)
{
    static Real(*const addr)(const Real&) = &exp;

    if(const detail::free_function_impl<Real,R1>* func = rtti::dynamicCast<const detail::free_function_impl<Real,R1>>(i_function.getFuncPtr()))
    {
        if(func->getFuncAddr() == addr)
        {
            return expr;
        }
    }

    return null_ptr;
}
template<int Component>
vector_function<Complex,C1> exp_derivative_helper::get_custom_derivative(const vector_function<Complex,C1>& i_function)
{
    static Complex(*const addr)(const Complex&) = &exp;

    if(const detail::free_function_impl<Complex,C1>* func = rtti::dynamicCast<const detail::free_function_impl<Complex,C1>>(i_function.getFuncPtr()))
    {
        if(func->getFuncAddr() == addr)
        {
            return expc;
        }
    }

    return null_ptr;
}

template<int Component>
vector_function<Real,R1> log_derivative_helper::get_custom_derivative(const vector_function<Real,R1>& i_function)
{
    typedef ytl::function<Real(const Real&)> real_function;
    static Real(*const addr)(const Real&) = &log;

    if(const detail::free_function_impl<Real,R1>* func = rtti::dynamicCast<const detail::free_function_impl<Real,R1>>(i_function.getFuncPtr()))
    {
        if(func->getFuncAddr() == addr)
        {
            return constant_function<Real,R1>(Real::ring::neutral_element()) / projection<Real,R1>(mpl::numeric_type<0>{});
        }
    }

    return null_ptr;
}
template<int Component>
vector_function<Complex,C1> log_derivative_helper::get_custom_derivative(const vector_function<Complex,C1>& i_function)
{
    typedef ytl::function<Complex(const Complex&)> complex_function;
    static Complex(*const addr)(const Complex&) = &log;

    if(const detail::free_function_impl<Complex,C1>* func = rtti::dynamicCast<const detail::free_function_impl<Complex,C1>>(i_function.getFuncPtr()))
    {
        if(func->getFuncAddr() == addr)
        {
            return constant_function<Complex,C1>(Complex::ring::neutral_element()) / projection<Complex,C1>(mpl::numeric_type<0>{});
        }
    }

    return null_ptr;
}

}
}
}
