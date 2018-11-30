
namespace yame
{
namespace math
{

template<set_type Im, set_type Dom>
void FunctionSpaceSetTraits<Im,Dom>::init(underlying_type& o_value)
{
    //nothing to be done (alreay handled by function destructor)
}
template<set_type Im, set_type Dom>
void FunctionSpaceSetTraits<Im,Dom>::init(underlying_type& o_value, const underlying_type& i_value)
{
    o_value = i_value;
}
template<set_type Im, set_type Dom>
void FunctionSpaceSetTraits<Im,Dom>::deinit(underlying_type& o_value)
{
    //nothing to be done (alreay handled by function destructor)
}
template<set_type Im, set_type Dom>
void FunctionSpaceSetTraits<Im,Dom>::assign(underlying_type& o_value, const underlying_type& i_value)
{
    o_value = i_value;
}
template<set_type Im, set_type Dom>
bool FunctionSpaceSetTraits<Im,Dom>::cmp(const underlying_type& i_lhs, const underlying_type& i_rhs)
{
    //???
}

template<group_type Im, set_type Dom>
void FunctionSpaceGroupTraits<Im,Dom>::sum(underlying_type& o_res, const underlying_type& i_lhs,const underlying_type& i_rhs)
{
    o_res = i_lhs + i_rhs;
}
template<group_type Im, set_type Dom>
void FunctionSpaceGroupTraits<Im,Dom>::inv(underlying_type& o_res, const underlying_type& i_value)
{
    o_res = -i_value;
}

template<ring_type Im, set_type Dom>
void FunctionSpaceModuleTraits<Im,Dom>::modProd(underlying_type& o_res, const ring& i_lhs, const underlying_type& i_rhs)
{
    o_res = i_lhs * i_rhs;
}
template<ring_type Im, set_type Dom>
void FunctionSpaceModuleTraits<Im,Dom>::modProd(underlying_type& o_res, const underlying_type& i_lhs, const ring& i_rhs)
{
    o_res = i_lhs * i_rhs;
}

}
}
