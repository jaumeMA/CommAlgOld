#pragma once

#include "Math/detail/cMathTypeConceptHelper.h"
#include "Math/cFunctionSpaceTraits.h"
#include "Math/detail/cAlgebraImpl.h"

namespace yame
{
namespace math
{

template<template <set_type,set_type> typename F, group_type Im, set_type Dom>
detail::cGroupImpl<FunctionSpaceGroupTraits<Im,Dom>,F<Im,Dom>> resolve_function_space_structure(const Im&,const Dom&);

template<template <set_type,set_type> typename F, ring_type Im, set_type Dom>
detail::cModuleImpl<FunctionSpaceModuleTraits<Im,Dom>,F<Im,Dom>> resolve_function_space_structure(const Im&,const Dom&);

template<template <set_type,set_type> typename F, field_type Im, set_type Dom>
detail::cModuleImpl<FunctionSpaceModuleTraits<Im,Dom>,F<Im,Dom>> resolve_function_space_structure(const Im&,const Dom&);

IS_STRUCTURE(function_space)

}
}
