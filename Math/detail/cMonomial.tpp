#include "Math/cMath.h"
#include "YTL/container/cAlgorithm.h"
#include "YTL/container/cIterable.h"

namespace yame
{
namespace math
{

template<typename T>
cMonomial<T>::cMonomial()
: m_coeff(T::group::neutral_element())
{}
template<typename T>
cMonomial<T>::cMonomial(const T& i_coeff, const container::cArray<s16>& i_poweredIndexes)
: m_coeff(i_coeff)
, m_poweredIndexes(i_poweredIndexes)
{}
template<typename T>
cMonomial<T>::cMonomial(const cMonomial<T>& other)
: m_coeff(other.m_coeff)
, m_poweredIndexes(other.m_poweredIndexes)
{}
template<typename T>
cMonomial<T>::cMonomial(cMonomial<T>&& other)
: m_coeff(mpl::move(other.m_coeff))
, m_poweredIndexes(mpl::move(other.m_poweredIndexes))
{}
template<typename T>
cMonomial<T> cMonomial<T>::operator*(const T& other) const
{
    return cMonomial<T>(m_coeff * other, m_poweredIndexes);
}
template<typename T>
cMonomial<T> cMonomial<T>::operator*(const cMonomial<T>& other) const
{
    cMonomial<T> res(m_coeff * other.m_coeff, {});

    yame::merge(m_poweredIndexes, other.m_poweredIndexes, res.m_poweredIndexes, [](ytl::optional<int> i_indexA, ytl::optional<int> i_indexB){ s16 res = 0; if(i_indexA) res += *i_indexA; if(i_indexB) res += *i_indexB; return res;});

    return res;
}
template<typename T>
cMonomial<T> cMonomial<T>::operator/(const T& other) const
{
    return cMonomial<T>(m_coeff / other.m_coeff, m_poweredIndexes);
}
template<typename T>
ytl::optional<cMonomial<T>> cMonomial<T>::operator/(const cMonomial<T>& other) const
{
    cMonomial<T> res(m_coeff / other.m_coeff, {});

    yame::merge(m_poweredIndexes, other.m_poweredIndexes, res.m_poweredIndexes, ytl::function<s16(ytl::optional<const s16&>,ytl::optional<const s16&>)>([](ytl::optional<const s16&> i_indexA, ytl::optional<const s16&> i_indexB){ s16 res = 0; if(i_indexA)res=*i_indexA; if(i_indexB)res-=*i_indexB; return res; }));

    //check that all of our indexes are bigger than other's
    if(yame::find_if(res.m_poweredIndexes, ytl::function<bool(const s16&)>([](s16 i_index){ return i_index < 0; })) != res.m_poweredIndexes.end())
    {
        return none;
    }
    else
    {
        return res;
    }
}
template<typename T>
cMonomial<T>& cMonomial<T>::operator=(const cMonomial<T>& other)
{
    m_coeff = other.m_coeff;
    m_poweredIndexes = other.m_poweredIndexes;

    return *this;
}
template<typename T>
cMonomial<T>& cMonomial<T>::operator=(cMonomial<T>&& other)
{
    m_coeff = mpl::move(other.m_coeff);
    m_poweredIndexes = mpl::move(other.m_poweredIndexes);

    return *this;
}
template<typename T>
void cMonomial<T>::setCoeff(const T& i_coeff)
{
    m_coeff = i_coeff;
}
template<typename T>
T cMonomial<T>::getCoeff() const
{
    return m_coeff;
}
template<typename T>
void cMonomial<T>::setPoweredIndex(size_t i_index, u16 i_power)
{
    if(i_index >= m_poweredIndexes.getSize())
    {
        m_poweredIndexes.resize(2 * i_index + 1);
    }

    m_poweredIndexes[i_index] = i_power;
}
template<typename T>
void cMonomial<T>::insertPoweredIndex(size_t i_pos, u16 i_power)
{
    m_poweredIndexes.insert(m_poweredIndexes.begin() + i_pos, i_power);
}
template<typename T>
u16 cMonomial<T>::getPowerOfIndex(size_t i_index) const
{
    return m_poweredIndexes[i_index];
}
template<typename T>
size_t cMonomial<T>::getNumIndexes() const
{
    return m_poweredIndexes.getSize();
}
template<typename T>
u16 cMonomial<T>::getTotalOrder() const
{
    return yame::sum(m_poweredIndexes);
}
template<typename T>
bool cMonomial<T>::_lexMoreComparison(const cMonomial<T>& i_lhs, const cMonomial<T>& i_rhs, bool leftToRight)
{
    //take first non zero difference on powered indexes
    if(i_lhs.m_poweredIndexes.empty() == false && i_rhs.m_poweredIndexes.empty() == false)
    {
        const container::cArray<s16>& lhsIndexes = i_lhs.m_poweredIndexes;
        const container::cArray<s16>& rhsIndexes = i_rhs.m_poweredIndexes;
        size_t currIndex = (leftToRight) ? 0 : min(lhsIndexes.getSize(),rhsIndexes.getSize());
        size_t maxIndex = max(lhsIndexes.getSize(),rhsIndexes.getSize());

        for(;currIndex >= 0 && currIndex<maxIndex;currIndex=(leftToRight) ? currIndex+1 : currIndex-1)
        {
            if(currIndex >= lhsIndexes.getSize())
            {
                return false;
            }
            else if(currIndex >= rhsIndexes.getSize())
            {
                return true;
            }
            else if(lhsIndexes[currIndex] != rhsIndexes[currIndex])
            {
                return (leftToRight) ? lhsIndexes[currIndex] - rhsIndexes[currIndex] > 0 : lhsIndexes[currIndex] - rhsIndexes[currIndex] < 0;
            }
        }

        return false;
    }
    else
    {
        return false;
    }
}
template<typename T>
bool cMonomial<T>::lexMoreComparison(cMonomial<T> i_lhs, cMonomial<T> i_rhs)
{
    return _lexMoreComparison(i_lhs,i_rhs,true);
}
template<typename T>
bool cMonomial<T>::grLexMoreComparison(cMonomial<T> i_lhs, cMonomial<T> i_rhs)
{
    int ilhsSum = yame::sum(i_lhs.m_poweredIndexes);
    int irhsSum = yame::sum(i_rhs.m_poweredIndexes);

    if(ilhsSum > irhsSum)
    {
        return true;
    }
    else if(ilhsSum == irhsSum)
    {
        return _lexMoreComparison(i_lhs,i_rhs,true);
    }
}
template<typename T>
bool cMonomial<T>::grRevLexMoreComparison(cMonomial<T> i_lhs, cMonomial<T> i_rhs)
{
    int ilhsSum = yame::sum(i_lhs.m_poweredIndexes);
    int irhsSum = yame::sum(i_rhs.m_poweredIndexes);

    if(ilhsSum > irhsSum)
    {
        return true;
    }
    else if(ilhsSum == irhsSum)
    {
        return _lexMoreComparison(i_lhs,i_rhs,false);
    }
}
template<typename T>
typename cMonomial<T>::power_index_view cMonomial<T>::getPoweredIndexes() const
{
    return m_poweredIndexes;
}

}

template<typename T>
container::string format(const math::cMonomial<T>& i_monomial)
{
    container::string res;

    res = format(i_monomial.getCoeff());

    const size_t numIndexes = i_monomial.getNumIndexes();
    for(size_t currIndex=0;currIndex<numIndexes;currIndex++)
    {
        res += container::string::Format("x_%d^%d",currIndex,i_monomial.getPowerOfIndex(currIndex));
    }

    return res;
}
}
