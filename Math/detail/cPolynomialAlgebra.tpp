
#include "Math/cPolynomialIterable.h"
#include "Math/cPolynomialOps.h"

namespace yame
{
namespace math
{

template<typename T, template<typename> class A>
polynomial<T,A>::polynomial()
: cSet<PolySetTraits>(static_cast<const cSet<PolySetTraits>&>(group::neutral_element()))
{
}
template<typename T, template<typename> class A>
polynomial<T,A>::polynomial(const cMonomial<T>& i_monomial)
: polynomial(detail::createPolyFromMonomial<A>(i_monomial))
{
}
template<typename T, template<typename> class A>
polynomial<T,A>::polynomial(const polynomial<T,A>& other)
: cSet<PolySetTraits>(other.getValue())
{
}
template<typename T, template<typename> class A>
polynomial<T,A>::polynomial(polynomial<T,A>&& other)
: cSet<PolySetTraits>(other.extractValue())
{
}
template<typename T, template<typename> class A>
polynomial<T,A>& polynomial<T,A>::operator=(const polynomial<T,A>& other)
{
    this->setValue(other.getValue());

    return *this;
}
template<typename T, template<typename> class A>
polynomial<T,A>& polynomial<T,A>::operator=(polynomial<T,A>&& other)
{
    this->setValue(mpl::move(other.getValue()));

    return *this;
}

}
}

