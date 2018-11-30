
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

#include "Math/detail/cAlgebraImpl.h"
#include "Math/cPolynomialAlgebraTraits.h"
#include "Math/cNumberSpaces.h"
#include "YTL/container/cString.h"

namespace yame
{
namespace math
{

template<typename>
class cMonomial;

template<typename T, template<typename> class A = memory::cTypedSystemAllocator>
class polynomial : public detail::cAlgebraImpl<PolynomialAlgebraTraits<T,A>,polynomial<T,A>>, public cSet<PolynomialSetTraits<T,A>>
{
public:
    typedef detail::cAlgebraImpl<PolynomialAlgebraTraits<T,A>,polynomial<T,A>> algebra;
    typedef typename algebra::module module;
    typedef typename module::group group;
    typedef PolynomialSetTraits<T,A> PolySetTraits;

	using cSet<PolySetTraits>::cSet;
	using cSet<PolySetTraits>::operator=;
	using cSet<PolySetTraits>::operator==;
	using cSet<PolySetTraits>::operator!=;

    polynomial();
	polynomial(const cMonomial<T>&);
	polynomial(const polynomial<T,A>&);
	polynomial(polynomial<T,A>&&);
    polynomial<T,A>& operator=(const polynomial<T,A>&);
    polynomial<T,A>& operator=(polynomial<T,A>&&);
};

typedef polynomial<Rational,memory::cTypedSystemAllocator> rational_polynomial;
typedef polynomial<Real,memory::cTypedSystemAllocator> real_polynomial;
typedef polynomial<Complex,memory::cTypedSystemAllocator> complex_polynomial;

}
}


#include "Math/detail/cPolynomialAlgebra.tpp"
