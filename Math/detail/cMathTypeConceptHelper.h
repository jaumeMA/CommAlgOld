#pragma once

#include "Math/detail/cAlgebraTemplateHelper.h"

namespace yame
{
namespace math
{

template<typename Type>
concept bool set_type = is_set<Type>::value;

template<typename Type>
concept bool group_type = set_type<Type> && is_group<Type>::value;

template<typename Type>
concept bool ring_type = group_type<Type> && is_ring<Type>::value;

template<typename Type>
concept bool division_ring_type = ring_type<Type> && is_division_ring<Type>::value;

template<typename Type>
concept bool field_type = ring_type<Type> && is_field<Type>::value;

template<typename Type>
concept bool module_type = group_type<Type> && is_module<Type>::value;

template<typename Type>
concept bool algebra_type = module_type<Type> && is_algebra<Type>::value;

template<typename Type>
concept bool vector_space_type = module_type<Type> && is_vector_space<Type>::value;

template<typename Type>
concept bool metric_space_type = is_metric_space<Type>::value;

template<typename Type>
concept bool vector_metric_space_type = metric_space_type<Type> && vector_space_type<Type>;

}
}
