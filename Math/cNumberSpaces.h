#pragma once

#include "Math/cNumberTraits.h"
#include "Math/cSet.h"
#include "Math/detail/cAlgebraImpl.h"
#include "Math/detail/cMetricSpaceImpl.h"
#include "YTL/container/cString.h"

namespace yame
{
namespace math
{

static const unsigned char k_defaultPrecision = 128;

struct RationalTraits : RationalDivisionRingTraits
{
    typedef RationalDivisionRingTraits field;
	typedef typename RationalDivisionRingTraits::underlying_type underlying_type;
};

class Rational : public detail::cFieldImpl<RationalTraits,Rational>, public cSet<RationalSetTraits>
{
public:
    typedef RationalTraits traits;
	using cSet<RationalSetTraits>::cSet;
	using cSet<RationalSetTraits>::operator=;
	using cSet<RationalSetTraits>::operator==;
	using cSet<RationalSetTraits>::operator!=;
    using cFieldImpl<RationalTraits,Rational>::cFieldImpl;
    using cFieldImpl<RationalTraits,Rational>::operator=;

    Rational(const Rational&) = default;
    Rational& operator=(const Rational&) = default;
};

template<unsigned char Precision = k_defaultPrecision>
struct RealTraits : RealDivisionRingTraits<Precision>, RealMetricSpaceTraits<Precision>
{
    typedef RealDivisionRingTraits<Precision> field;
    typedef RealMetricSpaceTraits<Precision> metric_space;
	typedef typename RealDivisionRingTraits<Precision>::underlying_type underlying_type;
};

class Real : public detail::cFieldImpl<RealTraits<>,Real>, public detail::cMetricSpaceImpl<RealMetricSpaceTraits<k_defaultPrecision>,Real>, public cSet<RealSetTraits<k_defaultPrecision>>
{
public:
    typedef RealSetTraits<k_defaultPrecision> traits;
	using cSet<RealSetTraits<k_defaultPrecision>>::cSet;
	using cSet<RealSetTraits<k_defaultPrecision>>::operator=;
	using cSet<RealSetTraits<k_defaultPrecision>>::operator==;
	using cSet<RealSetTraits<k_defaultPrecision>>::operator!=;

    Real(const Real&) = default;
    Real& operator=(const Real&) = default;
};

template<unsigned char Precision>
class RealP : public detail::cFieldImpl<RealTraits<Precision>,Real>, public detail::cMetricSpaceImpl<RealMetricSpaceTraits<Precision>,RealSetTraits<Precision>>, public cSet<RealSetTraits<Precision>>
{
public:
    typedef RealSetTraits<Precision> traits;
	using cSet<RealSetTraits<Precision>>::cSet;
	using cSet<RealSetTraits<Precision>>::operator=;
	using cSet<RealSetTraits<Precision>>::operator==;
	using cSet<RealSetTraits<Precision>>::operator!=;

    RealP(const RealP&) = default;
    RealP& operator=(const RealP&) = default;
};

template<unsigned char Precision = k_defaultPrecision>
struct ComplexTraits : ComplexDivisionRingTraits<Precision>, ComplexMetricSpaceTraits<Precision>
{
    typedef ComplexDivisionRingTraits<Precision> field;
    typedef ComplexMetricSpaceTraits<Precision> metric_space;
	typedef typename ComplexDivisionRingTraits<Precision>::underlying_type underlying_type;
};

class Complex : public detail::cFieldImpl<ComplexTraits<>,Complex>, public detail::cMetricSpaceImpl<ComplexMetricSpaceTraits<k_defaultPrecision>,Complex>, public cSet<ComplexSetTraits<k_defaultPrecision>>
{
public:
    typedef ComplexSetTraits<k_defaultPrecision> traits;
	using cSet<ComplexSetTraits<k_defaultPrecision>>::cSet;
	using cSet<ComplexSetTraits<k_defaultPrecision>>::operator=;
	using cSet<ComplexSetTraits<k_defaultPrecision>>::operator==;
	using cSet<ComplexSetTraits<k_defaultPrecision>>::operator!=;

    Complex(const Complex&) = default;
    Complex& operator=(const Complex&) = default;
};

template<unsigned char Precision = k_defaultPrecision>
struct QuaternionTraits : QuaternionDivisionRingTraits<Precision>, QuaternionMetricSpaceTraits<Precision>
{
    typedef QuaternionDivisionRingTraits<Precision> field;
    typedef QuaternionMetricSpaceTraits<Precision> metric_space;
	typedef typename QuaternionDivisionRingTraits<Precision>::underlying_type underlying_type;
};

class Quaternion : public detail::cDivisionRingImpl<QuaternionTraits<>,Quaternion>, public detail::cMetricSpaceImpl<QuaternionMetricSpaceTraits<k_defaultPrecision>,Quaternion>, public cSet<QuaternionSetTraits<k_defaultPrecision>>
{
public:
    typedef QuaternionSetTraits<k_defaultPrecision> traits;
	using cSet<QuaternionSetTraits<k_defaultPrecision>>::cSet;
	using cSet<QuaternionSetTraits<k_defaultPrecision>>::operator=;
	using cSet<QuaternionSetTraits<k_defaultPrecision>>::operator==;
	using cSet<QuaternionSetTraits<k_defaultPrecision>>::operator!=;

    Quaternion(const Quaternion&) = default;
    Quaternion& operator=(const Quaternion&) = default;
};

typedef Quaternion Q;

template<int p>
struct ZpTraits : ZpDivisionRingTraits<p>
{
    typedef ZpDivisionRingTraits<p> field;
	typedef typename ZpDivisionRingTraits<p>::underlying_type underlying_type;
};

template<int p>
class Zp : public detail::cFieldImpl<ZpTraits<p>,Zp<p>>, public cSet<ZpSetTraits<p>>
{
public:
    typedef ZpSetTraits<p> traits;
	using cSet<ZpSetTraits<p>>::cSet;
	using cSet<ZpSetTraits<p>>::operator=;
	using cSet<ZpSetTraits<p>>::operator==;
	using cSet<ZpSetTraits<p>>::operator!=;

    Zp(const Zp&) = default;
    Zp& operator=(const Zp&) = default;
};

}

container::string format(const math::Rational& i_value);
container::string format(const math::Real& i_value);
container::string format(const math::Complex& i_value);
container::string format(const math::Quaternion& i_value);
template<int p>
container::string format(const math::Zp<p>& i_value);
}

#include "Math/detail/cNumberSpaces.tpp"
