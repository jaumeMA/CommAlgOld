
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

#include "YTL/container/cMultiMapIterable.h"
#include "Math/cPolynomialAlgebra.h"
#include "Math/cMonomial.h"

namespace yame
{
namespace math
{

template<typename T, template<typename> class A = memory::cTypedSystemAllocator>
class cPolynomialIterable : public container::cMultiMapIterable<typename cPolynomial<T,A>::key_type,typename cPolynomial<T,A>::value_type,cMonomial<T>>
{
    typedef container::cMultiMapIterable<typename cPolynomial<T,A>::key_type,typename cPolynomial<T,A>::value_type,cMonomial<T>> base;
    typedef typename container::cMultiMapIterable<typename cPolynomial<T,A>::key_type,typename cPolynomial<T,A>::value_type,cMonomial<T>>::forwarded_value_type forwarded_value_type;
    typedef typename container::cMultiMapIterable<typename cPolynomial<T,A>::key_type,typename cPolynomial<T,A>::value_type,cMonomial<T>>::inner_value_type inner_value_type;
    typedef typename container::cMultiMapIterable<typename cPolynomial<T,A>::key_type,typename cPolynomial<T,A>::value_type,cMonomial<T>>::inner_iterator_type inner_iterator_type;
    typedef typename container::cMultiMapIterable<typename cPolynomial<T,A>::key_type,typename cPolynomial<T,A>::value_type,cMonomial<T>>::nested_iterable_interface nested_iterable_interface;
    typedef typename container::cMultiMapIterable<typename cPolynomial<T,A>::key_type,typename cPolynomial<T,A>::value_type,cMonomial<T>>::node_pointer_type node_pointer_type;
    typedef ytl::function<bool(cMonomial<T>,cMonomial<T>)> compare_func;

public:
	typedef typename cPolynomial<T,A>::MapNode MapNode;
    typedef typename container::cMultiMapIterable<typename cPolynomial<T,A>::key_type,typename cPolynomial<T,A>::value_type,cMonomial<T>>::iterator_type iterator_type;
    typedef typename container::cMultiMapIterable<typename cPolynomial<T,A>::key_type,typename cPolynomial<T,A>::value_type,cMonomial<T>>::const_iterator_type const_iterator_type;
    typedef typename container::cMultiMapIterable<typename cPolynomial<T,A>::key_type,typename cPolynomial<T,A>::value_type,cMonomial<T>>::reverse_iterator_type reverse_iterator_type;
    typedef typename container::cMultiMapIterable<typename cPolynomial<T,A>::key_type,typename cPolynomial<T,A>::value_type,cMonomial<T>>::const_reverse_iterator_type const_reverse_iterator_type;
	typedef typename container::cMultiMapIterable<typename cPolynomial<T,A>::key_type,typename cPolynomial<T,A>::value_type,cMonomial<T>>::value_type value_type;
	typedef typename container::cMultiMapIterable<typename cPolynomial<T,A>::key_type,typename cPolynomial<T,A>::value_type,cMonomial<T>>::reference reference;
	typedef typename container::cMultiMapIterable<typename cPolynomial<T,A>::key_type,typename cPolynomial<T,A>::value_type,cMonomial<T>>::const_reference const_reference;

    cPolynomialIterable(const polynomial<T,A>& i_polynomial);
    cPolynomialIterable(const cPolynomial<T,A>& i_polynomial);
    cPolynomialIterable(const polynomial<T,A>& i_polynomial, const compare_func& i_compare);
    cPolynomialIterable(const cPolynomial<T,A>& i_polynomial, const compare_func& i_compare);

    iterator_type begin(const ytl::function<bool(const_reference)>& filter = null_ptr) override;
    const_iterator_type cbegin(const ytl::function<bool(const_reference)>& filter = null_ptr) const override;
    reverse_iterator_type rbegin(const ytl::function<bool(const_reference)>& filter = null_ptr) override;
    const_reverse_iterator_type crbegin(const ytl::function<bool(const_reference)>& filter = null_ptr) const override;

private:
    node_pointer_type getNextElem(node_pointer_type currNode) const override;
    node_pointer_type getPrevElem(node_pointer_type currNode) const override;
    node_pointer_type getFirstElem() const override;
    node_pointer_type getLastElem() const override;
    static cMonomial<T> fromIterableNodeToMonomial(container::cPair<typename cPolynomial<T,A>::key_type,typename cPolynomial<T,A>::value_type>& i_val);

    compare_func m_compare;
};

template<typename T>
using polynomial_view = container::multimap_view<cMonomial<T>>;

}
}

#include "detail/cPolynomialIterable.tpp"
