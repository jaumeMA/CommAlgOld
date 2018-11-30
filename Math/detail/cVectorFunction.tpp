
namespace yame
{
namespace math
{
namespace detail
{

template<ring_type Im, vector_space_type Dom>
vector_function<Im,Dom>::vector_function(const base_function& i_base)
: base_function(i_base)
{
}
template<ring_type Im, vector_space_type Dom>
vector_function<Im,Dom> vector_function<Im,Dom>::clone(const func_ptr_base* i_funcPtr)
{
    vector_function<Im,Dom> res;

    if(i_funcPtr != NULL)
    {
        res.clone(*i_funcPtr);
    }

    return res;
}
template<ring_type Im, vector_space_type Dom>
template<int ... Indexs>
Im vector_function<Im,Dom>::_eval(const mpl::sequence<Indexs...>&, const Dom& i_point) const
{
    return eval(i_point.template get<Indexs>() ...);
}
template<ring_type Im, vector_space_type Dom>
Im vector_function<Im,Dom>::eval(const Dom& i_point) const
{
    typedef typename mpl::create_range_rank<0,Dom::dimension()>::type typeSeq;

    return _eval(typeSeq(),i_point);
}

}
}
}
