#pragma once

#include "Math/detail/cAlgebraTemplateHelper.h"

namespace yame
{
namespace math
{

//concepts over traits

template<typename Traits>
concept bool commutative_traits_type = Traits::is_commutative;

template<typename Traits>
concept bool with_unity_traits_type = requires { { Traits::neutral_element } };

template<typename Traits>
concept bool with_inverse_type = requires (typename Traits::underlying_type& o_res, const typename Traits::underlying_type& i_value){ { Traits::inv(o_res,i_value) } };

template<typename Traits>
concept bool additive_traits_type = requires (typename Traits::underlying_type& o_res, const typename Traits::underlying_type& i_lhs, const typename Traits::underlying_type& i_rhs){ { Traits::sum(o_res,i_lhs,i_rhs) } };

template<typename Traits>
concept bool semi_group_traits_type = commutative_traits_type<Traits> && additive_traits_type<Traits>;

template<typename Traits>
concept bool semi_ring_traits_type = with_unity_traits_type<Traits> && commutative_traits_type<Traits> && additive_traits_type<Traits>;

template<typename Traits>
concept bool group_traits_type = with_unity_traits_type<Traits> && commutative_traits_type<Traits> && with_inverse_type<Traits> && additive_traits_type<Traits>;

template<typename Traits>
concept bool multiplicative_traits_type = requires (typename Traits::underlying_type& o_res, const typename Traits::underlying_type& i_lhs, const typename Traits::underlying_type& i_rhs){ { Traits::prod(o_res,i_lhs,i_rhs) } };

template<typename Traits>
concept bool ring_traits_type = group_traits_type<typename Traits::group_traits> && with_unity_traits_type<Traits> && multiplicative_traits_type<Traits>;

template<typename Traits>
concept bool division_ring_traits_type = ring_traits_type<typename Traits::ring_traits> && with_inverse_type<Traits>;

template<typename Traits>
concept bool field_traits_type = division_ring_traits_type<Traits> && commutative_traits_type<typename Traits::ring_traits>;

template<typename Traits>
concept bool module_traits_type = group_traits_type<typename Traits::group_traits> && requires (typename Traits::underlying_type& o_res, const typename Traits::ring& i_lhs, const typename Traits::underlying_type& i_rhs){ { Traits::modProd(o_res,i_lhs,i_rhs) } };

template<typename Traits>
concept bool finitely_generated_module_type = module_traits_type<Traits> && requires{ { Traits::dimension } } && requires(size_t i_index){ { Traits::base(i_index) }  };

template<typename Traits>
concept bool vector_space_traits_type = finitely_generated_module_type<typename Traits::module_traits> && is_field<typename Traits::field>::value;

//we consider here general case of algebras, i.e. modules over a ring with inner product compatible with its module structure
template<typename Traits>
concept bool algebra_traits_type = module_traits_type<typename Traits::module_traits> && is_ring<typename Traits::module_traits::ring>::value && requires (typename Traits::underlying_type& o_res, const typename Traits::underlying_type& i_lhs, const typename Traits::underlying_type& i_rhs){ { Traits::algProd(o_res,i_lhs,i_rhs) } };

template<typename Traits>
concept bool metric_space_traits_type = requires(const typename Traits::underlying_type& i_lhs, const typename Traits::underlying_type& i_rhs){ { Traits::distance(i_lhs,i_rhs) }->double };

template<typename Traits>
concept bool vector_metric_space_traits_type = metric_space_traits_type<Traits> && vector_space_traits_type<Traits>;

}
}
