
/*This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#pragma once

#include "cMath.h"
#include "YTL/container/cTupla.h"
#include "YTL/container/cString.h"
#include "YTL/container/cRecursiveAVLTree.h"
#include "YTL/container/cMultiMap.h"

namespace yame
{
namespace math
{
template<typename T, template<typename> class A = memory::cTypedSystemAllocator>
class cPolynomialValue;
}

namespace mpl
{
template<typename T, template<typename> class A>
struct is_copy_constructible<math::cPolynomialValue<T,A>>
{
static const bool value = is_copy_constructible<T>::value;
};
}

namespace math
{

template<typename T, template<typename> class A>
class cPolynomialValue : public container::detail::cMultiMapValue<T,s16,container::cAVLtree,A,container::RecursiveBalancer<container::cAVLtree>::Balancer,cPolynomialValue<T,A>>
{
public:
    typedef container::detail::cMultiMapValue<T,s16,container::cAVLtree,A,container::RecursiveBalancer<container::cAVLtree>::Balancer,cPolynomialValue<T,A>> MapValue;
    typedef typename MapValue::underlying_type underlying_type;
    typedef typename MapValue::MapClass MapClass;
    typedef typename MapValue::MapNode MapNode;
    typedef typename MapValue::const_T const_T;
    typedef typename MapValue::const_R const_R;
    typedef typename MapValue::lvaluereference lvaluereference;
    typedef typename MapValue::const_lvaluereference const_lvaluereference;
    typedef typename MapValue::lkeyreference lkeyreference;
    typedef typename MapValue::const_lkeyreference const_lkeyreference;
    typedef typename MapValue::rvaluereference rvaluereference;
    typedef typename MapValue::rkeyreference rkeyreference;
    typedef typename MapValue::value_type value_type;
    typedef typename MapValue::key_type key_type;
    typedef typename MapValue::iterator_type iterator_type;
    typedef typename MapValue::const_iterator_type const_iterator_type;
    typedef typename MapValue::iterable_private_interface iterable_private_interface;

    using MapValue::MapValue;
    using MapValue::begin;
    using MapValue::cbegin;
    using MapValue::end;
    using MapValue::cend;
    using MapValue::operator=;
    using MapValue::operator[];
    using MapValue::operator underlying_type&;
    using MapValue::operator const underlying_type&;
    using MapValue::getValue;
    using MapValue::getTotalSize;
    using MapValue::getHolder;
    using MapValue::getNodeLocation;
    using MapValue::getAt;

    cPolynomialValue();
    cPolynomialValue(const cPolynomialValue&) = default;
    cPolynomialValue(cPolynomialValue&&) = default;
	template<typename ... Args>
	underlying_type& getAt(s16 key, Args&& ... keys);
	template<typename ... Args>
	const underlying_type& getAt(s16 key, Args&& ... keys) const;
	cPolynomialValue<T,A>& operator=(const cPolynomialValue<T,A>& other);
	cPolynomialValue<T,A>& operator=(cPolynomialValue<T,A>&& other);
	cPolynomialValue<T,A> operator+(const T& i_value) const;
	cPolynomialValue<T,A>& operator+=(const T& i_value);
	cPolynomialValue<T,A> operator-() const;
	cPolynomialValue<T,A> operator-(const T& i_value) const;
	cPolynomialValue<T,A>& operator-=(const T& i_value);
	cPolynomialValue<T,A> operator*(const T& i_value) const;
	cPolynomialValue<T,A>& operator*=(const T& i_value);
};

template<typename T, template<typename> class A = memory::cTypedSystemAllocator>
using cPolynomial = container::detail::cMultiMapImpl<cPolynomialValue<T,A>,s16,container::cAVLtree,A,container::MultiBalancer<container::cAVLtree>::Balancer>;

}
}

#include "Math/detail/cPolynomial.tpp"
