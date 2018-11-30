
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

#include "YTL/types/optional/optional.h"
#include "YTL/container/cTupla.h"

namespace yame
{
namespace math
{

template<typename T>
class cMonomial
{
public:
    typedef container::const_array_view<s16> power_index_view;

    cMonomial();
    cMonomial(const T& i_coeff, const container::cArray<s16>& i_poweredIndexes);
    cMonomial(const cMonomial<T>& other);
    cMonomial(cMonomial<T>&& other);
    cMonomial<T> operator*(const T& other) const;
    cMonomial<T> operator*(const cMonomial<T>& other) const;
    cMonomial<T> operator/(const T& other) const;
    ytl::optional<cMonomial<T>> operator/(const cMonomial<T>& other) const;
    cMonomial<T>& operator=(const cMonomial<T>& other);
    cMonomial<T>& operator=(cMonomial<T>&& other);
    void setCoeff(const T& i_coeff);
    T getCoeff() const;
    void insertPoweredIndex(size_t i_pos, u16 i_power);
    void setPoweredIndex(size_t i_index, u16 i_power);
    size_t getNumIndexes() const;
    u16 getPowerOfIndex(size_t i_index) const;
    u16 getTotalOrder() const;
    power_index_view getPoweredIndexes() const;

    //order static functions
    static bool lexMoreComparison(cMonomial<T> i_lhs, cMonomial<T> i_rhs);
    static bool grLexMoreComparison(cMonomial<T> i_lhs, cMonomial<T> i_rhs);
    static bool grRevLexMoreComparison(cMonomial<T> i_lhs, cMonomial<T> i_rhs);

private:
    static bool _lexMoreComparison(const cMonomial<T>& i_lhs, const cMonomial<T>& i_rhs, bool leftToRight);

    T m_coeff = T();
    container::cArray<s16> m_poweredIndexes;
};

}

template<typename T>
container::string format(const math::cMonomial<T>& i_poly);
}

#include "detail/cMonomial.tpp"
