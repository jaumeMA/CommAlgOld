#pragma once

#include "Math/cFunctionSpaceTraits.h"
#include "Math/detail/cAlgebraImpl.h"
#include "Math/detail/cMathTypeConceptHelper.h"
#include "Math/detail/cFunctionSpaceTemplateHelper.h"
#include "Math/detail/cMathTags.h"

namespace yame
{
namespace math
{

template<set_type Im, set_type Dom>
class cFunctionSpace;

namespace detail
{

template<set_type Im, set_type Dom>
struct ExtendedFunctionSpaceSet : virtual public detail::ISet<FunctionSpaceSetTraits<Im,Dom>>
{
    typedef decltype(underlying_function_type(std::declval<Im>(),std::declval<Dom>())) underlying_type;

    underlying_type get_nested_function();
    template<set_type IIm, set_type DDom>
    requires requires { mpl::is_type_constructible<Im,IIm>::value; mpl::is_type_constructible<Dom,DDom>::value; }
    static void init(underlying_type& o_value, const cFunctionSpace<IIm,DDom>& i_value);
};

}

template<set_type Im, set_type Dom>
class cFunctionSpace : public decltype(resolve_function_space_structure<cFunctionSpace>(std::declval<Im>(),std::declval<Dom>())), public cSet<FunctionSpaceSetTraits<Im,Dom>>, public detail::function_space_tag
{
public:
    typedef decltype(resolve_function_space_structure<cFunctionSpace>(std::declval<Im>(),std::declval<Dom>())) function_base;
    typedef typename function_base::traits traits;
	typedef typename traits::underlying_type underlying_type;

	using cSet<FunctionSpaceSetTraits<Im,Dom>>::cSet;
	using cSet<FunctionSpaceSetTraits<Im,Dom>>::operator=;
	using cSet<FunctionSpaceSetTraits<Im,Dom>>::operator==;
	using cSet<FunctionSpaceSetTraits<Im,Dom>>::operator!=;

    cFunctionSpace(const cFunctionSpace&) = default;
    cFunctionSpace& operator=(const cFunctionSpace&) = default;
};

template<ring_type Im, set_type Dom>
detail::ExtendedFunctionSpaceSet<Im,Dom> underlying_function_extension_type(const Im&,const Dom&);

}
}
